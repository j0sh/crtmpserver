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

#ifndef _BASEFEEDER_H
#define	_BASEFEEDER_H

#include "common.h"

#define DATA_TYPE_UNDEF 0
#define DATA_TYPE_CLIENT2SERVER 1
#define DATA_TYPE_SERVER2CLIENT 2

class MonitorRTMPProtocol;

class BaseFeeder {
private:
	File _client2ServerBin;
	File _server2ClientBin;
	File _allBin;
	IOBuffer _client2ServerBuffer;
	IOBuffer _server2ClientBuffer;
	MonitorRTMPProtocol *_pClient2Server;
	MonitorRTMPProtocol *_pServer2Client;
protected:
	string _fileName;
public:
	BaseFeeder();
	virtual ~BaseFeeder();

	virtual bool Init(string fileName, MonitorRTMPProtocol *pFlash2Server,
			MonitorRTMPProtocol *pServer2Flash);
	virtual void Run() = 0;
protected:
	bool Feed(const uint8_t *pBuffer, uint32_t length, uint8_t type);
};

#endif	/* _BASEFEEDER_H */

