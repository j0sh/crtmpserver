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

class Formatter {
private:
	string _formatString;

	struct LogField {
		bool isField;
		string value;

		LogField() {
			isField = false;
			value = "";
		}
	};
	vector<LogField *> _fields;
private:

	Formatter() {

	}
public:

	virtual ~Formatter() {
		for (uint32_t i = 0; i < _fields.size(); i++) {
			delete _fields[i];
		}
		_fields.clear();
	}

	static Formatter* GetInstance(string formatString) {
		Formatter *pResult = new Formatter();
		if (!pResult->Init(formatString)) {
			delete pResult;
			return NULL;
		}
		return pResult;
	}

	bool Init(string formatString) {
		string replacement = format("##__%s__##", STR(generateRandomString(5)));
		_formatString = formatString;

		//1. escape %%
		replace(_formatString, "%%", replacement);

		bool insideField = false;
		LogField *pTemp = new LogField();
		for (uint32_t i = 0; i < _formatString.length(); i++) {
			if (pTemp == NULL) {
				pTemp = new LogField;
			}
			if (_formatString[i] == '%') {
				if (insideField) {
					pTemp->isField = true;
					replace(pTemp->value, replacement, "%%");
					if (pTemp->value != "") {
						ADD_VECTOR_END(_fields, pTemp);
					}
					pTemp = NULL;
					insideField = false;
				} else {
					pTemp->isField = false;
					replace(pTemp->value, replacement, "%%");
					if (pTemp->value != "") {
						ADD_VECTOR_END(_fields, pTemp);
					}
					pTemp = NULL;
					insideField = true;
				}
			} else {
				pTemp->value += _formatString[i];
			}
		}
		if (pTemp != NULL) {
			pTemp->isField = false;
			replace(pTemp->value, replacement, "%%");
			if (pTemp->value != "") {
				ADD_VECTOR_END(_fields, pTemp);
			}
			pTemp = NULL;
		}

		return true;
	}

	string Format(Variant &le) {
		string result;
		for (uint32_t i = 0; i < _fields.size(); i++) {
			LogField *pTemp = _fields[i];
			if (!pTemp->isField) {
				result += pTemp->value;
			} else {
				if (le.HasKey(pTemp->value)) {
					if (le[pTemp->value] != V_NULL)
						result += (string) (le[pTemp->value]);
				} else if (le["carrier"].HasKey(pTemp->value)) {
					if (le["carrier"][pTemp->value] != V_NULL)
						result += (string) (le["carrier"][pTemp->value]);
				} else if (le["fields"].HasKey(pTemp->value)) {
					if (le["fields"][pTemp->value] != V_NULL)
						result += (string) (le["fields"][pTemp->value]);
				}
			}
		}
		return result;
	}
};

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

#endif /* HAS_SYSLOG */

