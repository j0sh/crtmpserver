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

#ifdef GLOBALLY_ACCOUNT_BYTES
#define ADD_IN_BYTES_MANAGED(t,b)  IOHandlerManager::AddInBytesManaged(t,b)
#define ADD_OUT_BYTES_MANAGED(t,b) IOHandlerManager::AddOutBytesManaged(t,b)
#define ADD_IN_BYTES_RAWUDP(b)   IOHandlerManager::AddInBytesRawUdp(b)
#define ADD_OUT_BYTES_RAWUDP(b)  IOHandlerManager::AddOutBytesRawUdp(b)
#else
#define ADD_IN_BYTES_MANAGED(t,b)
#define ADD_OUT_BYTES_MANAGED(t,b)
#define ADD_IN_BYTES_RAWUDP(b)
#define ADD_OUT_BYTES_RAWUDP(b)
#endif /* GLOBALLY_ACCOUNT_BYTES */

class DLLEXP BaseFdStats {
private:
	int64_t _current;
	int64_t _max;
	int64_t _total;
#ifdef GLOBALLY_ACCOUNT_BYTES
	uint64_t _inBytes;
	uint64_t _outBytes;
#endif /* GLOBALLY_ACCOUNT_BYTES */
public:
	BaseFdStats();
	virtual ~BaseFdStats();
	void Reset();

	inline void Increment() {
		o_assert(_current >= 0);
		o_assert(_max >= 0);
		_current++;
		_max = _max < _current ? _current : _max;
		_total++;
		o_assert(_current >= 0);
		o_assert(_max >= 0);
	}

	inline void Decrement() {
		o_assert(_current >= 0);
		o_assert(_max >= 0);
		_current--;
		o_assert(_current >= 0);
		o_assert(_max >= 0);
	}
#ifdef GLOBALLY_ACCOUNT_BYTES

	inline void AddInBytes(uint64_t bytes) {
		_inBytes += bytes;
	}

	inline void AddOutBytes(uint64_t bytes) {
		_outBytes += bytes;
	}
#endif /* GLOBALLY_ACCOUNT_BYTES */
	int64_t Current();
	int64_t Max();
	int64_t Total();
#ifdef GLOBALLY_ACCOUNT_BYTES
	uint64_t InBytes();
	uint64_t OutBytes();
#endif /* GLOBALLY_ACCOUNT_BYTES */
	void ResetMax();
	void ResetTotal();
#ifdef GLOBALLY_ACCOUNT_BYTES
	void ResetInBytes();
	void ResetOutBytes();
	void ResetInOutBytes();
#endif /* GLOBALLY_ACCOUNT_BYTES */
	operator string();
	Variant ToVariant();
};

class DLLEXP FdStats {
private:
	BaseFdStats _managedTcp;
	BaseFdStats _managedTcpAcceptors;
	BaseFdStats _managedTcpConnectors;
	BaseFdStats _managedUdp;
	BaseFdStats _managedNonTcpUdp;
	BaseFdStats _rawUdp;
	int64_t _max;
	double _lastUpdateSpeedsTime;
	uint64_t _lastInBytes;
	uint64_t _lastOutBytes;
	double _inSpeed;
	double _outSpeed;
public:
	FdStats();
	virtual ~FdStats();
	void Reset();

	int64_t Current();
	int64_t Max();
	int64_t Total();
#ifdef GLOBALLY_ACCOUNT_BYTES
	uint64_t InBytes();
	uint64_t OutBytes();
	double InSpeed();
	double OutSpeed();
#endif /* GLOBALLY_ACCOUNT_BYTES */
	void ResetMax();
	void ResetTotal();
#ifdef GLOBALLY_ACCOUNT_BYTES
	void ResetInBytes();
	void ResetOutBytes();
	void ResetInOutBytes();
#endif /* GLOBALLY_ACCOUNT_BYTES */

	BaseFdStats &GetManagedTcp();
	BaseFdStats &GetManagedTcpAcceptors();
	BaseFdStats &GetManagedTcpConnectors();
	BaseFdStats &GetManagedUdp();
	BaseFdStats &GetManagedNonTcpUdp();
	BaseFdStats &GetRawUdp();

	inline void RegisterManaged(IOHandlerType type) {
		AccountManaged(type, true);
	}

	inline void UnRegisterManaged(IOHandlerType type) {
		AccountManaged(type, false);
	}
#ifdef GLOBALLY_ACCOUNT_BYTES

	inline void AddInBytesManaged(IOHandlerType type, uint64_t bytes) {
		GetManaged(type)->AddInBytes(bytes);
	}

	inline void AddOutBytesManaged(IOHandlerType type, uint64_t bytes) {
		GetManaged(type)->AddOutBytes(bytes);
	}
#endif /* GLOBALLY_ACCOUNT_BYTES */

	inline void RegisterRawUdp() {
		AccountRawUdp(true);
	}

	inline void UnRegisterRawUdp() {
		AccountRawUdp(false);
	}
#ifdef GLOBALLY_ACCOUNT_BYTES

	inline void AddInBytesRawUdp(uint64_t bytes) {
		_rawUdp.AddInBytes(bytes);
	}

	inline void AddOutBytesRawUdp(uint64_t bytes) {
		_rawUdp.AddOutBytes(bytes);
	}

	inline void UpdateSpeeds() {
		double now;
		GETCLOCKS(now);
		uint64_t nowInBytes = InBytes();
		uint64_t nowOutBytes = OutBytes();
		if (_lastUpdateSpeedsTime > 0) {
			double period = now - _lastUpdateSpeedsTime;
			if (period > 0) {
				double inBytes = (double) (nowInBytes - _lastInBytes);
				double outBytes = (double) (nowOutBytes - _lastOutBytes);
				if (inBytes > 0)
					_inSpeed = inBytes / (period / (double) CLOCKS_PER_SECOND);
				if (outBytes > 0)
					_outSpeed = outBytes / (period / (double) CLOCKS_PER_SECOND);
			}
		} else {
			_inSpeed = 0;
			_outSpeed = 0;
		}
		_lastInBytes = nowInBytes;
		_lastOutBytes = nowOutBytes;
		_lastUpdateSpeedsTime = now;
	}
#endif /* GLOBALLY_ACCOUNT_BYTES */
	operator string();
	Variant ToVariant();
private:

	inline BaseFdStats *GetManaged(IOHandlerType type) {
		switch (type) {
			case IOHT_ACCEPTOR:
			{
				return &_managedTcpAcceptors;
			}
			case IOHT_TCP_CONNECTOR:
			{
				return &_managedTcpConnectors;
			}
			case IOHT_TCP_CARRIER:
			{
				return &_managedTcp;
			}
			case IOHT_UDP_CARRIER:
			{
				return &_managedUdp;
			}
			case IOHT_INBOUNDNAMEDPIPE_CARRIER:
			case IOHT_TIMER:
			case IOHT_STDIO:
			default:
			{
				return &_managedNonTcpUdp;
			}
		}
	}

	inline void AccountManaged(IOHandlerType type, bool increment) {
		BaseFdStats *pFdStats = GetManaged(type);
		if (increment)
			pFdStats->Increment();
		else
			pFdStats->Decrement();
		int64_t current = Current();
		_max = _max < current ? current : _max;
	}

	inline void AccountRawUdp(bool increment) {
		if (increment)
			_rawUdp.Increment();
		else
			_rawUdp.Decrement();
		int64_t current = Current();
		_max = _max < current ? current : _max;
	}
};

#endif	/* _FDSTATS_H */
