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

#include "netio/fdstats.h"

BaseFdStats::BaseFdStats() {
	Reset();
}

BaseFdStats::~BaseFdStats() {
	Reset();
}

void BaseFdStats::Reset() {
	_current = 0;
	_max = 0;
	_total = 0;
}

void BaseFdStats::Increment() {
	assert(_current >= 0);
	assert(_max >= 0);
	_current++;
	_max = _max < _current ? _current : _max;
	_total++;
	assert(_current >= 0);
	assert(_max >= 0);
}

void BaseFdStats::Decrement() {
	assert(_current >= 0);
	assert(_max >= 0);
	_current--;
	assert(_current >= 0);
	assert(_max >= 0);
}

int64_t BaseFdStats::Current() {
	return _current;
}

int64_t BaseFdStats::Max() {
	return _max;
}

int64_t BaseFdStats::Total() {
	return _total;
}

BaseFdStats::operator string() {
	return format("current: %"PRId64"; max: %"PRId64"; total: %"PRIu64,
			_current, _max, _total);
}

Variant BaseFdStats::ToVariant() {
	Variant result;
	result["current"] = (int64_t) _current;
	result["max"] = (int64_t) _max;
	result["total"] = (int64_t) _total;
	return result;
}

FdStats::FdStats() {
	Reset();
}

FdStats::~FdStats() {
	Reset();
}

void FdStats::Reset() {
	_managedTcp.Reset();
	_managedTcpAcceptors.Reset();
	_managedTcpConnectors.Reset();
	_managedUdp.Reset();
	_managedNonTcpUdp.Reset();
	_rawUdp.Reset();
	_max = 0;
}

int64_t FdStats::Current() {
	return _managedTcp.Current()
			+ _managedTcpAcceptors.Current()
			+ _managedTcpConnectors.Current()
			+ _managedUdp.Current()
			+ _managedNonTcpUdp.Current()
			+ _rawUdp.Current();
}

int64_t FdStats::Max() {
	return _max;
}

int64_t FdStats::Total() {
	return _managedTcp.Total()
			+ _managedTcpAcceptors.Total()
			+ _managedTcpConnectors.Total()
			+ _managedUdp.Total()
			+ _managedNonTcpUdp.Total()
			+ _rawUdp.Total();
}

BaseFdStats &FdStats::GetManagedTcp() {
	return _managedTcp;
}

BaseFdStats &FdStats::GetManagedTcpAcceptors() {
	return _managedTcpAcceptors;
}

BaseFdStats &FdStats::GetManagedTcpConnectors() {
	return _managedTcpConnectors;
}

BaseFdStats &FdStats::GetManagedUdp() {
	return _managedUdp;
}

BaseFdStats &FdStats::GetManagedNonTcpUdp() {
	return _managedNonTcpUdp;
}

BaseFdStats &FdStats::GetRawUdp() {
	return _rawUdp;
}

void FdStats::RegisterManaged(IOHandlerType type) {
	AccountManaged(type, true);
}

void FdStats::UnRegisterManaged(IOHandlerType type) {
	AccountManaged(type, false);
}

void FdStats::RegisterRawUdp() {
	AccountRawUdp(true);
}

void FdStats::UnRegisterRawUdp() {
	AccountRawUdp(false);
}

FdStats::operator string() {
	return format(
			"          managedTcp: %s\n"
			" managedTcpAcceptors: %s\n"
			"managedTcpConnectors: %s\n"
			"          managedUdp: %s\n"
			"    managedNonTcpUdp: %s\n"
			"              rawUdp: %s\n"
			"         grand total: current: %"PRId64"; max: %"PRId64"; total: %"PRIu64,
			STR(_managedTcp),
			STR(_managedTcpAcceptors),
			STR(_managedTcpConnectors),
			STR(_managedUdp),
			STR(_managedNonTcpUdp),
			STR(_rawUdp),
			Current(),
			Max(),
			Total());
}

Variant FdStats::ToVariant() {
	Variant result;
	result["managedTcp"] = _managedTcp.ToVariant();
	result["managedTcpAcceptors"] = _managedTcpAcceptors.ToVariant();
	result["managedTcpConnectors"] = _managedTcpConnectors.ToVariant();
	result["managedUdp"] = _managedUdp.ToVariant();
	result["managedNonTcpUdp"] = _managedNonTcpUdp.ToVariant();
	result["rawUdp"] = _rawUdp.ToVariant();
	result["grandTotal"]["current"] = (int64_t) Current();
	result["grandTotal"]["max"] = (int64_t) Max();
	result["grandTotal"]["total"] = (int64_t) Total();
	return result;
}

void FdStats::AccountManaged(IOHandlerType type, bool increment) {
	BaseFdStats *pFdStats = NULL;
	switch (type) {
		case IOHT_ACCEPTOR:
		{
			pFdStats = &_managedTcpAcceptors;
			break;
		}
		case IOHT_TCP_CONNECTOR:
		{
			pFdStats = &_managedTcpConnectors;
			break;
		}
		case IOHT_TCP_CARRIER:
		{
			pFdStats = &_managedTcp;
			break;
		}
		case IOHT_UDP_CARRIER:
		{
			pFdStats = &_managedUdp;
			break;
		}
		case IOHT_INBOUNDNAMEDPIPE_CARRIER:
		case IOHT_TIMER:
		case IOHT_STDIO:
		default:
		{
			pFdStats = &_managedNonTcpUdp;
			break;
		}
	}
	if (increment)
		pFdStats->Increment();
	else
		pFdStats->Decrement();
	int64_t current = Current();
	_max = _max < current ? current : _max;
	//FINEST("Stats:\n%s", STR(*this));
}

void FdStats::AccountRawUdp(bool increment) {
	if (increment)
		_rawUdp.Increment();
	else
		_rawUdp.Decrement();
	int64_t current = Current();
	_max = _max < current ? current : _max;
	//FINEST("Stats:\n%s", STR(*this));
}

