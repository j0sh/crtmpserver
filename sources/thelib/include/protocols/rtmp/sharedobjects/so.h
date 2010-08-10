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
#ifndef _SO_H
#define	_SO_H

#include "common.h"

typedef struct _DirtyInfo {
	string propertyName;
	uint8_t type;
} DirtyInfo;

typedef vector<DirtyInfo> Dirtyness;

class DLLEXP SO {
private:
	string _name;
	uint32_t _version;
	bool _persistent;
	Variant _payload;
	map<uint32_t, uint32_t> _registeredProtocols;
	map<uint32_t, Dirtyness> _dirtyPropsByProtocol;
	bool _versionIncremented;
public:
	SO(string name, bool persistent);
	virtual ~SO();
public:
	string GetName();
	uint32_t GetVersion();
	bool IsPersistent();
	Variant & GetPayload();

	void RegisterProtocol(uint32_t protocolId);
	void UnRegisterProtocol(uint32_t protocolId);
	uint32_t GetSubscribersCount();
	vector<string> GetPropertyNames();
	uint32_t PropertiesCount();

	bool HasProperties();
	bool HasProperty(string propertyName);

	string DumpTrack();
	void Track();

	operator string();

	Variant & Get(string key);
	Variant & Set(string key, Variant value, uint32_t protocolId = 0);
	void UnSet(string key);
};


#endif	/* _SO_H */

#endif /* HAS_PROTOCOL_RTMP */

