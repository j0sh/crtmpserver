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
#include "protocols/rtmp/streaming/infilertmpmp3stream.h"
#include "streaming/streamstypes.h"

InFileRTMPMP3Stream::InFileRTMPMP3Stream(BaseProtocol *pProtocol,
        StreamsManager *pStreamsManager, string name)
: InFileRTMPStream(pProtocol, pStreamsManager, ST_IN_FILE_RTMP_MP3, name) {

}

InFileRTMPMP3Stream::~InFileRTMPMP3Stream() {
}

bool InFileRTMPMP3Stream::BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
        IOBuffer &buffer) {
    //1. Seek into the data file at the correct position
    if (!pFile->SeekTo(mediaFrame.start)) {
        FATAL("Unable to seek to position %llu", mediaFrame.start);
        return false;
    }

    //2. Put the mp3 marker
    buffer.ReadFromRepeat(0x2f, 1);

    //3. Read the data
    if (!buffer.ReadFromFs(*pFile, mediaFrame.length)) {
        FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
        return false;
    }

    //3. Done
    return true;
}
#endif /* HAS_PROTOCOL_RTMP */

