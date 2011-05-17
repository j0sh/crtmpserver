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

#ifndef _BASEMEDIADOCUMENT_H
#define	_BASEMEDIADOCUMENT_H


#include "common.h"
#include "mediaformats/mediaframe.h"
#include "mediaformats/mediafile.h"
#include "streaming/streamcapabilities.h"

class BaseMediaDocument {
protected:
	MediaFile _mediaFile;
	vector<MediaFrame> _frames;
	uint32_t _audioSamplesCount;
	uint32_t _videoSamplesCount;
	Variant _metadata;
	string _mediaFilePath;
	string _seekFilePath;
	string _metaFilePath;
	bool _keyframeSeek;
	uint32_t _seekGranularity;
	StreamCapabilities _streamCapabilities;
public:
	BaseMediaDocument(Variant &metadata);
	virtual ~BaseMediaDocument();

	/*!
		@brief This functions do things like opening the media file, building frames, saving the meta, etc.
	*/
	bool Process();

	/*!
		@brief Returns the meta data
	*/
	Variant GetMetadata();

	/*!
		@brief Returns the media file
	*/
	MediaFile &GetMediaFile();
protected:
	static bool CompareFrames(const MediaFrame &frame1, const MediaFrame &frame2);
	virtual bool ParseDocument() = 0;
	virtual bool BuildFrames() = 0;
	virtual Variant GetRTMPMeta() = 0;
private:
	bool SaveSeekFile();
	bool SaveMetaFile();
};


#endif	/* _BASEMEDIADOCUMENT_H */

