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
#ifndef _NSVDOCUMENTS_H
#define	_NSVDOCUMENTS_H

#include "common.h"
#include "mediaformats/basemediadocument.h"

class NSVDocument
: public BaseMediaDocument {
private:
	IOBuffer _buffer;
	uint32_t _audioSamplesCount;
	uint32_t _videoSamplesCount;
	Variant _metadata;
	vector<MediaFrame> binaryHeaders;

	double _framerate;
	uint32_t aux_plus_vidlen;
	uint8_t num_aux;
	uint16_t audio_len;

public:
	NSVDocument(Variant &metadata);
	virtual ~NSVDocument();

protected:
	virtual bool ParseDocument();
	virtual bool BuildFrames();
	virtual Variant GetRTMPMeta();

private:
	bool ParseNSVs();
	bool ReadPayLoadData();
	double GetFrameRate(uint8_t framerate_index);
	bool IsValidVideoType(uint32_t vidfmt);
	bool IsValidAudioType(uint32_t audfmt);

	void GetAudioFrame();
	void ComputeMediaFrames(uint64_t currentCursor);

	int find_nal_unit(uint8_t* buf, uint32_t size, int* nal_start, int* nal_end);
};


#endif	/* _NSVDOCUMENTS_H */
#endif /* HAS_MEDIA_NSV */

