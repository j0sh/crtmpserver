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
	sleep(10);
	EnvStop();
	sleep(5);
}

void* worker(void *) {
	EnvRun("0.0.0.0", 5544);
	return NULL;
}
