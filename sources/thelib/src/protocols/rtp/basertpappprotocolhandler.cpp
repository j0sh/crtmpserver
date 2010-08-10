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
#include "protocols/rtp/basertpappprotocolhandler.h"

BaseRTPAppProtocolHandler::BaseRTPAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseRTPAppProtocolHandler::~BaseRTPAppProtocolHandler() {
}

void BaseRTPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
}

void BaseRTPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
}

#endif /* HAS_PROTOCOL_RTP */
