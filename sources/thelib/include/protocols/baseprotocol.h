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

/*!
	@class BaseProtocol
	@brief The base class on which all atomic protocols must derive from.
 */
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
	double _creationTimestamp;
public:

	BaseProtocol(uint64_t type);
	virtual ~BaseProtocol();

	//
	//general purpose, fixed methods
	//

	/*!
		@abstract Returns the type of the protocol
	 */
	uint64_t GetType();

	/*!
		@brief Returns the id of the protocol. Each protocol instance has an id. The id is unique across all protocols, no matter the type
	 */
	uint32_t GetId();

	/*!
		@brief Returns the creation timestamp expressed in milliseconds for 1970 epoch
	 */
	double GetSpawnTimestamp();

	/*! 
		@brief Gets the far protocol
	 */
	BaseProtocol *GetFarProtocol();

	/*! 
		@brief Sets the far protocol
		@param pProtocol
	 */
	void SetFarProtocol(BaseProtocol *pProtocol);

	/*!
		@brief Break the far side of the protocol chain by making far protocol NULL
	 */
	void ResetFarProtocol();

	/*!
		@brief Gets the near protocol
	 */
	BaseProtocol *GetNearProtocol();

	/*!
		@brief Sets the near protocol
		@param pProtocol
	 */
	void SetNearProtocol(BaseProtocol *pProtocol);

	/*!
		@brief Break the near side of the protocol chain by making near protocol NULL
	 */
	void ResetNearProtocol();

	//Normally, if this protocol will be deleted, all the stack will be deleted
	//Using the next 2 functions we can tell where the protocol deletion will stop
	/*!
		@brief Deletes the protocol
		@param deleteNear: Indicates which protocol to delete.
		@discussion Normally, if this protocol will be deleted, all the stack will be deleted. Using the DeleteNearProtocol and DeleteFarProtocol, we can tell where the protocol deletion will stop.
	 */
	void DeleteNearProtocol(bool deleteNear);
	/*!
		@brief Deletes the protocol
		@param deleteNear: Indicates which protocol to delete.
		@discussion Normally, if this protocol will be deleted, all the stack will be deleted. Using the DeleteNearProtocol and DeleteFarProtocol, we can tell where the protocol deletion will stop.
	 */
	void DeleteFarProtocol(bool deleteFar);

	/*!
		@brief Gets the far-most protocol in the protocl chain.
		@discussion Far-end protocol - is the one close to the transport layer
	 */

	BaseProtocol *GetFarEndpoint();

	/*!
		@brief Gets the near-most protocol in the protocl chain.
		@discussion Near-end protocol - is the one close to the business rules layer
	 */
	BaseProtocol *GetNearEndpoint();

	/*!
		@brief Tests to see if the protocol is enqueued for delete
	 */
	bool IsEnqueueForDelete();

	/*!
		@brief Gets the protocol's application
	 */
	BaseClientApplication * GetApplication();

	//This are functions for set/get the costom parameters
	//in case of outbound protocols
	/*!
		@brief Sets the custom parameters in case of outbound protocols
		@param customParameters: Variant contaitning the custom parameters
	 */
	void SetOutboundConnectParameters(Variant &customParameters);

	/*!
		@brief Gets the custom parameters
	 */
	Variant &GetCustomParameters();

	/*!
		@brief This will return complete information about all protocols in the current stack, including the carrier if available.
		@param info
	 */
	void GetStackStats(Variant &info, uint32_t namespaceId = 0);

	//utility functions
	operator string();

	//
	//virtuals
	//

	/*!
		@brief This is called to initialize internal resources specific to each protocol. This is called before adding the protocol to the stack.
		@param parameters
	 */
	virtual bool Initialize(Variant &parameters);

	/*!
		@brief Enqueues the protocol to the delete queue imediatly. All transport related routines called from now on, will be ignored.
	 */
	virtual void EnqueueForDelete();

	/*!
		@brief Enqueues the protocol to the delete queue if there is no outstanding outbound data.
		@param fromFarSide
	 */
	virtual void GracefullyEnqueueForDelete(bool fromFarSide = true);

	/*!
		@brief Returns the IO handler of the protocol chain.
	 */
	virtual IOHandler *GetIOHandler();

	/*!
		@brief Sets the IO Handler of the protocol chain.
		@param pCarrier
	 */
	virtual void SetIOHandler(IOHandler *pCarrier);

	/*!
		@brief Gets the input buffers
	 */
	virtual IOBuffer * GetInputBuffer();

	/*!
		@brief Gets the output buffers
	 */
	virtual IOBuffer * GetOutputBuffer();

	/*!
		@brief Get the total amount of bytes that this protocol transported (raw bytes).
	 */
	virtual uint64_t GetDecodedBytesCount();

	/*!
		@brief This function can be called by anyone who wants to signal the transport layer that there is data ready to be sent. pExtraParameters usually is a pointer to an OutboundBuffer, but this depends on the protocol type.
	 */
	virtual bool EnqueueForOutbound();

	/*! 
		@brief Enqueue the current protocol stack for timed event
		@param seconds
	 */
	virtual bool EnqueueForTimeEvent(uint32_t seconds);

	/*!
		@brief This is invoked by the framework when the protocol is enqueued for timing oprrations
	 */
	virtual bool TimePeriodElapsed();

	/*!
		@brief This is invoked by the framework when the underlaying system is ready to send more data
	 */
	virtual void ReadyForSend();

	/*!
	 *	@brief This is invoked from various parts of the project to signal inter-protocol events. 
		@param event
	 *   @discussion For example, you enqueue a DNS request and you want to be informed when the request is done and the results are available. It will always bubble up towards the near protocol
	 */
	virtual void SignalInterProtocolEvent(Variant &event);

	/*!
		@brief Sets the protocol's application
		@param application
	 */
	virtual void SetApplication(BaseClientApplication *pApplication);

	/*!
		@brief This is called by the framework when data is available for processing, when making use of connection-less protocols
		@param buffer
		@param pPeerAddress
	 */
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);

	/*!
		@brief This is called by the framework when data is available for processing, directly from the network i/o layer
		@brief recvAmount
		@param pPeerAddress
	 */
	virtual bool SignalInputData(int32_t recvAmount, sockaddr_in *pPeerAddress);

	/*!
		@brief This will return a Variant containing various statistic information. This should be overridden if more/less info is desired
		@param info
	 */
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	//
	//must be implemented by the class that inherits this class
	//

	/*!
		@brief Should return true if this protocol can be linked with a far protocol of type 'type'. Otherwise should return false
		@param type
		@discussion This function must be implemented by the class that inherits this class
	 */
	virtual bool AllowFarProtocol(uint64_t type) = 0;

	/*!
		@brief Should return true if this protocol can be linked with a near protocol of type 'type'. Otherwise should return false
		@param type
		@discussion This function must be implemented by the class that inherits this class
	 */
	virtual bool AllowNearProtocol(uint64_t type) = 0;

	/*!
		@brief This is called by the framework when data is available for processing, directly from the network i/o layer
		@param recvAmount
		@discussion This function must be implemented by the class that inherits this class
	 */
	virtual bool SignalInputData(int32_t recvAmount) = 0;

	/*!
		@brief This is called by the framework when data is available for processing, from the underlaying protocol (NOT i/o layer)
		@param buffer
		@discussion This function must be implemented by the class that inherits this class
	 */
	virtual bool SignalInputData(IOBuffer &buffer) = 0;

private:
	//utility function
	string ToString(uint32_t currentId);
};

#endif	/* _BASEPROTOCOL_H */



