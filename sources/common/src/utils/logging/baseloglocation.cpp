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


#include "utils/logging/baseloglocation.h"

BaseLogLocation::BaseLogLocation(Variant &configuration) {
	_level = -1;
	_name = "";
	_configuration = configuration;
	_specificLevel = 0;
	_singleLine = false;
}

BaseLogLocation::~BaseLogLocation() {
}

int32_t BaseLogLocation::GetLevel() {
	return _level;
}

void BaseLogLocation::SetLevel(int32_t level) {
	_level = level;
}

string BaseLogLocation::GetName() {
	return _name;
}

void BaseLogLocation::SetName(string name) {
	_name = name;
}

bool BaseLogLocation::EvalLogLevel(int32_t level, string &fileName, uint32_t lineNumber,
		string &functionName, string &message) {
	return EvalLogLevel(level);
}

bool BaseLogLocation::EvalLogLevel(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, Variant &le) {
	return EvalLogLevel(level);
}

bool BaseLogLocation::Init() {
	if (_configuration.HasKeyChain(_V_NUMERIC, false, 1,
			CONF_LOG_APPENDER_SPECIFIC_LEVEL))
		_specificLevel = (int32_t) _configuration.GetValue(
			CONF_LOG_APPENDER_SPECIFIC_LEVEL, false);
	if (_configuration.HasKeyChain(V_BOOL, false, 1, CONF_LOG_APPENDER_SINGLE_LINE))
		_singleLine = (bool)_configuration.GetValue(
			CONF_LOG_APPENDER_SINGLE_LINE, false);
	return true;
}

bool BaseLogLocation::EvalLogLevel(int32_t level) {
	if (_specificLevel != 0) {
		if (_specificLevel != level)
			return false;
		return true;
	} else {
		if (_level < 0 || level > _level)
			return false;
		return true;
	}
}
