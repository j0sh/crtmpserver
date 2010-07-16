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


#ifndef _CORE_H
#define	_CORE_H

//1. Include the configuration
//#include "config.h"

//2. Include the defines
#include "defines.h"

//3. Detect and include the platform specific things
#include "platform/platform.h"

//4. Include the generic functionality
#include "utils/genericfunctionality.h"
#include "utils/timersmanager.h"

//5. Include the variant support
#include "utils/variant.h"

//6. Include fast linked list support
#include "utils/linkedlist.h"

//8. Include logging support
#include "logging/logger.h"
#include "logging/consoleloglocation.h"
#include "logging/fileloglocation.h"

#endif	/* _CORE_H */

