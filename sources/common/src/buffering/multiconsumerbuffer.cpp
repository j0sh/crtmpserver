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


#include "buffering/multiconsumerbuffer.h"

MultiConsumerBuffer::MultiConsumerBuffer(uint32_t maxBuffer) {
    _maxBuffer = maxBuffer;
    _pBuffer = new uint8_t[_maxBuffer + 1];
    memset(_pBuffer, ' ', _maxBuffer + 1);
    _pBuffer[_maxBuffer] = 0;
    _totalWritten = 0;
    _writeIndex = 0;
    _minReadIndex = 0;
    _maxAmount = 0;
    _maxAmount--;
    FINEST("0x%x", _maxAmount);
}

MultiConsumerBuffer::~MultiConsumerBuffer() {
    delete _pBuffer;
}

MultiConsumerBuffer::operator string() {
    return format("BufferSize: %u; WriteIdx: %u; TotalWr: %llu; MinReadIdx: %llu",
            _maxBuffer,
            _writeIndex,
            _totalWritten,
            _minReadIndex);
}

void MultiConsumerBuffer::PutData(uint8_t *pData, uint32_t length) {
    uint32_t chunk = 0;
    uint32_t copied = 0;
    while (copied != length) {
        chunk = (length - copied)>(_maxBuffer - _writeIndex) ?
                (_maxBuffer - _writeIndex) : (length - copied);
        memcpy(_pBuffer + _writeIndex, pData + copied, chunk);
        copied += chunk;
        _totalWritten += chunk;
        _writeIndex = (_writeIndex + chunk) % _maxBuffer;
        if (_totalWritten > _maxBuffer)
            _minReadIndex = _totalWritten - _maxBuffer;
    }
    //    FINEST("%s", STR(*this));
    //    FINEST("Data: `%s`", _pBuffer);
}

uint64_t MultiConsumerBuffer::GetMinReadIndex() {
    return _minReadIndex;
}

void MultiConsumerBuffer::Ignore(uint64_t &startIndex, uint32_t length) {

}

