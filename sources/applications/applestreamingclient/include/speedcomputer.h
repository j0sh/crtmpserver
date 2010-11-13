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


#ifndef _SPEEDCOMPUTER_H
#define	_SPEEDCOMPUTER_H

#include "common.h"

#define HLU_COUNT 1
#define HLU_TIME 2

namespace app_applestreamingclient {

	class SpeedComputer {
	private:
		uint32_t _historyLength;
		uint8_t _historyLengthUnit;
		double _totalAmount;
		double _totalTransferTime;
		vector<pair<double, double> > _entries;
	public:
		SpeedComputer(uint32_t historyLength = 300, uint8_t historyLengthUnit = HLU_COUNT);
		virtual ~SpeedComputer();

		uint32_t GetHistoryLength();
		void SetHistoryLength(uint32_t historyLength);
		uint8_t GetHistoryLengthUnit();
		void SetHistoryLengthUnit(uint8_t historyLengthUnit);

		double GetMeanSpeed();

		void PushAmount(double amount, double transferTime);
	private:
		void UpdateEntries();
		double CurrentHistoryLength();
	};
};

#endif	/* _SPEEDCOMPUTER_H */

