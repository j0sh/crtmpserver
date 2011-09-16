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

#ifdef ANDROID
#include "utils/logging/logcatloglocation.h"
#include <android/log.h>

int LogCatLogLocation::_levelsMap[] = {
	ANDROID_LOG_FATAL,
	ANDROID_LOG_ERROR,
	ANDROID_LOG_WARN,
	ANDROID_LOG_INFO,
	ANDROID_LOG_DEBUG,
	ANDROID_LOG_VERBOSE,
	ANDROID_LOG_VERBOSE
};

LogCatLogLocation::LogCatLogLocation(Variant &configuration)
: BaseLogLocation(configuration) {

}

LogCatLogLocation::~LogCatLogLocation() {
}

void LogCatLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string message) {
	if (_level < 0 || level > _level) {
		return;
	}

	__android_log_write(_levelsMap[level], "rtmpd",
			STR(format("%s:%u %s", STR(fileName), lineNumber, STR(message))));
}

void LogCatLogLocation::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, Variant &le) {
	return;
}

void LogCatLogLocation::SignalFork() {

}

#endif /* ANDROID */
