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


#include "utils/misc/timersmanager.h"
#include "utils/logging/logging.h"

TimersManager::TimersManager(ProcessTimerEvent processTimerEvent) {
	_processTimerEvent = processTimerEvent;
	_lastTime = time(NULL);
	_currentSlotIndex = 0;
	_pSlots = NULL;
	_slotsCount = 0;
}

TimersManager::~TimersManager() {
	if (_pSlots != NULL)
		delete[] _pSlots;
}

void TimersManager::RemoveTimer(uint32_t eventTimerId) {
	for (uint32_t i = 0; i < _slotsCount; i++) {
		if (MAP_HAS1(_pSlots[i].timers, eventTimerId)) {
			_pSlots[i].timers.erase(eventTimerId);
		}
	}
}

void TimersManager::AddTimer(TimerEvent& timerEvent) {
	UpdatePeriods(timerEvent.period);
	uint32_t min = 999999999;
	uint32_t startIndex = 0;
	for (uint32_t i = 0; i < _slotsCount; i++) {
		if (min > _pSlots[i].timers.size()) {
			startIndex = i;
			min = _pSlots[i].timers.size();
		}
	}
	while (!MAP_HAS1(_pSlots[startIndex % _slotsCount].timers, timerEvent.id)) {
		_pSlots[startIndex % _slotsCount].timers[timerEvent.id] = timerEvent;
		startIndex += timerEvent.period;
	}
}

void TimersManager::TimeElapsed(uint64_t currentTime) {
	int64_t delta = currentTime - _lastTime;
	_lastTime = currentTime;

	if (delta <= 0 || _slotsCount == 0)
		return;

	for (int32_t i = 0; i < delta; i++) {

		FOR_MAP(_pSlots[_currentSlotIndex % _slotsCount].timers, uint32_t, TimerEvent, j) {
			_processTimerEvent(MAP_VAL(j));
		}
		_currentSlotIndex++;
	}
}

void TimersManager::UpdatePeriods(uint32_t period) {
	if (MAP_HAS1(_periodsMap, period))
		return;
	_periodsMap[period] = period;
	ADD_VECTOR_END(_periodsVector, period);
	uint32_t newSlotsCount = LCM(_periodsVector, 0);
	if (newSlotsCount == 0)
		newSlotsCount = period;
	if (newSlotsCount == _slotsCount)
		return;
	Slot *pNewSlots = new Slot[newSlotsCount];
	if (_slotsCount > 0) {
		for (uint32_t i = 0; i < newSlotsCount; i++) {
			pNewSlots[i] = _pSlots[i % _slotsCount];
		}
		delete[] _pSlots;
	}
	_pSlots = pNewSlots;
	_slotsCount = newSlotsCount;
}

uint32_t TimersManager::GCD(uint32_t a, uint32_t b) {
	while (b != 0) {
		uint32_t t = b;
		b = a % b;
		a = t;
	}
	return a;
}

uint32_t TimersManager::LCM(uint32_t a, uint32_t b) {
	if (a == 0 || b == 0)
		return 0;
	uint32_t result = a * b / GCD(a, b);
	FINEST("a: %u; b: %u; r: %u", a, b, result);
	return result;
}

uint32_t TimersManager::GCD(vector<uint32_t> numbers, uint32_t startIndex) {
	if (numbers.size() <= 1)
		return 0;
	if (numbers.size() <= startIndex)
		return 0;
	if (numbers.size() - startIndex > 2) {
		return GCD(numbers[startIndex], GCD(numbers, startIndex + 1));
	} else {
		return GCD(numbers[startIndex], numbers[startIndex + 1]);
	}
}

uint32_t TimersManager::LCM(vector<uint32_t> numbers, uint32_t startIndex) {
	if (numbers.size() <= 1)
		return 0;
	if (numbers.size() <= startIndex)
		return 0;
	if (numbers.size() - startIndex > 2) {
		return LCM(numbers[startIndex], LCM(numbers, startIndex + 1));
	} else {
		return LCM(numbers[startIndex], numbers[startIndex + 1]);
	}
}


/*
 *
 * TimersManager tm(NULL);
	TimerEvent t1 = {2, 1, NULL};
	TimerEvent t2 = {3, 2, NULL};
	TimerEvent t3 = {3, 3, NULL};
	TimerEvent t4 = {4, 4, NULL};
	TimerEvent t5 = {3, 5, NULL};
	TimerEvent t6 = {2, 6, NULL};
	TimerEvent t7 = {4, 7, NULL};

	tm.AddTimer(t1);
	tm.AddTimer(t2);
	tm.AddTimer(t3);
	tm.AddTimer(t4);
	tm.AddTimer(t5);
	tm.AddTimer(t6);
	tm.AddTimer(t7);
 *
 * */

