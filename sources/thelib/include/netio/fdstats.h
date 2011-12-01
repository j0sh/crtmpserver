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

#ifndef _FDSTATS_H
#define	_FDSTATS_H

#include "common.h"
#include "netio/iohandlertype.h"

class BaseFdStats {
private:
	int64_t _current;
	int64_t _max;
	int64_t _total;
public:
	BaseFdStats();
	virtual ~BaseFdStats();
	void Reset();
	inline void Increment();
	inline void Decrement();
	int64_t Current();
	int64_t Max();
	int64_t Total();
	void ResetMax();
	void ResetTotal();
	operator string();
	Variant ToVariant();
};

class FdStats {
private:
	BaseFdStats _managedTcp;
	BaseFdStats _managedTcpAcceptors;
	BaseFdStats _managedTcpConnectors;
	BaseFdStats _managedUdp;
	BaseFdStats _managedNonTcpUdp;
	BaseFdStats _rawUdp;
	int64_t _max;
public:
	FdStats();
	virtual ~FdStats();
	void Reset();

	int64_t Current();
	int64_t Max();
	int64_t Total();
	void ResetMax();
	void ResetTotal();

	BaseFdStats &GetManagedTcp();
	BaseFdStats &GetManagedTcpAcceptors();
	BaseFdStats &GetManagedTcpConnectors();
	BaseFdStats &GetManagedUdp();
	BaseFdStats &GetManagedNonTcpUdp();
	BaseFdStats &GetRawUdp();

	void RegisterManaged(IOHandlerType type);
	void UnRegisterManaged(IOHandlerType type);


	void RegisterRawUdp();
	void UnRegisterRawUdp();

	operator string();
	Variant ToVariant();
private:
	void AccountManaged(IOHandlerType type, bool increment);
	void AccountRawUdp(bool increment);
};

#endif	/* _FDSTATS_H */
