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

#include "speedcomputer.h"
using namespace app_applestreamingclient;

SpeedComputer::SpeedComputer(uint32_t historyLength, uint8_t historyLengthUnit) {
	_historyLength = historyLength;
	_historyLengthUnit = historyLengthUnit;
	_totalAmount = 0;
	_totalTransferTime = 0;
}

SpeedComputer::~SpeedComputer() {
}

uint32_t SpeedComputer::GetHistoryLength() {
	return _historyLength;
}

void SpeedComputer::SetHistoryLength(uint32_t historyLength) {
	_historyLength = historyLength;
	if (_historyLength == 0) {
		_entries.clear();
		return;
	}
	UpdateEntries();
}

uint8_t SpeedComputer::GetHistoryLengthUnit() {
	return _historyLengthUnit;
}

void SpeedComputer::SetHistoryLengthUnit(uint8_t historyLengthUnit) {
	_historyLengthUnit = historyLengthUnit;
}

double SpeedComputer::GetMeanSpeed() {
	if (_historyLength != 0) {
		if (CurrentHistoryLength() / _historyLength > 0.9) {
			if (_totalTransferTime == 0) {
				return 0xfffffffe;
			} else {
				return _totalAmount / _totalTransferTime;
			}
		} else {
			return 0;
		}
	} else {
		return _totalAmount / _totalTransferTime;
	}
}

void SpeedComputer::PushAmount(double amount, double transferTime) {
	_totalAmount += amount;
	_totalTransferTime += transferTime;
	if (_historyLength > 0) {
		pair<double, double> p = pair<double, double>(amount, transferTime);
		ADD_VECTOR_END(_entries, p);
		UpdateEntries();
	}
}

void SpeedComputer::UpdateEntries() {
	if (_historyLength == 0)
		return;

	while (CurrentHistoryLength() > _historyLength) {
		_totalAmount -= _entries[0].first;
		_totalTransferTime -= _entries[0].second;
		_entries.erase(_entries.begin());
	}
}

double SpeedComputer::CurrentHistoryLength() {
	if (_historyLengthUnit == HLU_COUNT)
		return _entries.size();
	else
		return _totalTransferTime;
}
