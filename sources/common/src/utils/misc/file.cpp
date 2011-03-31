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

File::File() {
	_path = "";
	_truncate = false;
	_append = false;
}

File::~File() {
	_file.flush();
	_file.close();
}

bool File::Initialize(string path) {
	return Initialize(path, FILE_OPEN_MODE_READ);
}

bool File::Initialize(string path, FILE_OPEN_MODE mode) {
	_path = path;
	ios_base::openmode openMode = ios_base::binary;
	switch (mode) {
		case FILE_OPEN_MODE_READ:
		{
			openMode |= ios_base::in;
			break;
		}
		case FILE_OPEN_MODE_TRUNCATE:
		{
			openMode |= ios_base::in;
			openMode |= ios_base::out;
			openMode |= ios_base::trunc;
			break;
		}
		case FILE_OPEN_MODE_APPEND:
		{
			openMode |= ios_base::in;
			openMode |= ios_base::out;
			if (fileExists(_path))
				openMode |= ios_base::app;
			else
				openMode |= ios_base::trunc;
			break;
		}
		default:
		{
			FATAL("Invalid open mode");
			return false;
		}
	}

	_file.open(STR(_path), openMode);
	if (_file.fail()) {
		FATAL("Unable to open file %s with mode 0x%x (%s)",
				STR(_path), (uint32_t) openMode, strerror(errno));
		return false;
	}

	if (!SeekEnd())
		return false;

	_size = _file.tellg();

	if (!SeekBegin())
		return false;

	return true;
}

void File::Close() {
	_file.flush();
	_file.close();
}

uint64_t File::Size() {
	return _size;
}

uint64_t File::Cursor() {
	return (uint64_t) _file.tellg();
}

bool File::IsEOF() {
	return _file.eof();
}

string File::GetPath() {
	return _path;
}

bool File::Failed() {
	return _file.fail();
}

bool File::IsOpen() {
	return _file.is_open();
}

bool File::SeekBegin() {
	_file.seekg(0, ios_base::beg);
	if (_file.fail()) {
		FATAL("Unable to seek to the beginning of file");
		return false;
	}
	return true;
}

bool File::SeekEnd() {
	_file.seekg(0, ios_base::end);
	if (_file.fail()) {
		FATAL("Unable to seek to the end of file");
		return false;
	}
	return true;
}

bool File::SeekAhead(int64_t count) {
	if (count < 0) {
		FATAL("Invali count");
		return false;
	}

	if (count + Cursor() > _size) {
		FATAL("End of file will be reached");
		return false;
	}

	_file.seekg(count, ios_base::cur);
	if (_file.fail()) {
		FATAL("Unable to seek ahead %"PRId64" bytes", count);
		return false;
	}
	return true;
}

bool File::SeekBehind(int64_t count) {
	if (count < 0) {
		FATAL("Invali count");
		return false;
	}

	if (Cursor() < (uint64_t) count) {
		FATAL("End of file will be reached");
		return false;
	}

	_file.seekg((-1) * count, ios_base::cur);
	if (_file.fail()) {
		FATAL("Unable to seek behind %"PRId64" bytes", count);
		return false;
	}
	return true;
}

bool File::SeekTo(uint64_t position) {
	if (_size < position) {
		FATAL("End of file will be reached");
		return false;
	}

	_file.seekg(position, ios_base::beg);
	if (_file.fail()) {
		FATAL("Unable to seek to position %"PRIu64, position);
		return false;
	}

	return true;
}

bool File::ReadI8(int8_t *pValue) {
	return ReadBuffer((uint8_t *) pValue, 1);
}

bool File::ReadI16(int16_t *pValue, bool networkOrder) {
	if (!ReadBuffer((uint8_t *) pValue, 2))
		return false;
	if (networkOrder)
		*pValue = ENTOHS(*pValue); //----MARKED-SHORT----
	return true;
}

bool File::ReadI24(int32_t *pValue, bool networkOrder) {
	*pValue = 0;
	if (!ReadBuffer((uint8_t *) pValue, 3))
		return false;
	if (networkOrder)
		*pValue = ENTOHL(*pValue) >> 8; //----MARKED-LONG---
	else
		*pValue = ((*pValue) << 8) >> 8;
	return true;
}

bool File::ReadI32(int32_t *pValue, bool networkOrder) {
	if (!ReadBuffer((uint8_t *) pValue, 4))
		return false;
	if (networkOrder)
		*pValue = ENTOHL(*pValue); //----MARKED-LONG---
	return true;
}

bool File::ReadSI32(int32_t *pValue) {
	if (!ReadI32(pValue, false))
		return false;
	*pValue = ENTOHA(*pValue); //----MARKED-LONG---
	return true;
}

bool File::ReadI64(int64_t *pValue, bool networkOrder) {
	if (!ReadBuffer((uint8_t *) pValue, 8))
		return false;
	if (networkOrder)
		*pValue = ENTOHLL(*pValue);
	return true;
}

bool File::ReadUI8(uint8_t *pValue) {
	return ReadI8((int8_t *) pValue);
}

bool File::ReadUI16(uint16_t *pValue, bool networkOrder) {
	return ReadI16((int16_t *) pValue, networkOrder);
}

bool File::ReadUI24(uint32_t *pValue, bool networkOrder) {
	return ReadI24((int32_t *) pValue, networkOrder);
}

bool File::ReadUI32(uint32_t *pValue, bool networkOrder) {
	return ReadI32((int32_t *) pValue, networkOrder);
}

bool File::ReadSUI32(uint32_t *pValue) {
	return ReadSI32((int32_t *) pValue);
}

bool File::ReadUI64(uint64_t *pValue, bool networkOrder) {
	return ReadI64((int64_t *) pValue, networkOrder);
}

bool File::ReadBuffer(uint8_t *pBuffer, uint64_t count) {
	_file.read((char *) pBuffer, count);
	if (_file.fail()) {
		FATAL("Unable to read %"PRIu64" bytes from the file. Cursor: %"PRIu64" (0x%"PRIx64"); %d (%s)",
				count, Cursor(), Cursor(), errno, strerror(errno));
		return false;
	}
	return true;
}

bool File::ReadLine(uint8_t *pBuffer, uint64_t &maxSize) {
	_file.getline((char *) pBuffer, maxSize);
	if (_file.fail()) {
		FATAL("Unable to read line from the file");
		return false;
	}
	return true;
}

bool File::ReadAll(string &str) {
	str = "";
	if (Size() >= 0xffffffff) {
		FATAL("ReadAll can only be done on files smaller than 2^32 bytes (4GB)");
		return false;
	}
	if (Size() == 0) {
		return true;
	}
	if (!SeekBegin()) {
		FATAL("Unable to seek to begin");
		return false;
	}
	uint8_t *pBuffer = new uint8_t[(uint32_t) Size()];
	if (!ReadBuffer(pBuffer, Size())) {
		FATAL("Unable to read data");
		delete[] pBuffer;
		return false;
	}
	str = string((char *) pBuffer, (uint32_t) Size());
	delete[] pBuffer;
	return true;
}

bool File::PeekI8(int8_t *pValue) {
	if (!ReadI8(pValue))
		return false;
	return SeekBehind(1);
}

bool File::PeekI16(int16_t *pValue, bool networkOrder) {
	if (!ReadI16(pValue, networkOrder))
		return false;
	return SeekBehind(2);
}

bool File::PeekI24(int32_t *pValue, bool networkOrder) {
	if (!ReadI24(pValue, networkOrder))
		return false;
	return SeekBehind(3);
}

bool File::PeekI32(int32_t *pValue, bool networkOrder) {
	if (!ReadI32(pValue, networkOrder))
		return false;
	return SeekBehind(4);
}

bool File::PeekSI32(int32_t *pValue) {
	if (!ReadSI32(pValue))
		return false;
	return SeekBehind(4);
}

bool File::PeekI64(int64_t *pValue, bool networkOrder) {
	if (!ReadI64(pValue, networkOrder))
		return false;
	return SeekBehind(8);
}

bool File::PeekUI8(uint8_t *pValue) {
	return PeekI8((int8_t *) pValue);
}

bool File::PeekUI16(uint16_t *pValue, bool networkOrder) {
	return PeekI16((int16_t *) pValue, networkOrder);
}

bool File::PeekUI24(uint32_t *pValue, bool networkOrder) {
	return PeekI24((int32_t *) pValue, networkOrder);
}

bool File::PeekUI32(uint32_t *pValue, bool networkOrder) {
	return PeekI32((int32_t *) pValue, networkOrder);
}

bool File::PeekSUI32(uint32_t *pValue) {
	return PeekSI32((int32_t *) pValue);
}

bool File::PeekUI64(uint64_t *pValue, bool networkOrder) {
	return PeekI64((int64_t *) pValue, networkOrder);
}

bool File::PeekBuffer(uint8_t *pBuffer, uint64_t count) {
	if (!ReadBuffer(pBuffer, count))
		return false;
	return SeekBehind(count);
}

bool File::WriteI8(int8_t value) {
	return WriteBuffer((uint8_t *) & value, 1);
}

bool File::WriteI16(int16_t value, bool networkOrder) {
	if (networkOrder)
		value = EHTONS(value); //----MARKED-SHORT----
	return WriteBuffer((uint8_t *) & value, 2);
}

bool File::WriteI24(int32_t value, bool networkOrder) {
	if (networkOrder)
		value = EHTONL(value << 8);
	return WriteBuffer((uint8_t *) & value, 3);
}

bool File::WriteI32(int32_t value, bool networkOrder) {
	if (networkOrder)
		value = EHTONL(value);
	return WriteBuffer((uint8_t *) & value, 4);
}

bool File::WriteSI32(int32_t value) {
	if (!WriteI24(value, true))
		return false;
	if (!WriteI8(value >> 24))
		return false;
	return true;
}

bool File::WriteI64(int64_t value, bool networkOrder) {
	if (networkOrder)
		value = EHTONLL(value);
	return WriteBuffer((uint8_t *) & value, 8);
}

bool File::WriteUI8(uint8_t value) {
	return WriteI8((int8_t) value);
}

bool File::WriteUI16(uint16_t value, bool networkOrder) {
	return WriteI16((int16_t) value, networkOrder);
}

bool File::WriteUI24(uint32_t value, bool networkOrder) {
	return WriteI24((int32_t) value, networkOrder);
}

bool File::WriteUI32(uint32_t value, bool networkOrder) {
	return WriteI32((int32_t) value, networkOrder);
}

bool File::WriteSUI32(uint32_t value) {
	return WriteSI32((int32_t) value);
}

bool File::WriteUI64(uint64_t value, bool networkOrder) {
	return WriteI64((int64_t) value, networkOrder);
}

bool File::WriteString(string &value) {
	return WriteBuffer((uint8_t *) STR(value), value.length());
}

bool File::WriteBuffer(const uint8_t *pBuffer, uint64_t count) {
	_file.write((char *) pBuffer, count);
	if (_file.fail()) {
		FATAL("Unable to write %"PRIu64" bytes to file", count);
		return false;
	}
	return true;
}

bool File::Flush() {
	_file.flush();
	if (_file.fail()) {
		FATAL("Unable to flush to file");
		return false;
	}
	return true;
}

