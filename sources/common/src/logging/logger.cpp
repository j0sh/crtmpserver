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


#include "logging/logger.h"
#include "logging/baseloglocation.h"



Logger *Logger::_pLogger = NULL;

Logger::Logger() {
    _freeAppenders = false;
}

Logger::~Logger() {
    if (_freeAppenders) {

        FOR_VECTOR(_logLocations, i) {
            delete _logLocations[i];
        }
        _logLocations.clear();
    }
}

void Logger::Init() {
    if (_pLogger != NULL)
        return;
    _pLogger = new Logger();
}

void Logger::Free(bool freeAppenders) {
    if (_pLogger != NULL) {
        _pLogger->_freeAppenders = freeAppenders;
        delete _pLogger;
        _pLogger = NULL;
    }
}

void Logger::Log(int32_t level, string fileName, uint32_t lineNumber,
        string functionName, string formatString, ...) {
    if (_pLogger == NULL)
        return;

    va_list arguments;
    va_start(arguments, formatString);
    string message = vformat(formatString, arguments);
    va_end(arguments);

    FOR_VECTOR(_pLogger->_logLocations, i) {
        _pLogger->_logLocations[i]->Log(level, fileName,
                lineNumber, functionName, message);
    }
}

bool Logger::AddLogLocation(BaseLogLocation *pLogLocation) {
    if (_pLogger == NULL)
        return false;
    ADD_VECTOR_END(_pLogger->_logLocations, pLogLocation);
    return true;
}

