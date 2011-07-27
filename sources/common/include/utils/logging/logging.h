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
#include "utils/logging/syslogloglocation.h"
#include "utils/logging/logeventfactory.h"
#include "utils/logging/logger.h"



#ifdef VALIDATE_FROMAT_SPECIFIERS
#define __VALIDATE_FROMAT_SPECIFIERS(...) \
do { \
   char ___tempLocation[1024]; \
   snprintf(___tempLocation,1023,__VA_ARGS__); \
}while (0)
#else
#define __VALIDATE_FROMAT_SPECIFIERS(...)
#endif /* VALIDATE_FROMAT_SPECIFIERS */

#define LOG(level,...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(level, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define FATAL(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_FATAL_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)

#define WARN(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_WARNING_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define INFO(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_INFO_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define DEBUG(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_DEBUG_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define FINE(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_FINE_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define FINEST(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_FINEST_, __FILE__, __LINE__, __func__, __VA_ARGS__);}while(0)
#define ASSERT(...) do{__VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);Logger::Log(_FATAL_, __FILE__, __LINE__, __func__, __VA_ARGS__);assert(false);abort();}while(0)
#define NYI WARN("%s not yet implemented",__func__);
#define NYIR do{NYI;return false;}while(0)
#define NYIA do{NYI;assert(false);abort();}while(0)
#define _PROD_ACCESS_ (_FATAL_+1)
#define _PROD_ERROR_ (_PROD_ACCESS_+1)
#define PROD_GENERIC(level, le) Logger::LogProd(_PROD_ACCESS_, __FILE__, __LINE__, __func__, le)
#define PROD_ACCESS(le) PROD_GENERIC(_PROD_ACCESS_,le)
#define PROD_ERROR(le) PROD_GENERIC(_PROD_ERROR_,le)
#endif /* _LOGGING_H */
