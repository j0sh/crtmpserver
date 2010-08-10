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
#define _PLATFORM_H

#include "new/platform/endianess/endianness.h"

#ifdef OSX
#include "new/platform/osx/osxplatform.h"
#define Platform OSXPlatform
#endif /* OSX */

#ifdef LINUX
#include "new/platform/linux/max.h"
#include "new/platform/linux/linuxplatform.h"
#define Platform LinuxPlatform
#endif /* LINUX */

#ifdef FREEBSD
#include "new/platform/freebsd/max.h"
#include "new/platform/freebsd/freebsdplatform.h"
#define Platform FreeBSDPlatform
#endif /* FREEBSD */

#ifdef SOLARIS
#include "new/platform/solaris/solarisplatform.h"
#define Platform SolarisPlatform
#endif /* SOLARIS */

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "new/platform/windows/max.h"
#include "new/platform/windows/win32platform.h"
#define Platform Win32Platform
#endif /* WIN32 */

#ifdef ANDROID
#include "new/platform/android/max.h"
#include "new/platform/android/androidplatform.h"
#define Platform AndroidPlatform
#endif /* ANDROID */

#endif /* _PLATFORM_H */
