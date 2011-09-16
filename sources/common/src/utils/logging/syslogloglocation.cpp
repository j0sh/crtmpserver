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
#include "utils/logging/formatter.h"
#include "common.h"
#include <syslog.h>

SyslogLogLocation::SyslogLogLocation(Variant &configuration, string identifier, bool appendSourceFileLine, int32_t specificLevel)
: BaseLogLocation(configuration) {
	_appendSourceFileLine = appendSourceFileLine;
	_identifier = identifier;
	_priorities[_FINEST_] = LOG_DEBUG;
	_priorities[_FINE_] = LOG_DEBUG;
	_priorities[_DEBUG_] = LOG_DEBUG;
	_priorities[_INFO_] = LOG_INFO;
	_priorities[_WARNING_] = LOG_WARNING;
	_priorities[_ERROR_] = LOG_ERR;
	_priorities[_FATAL_] = LOG_ERR;
	_priorities[_PROD_ACCESS_] = LOG_ERR;
	_priorities[_PROD_ERROR_] = LOG_ERR;
	_specificLevel = specificLevel;
	_enforceLoggerName = (_configuration[CONF_LOG_APPENDER_NAME] != (string) "");
	_pDefualtFormatter = NULL;
	InitFormatters();
}

SyslogLogLocation::~SyslogLogLocation() {
	if (_pDefualtFormatter != NULL) {
		delete _pDefualtFormatter;
		_pDefualtFormatter = NULL;
	}

	FOR_MAP(_formatters, string, Formatter *, i) {
		delete MAP_VAL(i);
	}
	_formatters.clear();
}

void SyslogLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string message) {
	return;
}

void SyslogLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, Variant &le) {
	if (_specificLevel != 0) {
		if (_specificLevel != level) {
			return;
		}
	} else {
		if (_level < 0 || level > _level) {
			return;
		}
	}
	if (_enforceLoggerName) {
		if (_configuration[CONF_LOG_APPENDER_NAME] != le["loggerName"]) {
			return;
		}
	}

	string finalMessage = ComputeMessage(le);
	if (finalMessage == "")
		return;

	int priority = (MAP_HAS1(_priorities, level) ? _priorities[level] : LOG_DEBUG) | LOG_USER;

	if (_appendSourceFileLine) {
		syslog(priority, "%s %s:%"PRIu32":%s %s",
				STR(le["loggerName"]),
				STR(fileName),
				lineNumber,
				STR(functionName),
				STR(finalMessage));
	} else {
		syslog(priority, "%s %s",
				STR(le["loggerName"]),
				STR(finalMessage));
	}
}

void SyslogLogLocation::InitFormatters() {
	if (!_configuration.HasKeyChain(V_MAP, false, 1, CONF_LOG_APPENDER_FORMAT))
		return;

	string defaultFormatter = "";
	if (_configuration[CONF_LOG_APPENDER_FORMAT][(uint32_t) 1] == V_STRING) {
		defaultFormatter = (string) _configuration[CONF_LOG_APPENDER_FORMAT][(uint32_t) 1];
	}
	_configuration[CONF_LOG_APPENDER_FORMAT].RemoveAt(1);
	if (defaultFormatter != "") {
		_pDefualtFormatter = Formatter::GetInstance(defaultFormatter);
	}

	FOR_MAP(_configuration[CONF_LOG_APPENDER_FORMAT], string, Variant, i) {
		if ((MAP_VAL(i) != V_STRING)
				|| ((string) MAP_VAL(i) == ""))
			continue;
		Formatter *pTemp = Formatter::GetInstance(MAP_VAL(i));
		_formatters[MAP_KEY(i)] = pTemp;
	}
}

string SyslogLogLocation::ComputeMessage(Variant &le) {
	Formatter *pFormatter = NULL;
	if (MAP_HAS1(_formatters, (string) le["operation"]))
		pFormatter = _formatters[(string) le["operation"]];
	else
		pFormatter = _pDefualtFormatter;

	if (pFormatter == NULL)
		return "";

	return pFormatter->Format(le);
}

void SyslogLogLocation::SignalFork() {

}

#endif /* HAS_SYSLOG */

