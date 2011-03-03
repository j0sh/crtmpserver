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
#ifndef _MP3MEDIATYPE_H
#define	_MP3MEDIATYPE_H

#include "common.h"
#include "mediaformats/mediaframe.h"

class MP3Media {
private:
	//ver/layer/bitRateIndex
	static int32_t _bitRates[4][4][16];
	static int32_t _samplingRates[4][4];
	static string _versionNames[4];
	static string _layerNames[4];
	static map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, uint64_t > > > > > _frameSizes;
	Variant _tags;

public:
	MP3Media();
	virtual ~MP3Media();
	bool GetAudioFrames(uint8_t *pBuffer, uint32_t &audioSamplesCount, MediaFrame &mediaFrame);

private:
	void InitFrameSizes();
};

#endif	/* _MP3MEDIATYPE_H */
#endif /* HAS_MEDIA_NSV*/
