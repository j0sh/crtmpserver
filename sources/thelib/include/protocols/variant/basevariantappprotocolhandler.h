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


#ifdef HAS_PROTOCOL_VAR
#ifndef _BASEVARIANTAPPPROTOCOLHANDLER_H
#define	_BASEVARIANTAPPPROTOCOLHANDLER_H

#include "application/baseappprotocolhandler.h"

class BaseVariantProtocol;

class DLLEXP BaseVariantAppProtocolHandler
: public BaseAppProtocolHandler {
private:
	Variant _urlCache;
	vector<uint64_t> _outboundHttpBinVariant;
	vector<uint64_t> _outboundHttpXmlVariant;
	vector<uint64_t> _outboundBinVariant;
	vector<uint64_t> _outboundXmlVariant;
public:
	BaseVariantAppProtocolHandler(Variant &configuration);
	virtual ~BaseVariantAppProtocolHandler();

	virtual void RegisterProtocol(BaseProtocol *pProtocol);
	virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

	//opens an OUTBOUNDXMLVARIANT or an OUTBOUNDBINVARIANT
	//and sends the variant
	bool Send(string ip, uint16_t port, Variant &variant, bool xml = true);

	//opens an OUTBOUNDHTTPXMLVARIANT or an OUTBOUNDHTTPBINVARIANT
	//and sends the variant
	bool Send(string url, Variant &variant, bool xml = true);

	//used internally
	static bool SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters);
	virtual void ConnectionFailed(Variant &parameters);

	//this is called whenever a message is received
	virtual bool ProcessMessage(BaseVariantProtocol *pProtocol,
			Variant &lastSent, Variant &lastReceived);

private:
	Variant GetScaffold(string uriString);
};


#endif	/* _BASEVARIANTAPPPROTOCOLHANDLER_H */
#endif	/* HAS_PROTOCOL_VAR */

