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


#include "utils/logging/logger.h"
#include "utils/logging/baseloglocation.h"
#include "version.h"

Logger *Logger::_pLogger = NULL;

string Version::GetBuildNumber() {
#ifdef CRTMPSERVER_VERSION_BUILD_NUMBER
	return CRTMPSERVER_VERSION_BUILD_NUMBER;
#else /* EVOSTREAM_VERSION_BUILD_NUMBER */
	return "";
#endif /* EVOSTREAM_VERSION_BUILD_NUMBER */
}

uint64_t Version::GetBuildDate() {
#ifdef CRTMPSERVER_VERSION_BUILD_DATE
	return CRTMPSERVER_VERSION_BUILD_DATE;
#else /* EVOSTREAM_VERSION_BUILD_DATE */
	return 0;
#endif /* EVOSTREAM_VERSION_BUILD_DATE */
}

string Version::GetBuildDateString() {
	time_t buildDate = (time_t) GetBuildDate();
	if (buildDate == 0) {
		return "";
	}
	Timestamp *pTs = gmtime(&buildDate);
	Variant v(*pTs);
	return (string) v;
}

string Version::GetReleaseNumber() {
#ifdef CRTMPSERVER_VERSION_RELEASE_NUMBER
	return CRTMPSERVER_VERSION_RELEASE_NUMBER;
#else /* EVOSTREAM_VERSION_RELEASE_NUMBER */
	return "";
#endif /* EVOSTREAM_VERSION_RELEASE_NUMBER */
}

string Version::GetCodeName() {
#ifdef CRTMPSERVER_VERSION_CODE_NAME
	return CRTMPSERVER_VERSION_CODE_NAME;
#else /* EVOSTREAM_VERSION_CODE_NAME */
	return "";
#endif /* EVOSTREAM_VERSION_CODE_NAME */
}

string Version::GetBanner() {
	string result = HTTP_HEADERS_SERVER_US;
	if (GetReleaseNumber() != "")
		result += " version " + GetReleaseNumber();
	result += " build " + GetBuildNumber() + " - " + GetBuildDateString();
	if (GetCodeName() != "")
		result += " (" + GetCodeName() + ")";
	return result;
}

Variant Version::GetAll() {
	Variant result;
	result["buildNumber"] = (string) GetBuildNumber();
	result["buildDate"] = (uint64_t) GetBuildDate();
	result["releaseNumber"] = (string) GetReleaseNumber();
	result["codeName"] = (string) GetCodeName();
	result["banner"] = (string) GetBanner();
	return result;
}

#ifdef HAS_SAFE_LOGGER
pthread_mutex_t *Logger::_pMutex = NULL;

class LogLocker {
private:
	pthread_mutex_t *_pMutex;
public:

	LogLocker(pthread_mutex_t *pMutex) {
		if (pMutex == NULL) {
			printf("Logger not initialized\n");
			o_assert(false);
		}
		_pMutex = pMutex;
		if (pthread_mutex_lock(_pMutex) != 0) {
			printf("Unable to lock the logger");
			o_assert(false);
		}
	};

	virtual ~LogLocker() {
		if (pthread_mutex_unlock(_pMutex) != 0) {
			printf("Unable to unlock the logger");
			o_assert(false);
		}
	}
};
#define LOCK LogLocker __LogLocker__(Logger::_pMutex);
#else
#define LOCK
#endif /* HAS_SAFE_LOGGER */

Logger::Logger() {
	LOCK;
	_freeAppenders = false;
}

Logger::~Logger() {
	LOCK;
	if (_freeAppenders) {

		FOR_VECTOR(_logLocations, i) {
			delete _logLocations[i];
		}
		_logLocations.clear();
	}
}

void Logger::Init() {
#ifdef HAS_SAFE_LOGGER
	if (_pMutex != NULL) {
		printf("logger already initialized");
		o_assert(false);
	}
	_pMutex = new pthread_mutex_t;
	if (pthread_mutex_init(_pMutex, NULL)) {
		printf("Unable to init the logger mutex");
		o_assert(false);
	}
#else
	if (_pLogger != NULL)
		return;
#endif /* HAS_SAFE_LOGGER */
	_pLogger = new Logger();
}

void Logger::Free(bool freeAppenders) {
	LOCK;
	if (_pLogger != NULL) {
		_pLogger->_freeAppenders = freeAppenders;
		delete _pLogger;
		_pLogger = NULL;
	}
}

void Logger::Log(int32_t level, string fileName, uint32_t lineNumber,
		string functionName, string formatString, ...) {
	LOCK;
	if (_pLogger == NULL)
		return;

	va_list arguments;
	va_start(arguments, formatString);
	string message = vFormat(formatString, arguments);
	va_end(arguments);

	FOR_VECTOR(_pLogger->_logLocations, i) {
		if (_pLogger->_logLocations[i]->EvalLogLevel(level, fileName, lineNumber,
				functionName, formatString))
			_pLogger->_logLocations[i]->Log(level, fileName,
				lineNumber, functionName, message);
	}
}

void Logger::LogProd(int32_t level, string fileName, uint32_t lineNumber, string functionName, Variant &le) {
	LOCK;
	if (_pLogger == NULL)
		return;

	FOR_VECTOR(_pLogger->_logLocations, i) {
		if (_pLogger->_logLocations[i]->EvalLogLevel(level, fileName, lineNumber,
				functionName, le))
			_pLogger->_logLocations[i]->Log(level, fileName,
				lineNumber, functionName, le);
	}
}

bool Logger::AddLogLocation(BaseLogLocation *pLogLocation) {
	LOCK;
	if (_pLogger == NULL)
		return false;
	if (!pLogLocation->Init())
		return false;
	ADD_VECTOR_END(_pLogger->_logLocations, pLogLocation);
	return true;
}

void Logger::SignalFork() {
	LOCK;
	if (_pLogger == NULL)
		return;

	FOR_VECTOR(_pLogger->_logLocations, i) {
		_pLogger->_logLocations[i]->SignalFork();
	}
}

void Logger::SetLevel(int32_t level) {
	LOCK;
	if (_pLogger == NULL)
		return;

	FOR_VECTOR(_pLogger->_logLocations, i) {
		_pLogger->_logLocations[i]->SetLevel(level);
	}
}
