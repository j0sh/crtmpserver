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


#ifndef _LOGGER_H
#define _LOGGER_H

#include "platform/platform.h"
#ifdef HAS_SAFE_LOGGER
#include <pthread.h>
#endif /* HAS_SAFE_LOGGER */

class BaseLogLocation;

/*!
	@class Logger
	@brief Class that implements logging capabilites of the Evostream server.
*/
class DLLEXP Logger {
private:
	static Logger *_pLogger; //! Pointer to the Logger class.
	vector<BaseLogLocation *> _logLocations; //! Vector that stores the location of the log file.
	bool _freeAppenders; //! Boolean that releases the logger.
#ifdef HAS_SAFE_LOGGER
public:
	static pthread_mutex_t *_pMutex;
#endif
public:
	Logger();
	virtual ~Logger();

	/*! @brief Initiates the logger */
	static void Init();
	/*! @brief Releases the logger
		@param freeApenders: Releases the logger when set to @c true
	*/
	static void Free(bool freeAppenders);

	/*! @brief Writes the log messages to the logger
		@param level: Variable that indicates how critical the log is about. It ranges from "INFO" to "FATAL".
		@param filename: Shows file name of the source code that displayed the log message.
		@param lineNumber: Shows line number in the source code that displayed the log message.
		@param functionName: Shows the name of the function that displayed the log message.
		@param formatString: Accepts the log message and displays it in the appropriate format.
	*/
	static void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string formatString, ...);

    static void LogProd(int32_t level, string fileName, uint32_t lineNumber, string functionName, Variant &le);
	/*! @brief Allows saving of the logs in a specified location
		@param plogLocation: Pointer to the BaseLogLocation
		@sa BaseLogLocation
	*/
	static bool AddLogLocation(BaseLogLocation *pLogLocation);
};


#endif /* _LOGGER_H */

