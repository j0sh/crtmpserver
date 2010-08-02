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


#ifdef HAS_PROTOCOL_RTMP
#ifndef _INFILERTMPMP3STREAM_H
#define	_INFILERTMPMP3STREAM_H

#include "protocols/rtmp/streaming/infilertmpstream.h"

class DLLEXP InFileRTMPMP3Stream
: public InFileRTMPStream {
public:
    InFileRTMPMP3Stream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
            string name);
    virtual ~InFileRTMPMP3Stream();
protected:
    virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
            IOBuffer &buffer);
};


#endif	/* _INFILERTMPMP3STREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

