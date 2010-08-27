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

#ifdef HAS_PROTOCOL_RTP
#include "protocols/rtp/connectivity/baseconnectivity.h"

//#define LIVE555WAY

BaseConnectivity::BaseConnectivity() {

}

BaseConnectivity::~BaseConnectivity() {
}

uint32_t BaseConnectivity::ToRTPTS(struct timeval &tv, uint32_t rate) {
#ifdef LIVE555WAY
	u_int32_t timestampIncrement = (rate * tv.tv_sec);
	timestampIncrement += (u_int32_t) ((2.0 * rate * tv.tv_usec + 1000000.0) / 2000000);
	return timestampIncrement;
#else
	return (uint32_t) ((((double) tv.tv_sec * 1000000.00 + (double) tv.tv_usec) / 1000000.00)*(double) rate);
#endif
}

uint32_t BaseConnectivity::ToRTPTS(double milliseconds, uint32_t rate) {
#ifdef LIVE555WAY
	struct timeval tv;
	tv.tv_sec = (milliseconds / 1000.00);
	tv.tv_usec = (((uint32_t) milliseconds) % 1000)*1000;
	return ToRTPTS(tv, rate);
#else
	return (uint32_t) ((milliseconds / 1000.00)*(double) rate);
#endif
}

#endif /* HAS_PROTOCOL_RTP */
