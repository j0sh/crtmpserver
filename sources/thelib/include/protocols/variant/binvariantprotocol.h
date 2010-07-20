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


#ifdef HAS_PROTOCOL_VAR
#ifndef _BINVARIANTPROTOCOL_H
#define	_BINVARIANTPROTOCOL_H

#include "protocols/variant/basevariantprotocol.h"

class DLLEXP BinVariantProtocol
: public BaseVariantProtocol {
public:
	BinVariantProtocol();
	virtual ~BinVariantProtocol();
protected:
	virtual bool Serialize(string &rawData, Variant &variant);
	virtual bool Deserialize(uint8_t *pBuffer, uint32_t bufferLength,
			Variant &result);
};


#endif	/* _BINVARIANTPROTOCOL_H */
#endif	/* HAS_PROTOCOL_VAR */

