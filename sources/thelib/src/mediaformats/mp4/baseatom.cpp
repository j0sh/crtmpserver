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


#ifdef HAS_MEDIA_MP4
#include "mediaformats/mp4/baseatom.h"
#include "mediaformats/mp4/mp4document.h"

string U32TOS(uint32_t type) {
	type = ENTOHL(type);
	return string((char *) & (type), 4);
}

BaseAtom::BaseAtom(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start) {
	_pDoc = pDocument;
	_pParent = NULL;
	_pDoc->AddAtom(this);
	_type = type;
	_size = size;
	_start = start;
}

BaseAtom::~BaseAtom() {
}

uint64_t BaseAtom::GetStart() {
	return _start;
}

uint64_t BaseAtom::GetSize() {
	return _size;
}

string BaseAtom::GetTypeString() {
	return U32TOS(_type);
}

uint32_t BaseAtom::GetTypeNumeric() {
	return _type;
}

MP4Document * BaseAtom::GetDoc() {
	return _pDoc;
}

BaseAtom::operator string() {
	return format("S: %"PRIu64"(0x%"PRIx64"); L: %"PRIu64"(0x%"PRIx64"); T: %u(%s)",
			_start, _start, _size, _size, GetTypeNumeric(), STR(GetTypeString()));
}

bool BaseAtom::IsIgnored() {
	return false;
}

BaseAtom * BaseAtom::GetPath(vector<uint32_t> path) {
	ASSERT("Operation not supported");
	return NULL;
}

BaseAtom *BaseAtom::GetParentAtom() {
	return _pParent;
}

void BaseAtom::SetParentAtom(BaseAtom *pParent) {
	_pParent = pParent;
}

bool BaseAtom::SkipRead(bool issueWarn) {
	if (issueWarn)
		WARN("Atom type %s skipped. Position 0x%"PRIx64"(%"PRIu64"); Size: 0x%"PRIx64"(%"PRIu64")",
			STR(GetTypeString()), _start, _start, _size, _size);
	return _pDoc->GetMediaFile().SeekTo(_start + _size);
}

uint64_t BaseAtom::CurrentPosition() {
	return _pDoc->GetMediaFile().Cursor();
}

bool BaseAtom::CheckBounds(uint64_t size) {
	if (CurrentPosition() + size > _start + _size) {
		FATAL("Reached the end of the atom: Current pos: %"PRIu64"; Wanted size: %"PRIu64"; atom start: %"PRIu64"; atom size: %"PRIu64,
				CurrentPosition(), size, _start, _size);
		return false;
	}
	return true;
}

bool BaseAtom::ReadArray(uint8_t *pBuffer, uint64_t length) {
	if (!CheckBounds(length))
		return false;
	return _pDoc->GetMediaFile().ReadBuffer(pBuffer, length);
}

bool BaseAtom::ReadUInt8(uint8_t &val) {
	if (!CheckBounds(1))
		return false;
	return _pDoc->GetMediaFile().ReadUI8(& val);
}

bool BaseAtom::ReadUInt16(uint16_t &val, bool networkOrder) {
	if (!CheckBounds(2))
		return false;
	return _pDoc->GetMediaFile().ReadUI16(& val, networkOrder);
}

bool BaseAtom::ReadInt16(int16_t &val, bool networkOrder) {
	if (!CheckBounds(2))
		return false;
	return _pDoc->GetMediaFile().ReadI16(& val, networkOrder);
}

bool BaseAtom::ReadUInt24(uint32_t &val, bool networkOrder) {
	if (!CheckBounds(3))
		return false;
	return _pDoc->GetMediaFile().ReadUI24(&val, networkOrder);
}

bool BaseAtom::ReadUInt32(uint32_t &val, bool networkOrder) {
	if (!CheckBounds(4))
		return false;
	return _pDoc->GetMediaFile().ReadUI32(&val, networkOrder);
}

bool BaseAtom::ReadInt32(int32_t &val, bool networkOrder) {
	if (!CheckBounds(4))
		return false;
	return _pDoc->GetMediaFile().ReadI32(&val, networkOrder);
}

bool BaseAtom::ReadUInt64(uint64_t &val, bool networkOrder) {
	if (!CheckBounds(8))
		return false;
	return _pDoc->GetMediaFile().ReadUI64(&val, networkOrder);
}

bool BaseAtom::ReadInt64(int64_t &val, bool networkOrder) {
	if (!CheckBounds(8))
		return false;
	return _pDoc->GetMediaFile().ReadI64(&val, networkOrder);
}

bool BaseAtom::SkipBytes(uint64_t count) {
	if (!CheckBounds(count))
		return false;
	return _pDoc->GetMediaFile().SeekAhead(count);
}

bool BaseAtom::ReadString(string &val, uint64_t size) {
	if (!CheckBounds(size))
		return false;
	char *pTemp = new char[(uint32_t)size + 1];
	memset(pTemp, 0, (uint32_t)size + 1);
	bool result = _pDoc->GetMediaFile().ReadBuffer((uint8_t *) pTemp, size);
	if (result)
		val = string(pTemp, (uint32_t)size);
	else
		val = "";
	delete[] pTemp;
	return result;
}

#endif /* HAS_MEDIA_MP4 */
