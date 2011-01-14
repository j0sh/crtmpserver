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


#ifndef BASELOGLOCATION_H_
#define BASELOGLOCATION_H_

#include "platform/platform.h"

/*!
	@class BaseLogLocation
	@brief Base class that all logging must derive from.
*/
class DLLEXP BaseLogLocation {
protected:
	int32_t _level;
	string _name;
public:
	BaseLogLocation();
	virtual ~BaseLogLocation();

	/*!
		@brief Returns the level of logging
	*/
	int32_t GetLevel();

	/*!
		@brief Sets the level of logging
		@param level - Level of logging
	*/
	void SetLevel(int32_t level);

	/*!
		@brief Gets the name of the log message based on its level
	*/
	string GetName();

	/*!
		@brief Sets the name of the type of log message
		@param name
	*/
	void SetName(string name);

	/*! @brief Logs the message
		@param level: Variable that indicates how critical the log is about. It ranges from "INFO" to "FATAL".
		@param filename: Shows file name of the source code that displayed the log message.
		@param lineNumber: Shows line number in the source code that displayed the log message.
		@param functionName: Shows the name of the function that displayed the log message.
		@param message: Accepts the log message and displays it in the appropriate format.
	
	*/
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message) = 0;
};


#endif /*BASELOGLOCATION_H_*/

