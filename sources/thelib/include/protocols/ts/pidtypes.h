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


#ifdef HAS_PROTOCOL_TS
#ifndef _PIDTYPES_H
#define	_PIDTYPES_H

#include "utils/core.h"

typedef enum _PIDType {
    PID_TYPE_UNKNOWN = 0x00,
    PID_TYPE_PAT,
    PID_TYPE_PMT,
    PID_TYPE_NIT,
    PID_TYPE_AUDIOSTREAM,
    PID_TYPE_VIDEOSTREAM,
    PID_TYPE_NULL
} PIDType;

DLLEXP string GetPIDTypeString(PIDType pidType);

#endif	/* _PIDTYPES_H */
#endif	/* HAS_PROTOCOL_TS */

