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

#ifdef HAS_PROTOCOL_RTMP
#include "protocols/rtmp/messagefactories/somessagefactory.h"
#include "protocols/rtmp/header.h"

Variant SOMessageFactory::GetSharedObject(uint32_t channelId, uint32_t streamId,
		double timeStamp, bool isAbsolute, string name, uint32_t version,
		bool persistent) {
	Variant result;

	VH(result, HT_FULL, channelId, timeStamp, 0,
			RM_HEADER_MESSAGETYPE_SHAREDOBJECT, streamId, isAbsolute);

	result[RM_SHAREDOBJECT][RM_SHAREDOBJECT_NAME] = name;
	result[RM_SHAREDOBJECT][RM_SHAREDOBJECT_VERSION] = version;
	result[RM_SHAREDOBJECT][RM_SHAREDOBJECT_PERSISTENCE] = persistent;

	return result;
}
#endif /* HAS_PROTOCOL_RTMP */

