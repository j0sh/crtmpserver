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


#ifndef _FILELOGLOCATION_H
#define	_FILELOGLOCATION_H

#include "utils/logging/baseloglocation.h"

/*!
	@class FileLogLocation
	@brief Class that derives the BaseLogLocation class. This class is used to save the log messages in a file.
*/
class DLLEXP FileLogLocation
: public BaseLogLocation {
private:
	ofstream _fileStream; //! Output file stream
	bool _canLog; //! Variable that prevents logging. If file stream fails, this is set to false.
	uint32_t _counter; //! Counts the number of lines in the log file. To lessen HDD load, flushing of the file stream only happens when this value is divisible by 1000
public:
	/*! @brief Constructor: It checks if there is error in the file stream. If there are errors, _canLog will become false and logging will not be done.
		@param path - The path of the file where the logs will be saved
		@param append - Boolean that indicates whether to append the logs in the existing file or to create a new one.
	*/
	FileLogLocation(string path, bool append);
	virtual ~FileLogLocation();
private:
	/*! @brief Logs the messages
		@param level: Variable that indicates how critical the log is about. It ranges from "INFO" to "FATAL".
		@param filename: Shows file name of the source code that displayed the log message.
		@param lineNumber: Shows line number in the source code that displayed the log message.
		@param functionName: Shows the name of the function that displayed the log message.
		@param message: Accepts the log message and displays it in the appropriate format.
	
	*/
	virtual void Log(int32_t level, string fileName, uint32_t lineNumber,
			string functionName, string message);
};


#endif	/* _FILELOGLOCATION_H */


