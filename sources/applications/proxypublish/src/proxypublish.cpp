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


#include "application/baseclientapplication.h"
#include "proxypublish.h"
#include "proxypublishapplication.h"

extern "C" BaseClientApplication *GetApplication(Variant configuration) {
	ProxyPublishApplication *pApplication = new ProxyPublishApplication(
			configuration);
	return pApplication;
}

extern "C" void ReleaseApplication(BaseClientApplication *pApplication) {
	if (pApplication != NULL) {
		delete pApplication;
	}
}


/*OLD STUFF
 * rtspStreams =
			{
				--[[{
					--url="rtsp://82.177.67.58/channel1",
					--url="rtsp://217.153.33.206/axis-media/media.amp",
					url="rtsp://82.177.67.61/axis-media/media.amp",
					--url="rtsp://media-us-2.soundreach.net/slcn_lifestyle.sdp",
					--url="rtsp://0.0.0.0:8080/test.sdp",
					forceTcp=true
				},]]--
			},
 */

