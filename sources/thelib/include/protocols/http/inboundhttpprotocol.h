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


#ifdef HAS_PROTOCOL_HTTP
#ifndef _BASEINBOUNDHTTPPROTOCOL_H
#define	_BASEINBOUNDHTTPPROTOCOL_H

#include "protocols/http/basehttpprotocol.h"

class InboundHTTPProtocol
: public BaseHTTPProtocol {
private:
	Variant _requestHeaders;
	uint16_t _statusCode;
public:
	InboundHTTPProtocol();
	virtual ~InboundHTTPProtocol();

	void SetStatusCode(uint16_t statusCode);

	virtual bool Initialize(Variant &parameters);
	virtual string GetOutputFirstLine();
	virtual bool ParseFirstLine(string &line, Variant &headers);
	virtual bool Authenticate();
private:
	bool SendAuthRequired(Variant &auth);
};


#endif	/* _BASEINBOUNDHTTPPROTOCOL_H */
#endif /* HAS_PROTOCOL_HTTP */

