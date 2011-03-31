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


#include "basefeeder.h"
#include "monitorrtmpprotocol.h"

BaseFeeder::BaseFeeder() {
	_pClient2Server = NULL;
	_pServer2Client = NULL;
}

BaseFeeder::~BaseFeeder() {
}

bool BaseFeeder::Init(string fileName, MonitorRTMPProtocol *pFlash2Server,
		MonitorRTMPProtocol *pServer2Flash) {
	_pClient2Server = pFlash2Server;
	_pServer2Client = pServer2Flash;
	_fileName = fileName;
	if (!_allBin.Initialize("all.bin", true, false)) {
		return false;
	}
	if (!_client2ServerBin.Initialize("client2Server.bin", true, false)) {
		return false;
	}
	if (!_server2ClientBin.Initialize("server2Client.bin", true, false)) {
		return false;
	}

	return true;
}

bool BaseFeeder::Feed(const uint8_t *pBuffer, uint32_t length, uint8_t type) {
	if (length == 0) {
		return true;
	}


	if (type == DATA_TYPE_SERVER2CLIENT) {
		_server2ClientBin.WriteBuffer(pBuffer, length);
		_server2ClientBin.Flush();
	} else {
		_client2ServerBin.WriteBuffer(pBuffer, length);
		_client2ServerBin.Flush();
	}
	_allBin.WriteBuffer(pBuffer, length);
	_allBin.Flush();
	switch (type) {
		case DATA_TYPE_CLIENT2SERVER:
		{
			if (!_client2ServerBuffer.ReadFromBuffer(pBuffer, length)) {
				FATAL("Unable to put %u bytes to _client2ServerBuffer buffer", length);
				return false;
			}

			return _pClient2Server->SignalInputData(_client2ServerBuffer);
		}
		case DATA_TYPE_SERVER2CLIENT:
		{
			if (!_server2ClientBuffer.ReadFromBuffer(pBuffer, length)) {
				FATAL("Unable to put %u bytes to _server2ClientBuffer buffer", length);
				return false;
			}

			return _pServer2Client->SignalInputData(_server2ClientBuffer);
		}
		default:
		{
			FATAL("Invalid data type: %hhu", type);
			return false;
		}
	}
}
