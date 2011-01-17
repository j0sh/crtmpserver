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
#ifndef _BASEOUTBOUNDHTTPPROTOCOL_H
#define	_BASEOUTBOUNDHTTPPROTOCOL_H

#include "protocols/http/basehttpprotocol.h"

class DLLEXP OutboundHTTPProtocol
: public BaseHTTPProtocol {
private:
	string _method;
	string _host;
	string _document;
public:
	OutboundHTTPProtocol();
	virtual ~OutboundHTTPProtocol();

	string Method();
	void Method(string method);
	string Document();
	void Document(string document);
	string Host();
	void Host(string host);
	virtual bool EnqueueForOutbound();

	virtual bool Is200OK();
protected:
	virtual string GetOutputFirstLine();
	virtual bool ParseFirstLine(string &line, Variant &firstLineHeader);
	virtual bool Authenticate();
};


#endif	/* _BASEOUTBOUNDHTTPPROTOCOL_H */
#endif /* HAS_PROTOCOL_HTTP */

