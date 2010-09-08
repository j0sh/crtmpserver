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
	EnvRun("0.0.0.0", 5544, ci, _FINEST_);
#else
	EnvRun("0.0.0.0", 5544, _FINEST_);
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

//#define MY_URL "http://localhost/~shiretu/ts10/200/prog_index.m3u8"
//#define MY_KEY ""
//#define MY_SESSION_ID ""

#define MY_URL "http://localhost/~shiretu/ts6_raw/master.m3u8"
#define MY_KEY ""
#define MY_SESSION_ID ""

void testCommands() {
	Variant result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));

	uint32_t contextId = (uint32_t) ASC_RES_PARAM(result, "contextId");
	result = CommandSetBitrates(contextId, "[350000]");
	FINEST("result:\n%s", STR(result.ToString()));

	result = CommandPlay(contextId, MY_URL, MY_SESSION_ID, MY_KEY);
	FINEST("result:\n%s", STR(result.ToString()));

	//	result = CommandPlay(contextId,
	//			"aHR0cDovL21sYmxpdmUtYWtjLm1sYi5jb20vbWxiYW0vMjAxMC8wOC8yOC9NTEJfR0FNRV9WSURFT19LQ0FDTEVfSE9NRV8yMDEwMDgyOC9tYXN0ZXJfbW9iaWxlLm0zdTh8R3ZjbFJoV2lOMHlyZWcvL3NUeXJBNDJ4VXkwPXxwbGF5YmFjaz1IVFRQX0NMT1VEX01PQklMRSZjb250ZW50SWQ9MTEyNzU3MDkmYXBwQWNjb3VudE5hbWU9bWxiJmV2ZW50SWQ9MTQtMjY1NzQwLTIwMTAtMDgtMjgmaXBpZD0xMTYyNzcwNiZzZXNzaW9uS2V5PUFlMHNDUUFNUjJ2ZzQ2dUduT28xJTJGbVVYbjdFJTNEJmRldmljZUlkPUFORFJPSURfQTEwMDAwMTM3NTY2MDUmY291bnRyeT1ybyZwb3N0YWxDb2RlPTAwMDAwJnBsYXRmb3JtPUFORFJPSUQ="
	//			);
	//	FINEST("result:\n%s", STR(result.ToString()));

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
