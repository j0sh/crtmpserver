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


#include "utils/buffering/iobuffer.h"
#include "utils/logging/logging.h"

#ifdef WITH_SANITY_INPUT_BUFFER
#define SANITY_INPUT_BUFFER \
assert(_consumed<=_published); \
assert(_published<=_size);
#define SANITY_INPUT_BUFFER_INDEX \
assert(index >= 0); \
assert((_published - _consumed - index) > 0);
#else
#define SANITY_INPUT_BUFFER
#define SANITY_INPUT_BUFFER_INDEX
#endif

IOBuffer::IOBuffer() {
	_pBuffer = NULL;
	_size = 0;
	_published = 0;
	_consumed = 0;
	_minChunkSize = 4096;
	_dummy = sizeof (sockaddr_in);
	SANITY_INPUT_BUFFER;
}

IOBuffer::~IOBuffer() {
	SANITY_INPUT_BUFFER;
	Cleanup();
	SANITY_INPUT_BUFFER;
}

void IOBuffer::Initialize(uint32_t expected) {
	if ((_pBuffer != NULL) ||
			(_size != 0) ||
			(_published != 0) ||
			(_consumed != 0))
		ASSERT("This buffer was used before. Please initialize it before using");
	EnsureSize(expected);
}

bool IOBuffer::ReadFromPipe(int32_t fd, uint32_t expected, int32_t &recvAmount) {
	//TODO: This is an UGLY hack.
#ifndef WIN32
	SANITY_INPUT_BUFFER;
	if (_published + expected > _size) {
		if (!EnsureSize(expected)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}
	recvAmount = read(fd, (char *) (_pBuffer + _published), expected);
	if (recvAmount > 0) {
		_published += (uint32_t) recvAmount;
		SANITY_INPUT_BUFFER;
		return true;
	} else {
		int32_t err = errno;
		if (err != EINPROGRESS) {
			FATAL("Unable to read from pipe: %d %s", err, strerror(err));
			SANITY_INPUT_BUFFER;
			return false;
		}
		SANITY_INPUT_BUFFER;
		return true;
	}
#else
	NYIA;
	return false;
#endif
}

bool IOBuffer::ReadFromTCPFd(int32_t fd, uint32_t expected, int32_t &recvAmount) {
	SANITY_INPUT_BUFFER;
	if (_published + expected > _size) {
		if (!EnsureSize(expected)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}

	recvAmount = recv(fd, (char *) (_pBuffer + _published), expected, MSG_NOSIGNAL);
	if (recvAmount > 0) {
		_published += (uint32_t) recvAmount;
		SANITY_INPUT_BUFFER;
		return true;
	} else {
		SANITY_INPUT_BUFFER;
		return false;
	}
}

bool IOBuffer::ReadFromUDPFd(int32_t fd, int32_t &recvAmount, sockaddr_in &peerAddress) {
	SANITY_INPUT_BUFFER;
	if (_published + 65536 > _size) {
		if (!EnsureSize(65536)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}

	recvAmount = recvfrom(fd, (char *) (_pBuffer + _published), 65536,
			MSG_NOSIGNAL, (sockaddr *) & peerAddress, &_dummy);
	if (recvAmount > 0) {
		_published += (uint32_t) recvAmount;
		SANITY_INPUT_BUFFER;
		return true;
	} else {
		SANITY_INPUT_BUFFER;
		return false;
	}
}

bool IOBuffer::ReadFromStdio(int32_t fd, uint32_t expected, int32_t &recvAmount) {
	SANITY_INPUT_BUFFER;
	if (_published + expected > _size) {
		if (!EnsureSize(expected)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}

	recvAmount = READ_FD(fd, (char *) (_pBuffer + _published), expected);
	if (recvAmount > 0) {
		_published += (uint32_t) recvAmount;
		SANITY_INPUT_BUFFER;
		return true;
	} else {
		SANITY_INPUT_BUFFER;
		return false;
	}
}

bool IOBuffer::ReadFromFs(fstream &fs, uint32_t size) {
	SANITY_INPUT_BUFFER;
	if (_published + size > _size) {
		if (!EnsureSize(size)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}
	fs.read((char *) _pBuffer + _published, size);
	if (fs.fail()) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	_published += size;
	SANITY_INPUT_BUFFER;
	return true;
}

bool IOBuffer::ReadFromFs(File &file, uint32_t size) {
	SANITY_INPUT_BUFFER;
	if (size == 0) {
		SANITY_INPUT_BUFFER;
		return true;
	}
	if (_published + size > _size) {
		if (!EnsureSize(size)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}
	if (!file.ReadBuffer(_pBuffer + _published, size)) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	_published += size;
	SANITY_INPUT_BUFFER;
	return true;
}

#ifdef HAS_MMAP

bool IOBuffer::ReadFromFs(MmapFile &file, uint32_t size) {
	SANITY_INPUT_BUFFER;
	if (size == 0) {
		SANITY_INPUT_BUFFER;
		return true;
	}
	if (_published + size > _size) {
		if (!EnsureSize(size)) {
			SANITY_INPUT_BUFFER;
			return false;
		}
	}
	if (!file.ReadBuffer(_pBuffer + _published, size)) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	_published += size;
	SANITY_INPUT_BUFFER;
	return true;
}
#endif /* HAS_MMAP */

bool IOBuffer::ReadFromBuffer(const uint8_t *pBuffer, const uint32_t size) {
	SANITY_INPUT_BUFFER;
	if (!EnsureSize(size)) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	memcpy(_pBuffer + _published, pBuffer, size);
	_published += size;
	SANITY_INPUT_BUFFER;
	return true;
}

void IOBuffer::ReadFromInputBuffer(IOBuffer *pInputBuffer, uint32_t start, uint32_t size) {
	SANITY_INPUT_BUFFER;
	ReadFromBuffer(GETIBPOINTER(*pInputBuffer) + start, size);
	SANITY_INPUT_BUFFER;
}

bool IOBuffer::ReadFromInputBuffer(const IOBuffer &buffer, uint32_t size) {
	SANITY_INPUT_BUFFER;
	if (!ReadFromBuffer(buffer._pBuffer + buffer._consumed, size)) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	SANITY_INPUT_BUFFER;
	return true;
}

bool IOBuffer::ReadFromString(string binary) {
	SANITY_INPUT_BUFFER;
	if (!ReadFromBuffer((uint8_t *) binary.c_str(), (uint32_t) binary.length())) {
		SANITY_INPUT_BUFFER;
		return false;
	}
	SANITY_INPUT_BUFFER;
	return true;
}

void IOBuffer::ReadFromByte(uint8_t byte) {
	SANITY_INPUT_BUFFER;
	EnsureSize(1);
	_pBuffer[_published] = byte;
	_published++;
	SANITY_INPUT_BUFFER;
}

bool IOBuffer::ReadFromBIO(BIO *pBIO) {
	SANITY_INPUT_BUFFER;
	if (pBIO == NULL) {
		SANITY_INPUT_BUFFER;
		return true;
	}
	int32_t bioAvailable = BIO_pending(pBIO);
	if (bioAvailable < 0) {
		FATAL("BIO_pending failed");
		SANITY_INPUT_BUFFER;
		return false;
	}
	if (bioAvailable == 0) {
		SANITY_INPUT_BUFFER;
		return true;
	}
	EnsureSize((uint32_t) bioAvailable);
	int32_t written = BIO_read(pBIO, _pBuffer + _published, bioAvailable);
	_published += written;
	SANITY_INPUT_BUFFER;
	return true;
}

void IOBuffer::ReadFromRepeat(uint8_t byte, uint32_t size) {
	SANITY_INPUT_BUFFER;
	EnsureSize(size);
	memset(_pBuffer + _published, byte, size);
	_published += size;
	SANITY_INPUT_BUFFER;
}

bool IOBuffer::WriteToTCPFd(int32_t fd, uint32_t size, int32_t &sentAmount) {
	SANITY_INPUT_BUFFER;
	bool result = true;
	sentAmount = send(fd, (char *) (_pBuffer + _consumed),
			//_published - _consumed,
			size > _published - _consumed ? _published - _consumed : size,
			MSG_NOSIGNAL);
	int err = LASTSOCKETERROR;

	if (sentAmount < 0) {
		if (err != SOCKERROR_SEND_IN_PROGRESS) {
			FATAL("Unable to send %u bytes of data data. Size advertised by network layer was %u [%d: %s]",
					_published - _consumed, size, err, strerror(err));
			FATAL("Permanent error!");
			result = false;
		}
	} else {
		_consumed += sentAmount;
	}
	if (result)
		Recycle();
	SANITY_INPUT_BUFFER;

	return result;
}

bool IOBuffer::WriteToStdio(int32_t fd, uint32_t size) {
	SANITY_INPUT_BUFFER;
	bool result = true;
	int32_t sent = WRITE_FD(fd, (char *) (_pBuffer + _consumed),
			_published - _consumed);
	//size > _published - _consumed ? _published - _consumed : size,
	int err = LASTSOCKETERROR;

	if (sent < 0) {
		FATAL("Unable to send %u bytes of data data. Size advertised by network layer was %u [%d: %s]",
				_published - _consumed, size, err, strerror(err));
		FATAL("Permanent error!");
		result = false;
	} else {
		_consumed += sent;
	}
	if (result)
		Recycle();
	SANITY_INPUT_BUFFER;

	return result;
}

uint32_t IOBuffer::GetMinChunkSize() {

	return _minChunkSize;
}

void IOBuffer::SetMinChunkSize(uint32_t minChunkSize) {

	assert(minChunkSize > 0 && minChunkSize < 16 * 1024 * 1024);
	_minChunkSize = minChunkSize;
}

uint32_t IOBuffer::GetCurrentWritePosition() {

	return _published;
}

uint8_t *IOBuffer::GetPointer() {

	return _pBuffer;
}

bool IOBuffer::Ignore(uint32_t size) {
	SANITY_INPUT_BUFFER;
	_consumed += size;
	Recycle();
	SANITY_INPUT_BUFFER;

	return true;
}

bool IOBuffer::IgnoreAll() {
	SANITY_INPUT_BUFFER;
	_consumed = _published;
	Recycle();
	SANITY_INPUT_BUFFER;

	return true;
}

bool IOBuffer::MoveData() {
	SANITY_INPUT_BUFFER;
	if (_published - _consumed <= _consumed) {
		memcpy(_pBuffer, _pBuffer + _consumed, _published - _consumed);
		_published = _published - _consumed;
		_consumed = 0;
	}
	SANITY_INPUT_BUFFER;

	return true;
}

bool IOBuffer::EnsureSize(uint32_t expected) {
	SANITY_INPUT_BUFFER;
	//1. Do we have enough space?
	if (_published + expected <= _size) {
		SANITY_INPUT_BUFFER;
		return true;
	}

	//2. Apparently we don't! Try to move some data
	MoveData();

	//3. Again, do we have enough space?
	if (_published + expected <= _size) {
		SANITY_INPUT_BUFFER;
		return true;
	}

	//4. Nope! So, let's get busy with making a brand new buffer...
	//First, we allocate at least 1/3 of what we have and no less than _minChunkSize
	if ((_published + expected - _size)<(_size / 3)) {
		expected = _size + _size / 3 - _published;
	}

	if (expected < _minChunkSize) {
		expected = _minChunkSize;
	}

	//5. Allocate
	uint8_t *pTempBuffer = new uint8_t[_published + expected];

	//6. Copy existing data if necessary and switch buffers
	if (_pBuffer != NULL) {
		memcpy(pTempBuffer, _pBuffer, _published);
		delete[] _pBuffer;
	}
	_pBuffer = pTempBuffer;

	//7. Update the size
	_size = _published + expected;
	SANITY_INPUT_BUFFER;

	return true;
}

string IOBuffer::DumpBuffer(uint8_t *pBuffer, uint32_t length) {
	IOBuffer temp;
	temp.ReadFromBuffer(pBuffer, length);
	return temp.ToString();
}

string IOBuffer::ToString(uint32_t startIndex, uint32_t limit) {
	SANITY_INPUT_BUFFER;
	string allowedCharacters = " 1234567890-=qwertyuiop[]asdfghjkl;'\\`zxcvbnm";
	allowedCharacters += ",./!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\"|~ZXCVBNM<>?";
	stringstream ss;
	ss << "Size: " << _size << endl;
	ss << "Published: " << _published << endl;
	ss << "Consumed: " << _consumed << endl;
	ss << format("Address: %p", _pBuffer) << endl;
	if (limit != 0) {
		ss << format("Limited to %u bytes", limit) << endl;
	}
	string address = "";
	string part1 = "";
	string part2 = "";
	string hr = "";
	limit = (limit == 0) ? _published : limit;
	for (uint32_t i = startIndex; i < limit; i++) {
		if (((i % 16) == 0) && (i > 0)) {
			ss << address << "  " << part1 << " " << part2 << " " << hr << endl;
			part1 = "";
			part2 = "";
			hr = "";
		}
		address = format("%08u", i - (i % 16));

		if ((i % 16) < 8) {
			part1 += format("%02hhx", _pBuffer[i]);
			part1 += " ";
		} else {
			part2 += format("%02hhx", _pBuffer[i]);
			part2 += " ";
		}

		if (allowedCharacters.find(_pBuffer[i], 0) != string::npos)
			hr += _pBuffer[i];
		else
			hr += '.';
	}

	if (part1 != "") {
		part1 += string(24 - part1.size(), ' ');
		part2 += string(24 - part2.size(), ' ');
		hr += string(16 - hr.size(), ' ');
		ss << address << "  " << part1 << " " << part2 << " " << hr << endl;
	}
	SANITY_INPUT_BUFFER;

	return ss.str();
}

IOBuffer::operator string() {

	return ToString(0, 0);
}

void IOBuffer::Cleanup() {
	if (_pBuffer != NULL) {

		delete[] _pBuffer;
		_pBuffer = NULL;
	}
	_size = 0;
	_published = 0;
	_consumed = 0;
}

void IOBuffer::Recycle() {
	if (_consumed != _published)
		return;
	SANITY_INPUT_BUFFER;
	_consumed = 0;
	_published = 0;
	SANITY_INPUT_BUFFER;
}

