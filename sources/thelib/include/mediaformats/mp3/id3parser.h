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
#ifndef _ID3PARSER_H
#define	_ID3PARSER_H

#include "common.h"
#include "mediaformats/mediafile.h"

class ID3Parser {
protected:
	bool _unsynchronisation;
	bool _compression;
	bool _extendedHeader;
	bool _experimentalIndicator;
	bool _footerPresent;
	Variant _metadata;
	uint32_t _majorVersion;
	uint32_t _minorVersion;
public:
	ID3Parser(uint32_t majorVersion, uint32_t minorVersion);
	virtual ~ID3Parser();

	Variant GetMetadata();
	bool Parse(MediaFile &file);
private:
	bool ParseTags(IOBuffer &buffer);
	bool ReadStringWithSize(IOBuffer &buffer, Variant &value, uint32_t size, bool hasEncoding);
	bool ReadStringNullTerminated(IOBuffer &buffer, Variant &value, bool unicode);
	bool ParseTextTag(IOBuffer &buffer, Variant &tag);
	bool ParseUSLT(IOBuffer &buffer, Variant &tag);
	bool ParseAPIC(IOBuffer &buffer, Variant &tag);
	bool ParseCOMM(IOBuffer &buffer, Variant &tag);
	bool ParseUrlTag(IOBuffer &buffer, Variant &tag);
	bool ParseWXXX(IOBuffer &buffer, Variant &tag);
	bool ParseTXXX(IOBuffer &buffer, Variant &tag);
};


#endif	/* _ID3PARSER_H */
#endif /* HAS_MEDIA_MP3 */
