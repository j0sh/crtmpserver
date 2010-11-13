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
#include "tsappprotocolhandler.h"
#include "application/baseclientapplication.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/rtmp/sharedobjects/so.h"
#include "protocols/rtmp/sharedobjects/somanager.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/protocoltypes.h"
using namespace app_flvplayback;

TSAppProtocolHandler::TSAppProtocolHandler(Variant &configuration)
: BaseTSAppProtocolHandler(configuration) {

}

TSAppProtocolHandler::~TSAppProtocolHandler() {
}
#endif	/* HAS_PROTOCOL_TS */

