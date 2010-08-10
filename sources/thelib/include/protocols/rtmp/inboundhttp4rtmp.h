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
#ifdef HAS_PROTOCOL_RTMP
#ifndef _INBOUNDHTTP4RTMP_H
#define	_INBOUNDHTTP4RTMP_H

#include "protocols/baseprotocol.h"

class DLLEXP InboundHTTP4RTMP
: public BaseProtocol {
private:
	IOBuffer _inputBuffer;
	IOBuffer _outputBuffer;
	static map<string, string> _generatedSids;
	static map<string, uint32_t> _protocolsBySid;
public:
	InboundHTTP4RTMP();
	virtual ~InboundHTTP4RTMP();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual IOBuffer * GetOutputBuffer();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual bool EnqueueForOutbound();
	virtual void ReadyForSend();
private:
	BaseProtocol *Bind(string sid);
	bool ProcessFcs(vector<string> &parts);
	bool ProcessOpen(vector<string> &parts);
	bool ProcessIdle(vector<string> &parts);
	bool ProcessSend(vector<string> &parts);
};


#endif	/* _INBOUNDHTTP4RTMP_H */

#endif /* HAS_PROTOCOL_RTMP */
#endif /* HAS_PROTOCOL_HTTP */


