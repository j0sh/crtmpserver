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

#ifdef HAS_SYSLOG
#ifndef _SYSLOGLOGLOCATION_H
#define	_SYSLOGLOGLOCATION_H

#include "utils/logging/baseloglocation.h"

class Formatter;

class SyslogLogLocation
: public BaseLogLocation {
private:
	bool _appendSourceFileLine;
	string _identifier;
	map<uint32_t, int> _priorities;
	bool _enforceLoggerName;

	map<string, Formatter *> _formatters;
	Formatter *_pDefualtFormatter;
public:
	SyslogLogLocation(Variant &configuration, string identifier = "",
			bool appendSourceFileLine = true, int32_t specificLevel = 0);
	virtual ~SyslogLogLocation();
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message);
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, Variant &le);
	virtual void SignalFork();
private:
	void InitFormatters();
	string ComputeMessage(Variant &le);
};


#endif	/* _SYSLOGLOGLOCATION_H */
#endif /* HAS_SYSLOG */

