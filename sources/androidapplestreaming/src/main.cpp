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

void* worker(void *);

int main(void) {
	//4. Start the worker thread
	pthread_t threadId;
	int32_t error = 0;
	if ((error = pthread_create(&threadId, NULL, worker, NULL)) != 0) {
		return -1;
	}
	sleep(2);
	Variant result;

	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextClose(1444);
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextClose(1);
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextCloseAll();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextList();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = CommandPlay(6,
			"http://mlbvod-akc.mlb.com/mlbam/2010/04/08/MLB_GAME_VIDEO_LANPIT_HOME_20100408/master_wired.m3u8",
			"playback=HTTP_CLOUD_WIRED&contentId=7320085&appAccountName=mlb&eventId=14-263849-2010-04-08&ipid=11627706&sessionKey=Jbptc%2BZDsFBHf4cgNotFuJV0WLc%3D",
			"AJ2yhe6d0gdlosKJLLITvziVFsY=");
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = CommandPause(9101);
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = CommandResume(1213);
	FINEST("result:\n%s", STR(result.ToString()));

	sleep(2000);

	EnvStop();
	sleep(2);
	return 0;
}

void* worker(void *) {
	EnvRun("0.0.0.0", 5544);
	return NULL;
}
