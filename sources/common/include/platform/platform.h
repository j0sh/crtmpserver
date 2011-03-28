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

#include "platform/endianess/endianness.h"

#ifdef OSX
#include "platform/osx/osxplatform.h"
#define Platform OSXPlatform
#endif /* OSX */

#ifdef LINUX
#include "platform/linux/max.h"
#include "platform/linux/linuxplatform.h"
#define Platform LinuxPlatform
#endif /* LINUX */

#ifdef FREEBSD
#include "platform/freebsd/max.h"
#include "platform/freebsd/freebsdplatform.h"
#define Platform FreeBSDPlatform
#endif /* FREEBSD */

#ifdef OPENBSD
#include "platform/openbsd/max.h"
#include "platform/openbsd/openbsdplatform.h"
#define Platform OpenBSDPlatform
#endif /* OPENBSD */

#ifdef DFREEBSD
#include "platform/dfreebsd/max.h"
#include "platform/dfreebsd/dfreebsdplatform.h"
#define Platform DFreeBSDPlatform
#endif /* DFREEBSD */

#ifdef SOLARIS
#include "platform/solaris/solarisplatform.h"
#define Platform SolarisPlatform
#endif /* SOLARIS */

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "platform/windows/max.h"
#include "platform/windows/win32platform.h"
#define Platform Win32Platform
#endif /* WIN32 */

#ifdef ANDROID
#include "platform/android/max.h"
#include "platform/android/androidplatform.h"
#define Platform AndroidPlatform
#endif /* ANDROID */

#endif /* _PLATFORM_H */
