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


#ifndef CONSOLELOGLOCATION_H_
#define CONSOLELOGLOCATION_H_

#include "utils/logging/baseloglocation.h"

/*!
	@class ConsoleLogLocation
	@brief Class that derives the BaseLogLocation class. This class is used to display the log messages in the console
*/
class DLLEXP ConsoleLogLocation
: public BaseLogLocation {
private:
	bool _allowColors;
	static vector<COLOR_TYPE> _colors;
public:
	/*! Constructor.
		@param allowColors - when set to true, the constructor assigns color on messages based on the level of warning
	*/
	ConsoleLogLocation(bool allowColors = true);
	virtual ~ConsoleLogLocation();

	/*! Logs the messages
		@param level: Variable that indicates how critical the log is about. It ranges from "INFO" to "FATAL".
		@param filename: Shows file name of the source code that displayed the log message.
		@param lineNumber: Shows line number in the source code that displayed the log message.
		@param functionName: Shows the name of the function that displayed the log message.
		@param message: Accepts the log message and displays it in the appropriate format.
	
	*/
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message);
};

#endif /*CONSOLELOGLOCATION_H_*/

