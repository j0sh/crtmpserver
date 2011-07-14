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
#include "defines.h"
#include <syslog.h>

SyslogLogLocation::SyslogLogLocation(string identifier, bool appendSourceFileLine) {
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
}

SyslogLogLocation::~SyslogLogLocation() {
}

void SyslogLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string message) {
	if (_level < 0 || level > _level) {
		return;
	}
	int priority = MAP_HAS1(_priorities, level) ? _priorities[level] : LOG_DEBUG;
	if (_appendSourceFileLine) {
		syslog(priority, "%s:%"PRIu32":%s %s", STR(fileName), lineNumber, STR(functionName), STR(message));
	} else {
		syslog(priority, "%s", STR(message));
	}
}
#endif /* HAS_SYSLOG */

