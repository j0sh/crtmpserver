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

#ifdef HAS_MEDIA_FLV
#ifndef _FLVDOCUMENT
#define _FLVDOCUMENT

#include "common.h"
#include "mediaformats/basemediadocument.h"

class FLVDocument
: public BaseMediaDocument {
private:
	uint32_t _audioSamplesCount;
	uint32_t _videoSamplesCount;
	Variant _metadata;
public:
	FLVDocument(Variant &metadata);
	virtual ~FLVDocument();

protected:
	virtual bool ParseDocument();
	virtual bool BuildFrames();
	virtual Variant GetRTMPMeta();
};

#endif /* _FLVDOCUMENT */
#endif /* HAS_MEDIA_FLV */
