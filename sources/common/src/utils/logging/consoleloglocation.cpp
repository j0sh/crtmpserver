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


#include "utils/logging/consoleloglocation.h"

ConsoleLogLocation::ConsoleLogLocation(Variant &configuration)
: BaseLogLocation(configuration) {
	_allowColors = false;
	ADD_VECTOR_END(_colors, FATAL_COLOR);
	ADD_VECTOR_END(_colors, ERROR_COLOR);
	ADD_VECTOR_END(_colors, WARNING_COLOR);
	ADD_VECTOR_END(_colors, INFO_COLOR);
	ADD_VECTOR_END(_colors, DEBUG_COLOR);
	ADD_VECTOR_END(_colors, FINE_COLOR);
	ADD_VECTOR_END(_colors, FINEST_COLOR);
}

ConsoleLogLocation::~ConsoleLogLocation() {
	SET_CONSOLE_TEXT_COLOR(NORMAL_COLOR);
}

bool ConsoleLogLocation::Init() {
	if (!BaseLogLocation::Init()) {
		return false;
	}
	if (_configuration.HasKeyChain(V_BOOL, false, 1, CONF_LOG_APPENDER_COLORED))
		_allowColors = (bool)_configuration.GetValue(
			CONF_LOG_APPENDER_COLORED, false);
	return true;
}

void ConsoleLogLocation::Log(int32_t level, string fileName,
		uint32_t lineNumber, string functionName, string message) {
	if (_singleLine) {
		replace(message, "\r", "\\r");
		replace(message, "\n", "\\n");
	}
#ifdef ANDROID
	if (_allowColors) {
		printf("%s%s:%u %s%s\n",
				STR(_colors[level]),
				STR(fileName),
				lineNumber,
				STR(message),
				STR(_colors[6]));
	} else {
		printf("%s:%u %s\n",
				STR(fileName),
				lineNumber,
				STR(message));
	}
#else
	if (_allowColors) {
		SET_CONSOLE_TEXT_COLOR(_colors[level]);
		cout << fileName << ":" << lineNumber << " " << message;
		SET_CONSOLE_TEXT_COLOR(_colors[6]);
		cout << endl;
	} else {
		cout << fileName << ":" << lineNumber << " " << message << endl;
	}
#endif /* ANDROID */
}

void ConsoleLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, Variant &le) {
	return;
}

void ConsoleLogLocation::SignalFork() {

}
