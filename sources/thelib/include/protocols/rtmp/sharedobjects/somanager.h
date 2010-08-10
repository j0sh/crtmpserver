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



#ifdef HAS_PROTOCOL_RTMP
#ifndef _SOMANAGER_H
#define	_SOMANAGER_H

#include "common.h"

class BaseRTMPProtocol;
class SO;

class DLLEXP SOManager {
private:
	map<string, SO *> _sos;
	map<uint32_t, vector<SO *> > _protocolSos;
public:
	SOManager();
	virtual ~SOManager();
public:
	void UnRegisterProtocol(BaseRTMPProtocol *pProtocol);
	bool Process(BaseRTMPProtocol *pFrom, Variant &request);
	SO * GetSO(string name, bool persistent);
	bool ContainsSO(string name);
private:
	bool ProcessFlexSharedObject(BaseRTMPProtocol *pFrom, Variant &request);
	bool ProcessSharedObject(BaseRTMPProtocol *pFrom, Variant &request);
	bool ProcessSharedObjectPrimitive(BaseRTMPProtocol *pFrom, SO *pSO,
			string name, Variant &request, uint32_t primitiveId);
};


#endif	/* _SOMANAGER_H */

#endif /* HAS_PROTOCOL_RTMP */

