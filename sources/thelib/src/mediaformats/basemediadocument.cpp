/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mediaformats/basemediadocument.h"

BaseMediaDocument::BaseMediaDocument(Variant &metadata) {
	_audioSamplesCount = 0;
	_videoSamplesCount = 0;
	_metadata = metadata;
	_keyframeSeek = false;
}

BaseMediaDocument::~BaseMediaDocument() {
}

bool BaseMediaDocument::Process() {
	double startTime = 0;
	double endTime = 0;
	GETCLOCKS(startTime);

	//1. Compute the names
	_mediaFilePath = (string) _metadata[META_SERVER_FULL_PATH];
	_metaFilePath = _mediaFilePath + "."MEDIA_TYPE_META;
	_seekFilePath = _mediaFilePath + "."MEDIA_TYPE_SEEK;
	_keyframeSeek = (bool)_metadata[CONF_APPLICATION_KEYFRAMESEEK];
	_seekGranularity = (uint32_t) _metadata[CONF_APPLICATION_SEEKGRANULARITY];

	//1. Open the media file
#ifdef HAS_MMAP
	if (!_mediaFile.Initialize(_mediaFilePath, 4 * 1024 * 1024, true)) {
		FATAL("Unable to open media file: %s", STR(_mediaFilePath));
		return false;
	}
#else
	if (!_mediaFile.Initialize(_mediaFilePath)) {
		FATAL("Unable to open media file: %s", STR(_mediaFilePath));
		return false;
	}
#endif

	//4. Read the document
	if (!ParseDocument()) {
		FATAL("Unable to parse document");
		return false;
	}

	//5. Build the frames
	if (!BuildFrames()) {
		FATAL("Unable to build frames");
		return false;
	}

	//6. Save the seek file
	if (!SaveSeekFile()) {
		FATAL("Unable to save seeking file");
		return false;
	}

	//7. Build the meta
	if (!SaveMetaFile()) {
		FATAL("Unable to save meta file");
		return false;
	}

	GETCLOCKS(endTime);

	INFO("%"PRIz"u frames computed in %.2f seconds at a speed of %.2f FPS",
			_frames.size(),
			(endTime - startTime) / (double) CLOCKS_PER_SECOND,
			(double) _frames.size() / ((endTime - startTime) / (double) CLOCKS_PER_SECOND));
	if (_frames.size() != 0) {
		uint32_t totalSeconds = (uint32_t) (((uint32_t) _frames[_frames.size() - 1].absoluteTime) / 1000);
		uint32_t hours = totalSeconds / 3600;
		uint32_t minutes = (totalSeconds - hours * 3600) / 60;
		uint32_t seconds = (totalSeconds - hours * 3600 - minutes * 60);
		INFO("File size: %"PRIu64" bytes; Duration: %u:%u:%u (%u sec); Optimal bandwidth: %.2f kb/s",
				_mediaFile.Size(),
				hours, minutes, seconds,
				totalSeconds,
				(double) _streamCapabilities.bandwidthHint);
	}

	moveFile(_seekFilePath + ".tmp", _seekFilePath);
	moveFile(_metaFilePath + ".tmp", _metaFilePath);

	chmod(STR(_seekFilePath), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	chmod(STR(_metaFilePath), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

	return true;
}

Variant BaseMediaDocument::GetMetadata() {
	return _metadata;
}

MediaFile &BaseMediaDocument::GetMediaFile() {
	return _mediaFile;
}

bool BaseMediaDocument::CompareFrames(const MediaFrame &frame1, const MediaFrame &frame2) {
	if (frame1.absoluteTime == frame2.absoluteTime)
		return frame1.start < frame2.start;
	return frame1.absoluteTime < frame2.absoluteTime;
}

bool BaseMediaDocument::SaveSeekFile() {
	if (_frames.size() <= 2) {
		FATAL("No frames found");
		return false;
	}

	File seekFile;

	//1. Open the file
	if (!seekFile.Initialize(_seekFilePath + ".tmp", FILE_OPEN_MODE_TRUNCATE)) {
		FATAL("Unable to open seeking file %s", STR(_seekFilePath));
		return false;
	}

	//2. Setup the bandwidth hint in bytes/second
	uint32_t totalSeconds = (uint32_t) (((uint32_t) _frames[_frames.size() - 1].absoluteTime) / 1000);
	_streamCapabilities.bandwidthHint =
			(uint32_t) ((double) _mediaFile.Size() / (double) totalSeconds / 1024.0 * 8.0);

	//2. Serialize the stream capabilities
	IOBuffer raw;
	if (!_streamCapabilities.Serialize(raw)) {
		FATAL("Unable to serialize stream capabilities");
		return false;
	}
	if (!seekFile.WriteUI32(GETAVAILABLEBYTESCOUNT(raw), false)) {
		FATAL("Unable to serialize stream capabilities");
		return false;
	}
	if (!seekFile.WriteBuffer(GETIBPOINTER(raw), GETAVAILABLEBYTESCOUNT(raw))) {
		FATAL("Unable to serialize stream capabilities");
		return false;
	}

	//2. Write the number of frames
	uint32_t framesCount = _frames.size();
	if (!seekFile.WriteUI32(framesCount, false)) {
		FATAL("Unable to write frame count");
		return false;
	}

	//3. Write the frames
	bool hasVideo = false;
	uint64_t maxFrameSize = 0;

	FOR_VECTOR(_frames, i) {
		MediaFrame frame = _frames[i];
		if (maxFrameSize < frame.length) {
			//WARN("maxFrameSize bumped up: %"PRIu64" -> %"PRIu64, maxFrameSize, frame.length);
			maxFrameSize = frame.length;
		}
		hasVideo |= (frame.type == MEDIAFRAME_TYPE_VIDEO);
		if (!seekFile.WriteBuffer((uint8_t *) & frame, sizeof (frame))) {
			FATAL("Unable to write frame");
			return false;
		}
	}
	_keyframeSeek &= hasVideo;

	//4. Write the seek granularity
	if (!seekFile.WriteUI32(_seekGranularity, false)) {
		FATAL("Unable to write sampling rate");
		return false;
	}

	//4. create the time to frame index table. First, see what is the total time
	double totalTime = 0;

	if (framesCount >= 1) {
		totalTime = _frames[framesCount - 1].absoluteTime;

		//5. build the table
		uint32_t frameIndex = 0;
		uint32_t seekPoint = 0;
		for (double i = 0; i <= totalTime; i += _seekGranularity) {
			while (_frames[frameIndex].absoluteTime < i) {
				frameIndex++;
				if (frameIndex >= framesCount)
					break;

				if (_keyframeSeek) {
					if ((_frames[frameIndex].type == MEDIAFRAME_TYPE_VIDEO)
							&& (_frames[frameIndex].isKeyFrame)) {
						seekPoint = frameIndex;
					}
				} else {
					seekPoint = frameIndex;
				}
			}
			if (frameIndex >= framesCount)
				break;
			if (!seekFile.WriteUI32(seekPoint, false)) {
				FATAL("Unable to write frame index");
				return false;
			}
		}
	}

	//6. Save the max frame size
	if (!seekFile.WriteUI64(maxFrameSize, false)) {
		FATAL("Unable to write frame count");
		return false;
	}

	//7. Done
	return true;
}

bool BaseMediaDocument::SaveMetaFile() {
	_metadata[META_AUDIO_FRAMES_COUNT] = _audioSamplesCount;
	_metadata[META_VIDEO_FRAMES_COUNT] = _videoSamplesCount;
	_metadata[META_TOTAL_FRAMES_COUNT] = (uint32_t) _frames.size();

	_metadata[META_FILE_SIZE] = (uint64_t) _mediaFile.Size();
	if (_frames.size() > 0) {
		_metadata[META_FILE_DURATION] = (uint32_t) _frames[_frames.size() - 1].absoluteTime;
		_metadata[META_FILE_BANDWIDTH] = _streamCapabilities.bandwidthHint;
	} else {
		_metadata[META_FILE_DURATION] = (uint32_t) 0;
	}

	_metadata[META_RTMP_META] = GetRTMPMeta();
	_metadata[META_RTMP_META]["duration"] = (double) _metadata[META_FILE_DURATION] / 1000.00;
	_metadata[META_RTMP_META][META_FILE_BANDWIDTH] = _streamCapabilities.bandwidthHint;

	return _metadata.SerializeToBinFile(_metaFilePath + ".tmp");
}
