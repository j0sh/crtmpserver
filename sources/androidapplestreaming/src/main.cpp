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


#include "api.h"
#include <pthread.h>
#include "protocols/variant/messagestructure.h"

void* worker(void *);

void testContexts();
void testCommands();

int main(void) {
	//4. Start the worker thread
	pthread_t threadId;
	int32_t error = 0;
	if ((error = pthread_create(&threadId, NULL, worker, NULL)) != 0) {
		return -1;
	}
	sleep(2);

	//testContexts();
	testCommands();

	sleep(2000);

	EnvStop();
	sleep(2);
	return 0;
}

void* worker(void *) {
#ifdef ANDROID
	CallBackInfo ci;
	EnvRun("0.0.0.0", 5544, ci);
#else
	EnvRun("0.0.0.0", 5544);
#endif /* ANDROID */
	return NULL;
}

void testContexts() {
	Variant result;
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextClose(1444);
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextClose(1);
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCloseAll();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
}

//#define MY_URL "http://mediadownloads.mlb.com/mlbam/2010/06/29/9505835_m3u8/128/dropf_9505835_100m_128K.m3u8"
//#define MY_SESSION_ID ""
//#define MY_KEY ""

//#define MY_URL "http://mlbvod-akc.mlb.com/mlbam/2010/04/08/MLB_GAME_VIDEO_LANPIT_HOME_20100408/master_wired.m3u8"
//#define MY_SESSION_ID "playback=HTTP_CLOUD_WIRED&contentId=7320085&appAccountName=mlb&eventId=14-263849-2010-04-08&ipid=11627706&sessionKey=3HJ3abDj%2BPBkmIn8cprQTgkYIa8%3D"
//#define MY_KEY "ugkuOk9Qy4HNS5uRxQcQXHfLuAI="

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/17/MLB_GAME_VIDEO_DETNYA_HOME_20100817/master_mobile.m3u8"
//#define MY_SESSION_ID "playback=HTTP_CLOUD_WIRED&contentId=7320085&appAccountName=mlb&eventId=14-263849-2010-04-08&ipid=11627706&sessionKey=vlPZfM8Rj1TaGDmMrtDSQh1MIWc%3D"
//#define MY_KEY "Q8TZUMmOkyVgVSMotTCRMQ=="

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/17/MLB_GAME_VIDEO_DETNYA_HOME_20100817/master_mobile.m3u8"
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=10111969&appAccountName=mlb&eventId=14-265611-2010-08-18&ipid=23272205&sessionKey=JfPr0nOJ%2BfVzHc%2BfNV9umf9p6Yw%3D&country=us&postalCode=10292&platform=ANDROID"
//#define MY_KEY "7pG+lxgC/bRQ44iXCb0ydoVDCJA="

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/17/MLB_GAME_VIDEO_DETNYA_HOME_20100817/master_mobile.m3u8"
//#define MY_KEY "7pG+lxgC/bRQ44iXCb0ydoVDCJA="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=10111969&appAccountName=mlb&eventId=14-265611-2010-08-18&ipid=23272205&sessionKey=JfPr0nOJ%2BfVzHc%2BfNV9umf9p6Yw%3D&country=us&postalCode=10292&platform=ANDROID"

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/17/MLB_GAME_VIDEO_DETNYA_HOME_20100817/master_mobile.m3u8"
//#define MY_KEY "fzi/ekWlN/nJY7M7zyODvnWvwCk="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=10111969&appAccountName=mlb&eventId=14-265611-2010-08-18&ipid=23272205&sessionKey=lCzgmwnNPsLrnlcqm8bZ8Fk6GEM%3D&country=us&postalCode=10292&platform=ANDROID"


//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/23/MLB_GAME_VIDEO_TBAANA_HOME_20100823/master_mobile.m3u8"
//#define MY_KEY "adCJvZ98pHD+RtwZXu5DAnud3zk="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=100000000001&appAccountName=mlb&eventId=14-265677-2010-08-23&ipid=23272205&sessionKey=1Mdu%2B0CADyV4ewOJTgKsPSqMPQE%3D&deviceId=ANDROID_354957033071773&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/17/MLB_GAME_VIDEO_DETNYA_HOME_20100817/master_mobile.m3u8"
//#define MY_KEY "pFD3++v0LKfm38F2mCgIu0sXzn4="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=10111969&appAccountName=mlb&eventId=14-265611-2010-08-18&ipid=23272205&sessionKey=6bdsqzUU7oBI4gixJxFFON2QHwA%3D&deviceId=ANDROID_354957033071773&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://mlbsegqa.mlb.com/mlbam/2010/08/24/MLB_GAME_VIDEO_TBAANA_HOME_20100824/master_mobile.m3u8"
//#define MY_KEY "pFD3++v0LKfm38F2mCgIu0sXzn4="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=100000000087&appAccountName=mlb&eventId=14-265692-2010-08-24&ipid=23272205&sessionKey=6bdsqzUU7oBI4gixJxFFON2QHwA%3D&deviceId=ANDROID_354957033071773&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://mlblive-akc.mlb.com/mlbam/2010/08/25/MLB_GAME_VIDEO_KCADET_HOME_20100825/master_mobile.m3u8"
//#define MY_KEY "kL8KOUYTmywS7SiCmJmAvJjH51E="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=11089833&appAccountName=mlb&eventId=14-265702-2010-08-25&ipid=11627706&sessionKey=Pff4k8IQCTNjs46Plh6%2BBOvMEBw%3D&deviceId=ANDROID_356698030670609&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://mlblive-akc.mlb.com/mlbam/2010/08/25/MLB_GAME_VIDEO_ATLCOL_HOME_20100825/master_mobile.m3u8"
//#define MY_KEY "kL8KOUYTmywS7SiCmJmAvJjH51E="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=11311045&appAccountName=mlb&eventId=14-265696-2010-08-25&ipid=11627706&sessionKey=Pff4k8IQCTNjs46Plh6%2BBOvMEBw%3D&deviceId=ANDROID_A0000022419B22&country=us&postalCode=10292&platform=ANDROID"

//#define MY_URL "http://mlblive-akc.mlb.com/mlbam/2010/08/25/MLB_GAME_VIDEO_CINSFN_HOME_20100825/master_mobile.m3u8"
//#define MY_KEY "kL8KOUYTmywS7SiCmJmAvJjH51E="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=11090005&appAccountName=mlb&eventId=14-265699-2010-08-25&ipid=11627706&sessionKey=Pff4k8IQCTNjs46Plh6%2BBOvMEBw%3D&deviceId=ANDROID_A0000022419B22&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://mlblive-akc.mlb.com/mlbam/2010/08/25/MLB_GAME_VIDEO_CHNWAS_HOME_20100825/master_mobile.m3u8"
//#define MY_KEY "kL8KOUYTmywS7SiCmJmAvJjH51E="
//#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=11090061&appAccountName=mlb&eventId=14-265698-2010-08-25&ipid=11627706&sessionKey=Pff4k8IQCTNjs46Plh6%2BBOvMEBw%3D&deviceId=ANDROID_A0000022419B22&country=us&postalCode=10011&platform=ANDROID"

//#define MY_URL "http://localhost/~shiretu/ts10/200/prog_index.m3u8"
//#define MY_KEY ""
//#define MY_SESSION_ID ""

#define MY_URL "http://mlblive-akc.mlb.com/mlbam/2010/08/26/MLB_GAME_VIDEO_ARISDN_HOME_20100826/master_mobile.m3u8"
#define MY_KEY "Ey5xHlceWWeYwHOlMRuYMm/DZqE="
#define MY_SESSION_ID "playback=HTTP_CLOUD_MOBILE&contentId=11131241&appAccountName=mlb&eventId=14-265710-2010-08-26&ipid=11627706&sessionKey=8EcVAKWRJQVyLjuxvLCJXgz6aSI%3D&deviceId=ANDROID_null&country=us&postalCode=10011&platform=ANDROID"

//#define CONDENSED_CONN_STRING "base64:aHR0cDovL21sYnZvZC1ha2MubWxiLmNvbS9tbGJhbS8yMDEwLzA0LzA4L01MQl9HQU1FX1ZJREVPX0xBTlBJVF9IT01FXzIwMTAwNDA4L21hc3Rlcl93aXJlZC5tM3U4fHlNQmVFcEl6OEJEaUFYV0k0dUdIUm1UaHdIRT18cGxheWJhY2s9SFRUUF9DTE9VRF9XSVJFRCZjb250ZW50SWQ9NzMyMDA4NSZhcHBBY2NvdW50TmFtZT1tbGImZXZlbnRJZD0xNC0yNjM4NDktMjAxMC0wNC0wOCZpcGlkPTExNjI3NzA2JnNlc3Npb25LZXk9cUNveGRIaUlOY3FDbHlEcEZoU2dma3dJSWc0JTNE"
//#define CONDENSED_CONN_STRING "aHR0cDovL21sYnZvZC1ha2MubWxiLmNvbS9tbGJhbS8yMDEwLzA0LzA4L01MQl9HQU1FX1ZJREVPX0xBTlBJVF9IT01FXzIwMTAwNDA4L21hc3Rlcl93aXJlZC5tM3U4fGJsOEQyRUFTRUY4V1VyeloreTNvME5pUXlBaz18cGxheWJhY2s9SFRUUF9DTE9VRF9XSVJFRCZjb250ZW50SWQ9NzMyMDA4NSZhcHBBY2NvdW50TmFtZT1tbGImZXZlbnRJZD0xNC0yNjM4NDktMjAxMC0wNC0wOCZpcGlkPTExNjI3NzA2JnNlc3Npb25LZXk9R0JuY1BzeFJCVjAySUM5bjgwUlAxU2RyOWV3JTNE"

void testCommands() {
	Variant result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));

	uint32_t contextId = (uint32_t) ASC_RES_PARAM(result, "contextId");
	result = CommandSetBitrates(contextId, " [800000]");
	FINEST("result:\n%s", STR(result.ToString()));

	result = CommandPlay(contextId, MY_URL, MY_SESSION_ID, MY_KEY);
	FINEST("result:\n%s", STR(result.ToString()));

	//	sleep(30);
	//
	//	ContextCloseAll();
	//	result = CommandPlay(contextId, CONDENSED_CONN_STRING);
	//	FINEST("result:\n%s", STR(result.ToString()));

	//	for (;;) {
	//		result = InfoBandwidth(contextId);
	//		FINEST("InfoBandwidth:\n%s", STR(result.ToString()));
	//		//		result = InfoPlayback(contextId);
	//		//		FINEST("InfoBandwidth:\n%s", STR(result.ToString()));
	//		sleep(1);
	//	}

	//	sleep(5);
	//	result = InfoListStreams(contextId);
	//	FINEST("result:\n%s", STR(result.ToString()));
	//
	//	sleep(5);
	//	result = InfoListAllStreams();
	//	FINEST("result:\n%s", STR(result.ToString()));
	//
	//	result = CommandPause(9101);
	//	FINEST("result:\n%s", STR(result.ToString()));
	//	//sleep(1);
	//	result = CommandResume(1213);
	//	FINEST("result:\n%s", STR(result.ToString()));
}
