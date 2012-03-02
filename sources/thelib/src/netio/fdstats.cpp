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
#ifdef GLOBALLY_ACCOUNT_BYTES
	_inBytes = 0;
	_outBytes = 0;
#endif /* GLOBALLY_ACCOUNT_BYTES */
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
#ifdef GLOBALLY_ACCOUNT_BYTES

uint64_t BaseFdStats::InBytes() {
	return _inBytes;
}

uint64_t BaseFdStats::OutBytes() {
	return _outBytes;
}
#endif /* GLOBALLY_ACCOUNT_BYTES */

void BaseFdStats::ResetMax() {
	_max = _current;
}

void BaseFdStats::ResetTotal() {
	ResetMax();
	_total = _current;
}
#ifdef GLOBALLY_ACCOUNT_BYTES

void BaseFdStats::ResetInBytes() {
	_inBytes = 0;
}

void BaseFdStats::ResetOutBytes() {
	_outBytes = 0;
}

void BaseFdStats::ResetInOutBytes() {
	_inBytes = 0;
	_outBytes = 0;
}
#endif /* GLOBALLY_ACCOUNT_BYTES */

BaseFdStats::operator string() {
#ifdef GLOBALLY_ACCOUNT_BYTES
	return format("current: %"PRId64"; max: %"PRId64"; total: %"PRIu64"; in: %"PRIu64"; out: %"PRIu64,
			_current, _max, _total, _inBytes, _outBytes);
#else
	return format("current: %"PRId64"; max: %"PRId64"; total: %"PRIu64,
			_current, _max, _total);
#endif /* GLOBALLY_ACCOUNT_BYTES */
}

Variant BaseFdStats::ToVariant() {
	Variant result;
	result["current"] = (int64_t) _current;
	result["max"] = (int64_t) _max;
	result["total"] = (int64_t) _total;
#ifdef GLOBALLY_ACCOUNT_BYTES
	result["inBytes"] = (uint64_t) _inBytes;
	result["outBytes"] = (uint64_t) _outBytes;
#endif /* GLOBALLY_ACCOUNT_BYTES */
	return result;
}

FdStats::FdStats() {
	Reset();
}

FdStats::~FdStats() {
	Reset();
}

void FdStats::Reset() {
	_lastUpdateSpeedsTime = -1;
	_lastInBytes = 0;
	_lastOutBytes = 0;
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
#ifdef GLOBALLY_ACCOUNT_BYTES

uint64_t FdStats::InBytes() {
	return _managedTcp.InBytes()
			+ _managedTcpAcceptors.InBytes()
			+ _managedTcpConnectors.InBytes()
			+ _managedUdp.InBytes()
			+ _managedNonTcpUdp.InBytes()
			+ _rawUdp.InBytes();
}

uint64_t FdStats::OutBytes() {
	return _managedTcp.OutBytes()
			+ _managedTcpAcceptors.OutBytes()
			+ _managedTcpConnectors.OutBytes()
			+ _managedUdp.OutBytes()
			+ _managedNonTcpUdp.OutBytes()
			+ _rawUdp.OutBytes();
}

double FdStats::InSpeed() {
	return _inSpeed;
}

double FdStats::OutSpeed() {
	return _outSpeed;
}
#endif /* GLOBALLY_ACCOUNT_BYTES */

void FdStats::ResetMax() {
	_managedTcp.ResetMax();
	_managedTcpAcceptors.ResetMax();
	_managedTcpConnectors.ResetMax();
	_managedUdp.ResetMax();
	_managedNonTcpUdp.ResetMax();
	_rawUdp.ResetMax();
	_max = Current();
}

void FdStats::ResetTotal() {
	ResetMax();
	_managedTcp.ResetTotal();
	_managedTcpAcceptors.ResetTotal();
	_managedTcpConnectors.ResetTotal();
	_managedUdp.ResetTotal();
	_managedNonTcpUdp.ResetTotal();
	_rawUdp.ResetTotal();
}
#ifdef GLOBALLY_ACCOUNT_BYTES

void FdStats::ResetInBytes() {
	_managedTcp.ResetInBytes();
	_managedTcpAcceptors.ResetInBytes();
	_managedTcpConnectors.ResetInBytes();
	_managedUdp.ResetInBytes();
	_managedNonTcpUdp.ResetInBytes();
	_rawUdp.ResetInBytes();
	_lastUpdateSpeedsTime = -1;
	_lastInBytes = 0;
	_lastOutBytes = 0;
}

void FdStats::ResetOutBytes() {
	_managedTcp.ResetOutBytes();
	_managedTcpAcceptors.ResetOutBytes();
	_managedTcpConnectors.ResetOutBytes();
	_managedUdp.ResetOutBytes();
	_managedNonTcpUdp.ResetOutBytes();
	_rawUdp.ResetOutBytes();
	_lastUpdateSpeedsTime = -1;
	_lastInBytes = 0;
	_lastOutBytes = 0;
}

void FdStats::ResetInOutBytes() {
	_managedTcp.ResetInOutBytes();
	_managedTcpAcceptors.ResetInOutBytes();
	_managedTcpConnectors.ResetInOutBytes();
	_managedUdp.ResetInOutBytes();
	_managedNonTcpUdp.ResetInOutBytes();
	_rawUdp.ResetInOutBytes();
	_lastUpdateSpeedsTime = -1;
	_lastInBytes = 0;
	_lastOutBytes = 0;
}
#endif /* GLOBALLY_ACCOUNT_BYTES */

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

FdStats::operator string() {
#ifdef GLOBALLY_ACCOUNT_BYTES
	return format(
			"          managedTcp: %s\n"
			" managedTcpAcceptors: %s\n"
			"managedTcpConnectors: %s\n"
			"          managedUdp: %s\n"
			"    managedNonTcpUdp: %s\n"
			"              rawUdp: %s\n"
			"         grand total: current: %"PRId64"; max: %"PRId64"; total: %"PRIu64"; in: %"PRIu64"; out: %"PRIu64,
			STR(_managedTcp),
			STR(_managedTcpAcceptors),
			STR(_managedTcpConnectors),
			STR(_managedUdp),
			STR(_managedNonTcpUdp),
			STR(_rawUdp),
			Current(),
			Max(),
			Total(),
			InBytes(),
			OutBytes());
#else
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
#endif /* GLOBALLY_ACCOUNT_BYTES */
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
#ifdef GLOBALLY_ACCOUNT_BYTES
	result["grandTotal"]["inBytes"] = (uint64_t) InBytes();
	result["grandTotal"]["outBytes"] = (uint64_t) OutBytes();
	result["grandTotal"]["inSpeed"] = _inSpeed;
	result["grandTotal"]["outSpeed"] = _outSpeed;
#endif /* GLOBALLY_ACCOUNT_BYTES */
	return result;
}
