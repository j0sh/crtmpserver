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


#ifndef _BITARRAY_H
#define	_BITARRAY_H

#include "new/utils/buffering/iobuffer.h"

class BitArray
: public IOBuffer {
private:
	uint32_t _cursor;
public:

	BitArray() : IOBuffer() {
		_cursor = 0;
	}

	virtual ~BitArray() {

	}
public:

	template<typename T>
	bool PutBits(T data, uint8_t count) {
		//WARN("data: %x; count: %d", data, count);
		if (!EnsureSize(sizeof (T)))
			return false;

		uint8_t *pBuffer = GETIBPOINTER(*this);

		for (uint8_t i = 0; i < count; i++) {
			//FINEST("sizeof (T): %d; count: %d; i: %d", sizeof (T)*8, count, i);
			uint8_t srcBitIndex = sizeof (T)*8 - count + i;
			uint8_t dstByteIndex = (uint8_t) ((_cursor + i) / 8);
			uint8_t dstBitIndex = (_cursor + i) % 8;
			//FINEST("srcBitIndex: %d; dstByteIndex: %d; dstBitIndex: %d",srcBitIndex,dstByteIndex,dstBitIndex);
			if (dstBitIndex == 0) {
				//INFO("New byte. reset it");
				pBuffer[dstByteIndex] = 0;
			}
			T flag = (data << srcBitIndex);
			flag = (flag >> (sizeof (T)*8 - 1));
			//FINEST("flag: %d", flag);

			//            FINEST("(data << srcBitIndex): %x", ((T)data << srcBitIndex));
			//            FINEST("(sizeof (T)*8 - 1): %d",(sizeof (T)*8 - 1));
			//            FINEST("All: %d", ((data << srcBitIndex) >> (sizeof (T)*8 - 1)));
			pBuffer[dstByteIndex] |= (flag << (7 - dstBitIndex));
			//            FINEST("srcBitIndex: %d; dstByteIndex: %d; dstBitIndex: %d; pBuffer[dstByteIndex]: %02x",
			//                    srcBitIndex, dstByteIndex, dstBitIndex, pBuffer[dstByteIndex]);
		}
		_cursor += count;
		_published = (_cursor / 8)+(((_cursor % 8) == 0) ? 0 : 1);
		//WARN("data: %x; count: %d; Res: %s", data, count, STR(*this));
		return true;
	}

	template<typename T>
	T ReadBits(uint8_t count) {
		T result = PeekBits<T > (count);
		IgnoreBits(count);
		return result;
	}

	template<typename T>
	T PeekBits(uint8_t count) {
		T result = 0;

		if (GETAVAILABLEBYTESCOUNT(*this) == 0) {
			assert(false);
		}

		if (GETAVAILABLEBYTESCOUNT(*this) < (_cursor + count) / 8) {
			assert(false);
		}

		if (count>sizeof (T)*8) {
			assert(false);
		}

		for (uint8_t i = 0; i < count; i++) {
			uint32_t currentCursor = _cursor + i;
			uint8_t currentByteIndex = (uint8_t) (currentCursor / 8);
			uint8_t currentBitIndex = currentCursor % 8;
			uint8_t currentByte = GETIBPOINTER(*this)[currentByteIndex];
			//            FINEST("_cursor: %d; currentCursor: %d; currentByteIndex: %d; currentBitIndex: %d; currentByte: %02x",
			//                    _cursor, currentCursor, currentByteIndex, currentBitIndex, currentByte);


			result = (result << 1) | ((currentByte >> (7 - currentBitIndex))&0x01);
		}


		return result;
	}

	void IgnoreBits(uint32_t count) {
		if (GETAVAILABLEBYTESCOUNT(*this) == 0) {
			assert(false);
		}

		if (GETAVAILABLEBYTESCOUNT(*this) < (_cursor + count) / 8) {
			assert(false);
		}

		_cursor += count;
		//        FINEST("_cursor: %d", _cursor);
	}

	uint32_t AvailableBits() {
		return GETAVAILABLEBYTESCOUNT(*this)*8 - _cursor;
	}

	uint64_t ReadExpGolomb() {
		uint32_t zeroBitsCount = 0;
		while (!ReadBits<bool>(1))
			zeroBitsCount++;
		uint32_t value = 1;
		for (uint32_t i = 0; i < zeroBitsCount; i++) {
			value = ((value << 1) | (ReadBits<uint8_t > (1)));
		}
		return value - 1;
	}
};


#endif	/* _BITARRAY_H */


