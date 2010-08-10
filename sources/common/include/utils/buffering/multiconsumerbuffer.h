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

#if 0 //NOT YET IMPLEMENTED
#ifndef _MULTICONSUMERBUFFER_H
#define	_MULTICONSUMERBUFFER_H

#include "buffering/iobuffer.h"

class MultiConsumerBuffer {
private:
	uint32_t _maxBuffer;
	uint8_t *_pBuffer;
	uint64_t _totalWritten;
	uint32_t _writeIndex;
	uint64_t _minReadIndex;
	uint64_t _maxAmount;
public:
	MultiConsumerBuffer(uint32_t maxBuffer);
	virtual ~MultiConsumerBuffer();
	operator string();

	void PutData(uint8_t *pData, uint32_t length);
	uint64_t GetMinReadIndex();
	void Ignore(uint64_t &startIndex, uint32_t length);


};

#endif	/* _MULTICONSUMERBUFFER_H */
#endif


