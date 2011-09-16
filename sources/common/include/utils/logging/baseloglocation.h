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


#ifndef BASELOGLOCATION_H_
#define BASELOGLOCATION_H_

#include "platform/platform.h"
#include "utils/misc/variant.h"

/*!
	@class BaseLogLocation
	@brief Base class that all logging must derive from.
 */
class DLLEXP BaseLogLocation {
protected:
	int32_t _level;
	string _name;
	int32_t _specificLevel;
	bool _singleLine;
	Variant _configuration;
public:

	BaseLogLocation(Variant &configuration);
	virtual ~BaseLogLocation();
	int32_t GetLevel();
	void SetLevel(int32_t level);
	string GetName();
	void SetName(string name);
	virtual bool EvalLogLevel(int32_t level, string &fileName, uint32_t lineNumber,
			string &functionName, string &message);
	virtual bool EvalLogLevel(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, Variant &le);
	virtual bool Init();
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message) = 0;
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, Variant &le) = 0;
	virtual void SignalFork() = 0;
private:
	bool EvalLogLevel(int32_t level);
};


#endif /*BASELOGLOCATION_H_*/

