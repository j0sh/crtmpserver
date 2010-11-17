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


#ifndef _NETIO_H
#define	_NETIO_H

#ifdef NET_KQUEUE
#include "netio/kqueue/iohandler.h"
#include "netio/kqueue/iohandlermanager.h"
#include "netio/kqueue/iohandlermanagertoken.h"
#include "netio/kqueue/iotimer.h"
#include "netio/kqueue/tcpacceptor.h"
#include "netio/kqueue/tcpcarrier.h"
#include "netio/kqueue/udpcarrier.h"
#include "netio/kqueue/inboundnamedpipecarrier.h"
#include "netio/kqueue/tcpconnector.h"
#include "netio/kqueue/stdiocarrier.h"
#ifdef HAS_KQUEUE_TIMERS
#define NETWORK_REACTOR "kqueue with EVFILT_TIMER support"
#else
#define NETWORK_REACTOR "kqueue without EVFILT_TIMER support"
#endif
#endif

#ifdef NET_EPOLL
#include "netio/epoll/iohandler.h"
#include "netio/epoll/iohandlermanager.h"
#include "netio/epoll/iohandlermanagertoken.h"
#include "netio/epoll/iotimer.h"
#include "netio/epoll/tcpacceptor.h"
#include "netio/epoll/tcpcarrier.h"
#include "netio/epoll/udpcarrier.h"
#include "netio/epoll/tcpconnector.h"
#include "netio/epoll/inboundnamedpipecarrier.h"
#include "netio/epoll/stdiocarrier.h"
#define NETWORK_REACTOR "epoll"
#endif

#ifdef NET_SELECT
#include "netio/select/iohandler.h"
#include "netio/select/iohandlermanager.h"
#include "netio/select/iotimer.h"
#include "netio/select/tcpacceptor.h"
#include "netio/select/tcpcarrier.h"
#include "netio/select/udpcarrier.h"
#include "netio/select/inboundnamedpipecarrier.h"
#include "netio/select/tcpconnector.h"
#include "netio/select/stdiocarrier.h"
#define NETWORK_REACTOR "select"
#endif

#ifdef NET_IOCP
#include "netio/iocp/iohandler.h"
#include "netio/iocp/iohandlermanager.h"
#include "netio/iocp/iohandlermanagertoken.h"
#include "netio/iocp/iotimer.h"
#include "netio/iocp/tcpacceptor.h"
#include "netio/iocp/tcpcarrier.h"
#include "netio/iocp/udpcarrier.h"
#include "netio/iocp/tcpconnector.h"
#define NETWORK_REACTOR "iocp"
#endif

#ifdef NET_IOCP2
#include "netio/iocp2/iohandler.h"
#include "netio/iocp2/iohandlermanager.h"
#include "netio/iocp2/iohandlermanagertoken.h"
#include "netio/iocp2/iotimer.h"
#include "netio/iocp2/tcpacceptor.h"
#include "netio/iocp2/tcpcarrier.h"
#include "netio/iocp2/udpcarrier.h"
#include "netio/iocp2/tcpconnector.h"
#include "netio/iocp2/stdiocarrier.h"
#define NETWORK_REACTOR "iocp2"
#endif

#endif	/* _NETIO_H */


