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

#include "common.h"

class BaseStream;
class BaseOutStream;
class BaseClientApplication;

/*!
	@class StreamsManager
 */
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

	/*!
		@brief Returns an unique id to be used inside newly created streams
	 */
	uint32_t GenerateUniqueId();

	/*!
		@brief Registers the stream to the streams manager
		@param pStream
	 */
	bool RegisterStream(BaseStream *pStream);

	/*!
		@brief Erases the stream to the streams manager
		@param pStream
	 */
	void UnRegisterStream(BaseStream *pStream);

	/*!
		@brief Erases the stream to the streams manager using the protocol ID.
		@param protocolId
	 */
	void UnRegisterStreams(uint32_t protocolId);

	/*!
		@brief Checks and see if the duplicate inbound network streams are available. Always returns true if allowDuplicateNetworkStreams is set to true inside the config file
		@param streamName - The stream name we want to see is free or not
	 */
	bool StreamNameAvailable(string streamName);

	/*!
		@brief Returns the entire collection of streams
	 */
	map<uint32_t, BaseStream *> GetAllStreams();

	/*!
		@brief Returns all the outbound streams waiting for a inbound stream called
		@param streamName who has the type inboundStreamType
		@param inboundStreamType
	 */
	map<uint32_t, BaseOutStream *> GetWaitingSubscribers(string streamName,
			uint64_t inboundStreamType);

	/*!
		@brief Find streams by name
		@param name - the complete/partial name to search for
		@param partial - if true, it will match all streams that begins with name otherwise it will exactly search for that name
	 */
	map<uint32_t, BaseStream *> FindByName(string name, bool partial = false);

	/*!
		@brief Find streams by type
		@param type - the complete/mask type to search for
		@param partial - if true, type is considered a mask
	 */
	map<uint32_t, BaseStream *> FindByType(uint64_t type, bool partial = false);

	/*!
		@brief Find streams by type and by name
		@param type - the complete/mask type to search for
		@param name - the complete/partial name to search for
		@param partialType - if true, type is considered a mask
		@param partialName - if true, it will match all streams that begins with name otherwise it will exactly search for that name
	 */
	map<uint32_t, BaseStream *> FindByTypeByName(uint64_t type, string name,
			bool partialType = false, bool partialName = false);

	/*!
		@brief Find all the streams belonging to a protocol
		@param protocolId - the protocol that owns the result streams
	 */
	map<uint32_t, BaseStream *> FindByProtocolId(uint32_t protocolId);

	/*!
		@brief Find all the streams belonging to a protocol with a certain type
		@param protocolId - the protocol that owns the result streams
		@param type - the complete/mask type to search for
		@param partial - if true, type is considered a mask
	 */
	map<uint32_t, BaseStream *> FindByProtocolIdByType(uint32_t protocolId,
			uint64_t type, bool partial = false);

	/*!
		@brief Find all the streams belonging to a protocol with a certain name
		@param protocolId - the protocol that owns the result streams
		@param name - the complete/partial name to search for
		@param partial - if true, it will match all streams that begins with name otherwise it will exactly search for that name
	 */
	map<uint32_t, BaseStream *> FindByProtocolIdByName(uint32_t protocolId,
			string name, bool partial = false);

	/*!
		@brief Find all the streams belonging to a protocol with a certain type and name
		@param protocolId - the protocol that owns the result streams
		@param type - the complete/mask type to search for
		@param name - the complete/partial name to search for
		@param partialType - if true, type is considered a mask
		@param partialName - if true, it will match all streams that begins with name otherwise it will exactly search for that name
	 */
	map<uint32_t, BaseStream *> FindByProtocolIdByTypeByName(uint32_t protocolId,
			uint64_t type, string name, bool partialType = false,
			bool partialName = false);

	/*!
		@brief Returns the stream with the uniqe id specified by uniqueId
		@param uniqueId
	 */
	BaseStream * FindByUniqueId(uint32_t uniqueId);
};

#endif	/* _STREAMSMANAGER_H */

