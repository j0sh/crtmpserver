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


#ifndef _BASECLIENTAPPLICATION_H
#define	_BASECLIENTAPPLICATION_H

#include "common.h"
#include "streaming/streamsmanager.h"
#include "netio/netio.h"

class BaseProtocol;
class BaseAppProtocolHandler;
class BaseStream;
class IOHandler;

/*!
	@brief
 */
class DLLEXP BaseClientApplication {
private:
	static uint32_t _idGenerator;
	uint32_t _id;
	string _name;
	vector<string> _aliases;
	map<uint64_t, BaseAppProtocolHandler *> _protocolsHandlers;
	StreamsManager _streamsManager;
	bool _allowDuplicateInboundNetworkStreams;
protected:
	Variant _configuration;
	bool _isDefault;
	Variant _authSettings;
public:
	BaseClientApplication(Variant &configuration);
	virtual ~BaseClientApplication();

	/*!
		@brief Returns the application's id. The id is auto-generated in the constructor
	 */
	uint32_t GetId();

	/*!
		@brief Returns the name of the application, taken from the configuration file.
	 */
	string GetName();

	/*!
		@brief Returns the variant that contains the configuration information about the application.
	 */
	Variant &GetConfiguration();

	/*!
		@brief Returns the alias of the application from the configuration file
	 */
	vector<string> GetAliases();

	/*!
		@brief Returns the boolean that tells if the application is the default application.
	 */
	bool IsDefault();
	StreamsManager *GetStreamsManager();

	virtual bool Initialize();

	virtual bool ActivateAcceptors(vector<IOHandler *> &acceptors);
	virtual bool ActivateAcceptor(IOHandler *pIOHandler);
	string GetServicesInfo();
	virtual bool AcceptTCPConnection(TCPAcceptor *pTCPAcceptor);

	/*!
		@brief Registers this application to the BaseAppProtocolHandler.
		@protocolType - Type of protocol
		@pAppProtocolHandler
	 */
	void RegisterAppProtocolHandler(uint64_t protocolType,
			BaseAppProtocolHandler *pAppProtocolHandler);
	/*!
		@brief Erases this application to the BaseAppProtocolHandler by setting it to NULL.
		@param protocolType - Type of protocol
	 */
	void UnRegisterAppProtocolHandler(uint64_t protocolType);

	/*!
		@brief Get the ability to handle duplicate inbound network streams
	 */
	bool GetAllowDuplicateInboundNetworkStreams();

	/*!
		@brief Checks and see if the duplicate inbound network streams are available. Always returns true if allowDuplicateNetworkStreams is set to true inside the config file
		@param streamName - The stream name we want to see is free or not
		@param pProtocol - The protocol associated with this request (can be NULL)
	 */
	virtual bool StreamNameAvailable(string streamName, BaseProtocol *pProtocol);

	template<class T>
	T *GetProtocolHandler(BaseProtocol *pProtocol) {
		return (T *) GetProtocolHandler(pProtocol);
	}
	BaseAppProtocolHandler *GetProtocolHandler(BaseProtocol *pProtocol);
	BaseAppProtocolHandler *GetProtocolHandler(uint64_t protocolType);

	template<class T>
	T *GetProtocolHandler(string &scheme) {
		return (T *) GetProtocolHandler(scheme);
	}
	virtual BaseAppProtocolHandler *GetProtocolHandler(string &scheme);

	/*!
		@brief This is called bt the framework when an outbound connection failed to connect
		@param customParameters
	 */
	virtual bool OutboundConnectionFailed(Variant &customParameters);

	/*!
		@brief Registers the protocol to the client application
		@param pProtocol
	 */
	virtual void RegisterProtocol(BaseProtocol *pProtocol);

	/*!
		@brief Erases the protocol to the client application
		@param pProtocol
	 */
	virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

	/*!
		@brief Displays the registered stream's ID, type, and name in the logs 
		@param pStream
	 */
	virtual void SignalStreamRegistered(BaseStream *pStream);

	/*!
		@brief Displays the unregistered stream's ID, type, and name in the logs 
		@param pStream
	 */
	virtual void SignalStreamUnRegistered(BaseStream *pStream);

	virtual bool PullExternalStreams();
	virtual bool PullExternalStream(Variant streamConfig);
	virtual bool PushLocalStream(Variant streamConfig);
	bool ParseAuthentication();

	/*!
		@brief Deletes all active protocols and IOHandlers bound to the application.
		@param pApplication
	 */
	static void Shutdown(BaseClientApplication *pApplication);
private:
	string GetServiceInfo(IOHandler *pIOHander);
};


#endif	/* _BASECLIENTAPPLICATION_H */


