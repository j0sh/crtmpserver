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

#include "utils/misc/mmapfile.h"
#include "utils/logging/logging.h"

#ifdef HAS_MMAP
#define BS_NONE 1
#define BS_FRONT 2
#define BS_OVERLAP 3
#define BS_SPARSE 4

//#define LOG_MMAP(...) FINEST(__VA_ARGS__)
#define LOG_MMAP(...)

map<string, __FileInfo__ > MmapFile::_fds;
int32_t MmapFile::_pageSize = 0;

MmapFile::MmapFile() {
	_cursor = 0;
	_size = 0;
	_failed = false;
	if (_pageSize == 0) {
		_pageSize = getpagesize();
		LOG_MMAP("_pageSize: %u", _pageSize);
	}
	_windowSize = 0;
	memset(&_pointer1, 0, sizeof (MmapPointer));
	memset(&_pointer2, 0, sizeof (MmapPointer));
}

MmapFile::~MmapFile() {
	_pointer1.Free();
	_pointer2.Free();

	if (MAP_HAS1(_fds, _path)) {
		_fds[_path].useCount = _fds[_path].useCount - 1;
		if (_fds[_path].useCount == 0) {
			close(_fds[_path].fd);
			_fds.erase(_path);
		}
	}
}

MmapPointer::MmapPointer() {

}

MmapPointer::~MmapPointer() {

}

bool MmapPointer::Allocate(int fd, uint64_t cursor,
		uint32_t windowSize, uint64_t size) {
	Free();

	if (size > windowSize) {
		FATAL("size is greater than window size: %"PRIu64" > %u", size, windowSize);
		return false;
	}

	//1. Compute the new cursor
	_cursor = cursor;
	_cursor = (_cursor / MmapFile::_pageSize) * MmapFile::_pageSize;

	//2. Compute the new size
	_size = windowSize;
	while (_cursor + _size < cursor + size) {
		LOG_MMAP("We need to go a little bit further; Wanted: %"PRIu64"; got: %"PRIu64,
				cursor + size, _cursor + _size);
		_size += MmapFile::_pageSize;
	}

	LOG_MMAP("Reading %"PRIu64" bytes from disk", _size);
	_pData = (uint8_t *) mmap(NULL,
			_size,
			PROT_READ,
			MAP_FILE | MAP_PRIVATE | MAP_NOCACHE | MAP_NOEXTEND,
			fd,
			_cursor);
	if (_pData == MAP_FAILED) {
		_pData = NULL;
		FATAL("Unable to mmap: %d %s", errno, strerror(errno));
		return false;
	}

	return true;
}

bool MmapPointer::Free() {
	if (_size == 0)
		return true;
	if (munmap(_pData, _size) != 0) {
		FATAL("Unable to munmap: %d %s", errno, strerror(errno));
		return false;
	}
	_pData = NULL;
	_size = 0;
	_cursor = 0;
	_bytesRead = 0;
	return true;
}

uint64_t MmapPointer::Copy(void *pBuffer, uint64_t cursor, uint64_t delta, uint64_t count) {
	uint64_t available = _cursor + _size - (cursor + delta);
	uint64_t result = count > available ? available : count;
	memcpy(pBuffer, _pData + (cursor + delta - _cursor), result);
	_bytesRead += result;
	return result;
}

uint8_t MmapPointer::GetState(MmapPointer & backBuffer) {
	if (_size == 0) {
		return BS_NONE;
	}

	if (backBuffer._size == 0) {
		return BS_FRONT;
	}

	if (backBuffer._cursor + backBuffer._size >= _cursor) {
		return BS_OVERLAP;
	}

	return BS_SPARSE;
}

bool MmapPointer::HasRange(uint64_t cursor, uint64_t count) {
	return (cursor >= _cursor) && ((cursor + count) <= (_cursor + _size));
}

MmapPointer::operator string() {
	if (_size == 0)
		return "[N - N](0)";
	return format("[%"PRIu64" - %"PRIu64"](%u)", _cursor, _cursor + _size - 1, _size);
}

bool MmapFile::Initialize(string path, uint32_t windowSize, bool exclusive) {
	//1. Do we have this file open?
	LOG_MMAP("Initial window size: %u", windowSize);
	uint32_t pagesCount = windowSize / _pageSize;
	LOG_MMAP("pagesCount: %u", pagesCount);
	_windowSize = (pagesCount * _pageSize) +
			(((windowSize % _pageSize) != 0) ? _pageSize : 0);
	_path = path;
	LOG_MMAP("_windowSize: %u; file: `%s`", _windowSize, STR(_path));
	if (!MAP_HAS1(_fds, _path)) {
		__FileInfo__ fi = {0, 0, 0};

		//2. Open the file
		if (exclusive) {
			fi.fd = open(STR(_path), O_RDWR);
		} else {
			fi.fd = open(STR(_path), O_RDONLY);
		}
		if (fi.fd <= 0) {
			FATAL("Unable to open file %s: %d: %s", STR(_path), errno, strerror(errno));
			_failed = true;
			return false;
		}
		if (exclusive) {
			if (lockf(fi.fd, F_TLOCK, 0) != 0) {
				FATAL("Unable to lock file %s: %d: %s", STR(_path), errno, strerror(errno));
				_failed = true;
				close(fi.fd);
				return false;
			}
		}

		//2. Get its size
		struct stat s;
		if (fstat(fi.fd, &s) != 0) {
			FATAL("Unable to stat file %s: %d: %s", STR(_path), errno, strerror(errno));
			_failed = true;
			close(fi.fd);
			return false;
		}
		fi.size = s.st_size;
		fi.useCount = 0;

		//3. store the file info
		_fds[_path] = fi;
	}

	//4. Increment the usage counter
	_fds[_path].useCount = _fds[_path].useCount + 1;

	//5. Get the info and save it in this instance
	_fd = _fds[_path].fd;
	_path = _path;
	_size = _fds[_path].size;

	//6. We are done
	return true;
}

uint64_t MmapFile::Size() {
	return _size;
}

uint64_t MmapFile::Cursor() {
	return _cursor;
}

bool MmapFile::IsEOF() {
	return _cursor == _size;
}

string MmapFile::GetPath() {
	return _path;
}

bool MmapFile::Failed() {
	return _failed;
}

bool MmapFile::IsOpen() {
	return _fd != 0;
}

bool MmapFile::SeekBegin() {
	return SeekTo(0);
}

bool MmapFile::SeekEnd() {
	return SeekTo(_size - 1);
}

bool MmapFile::SeekAhead(int64_t count) {
	return SeekTo(_cursor + count);
}

bool MmapFile::SeekBehind(int64_t count) {
	return SeekTo(_cursor - count);
}

bool MmapFile::SeekTo(uint64_t position) {
	if (_failed) {
		FATAL("This mmap file is in inconsistent state");
		return false;
	}
	if (position > _size) {
		FATAL("Invalid position: %"PRIu64". Must be at most: %"PRIu64, position, _size - 1);
		_failed = true;
		return false;
	}
	_cursor = position;
	return true;
}

bool MmapFile::ReadI8(int8_t *pValue) {
	if (!PeekI8(pValue))
		return false;
	return SeekAhead(1);
}

bool MmapFile::ReadI16(int16_t *pValue, bool networkOrder) {
	if (!PeekI16(pValue, networkOrder))
		return false;
	return SeekAhead(2);
}

bool MmapFile::ReadI24(int32_t *pValue, bool networkOrder) {
	if (!PeekI24(pValue, networkOrder))
		return false;
	return SeekAhead(3);
}

bool MmapFile::ReadI32(int32_t *pValue, bool networkOrder) {
	if (!PeekI32(pValue, networkOrder))
		return false;
	return SeekAhead(4);
}

bool MmapFile::ReadSI32(int32_t *pValue) {
	if (!PeekSI32(pValue))
		return false;
	return SeekAhead(4);
}

bool MmapFile::ReadI64(int64_t *pValue, bool networkOrder) {
	if (!PeekI64(pValue, networkOrder))
		return false;
	return SeekAhead(8);
}

bool MmapFile::ReadUI8(uint8_t *pValue) {
	return ReadI8((int8_t *) pValue);
}

bool MmapFile::ReadUI16(uint16_t *pValue, bool networkOrder) {
	return ReadI16((int16_t *) pValue, networkOrder);
}

bool MmapFile::ReadUI24(uint32_t *pValue, bool networkOrder) {
	return ReadI24((int32_t *) pValue, networkOrder);
}

bool MmapFile::ReadUI32(uint32_t *pValue, bool networkOrder) {
	return ReadI32((int32_t *) pValue, networkOrder);
}

bool MmapFile::ReadSUI32(uint32_t *pValue) {
	return ReadSI32((int32_t *) pValue);
}

bool MmapFile::ReadUI64(uint64_t *pValue, bool networkOrder) {
	return ReadI64((int64_t *) pValue, networkOrder);
}

bool MmapFile::ReadBuffer(uint8_t *pBuffer, uint64_t count) {
	if (!PeekBuffer(pBuffer, count))
		return false;
	return SeekAhead(count);
}

bool MmapFile::PeekI8(int8_t *pValue) {
	return PeekBuffer((uint8_t *) pValue, 1);
}

bool MmapFile::PeekI16(int16_t *pValue, bool networkOrder) {
	if (!PeekBuffer((uint8_t *) pValue, 2))
		return false;
	if (networkOrder)
		*pValue = ENTOHS(*pValue); //----MARKED-SHORT----
	return true;
}

bool MmapFile::PeekI24(int32_t *pValue, bool networkOrder) {
	*pValue = 0;
	if (!PeekBuffer((uint8_t *) pValue, 3))
		return false;
	if (networkOrder)
		*pValue = ENTOHL(*pValue) >> 8; //----MARKED-LONG---
	else
		*pValue = *pValue << 8;
	return true;
}

bool MmapFile::PeekI32(int32_t *pValue, bool networkOrder) {
	if (!PeekBuffer((uint8_t *) pValue, 4))
		return false;
	if (networkOrder)
		*pValue = ENTOHL(*pValue); //----MARKED-LONG---
	return true;
}

bool MmapFile::PeekSI32(int32_t *pValue) {
	if (!PeekI32(pValue, false))
		return false;
	*pValue = ENTOHA(*pValue); //----MARKED-LONG---
	return true;
}

bool MmapFile::PeekI64(int64_t *pValue, bool networkOrder) {
	if (!PeekBuffer((uint8_t *) pValue, 8))
		return false;
	if (networkOrder)
		*pValue = ENTOHLL(*pValue);
	return true;
}

bool MmapFile::PeekUI8(uint8_t *pValue) {
	return PeekI8((int8_t *) pValue);
}

bool MmapFile::PeekUI16(uint16_t *pValue, bool networkOrder) {
	return PeekI16((int16_t *) pValue, networkOrder);
}

bool MmapFile::PeekUI24(uint32_t *pValue, bool networkOrder) {
	return PeekI24((int32_t *) pValue, networkOrder);
}

bool MmapFile::PeekUI32(uint32_t *pValue, bool networkOrder) {
	return PeekI32((int32_t *) pValue, networkOrder);
}

bool MmapFile::PeekSUI32(uint32_t *pValue) {
	return ReadSI32((int32_t *) pValue);
}

bool MmapFile::PeekUI64(uint64_t *pValue, bool networkOrder) {
	return PeekI64((int64_t *) pValue, networkOrder);
}

bool MmapFile::PeekBuffer(uint8_t *pDestBuffer, uint64_t count) {
	//1. Sanity checks
	if (_failed) {
		DEBUG("_cursor: %"PRIu64"; count: %"PRIu64"; %s", _cursor, count, STR(_path));
		FATAL("This mmap file is in inconsistent state");
		return false;
	}

	if (_windowSize < count) {
		DEBUG("_cursor: %"PRIu64"; count: %"PRIu64"; %s", _cursor, count, STR(_path));
		FATAL("Invalid window size: _windowSize < count %u < %"PRIu64,
				_windowSize, count);
		_failed = true;
		return false;
	}

	if (_cursor + count > _size) {
		DEBUG("_cursor: %"PRIu64"; count: %"PRIu64"; %s", _cursor, count, STR(_path));
		FATAL("EOF will be reached: cursor: %"PRIu64"; count: %"PRIu64"; size: %"PRIu64,
				_cursor, count, _size);
		_failed = true;
		return false;
	}

	//2. reposition the pointer if necessary
	MmapPointer *pPointer = NULL;
	if (_pointer1.HasRange(_cursor, count)) {
		pPointer = &_pointer1;
	} else {
		if (_pointer2.HasRange(_cursor, count)) {
			pPointer = &_pointer2;
		} else {
			if (_pointer1._cursor < _pointer2._cursor) {
				pPointer = &_pointer1;
			} else {
				pPointer = &_pointer2;
			}
			if (!pPointer->Allocate(_fd, _cursor, _windowSize, count)) {
				FATAL("Unable to allocate mmap pointer");
				_failed = true;
				return false;
			}
		}
	}

	//3. Do the read
	if (pPointer->Copy(pDestBuffer, _cursor, 0, count) != count) {
		FATAL("Unable to copy %"PRIu64" bytes", count);
		_failed = true;
		return false;
	}

	return true;
}
#endif /* HAS_MMAP */


