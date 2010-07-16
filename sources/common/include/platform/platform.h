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


#ifndef _PLATFORM_H
#define	_PLATFORM_H

#include "platform/max.h"
#include "endianness/endianness.h"

#ifdef OSX
#include "platform/osx/types.h"
#include "platform/osx/stl.h"
#include "platform/osx/misc.h"
#include "platform/osx/networking.h"
#include "platform/max.h"
#endif /* OSX */

#ifdef LINUX
#include "platform/linux/types.h"
#include "platform/linux/stl.h"
#include "platform/linux/misc.h"
#include "platform/linux/networking.h"
#include "platform/max.h"
#endif /* LINUX */

#ifdef FREEBSD
#include "platform/freebsd/types.h"
#include "platform/freebsd/stl.h"
#include "platform/freebsd/misc.h"
#include "platform/freebsd/networking.h"
#include "platform/max.h"
#endif /* FREEBSD */

#ifdef SOLARIS
#include "platform/solaris/types.h"
#include "platform/solaris/stl.h"
#include "platform/solaris/misc.h"
#include "platform/solaris/networking.h"
#include "platform/max.h"
#endif /* SOLARIS */

#ifdef WIN32
#include "platform/win32/types.h"
#include "platform/win32/stl.h"
#include "platform/win32/misc.h"
#include "platform/win32/networking.h"
#include "platform/max.h"
#endif /* WIN32 */

#ifdef ANDROID
#include "platform/android/types.h"
#include "platform/android/stl.h"
#include "platform/android/misc.h"
#include "platform/android/networking.h"
#endif /* ANDROID */

#endif	/* _PLATFORM_H */


