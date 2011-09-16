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


#ifndef _FILELOGLOCATION_H
#define	_FILELOGLOCATION_H

#include "utils/logging/baseloglocation.h"

class DLLEXP FileLogLocation
: public BaseLogLocation {
private:
	ofstream _fileStream;
	bool _canLog;
	uint32_t _counter;
	string _newLineCharacters;
	string _fileName;
	uint32_t _fileHistorySize;
	uint32_t _fileLength;
	uint32_t _currentLength;
	vector<string> _history;
	bool _fileIsClosed;
public:
	FileLogLocation(Variant &configuration);
	virtual ~FileLogLocation();

	virtual bool Init();
	virtual bool EvalLogLevel(int32_t level, string &fileName, uint32_t lineNumber,
			string &functionName, string &message);
	virtual bool EvalLogLevel(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, Variant &le);
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message);
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, Variant &le);
	virtual void SignalFork();
private:
	bool OpenFile();
};


#endif	/* _FILELOGLOCATION_H */

