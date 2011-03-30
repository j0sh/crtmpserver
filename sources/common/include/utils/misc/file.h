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


#ifndef _FILE_H
#define	_FILE_H

#include "platform/platform.h"

enum FILE_OPEN_MODE {
	FILE_OPEN_MODE_READ,
	FILE_OPEN_MODE_TRUNCATE,
	FILE_OPEN_MODE_APPEND
};

class DLLEXP File {
private:
	fstream _file;
	uint64_t _size;
	string _path;
	bool _truncate;
	bool _append;
public:
	File();
	virtual ~File();

	//Init
	bool Initialize(string path);
	bool Initialize(string path, FILE_OPEN_MODE mode);
	void Close();

	//info
	uint64_t Size();
	uint64_t Cursor();
	bool IsEOF();
	string GetPath();
	bool Failed();
	bool IsOpen();

	//seeking
	bool SeekBegin();
	bool SeekEnd();
	bool SeekAhead(int64_t count);
	bool SeekBehind(int64_t count);
	bool SeekTo(uint64_t position);

	//read data
	bool ReadI8(int8_t *pValue);
	bool ReadI16(int16_t *pValue, bool networkOrder = true);
	bool ReadI24(int32_t *pValue, bool networkOrder = true);
	bool ReadI32(int32_t *pValue, bool networkOrder = true);
	bool ReadSI32(int32_t *pValue);
	bool ReadI64(int64_t *pValue, bool networkOrder = true);
	bool ReadUI8(uint8_t *pValue);
	bool ReadUI16(uint16_t *pValue, bool networkOrder = true);
	bool ReadUI24(uint32_t *pValue, bool networkOrder = true);
	bool ReadUI32(uint32_t *pValue, bool networkOrder = true);
	bool ReadSUI32(uint32_t *pValue);
	bool ReadUI64(uint64_t *pValue, bool networkOrder = true);
	bool ReadBuffer(uint8_t *pBuffer, uint64_t count);
	bool ReadLine(uint8_t *pBuffer, uint64_t &maxSize);
	bool ReadAll(string &str);

	//peek data
	bool PeekI8(int8_t *pValue);
	bool PeekI16(int16_t *pValue, bool networkOrder = true);
	bool PeekI24(int32_t *pValue, bool networkOrder = true);
	bool PeekI32(int32_t *pValue, bool networkOrder = true);
	bool PeekSI32(int32_t *pValue);
	bool PeekI64(int64_t *pValue, bool networkOrder = true);
	bool PeekUI8(uint8_t *pValue);
	bool PeekUI16(uint16_t *pValue, bool networkOrder = true);
	bool PeekUI24(uint32_t *pValue, bool networkOrder = true);
	bool PeekUI32(uint32_t *pValue, bool networkOrder = true);
	bool PeekSUI32(uint32_t *pValue);
	bool PeekUI64(uint64_t *pValue, bool networkOrder = true);
	bool PeekBuffer(uint8_t *pBuffer, uint64_t count);

	//write data
	bool WriteI8(int8_t value);
	bool WriteI16(int16_t value, bool networkOrder = true);
	bool WriteI24(int32_t value, bool networkOrder = true);
	bool WriteI32(int32_t value, bool networkOrder = true);
	bool WriteSI32(int32_t value);
	bool WriteI64(int64_t value, bool networkOrder = true);
	bool WriteUI8(uint8_t value);
	bool WriteUI16(uint16_t value, bool networkOrder = true);
	bool WriteUI24(uint32_t value, bool networkOrder = true);
	bool WriteUI32(uint32_t value, bool networkOrder = true);
	bool WriteSUI32(uint32_t value);
	bool WriteUI64(uint64_t value, bool networkOrder = true);
	bool WriteString(string &value);
	bool WriteBuffer(const uint8_t *pBuffer, uint64_t count);
	bool Flush();
};


#endif	/* _FILE_H */


