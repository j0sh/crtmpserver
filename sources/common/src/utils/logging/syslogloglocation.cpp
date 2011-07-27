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
#include "utils/logging/syslogloglocation.h"
#include "common.h"
#include <syslog.h>

SyslogLogLocation::SyslogLogLocation(Variant &configuration, string identifier, bool appendSourceFileLine, int32_t specificLevel)
: BaseLogLocation(configuration) {
	_appendSourceFileLine = appendSourceFileLine;
	_identifier = identifier;
	openlog(STR(_identifier), LOG_PID, LOG_USER);
	_priorities[_FINEST_] = LOG_DEBUG;
	_priorities[_FINE_] = LOG_DEBUG;
	_priorities[_DEBUG_] = LOG_DEBUG;
	_priorities[_INFO_] = LOG_INFO;
	_priorities[_WARNING_] = LOG_WARNING;
	_priorities[_ERROR_] = LOG_ERR;
	_priorities[_FATAL_] = LOG_ERR;
	_priorities[_PROD_ACCESS_] = LOG_INFO;
	_priorities[_PROD_ERROR_] = LOG_INFO;
	_specificLevel = specificLevel;
}

SyslogLogLocation::~SyslogLogLocation() {
}

void SyslogLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string message) {

	return;
}

void SyslogLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber, string functionName, Variant &le) {
	cout << " test message" << endl;

	if (_specificLevel != 0) {
		if (_specificLevel != level) {
			return;
		}
	} else {
		if (_level < 0 || level > _level) {
			return;
		}
	}


	/*
	 * -------
	 *        |
	 *        |
	 *        ----------------------------
	 * 
	 */

	string finalMessage = ComputeMessage(le);

	cout << finalMessage << endl;

	int priority = MAP_HAS1(_priorities, level) ? _priorities[level] : LOG_DEBUG;
	if (_appendSourceFileLine) {
		syslog(priority, "%s:%"PRIu32":%s %s", STR(fileName), lineNumber, STR(functionName), STR(finalMessage));
	} else {
		syslog(priority, "%s", STR(finalMessage));
	}
}

string SyslogLogLocation::ComputeMessage(Variant &le) {
	return "not yet implemented";
}

#endif /* HAS_SYSLOG */

