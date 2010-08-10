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


#ifndef _LOGGING_H
#define _LOGGING_H

#include "defines.h"
#include "utils/logging/baseloglocation.h"
#include "utils/logging/consoleloglocation.h"
#include "utils/logging/fileloglocation.h"
#include "utils/logging/logcatloglocation.h"
#include "utils/logging/logger.h"

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

#endif /* _LOGGING_H */
