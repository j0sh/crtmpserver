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


#ifndef _BASEPROTOCOL_H
#define	_BASEPROTOCOL_H


#include "common.h"
#include "protocols/protocoltypes.h"


class IOBuffer;
class IOHandler;
class BaseClientApplication;

class DLLEXP BaseProtocol {
private:
	static uint32_t _idGenerator;
	uint32_t _id;
	BaseClientApplication *_pApplication;
protected:
	uint64_t _type;
	BaseProtocol *_pFarProtocol;
	BaseProtocol *_pNearProtocol;
	bool _deleteFar;
	bool _deleteNear;
	bool _enqueueForDelete;
	bool _gracefullyEnqueueForDelete;
	Variant _customParameters;
public:

	BaseProtocol(uint64_t type);
	virtual ~BaseProtocol();

	//
	//general purpose, fixed methods
	//

	//returns the type of the protocol
	uint64_t GetType();

	//returns the id of the protocol. Each protocol instance has an id
	//the id is unique across all protocols, no matter the type
	uint32_t GetId();

	//Gets the far protocol
	BaseProtocol *GetFarProtocol();

	//Sets the far protocol
	void SetFarProtocol(BaseProtocol *pProtocol);

	//Break the far side of the protocol chain by making far protocol NULL
	void ResetFarProtocol();

	//Gets the near protocol
	BaseProtocol *GetNearProtocol();

	//Sets the near protocol
	void SetNearProtocol(BaseProtocol *pProtocol);

	//Break the near side of the protocol chain by making near protocol NULL
	void ResetNearProtocol();

	//Normally, if this protocol will be deleted, all the stack will be deleted
	//Using the next 2 functions we can tell where the protocol deletion will stop
	void DeleteNearProtocol(bool deleteNear);
	void DeleteFarProtocol(bool deleteFar);

	//Gets the far-most protocol in the protocl chain
	//(the one close to transport layer)
	BaseProtocol *GetFarEndpoint();

	//Gets the near-most protocol in the protocol chain
	//(the one close to the business rules layer)
	BaseProtocol *GetNearEndpoint();

	//Tests to see if the protocol is enqueued for delete
	bool IsEnqueueForDelete();

	//Gets the protocol's application
	BaseClientApplication * GetApplication();

	//This are functions for set/get the costom parameters
	//in case of outbound protocols
	void SetOutboundConnectParameters(Variant &customParameters);
	Variant &GetCustomParameters();

	//utility functions
	operator string();

	//
	//virtuals
	//

	//This is called to initialize internal resources specific to
	//each protocol. This is called before adding the protocol to the stack
	virtual bool Initialize(Variant &parameters);

	//Enqueues the protocol to the delete queue imediatly. All transport related
	//routines called from now on, will be ignored
	virtual void EnqueueForDelete();

	//Enqueues the protocol to the delete queue if there is no outstanding
	//outbound data
	virtual void GracefullyEnqueueForDelete(bool fromFarSide = true);

	//returns the IO handler of the protocol chain
	virtual IOHandler *GetIOHandler();

	//Sets the IO Handler of the protocol chain
	virtual void SetIOHandler(IOHandler *pCarrier);

	//Gets the input/output buffers
	virtual IOBuffer * GetInputBuffer();
	virtual IOBuffer * GetOutputBuffer();

	//Get the total amount of bytes that this protocol transported (raw bytes)
	virtual uint32_t GetDecodedBytesCount();

	//This function can be called by anyone who wants to signal the
	//transport layer that there is data ready to be sent. pExtraParameters usually
	//is a pointer to an OutboundBuffer, but this depends on the protocol type
	virtual bool EnqueueForOutbound();

	//enqueue the current protocol stack for timed event
	virtual bool EnqueueForTimeEvent(uint32_t seconds);

	//This is invoked by the framework when the protocol is enqueued for
	//timing oprrations
	virtual bool TimePeriodElapsed();

	//This is invoked by the framework when the underlaying system is ready
	//to send more data
	virtual void ReadyForSend();

	//This is invoked from various parts of the project to signal inter-protocol
	//events. For example, you enqueue a DNS request and you want to be informed
	//when the request is done and the results are available. It will always bubble
	//up towards the near protocol
	virtual void SignalInterProtocolEvent(Variant &event);

	//Sets the protocol's application
	virtual void SetApplication(BaseClientApplication *pApplication);

	//This is called by the framework when data is available for processing,
	//when making use of connection-less protocols
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);

	//This is called by the framework when data is available for processing,
	//directly from the network i/o layer
	virtual bool SignalInputData(int32_t recvAmount, sockaddr_in *pPeerAddress);

	//This will return a Variant containing various statistic information
	//This should be overriden if more/less info is desired
	virtual void GetStats(Variant &info);

	//
	//must be implemented by the class that inherits this class
	//

	//Should return true if this protocol can be linked with a far protocol
	//of type 'type'. Otherwise should return false
	virtual bool AllowFarProtocol(uint64_t type) = 0;

	//Should return true if this protocol can be linked with a near protocol
	//of type 'type'. Otherwise should return false
	virtual bool AllowNearProtocol(uint64_t type) = 0;

	//This is called by the framework when data is available for processing,
	//directly from the network i/o layer
	virtual bool SignalInputData(int32_t recvAmount) = 0;

	//This is called by the framework when data is available for processing,
	//from the underlaying protocol (NOT i/o layer)
	virtual bool SignalInputData(IOBuffer &buffer) = 0;

private:
	//utility function
	string ToString(uint32_t currentId);
};

#endif	/* _BASEPROTOCOL_H */



