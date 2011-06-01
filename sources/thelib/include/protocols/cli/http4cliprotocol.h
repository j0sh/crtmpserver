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

#ifndef _HTTP4CLIPROTOCOL_H
#define	_HTTP4CLIPROTOCOL_H

#include "protocols/baseprotocol.h"

class HTTP4CLIProtocol
: public BaseProtocol {
private:
	IOBuffer _localInputBuffer;
	IOBuffer _localOutputBuffer;
public:
	HTTP4CLIProtocol();
	virtual ~HTTP4CLIProtocol();
	
	virtual bool Initialize(Variant &parameters);
	virtual bool EnqueueForOutbound();
	virtual IOBuffer * GetOutputBuffer();
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
};


#endif	/* _HTTP4CLIPROTOCOL_H */

