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

#ifdef HAS_PROTOCOL_CLI
#ifndef _BASECLIAPPPROTOCOLHANDLER_H
#define	_BASECLIAPPPROTOCOLHANDLER_H

#include "application/baseappprotocolhandler.h"

class BaseProtocol;

class DLLEXP BaseCLIAppProtocolHandler
: public BaseAppProtocolHandler {
public:
	BaseCLIAppProtocolHandler(Variant &configuration);
	virtual ~BaseCLIAppProtocolHandler();

	void RegisterProtocol(BaseProtocol *pProtocol);
	void UnRegisterProtocol(BaseProtocol *pProtocol);

	virtual bool ProcessMessage(BaseProtocol *pFrom, Variant &message) = 0;
protected:
	bool Send(BaseProtocol *pTo, string status, string description, Variant &data);
	bool SendFail(BaseProtocol *pTo, string description);
	bool SendSuccess(BaseProtocol *pTo, string description, Variant &data);
};

#endif	/* _BASECLIAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_CLI */
