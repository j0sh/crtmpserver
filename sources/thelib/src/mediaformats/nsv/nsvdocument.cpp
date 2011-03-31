/*
 *  Copyright (c) 2010,
 *  EvoStream (contact@evostream.com)
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

#ifdef HAS_MEDIA_NSV
#include "mediaformats/nsv/nsvdocument.h"
#include "protocols/rtmp/amf0serializer.h"
#include "streaming/nalutypes.h"
#include "mediaformats/nsv/mp3media.h"

NSVDocument::NSVDocument(Variant &metadata)
: BaseMediaDocument(metadata) {
	_framerate = 0;
	binaryHeaders.empty();
}

NSVDocument::~NSVDocument() {

}

bool NSVDocument::ParseDocument() {
	return true;
}

bool NSVDocument::BuildFrames() {

	//1. Go to the beginning of the file
	if (!_mediaFile.SeekBegin()) {
		FATAL("Unable to seek in file");
		return false;
	}

	// 2. Read for bytes if header is available
	// 3. If NSVf(file header) is available then read header size
	//    and jump ahead to stream data
	//    else if not available check if already a NSVs(frame header)

	_audioSamplesCount = 0;
	_videoSamplesCount = 0;
	while (_mediaFile.Cursor() != _mediaFile.Size()) {
		uint16_t marker_frame = 0;
		if (!_mediaFile.PeekUI16(&marker_frame)) {
			FATAL("Unable to peek byte");
			return false;
		}

		// NS
		if ((marker_frame != 0x4e53) && (marker_frame != 0xefbe)) {
			if (!_mediaFile.SeekAhead(1)) {
				FATAL("Unable to seek ahead ");
				return false;
			}
			continue;
		}
		if (!_mediaFile.SeekAhead(2)) {
			FATAL("Unable to seek ahead ");
			return false;
		}

		if (marker_frame == 0x4e53) {
			if (!_mediaFile.PeekUI16(&marker_frame)) {
				FATAL("Unable to peek byte");
				return false;
			}

			if (marker_frame != 0x5673) {
				if (!_mediaFile.SeekAhead(1)) {
					FATAL("Unable to seek ahead ");
					return false;
				}
				continue;
			}
			if (!_mediaFile.SeekAhead(2)) {
				FATAL("Unable to seek ahead ");
				return false;
			}
			if (!ParseNSVs()) {
				FATAL("Unable to parse NSVs");
				return false;
			}
		}
		ReadPayLoadData();
	}

	sort(_frames.begin(), _frames.end(), CompareFrames);

	//15. Add the binary headers
	for (uint32_t i = 0; i < binaryHeaders.size(); i++) {
		assert(binaryHeaders[i].absoluteTime == 0);
		ADD_VECTOR_BEGIN(_frames, binaryHeaders[i]);
	}

	return true;
}

bool NSVDocument::ParseNSVs() {
	//6. get video format
	uint32_t vidfmt = 0;
	if (!_mediaFile.ReadUI32(&vidfmt)) {
		FATAL("Unable to read video format ");
		return false;
	}
	// check for video format
	if (!IsValidVideoType(vidfmt)) {
		FATAL("video format not soppurted...");
		return false;
	}

	//7. get audio format
	uint32_t audfmt = 0;
	if (!_mediaFile.ReadUI32(&audfmt)) {
		FATAL("Unable to read audio format ");
		return false;
	}
	// check for audio format
	if (!IsValidAudioType(audfmt)) {
		FATAL("Unable to read audio format ");
		return false;
	}

	//8. get video width
	uint16_t vid_width = 0;
	if (!_mediaFile.ReadUI16(&vid_width, false)) {
		FATAL("Unable to read video width ");
		return false;
	}

	//9  get video height
	uint16_t vid_height = 0;
	if (!_mediaFile.ReadUI16(&vid_height, false)) {
		FATAL("Unable to read video height ");
		return false;
	}

	// get framerate_idx and check from
	// frame rate table
	uint8_t framerate_idx = 0;
	if (!_mediaFile.ReadUI8(&framerate_idx)) {
		FATAL("Unable to read framerate idx ");
		return false;
	}
	_framerate = GetFrameRate(framerate_idx);

	//10 get syncoffs
	uint16_t syncoffs = 0;
	if (!_mediaFile.ReadUI16(&syncoffs, false)) {
		FATAL("Unable to read syncoffs ");
		return false;
	}

	return true;
}

Variant NSVDocument::GetRTMPMeta() {
	return _metadata;
}

bool NSVDocument::ReadPayLoadData() {

	//13. get the aux plus video len
	if (!_mediaFile.ReadUI24(&aux_plus_vidlen, false)) {
		FATAL("Unable to read aux_plus vid len data chuncks ");
		return false;
	}

	num_aux = aux_plus_vidlen & 0x0f;
	aux_plus_vidlen = aux_plus_vidlen >> 4;

	//14. audio len
	if (!_mediaFile.ReadUI16(&audio_len, false)) {
		FATAL("Unable to read audio len ");
		return false;
	}

	uint32_t auxLength = 0;
	if (num_aux > 0) {
		NYIA;
	}

	if (aux_plus_vidlen > 0) {
		MediaFrame video_frame = {0, 0, 0, 0, 0, 0, 0, 0};
		video_frame.type = MEDIAFRAME_TYPE_VIDEO;
		if (aux_plus_vidlen > 5) {
			uint64_t nt;
			if (!_mediaFile.PeekUI64(&nt)) {
				FATAL("Unable to peek  video data ");
				return false;
			}
			nt = (nt >> 24)&0xff;
			// 00 00 00 01 06 05 ff d5 followed by payload
			//             06 - SEI NAL unit
			if (NALU_TYPE_SEI == NALU_TYPE(nt)) {
				// get binary header from payload data
				FINEST("video data offset %"PRIx64, _mediaFile.Cursor());
				uint64_t currentCursor = _mediaFile.Cursor();
				_buffer.IgnoreAll();
				_buffer.ReadFromFs(_mediaFile, aux_plus_vidlen);
				ComputeMediaFrames(currentCursor);
			} else {
				if (NALU_TYPE_SPS == NALU_TYPE(nt)) {
					//skip this video we already have SPS..
					if (!_mediaFile.SeekAhead(aux_plus_vidlen - auxLength)) {
						FATAL("Unable to seek ahead video data ");
						return false;
					}
				} else {
					// get video frame
					video_frame.start = _mediaFile.Cursor() + 4;
					video_frame.length = aux_plus_vidlen - auxLength - 4;
					video_frame.deltaTime = 0;
					video_frame.absoluteTime = (double) _videoSamplesCount / _framerate * 1000.0;
					video_frame.compositionOffset = 0;
					video_frame.isBinaryHeader = false;
					video_frame.isKeyFrame = false;
					_videoSamplesCount++;
					// video
					if (!_mediaFile.SeekAhead(aux_plus_vidlen - auxLength)) {
						FATAL("Unable to seek ahead video data ");
						return false;
					}
					if (video_frame.isBinaryHeader) {
						video_frame.absoluteTime = 0;
						ADD_VECTOR_END(binaryHeaders, video_frame);
					} else {
						ADD_VECTOR_END(_frames, video_frame);
					}
				}
			}
		}
	}

	GetAudioFrame();

	return true;
}

double NSVDocument::GetFrameRate(uint8_t framerate_index) {
	static double frate_table[] = {
		30.0,
		30.0 * 1000.0 / 1001.0,
		25.0,
		24.0 * 1000.0 / 1001.0,
	};
	if (!(framerate_index & 0x80)) {
		return (double) framerate_index;
	}
	double sc;
	int d = (framerate_index & 0x7f) >> 2;
	if (d < 16) sc = 1.0 / (double) (d + 1);
	else sc = d - 15;

	return frate_table[framerate_index & 3] * sc;
}

bool NSVDocument::IsValidVideoType(uint32_t vidfmt) {
	// H264
	if (vidfmt == 0x48323634) {
		return true;
	}
	FINEST("video format not supported yet...%x", vidfmt);
	return false;
}

bool NSVDocument::IsValidAudioType(uint32_t audfmt) {
	//             MP3                     AAC
	if (audfmt == 0x4d503320) { // || (audfmt == 0x00414143)) {
		return true;
	}
	FINEST("audio format not supported yet...%x", audfmt);
	return false;
}

void NSVDocument::GetAudioFrame() {

	uint8_t firstBytes[4];
	//1. Read the first 4 bytes
	if (!_mediaFile.PeekBuffer(firstBytes, 4)) {
		FATAL("Unable to read 4 bytes");
		return;
	}

	if ((firstBytes[0] != 0xff) || ((firstBytes[1] >> 5) != 7)) {
		// not an mp3 header skip..
		if (!_mediaFile.SeekAhead(audio_len)) {
			FATAL("Unable to seek ahead video data ");
			return;
		}
		return;
	}

	MediaFrame audio_frame = {0, 0, 0, 0, 0, 0, 0, 0};
	audio_frame.start = _mediaFile.Cursor();
	_buffer.IgnoreAll();
	_buffer.ReadFromFs(_mediaFile, audio_len);
	uint8_t *pBuffer = GETIBPOINTER(_buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(_buffer);
	while (length != 0) {
		MP3Media mp3;
		mp3.GetAudioFrames(pBuffer, _audioSamplesCount, audio_frame);
		audio_frame.compositionOffset = 0;
		ADD_VECTOR_END(_frames, audio_frame);
		// to check if there are more audio frame to follow
		pBuffer += audio_frame.length;
		audio_frame.start += audio_frame.length;
		length -= audio_frame.length;
	}
}

void NSVDocument::ComputeMediaFrames(uint64_t currentCursor) {
	uint8_t *pBuffer = GETIBPOINTER(_buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(_buffer);
	int nal_start = 0;
	int nal_end = 0;
	uint8_t nal_type = 0;
	MediaFrame video_frame = {0, 0, 0, 0, 0, 0, 0, 0};
	while (length != 0) {
		find_nal_unit(pBuffer, length, &nal_start, &nal_end);
		nal_type = pBuffer[nal_start];
		if (NALU_TYPE_SEI != NALU_TYPE(nal_type)) {
			video_frame.start = nal_start + currentCursor;
			video_frame.length = nal_end - nal_start;
			video_frame.type = MEDIAFRAME_TYPE_VIDEO;
			video_frame.deltaTime = 0;
			video_frame.absoluteTime = _videoSamplesCount / _framerate;
			video_frame.compositionOffset = 0;
			_videoSamplesCount++;
			switch (NALU_TYPE(nal_type)) {
				case NALU_TYPE_SPS:
					video_frame.isBinaryHeader = true;
					video_frame.isKeyFrame = false;
					break;

				case NALU_TYPE_PPS:
					video_frame.isBinaryHeader = true;
					video_frame.isKeyFrame = false;
					break;

				case NALU_TYPE_IDR:
					video_frame.isBinaryHeader = false;
					video_frame.isKeyFrame = true;
					break;
			}
			if (video_frame.isBinaryHeader) {
				video_frame.absoluteTime = 0;
				ADD_VECTOR_END(binaryHeaders, video_frame);
			} else {
				ADD_VECTOR_END(_frames, video_frame);
			}
		}
		pBuffer += nal_end;
		length -= nal_end;
		currentCursor += nal_end;
	}
}

int NSVDocument::find_nal_unit(uint8_t* buf, uint32_t size, int* nal_start, int* nal_end) {
	uint32_t i = 0;
	// find start
	*nal_start = 0;
	*nal_end = 0;

	while (//( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
			(buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) &&
			(buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01)
			) {
		i++; // skip leading zero
		if (i + 4 >= size) {
			return 0;
		} // did not find nal start
	}

	if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) // ( next_bits( 24 ) != 0x000001 )
	{
		i++;
	}

	if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01) {
		/* error, should never happen */ return 0;
	}
	i += 3;
	*nal_start = i;

	while (//( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
			(buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0) &&
			(buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01)
			) {
		i++;
		// FIXME the next line fails when reading a nal that ends exactly at the end of the data
		if (i + 3 >= size) {
			*nal_end = size;
			return -1;
		} // did not find nal end, stream ended first
	}

	*nal_end = i;
	return (*nal_end - *nal_start);
}

#endif /*HAS_MEDIA_NSV*/
