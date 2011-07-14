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


#include "utils/logging/fileloglocation.h"

FileLogLocation::FileLogLocation(string path, bool append)
: BaseLogLocation() {
	_fileName = path;
	_canLog = true;
	_counter = 0;
	RollOver();
}

FileLogLocation::~FileLogLocation() {
	_fileStream.close();
}

void FileLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string message) {
	if (!_canLog)
		return;
	if (_level < 0 || level > _level) {
		return;
	}
	string logEntry = format("%"PRIu64":%d:%s:%u:%s:%s\n",
			(uint64_t)time(NULL), level, STR(fileName), lineNumber, STR(functionName),
			STR(message));
	_fileStream.write(STR(logEntry), logEntry.size());
	_fileStream.flush();
	_counter++;
	if (_counter >= 100000)
		RollOver();
}

void FileLogLocation::RollOver() {
	_canLog = false;
	_fileStream.flush();
	_fileStream.close();

	moveFile(
			format("%s.%u", STR(_fileName), getuid()),
			format("%s.old.%u", STR(_fileName), getuid())
			);
	ios_base::openmode openMode = ios_base::out | ios_base::binary | ios_base::trunc;
	_fileStream.open(
			STR(format("%s.%u", STR(_fileName), getuid())),
			openMode
			);
	if (_fileStream.fail()) {
		_canLog = false;
	} else {
		_counter = 0;
		_canLog = true;
	}
}


