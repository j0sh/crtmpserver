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


#ifndef _IOBUFFER_H
#define _IOBUFFER_H

#include "utils/core.h"
#include "utils/file.h"
#include "utils/mmapfile.h"
#include "utils/crypto.h"

#define GETAVAILABLEBYTESCOUNT(x)     ((x)._published - (x)._consumed)
#define GETIBPOINTER(x)     ((uint8_t *)((x)._pBuffer + (x)._consumed))

class DLLEXP IOBuffer {
public:
    uint8_t *_pBuffer;
    uint32_t _size;
    uint32_t _published;
    uint32_t _consumed;
    uint32_t _minChunkSize;
    socklen_t _dummy;
public:
    //constructor/destructor and initialization
    IOBuffer();
    virtual ~IOBuffer();
    void Initialize(uint32_t expected);

    //Read from a source and put to this buffer
    bool ReadFromPipe(int32_t fd, uint32_t expected, int32_t &recvAmount);
    bool ReadFromTCPFd(int32_t fd, uint32_t expected, int32_t &recvAmount);
    bool ReadFromUDPFd(int32_t fd, int32_t &recvAmount, sockaddr_in &peerAddress);
    bool ReadFromStdio(int32_t fd, uint32_t expected, int32_t &recvAmount);
    bool ReadFromFs(fstream &fs, uint32_t size);
    bool ReadFromFs(File &file, uint32_t size);
#ifdef HAS_MMAP
    bool ReadFromFs(MmapFile &file, uint32_t size);
#endif /* HAS_MMAP */
    bool ReadFromBuffer(const uint8_t *pBuffer, const uint32_t size);
    void ReadFromInputBuffer(IOBuffer *pInputBuffer, uint32_t start, uint32_t size);
    bool ReadFromInputBuffer(const IOBuffer &buffer, uint32_t size);
    bool ReadFromString(string binary);
    void ReadFromByte(uint8_t byte);
    bool ReadFromBIO(BIO *pBIO);
    void ReadFromRepeat(uint8_t byte, uint32_t size);

    //Read from this buffer and put to a destination
    bool WriteToTCPFd(int32_t fd, uint32_t size);
    bool WriteToStdio(int32_t fd, uint32_t size);

    //Utility functions
    uint32_t GetMinChunkSize();
    void SetMinChunkSize(uint32_t minChunkSize);
    uint32_t GetCurrentWritePosition();
    uint8_t *GetPointer();

    //memory management
    bool Ignore(uint32_t size);
    bool IgnoreAll();
    bool MoveData();
    bool EnsureSize(uint32_t expected);

    //pretty print
    string ToString(uint32_t startIndex = 0, uint32_t limit = 0);
    operator string();

protected:
    void Cleanup();
    void Recycle();
};
#endif //_IOBUFFER_H

