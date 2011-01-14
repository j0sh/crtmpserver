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


#ifndef _TIMERSMANAGER_H
#define	_TIMERSMANAGER_H

#include "platform/platform.h"

typedef struct _TimerEvent {
	uint32_t period;
	uint32_t id;
	void *pUserData;
} TimerEvent;

typedef struct _Slot {
	map<uint32_t, TimerEvent> timers;
} Slot;

typedef void (*ProcessTimerEvent)(TimerEvent &event);

/*!
	@class TimersManager
*/
class DLLEXP TimersManager {
private:
	uint32_t _currentSlotIndex;
	uint64_t _lastTime;
	Slot *_pSlots;
	uint32_t _slotsCount;
	ProcessTimerEvent _processTimerEvent;
	vector<uint32_t> _periodsVector;
	map<uint32_t, uint32_t> _periodsMap;
public:
	TimersManager(ProcessTimerEvent processTimerEvent);
	virtual ~TimersManager();

	/*!
		@brief Erases the timer designated by its ID
		@param eventTimerId - Id of the timer to be erased
	*/	
	void RemoveTimer(uint32_t eventTimerId);

	/*!
		@brief Adds a timer event
		@param timerEvent

	*/
	void AddTimer(TimerEvent &timerEvent);

	/*!
		@brief Updates the timer event based on the time elapsed
		@param currentTime - Value of the time in the timer at the time the function is called. This value is used to calculate the elapsed time.
	*/
	void TimeElapsed(uint64_t currentTime);
private:
	void UpdatePeriods(uint32_t period);

	uint32_t GCD(uint32_t a, uint32_t b);
	uint32_t LCM(uint32_t a, uint32_t b);
	uint32_t GCD(vector<uint32_t> numbers, uint32_t startIndex);
	uint32_t LCM(vector<uint32_t> numbers, uint32_t startIndex);
};

#endif	/* _TIMERSMANAGER_H */


