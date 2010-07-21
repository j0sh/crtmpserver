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
	EnvRun("0.0.0.0", 5544);
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

#define MY_URL "http://mediadownloads.mlb.com/mlbam/2010/06/29/9505835_m3u8/128/dropf_9505835_100m_128K.m3u8"
#define MY_SESSION_ID ""
#define MY_KEY ""

void testCommands() {
	Variant result = ContextCreate();
	FINEST("result:\n%s", STR(result.ToString()));
	result = CommandPlay((uint32_t) ASC_RES_PARAM(result, "contextId"),
			MY_URL,
			MY_SESSION_ID,
			MY_KEY);
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = CommandPause(9101);
	FINEST("result:\n%s", STR(result.ToString()));
	//sleep(1);
	result = CommandResume(1213);
	FINEST("result:\n%s", STR(result.ToString()));
}
