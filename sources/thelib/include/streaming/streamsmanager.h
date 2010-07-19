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


#ifndef _STREAMSMANAGER_H
#define	_STREAMSMANAGER_H

#include "utils/core.h"

class BaseStream;
class BaseClientApplication;

class DLLEXP StreamsManager {
private:
	BaseClientApplication *_pApplication;
	uint32_t _uniqueIdGenerator;
	map<uint32_t, BaseStream *> _streamsByUniqueId;
	map<uint32_t, map<uint32_t, BaseStream *> > _streamsByProtocolId;
	map<uint64_t, map<uint32_t, BaseStream * > > _streamsByType;
	map<string, map<uint32_t, BaseStream * > > _streamsByName;
public:
	StreamsManager(BaseClientApplication *pApplication);
	virtual ~StreamsManager();

	/*
	 * returns an unique id to be used inside newly created streams
	 * */
	uint32_t GenerateUniqueId();

	/*
	 * (Un)Registers a stream to the streams manager
	 * */
	bool RegisterStream(BaseStream *pStream);
	void UnRegisterStream(BaseStream *pStream);
	void UnRegisterStreams(uint32_t protocolId);

	/*
	 * Find streams by name
	 * name - the complete/partial name to search for
	 * partial - if true, it will match all streams that begins with name
	 *           otherwise it will exactly search for that name
	 * */
	map<uint32_t, BaseStream *> FindByName(string name, bool partial = false);

	/*
	 * Find streams by type
	 * type - the complete/mask type to search for
	 * partial - if true, type is considered a mask
	 * */
	map<uint32_t, BaseStream *> FindByType(uint64_t type, bool partial = false);

	/*
	 * Find streams by type and by name
	 * type - the complete/mask type to search for
	 * name - the complete/partial name to search for
	 * partialType - if true, type is considered a mask
	 * partialName - if true, it will match all streams that begins with name
	 *               otherwise it will exactly search for that name
	 * */
	map<uint32_t, BaseStream *> FindByTypeByName(uint64_t type, string name,
			bool partialType = false, bool partialName = false);

	/*
	 * Find all the streams belonging to a protocol
	 * protocolId - the protocol that owns the result streams
	 * */
	map<uint32_t, BaseStream *> FindByProtocolId(uint32_t protocolId);

	/*
	 * Find all the streams belonging to a protocol with a certain type
	 * protocolId - the protocol that owns the result streams
	 * type - the complete/mask type to search for
	 * partial - if true, type is considered a mask
	 * */
	map<uint32_t, BaseStream *> FindByProtocolIdByType(uint32_t protocolId,
			uint64_t type, bool partial = false);

	/*
	 * Find all the streams belonging to a protocol with a certain name
	 * protocolId - the protocol that owns the result streams
	 * name - the complete/partial name to search for
	 * partial - if true, it will match all streams that begins with name
	 *           otherwise it will exactly search for that name
	 * */
	map<uint32_t, BaseStream *> FindByProtocolIdByName(uint32_t protocolId,
			string name, bool partial = false);

	/*
	 * Find all the streams belonging to a protocol with a certain type and name
	 * protocolId - the protocol that owns the result streams
	 * type - the complete/mask type to search for
	 * name - the complete/partial name to search for
	 * partialType - if true, type is considered a mask
	 * partialName - if true, it will match all streams that begins with name
	 *               otherwise it will exactly search for that name
	 * */
	map<uint32_t, BaseStream *> FindByProtocolIdByTypeByName(uint32_t protocolId,
			uint64_t type, string name, bool partialType = false,
			bool partialName = false);

	/*
	 * Returns the stream with the uniqe id specified by uniqueId
	 * */
	BaseStream * FindByUniqueId(uint32_t uniqueId);
};

#endif	/* _STREAMSMANAGER_H */

