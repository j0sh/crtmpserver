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

#ifdef HAS_MEDIA_MP3
#ifndef _MP3DOCUMENT_H
#define	_MP3DOCUMENT_H

#include "mediaformats/basemediadocument.h"

class MP3Document
: public BaseMediaDocument {
private:
	//ver/layer/bitRateIndex
	static int32_t _bitRates[4][4][16];
	static int32_t _samplingRates[4][4];
	static string _versionNames[4];
	static string _layerNames[4];
	static map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, uint64_t > > > > > _frameSizes;
	Variant _tags;
public:
	MP3Document(Variant &metadata);
	virtual ~MP3Document();
protected:
	virtual bool ParseDocument();
	virtual bool BuildFrames();
	virtual Variant GetRTMPMeta();
private:
	bool FindFrameData();
	bool ParseMetadata();
	void InitFrameSizes();
};


#endif /* _MP3DOCUMENT_H */
#endif /* HAS_MEDIA_MP3 */
