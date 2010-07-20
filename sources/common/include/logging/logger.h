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


#ifndef LOGGER_H_
#define LOGGER_H_

#include "utils/core.h"
#ifdef HAS_SAFE_LOGGER
#include <pthread.h>
#endif /* HAS_SAFE_LOGGER */

#define LOG(level,...) Logger::Log(level, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define FATAL(...) Logger::Log(_FATAL_, __FILE__, __LINE__, __func__, __VA_ARGS__)
//TODO: Find a better way for naming this ERR thing.
//#define ERR(...) Logger::Log(_ERROR_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARN(...) Logger::Log(_WARNING_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define INFO(...) Logger::Log(_INFO_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define DEBUG(...) Logger::Log(_DEBUG_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define FINE(...) Logger::Log(_FINE_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define FINEST(...) Logger::Log(_FINEST_, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ASSERT(...) {Logger::Log(_FATAL_, __FILE__, __LINE__, __func__, __VA_ARGS__);assert(false);abort();}
#define NYI WARN("%s not yet implemented",__func__);
#define NYIR {NYI;return false;}
#define NYIA {NYI;assert(false);abort();}

class BaseLogLocation;

class DLLEXP Logger {
private:
	static Logger *_pLogger;
	vector<BaseLogLocation *> _logLocations;
	bool _freeAppenders;
public:
#ifdef HAS_SAFE_LOGGER
	static pthread_mutex_t *_pMutex;
#endif
private:
	Logger();
public:
	virtual ~Logger();

	static void Init();
	static void Free(bool freeAppenders);
	static void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string formatString, ...);
	static bool AddLogLocation(BaseLogLocation *pLogLocation);
};


#endif /*LOGGER_H_*/

