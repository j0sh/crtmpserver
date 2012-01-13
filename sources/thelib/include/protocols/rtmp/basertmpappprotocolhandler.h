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
#ifndef _BASERTMPAPPPROTOCOLHANDLER_H
#define	_BASERTMPAPPPROTOCOLHANDLER_H

#include "application/baseappprotocolhandler.h"
#include "protocols/rtmp/header.h"
#include "protocols/rtmp/rtmpprotocolserializer.h"
#include "protocols/rtmp/sharedobjects/somanager.h"
#include "streaming/baseoutstream.h"
#include "streaming/baseoutfilestream.h"

class OutboundRTMPProtocol;
class BaseRTMPProtocol;
class BaseOutFileStream;

class DLLEXP BaseRTMPAppProtocolHandler
: public BaseAppProtocolHandler {
protected:
	RTMPProtocolSerializer _rtmpProtocolSerializer;
	SOManager _soManager;
	bool _validateHandshake;
	bool _renameBadFiles;
	string _mediaFolder;
	bool _externSeekGenerator;
	bool _enableCheckBandwidth;
	Variant _onBWCheckMessage;
	Variant _onBWCheckStrippedMessage;
	map<uint32_t, BaseRTMPProtocol *> _connections;
	map<uint32_t, uint32_t> _nextInvokeId;
	map<uint32_t, map<uint32_t, Variant > > _resultMessageTracking;
	bool _keyframeSeek;
	int32_t _clientSideBuffer;
	uint32_t _seekGranularity;
	Variant _adobeAuthSettings;
	string _authMethod;
	string _adobeAuthSalt;
	double _lastUsersFileUpdate;
	Variant _users;
public:
	BaseRTMPAppProtocolHandler(Variant &configuration);
	virtual ~BaseRTMPAppProtocolHandler();

	virtual bool ParseAuthenticationNode(Variant &node, Variant &result);

	/*
	 * This will return true if the application has the validateHandshake flag
	 * */
	bool ValidateHandshake();

	/*
	 * This will return the shared objects manager for this particular application
	 * */
	SOManager *GetSOManager();

	/*
	 * (Un)Register connection. This is called by the framework
	 * each time a connections is going up or down
	 * pProtocol - the conection which is going up or down
	 * */
	virtual void RegisterProtocol(BaseProtocol *pProtocol);
	virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

	/*
	 * This is called by the framework when a stream needs to be pulled in
	 * Basically, this will open a RTMP client and start playback a stream
	 * */
	virtual bool PullExternalStream(URI uri, Variant streamConfig);

	/*
	 * This is called by the framework when a stream needs to be pushed forward
	 * Basically, this will open a RTMP client and start publishing a stream
	 * */
	virtual bool PushLocalStream(Variant streamConfig);

	/*
	 * This is called bt the framework when an outbound connection was established
	 * */
	virtual bool OutboundConnectionEstablished(OutboundRTMPProtocol *pFrom);

	/*
	 * This is called by the framework when authentication is needed upon
	 * connect invoke.
	 * pFrom - the connection requesting authentication
	 * request - full connect request
	 * */
	virtual bool AuthenticateInbound(BaseRTMPProtocol *pFrom, Variant &request,
			Variant &authState);

	/*
	 * This is called by the framework when outstanding data is ready for processing
	 * pFrom - the connection that has data ready for processing
	 * inputBuffer - raw data
	 * */
	virtual bool InboundMessageAvailable(BaseRTMPProtocol *pFrom, Header &header,
			IOBuffer &inputBuffer);

	/*
	 * This is called by the framework when a new message was successfully deserialized
	 * and is ready for processing
	 * pFrom - the connection that has data ready for processing
	 * request - complete request ready for processing
	 * */
	virtual bool InboundMessageAvailable(BaseRTMPProtocol *pFrom, Variant &request);

	/*
	 * This will generate all the meta and seek files inside the mediaFolder
	 * */
	void GenerateMetaFiles();

	//TODO: Commenting ou the protected section bellow is a quite nasty hack
	//It is done to support virtual machines and out-of-hierarchy calls
	//for the base class. This should be definitely removed and re-factored
	//properly. I will leave it here for the time being
	//protected:

	/*
	 * The following list of functions are called when the corresponding
	 * message is received. All of them have the same parameters
	 * pFrom - the origin of the request
	 * request - the complete request
	 * */
	virtual bool ProcessAbortMessage(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessWinAckSize(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessPeerBW(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessAck(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessChunkSize(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessUsrCtrl(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessNotify(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessFlexStreamSend(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessSharedObject(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessInvoke(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessInvokeConnect(BaseRTMPProtocol *pFrom, Variant &request);
	virtual bool ProcessInvokeCreateStream(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokePublish(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeSeek(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokePlay(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokePauseRaw(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokePause(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeCloseStream(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeReleaseStream(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeDeleteStream(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeOnStatus(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeFCPublish(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeGetStreamLength(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeOnBWDone(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeCheckBandwidth(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
			Variant &request);
	virtual bool ProcessInvokeResult(BaseRTMPProtocol *pFrom,
			Variant &result);

	/*
	 * The following functions are called by the framework when a result
	 * is received from the distant peer (server made the request, and we just
	 * got a response). All of them have the same parameters
	 * pFrom - the connection which sent us the response
	 * request - the initial request
	 * response - the response
	 * */
	virtual bool ProcessInvokeResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	virtual bool ProcessInvokeConnectResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	virtual bool ProcessInvokeCreateStreamResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	virtual bool ProcessInvokeFCSubscribeResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	virtual bool ProcessInvokeOnBWCheckResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	virtual bool ProcessInvokeGenericResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);

	/*
	 * Adobe authentication method used by FMLE.
	 * */
	virtual bool AuthenticateInboundAdobe(BaseRTMPProtocol *pFrom, Variant &request,
			Variant &authState);

	/*
	 * This will return the password assigned to the specified user
	 * */
	virtual string GetAuthPassword(string user);

	/*
	 * This will try to extract complete metadata for the specified stream name
	 * streamName - the name of the stream for which the metadata is desired.
	 *              the stream name has special formats.
	 *              see http://rtmpd.com/browser/trunk/media/README.txt
	 * extractInnerMetadata - will try to read the metadata from the file
	 *                        if applicable
	 * */
	Variant GetMetaData(string streamName, bool extractInnerMetadata);

	/*
	 * Used to send generi RTMP messages
	 * pTo - target connection
	 * message - complete RTMP message
	 * trackResponse - if true, a response is expected after sending this message
	 *                 if one becomes available, ProcessInvokeResult will be called
	 *                 if false, no response is expected
	 * */
	bool SendRTMPMessage(BaseRTMPProtocol *pTo, Variant message,
			bool trackResponse = false);

	/*
	 * Create a file stream for writing to disk.
	 * pFrom - The connection which wants to stream to disk
	 * meta - Stream metadata
	 * append - Whether to append this stream to an exiting file
	 * */
	virtual BaseOutFileStream *CreateOutFileStream(BaseRTMPProtocol *pFrom, Variant &meta, bool append);
private:
	/*
	 * Will transform stream names of type streamName?param1=value1&param2=value2&...
	 * into streamName-param1_value1-param2_value2...
	 * All the '-' characters inside values will be replaced with '_'
	 */
	string NormalizeStreamName(string streamName);

	/*
	 * Tries to create an outbound stream and link it to the live inbound stream
	 * streamName - the name of the live inbound stream
	 * linked - will be set to true if the linking succeeded. Otherwise it will
	 *          be set to false
	 * returns true if process returned successfully, or false otherwise
	 * */
	bool TryLinkToLiveStream(BaseRTMPProtocol *pFrom, uint32_t streamId,
			string streamName, bool &linked);

	/*
	 * Tries to create an outbound stream and link it to the file inbound stream
	 * streamName - the name of the file inbound stream
	 * linked - will be set to true if the linking succeeded. Otherwise it will
	 *          be set to false
	 * returns true if process returned successfully, or false otherwise
	 * */
	bool TryLinkToFileStream(BaseRTMPProtocol *pFrom, uint32_t streamId,
			Variant &metadata, string streamName, double startTime,
			double length, bool &linked);

	/*
	 * This will return true if the connection is an outbound connection
	 * which needs to pull in a stream
	 * */
	bool NeedsToPullExternalStream(BaseRTMPProtocol *pFrom);

	/*
	 * This will return true if the connection is an outbound connection
	 * which is used to export a local stream
	 * */
	bool NeedsToPushLocalStream(BaseRTMPProtocol *pFrom);

	/*
	 * Initiate the stream pulling sequence: connect->createStream->Play
	 * */
	bool PullExternalStream(BaseRTMPProtocol *pFrom);

	/*
	 * Initiate the stream pushing sequence: connect->createStream->Publish
	 * */
	bool PushLocalStream(BaseRTMPProtocol *pFrom);

	/*
	 * Send the initial connect invoke
	 * */
	bool ConnectForPullPush(BaseRTMPProtocol *pFrom, string uriPath,
			Variant &config, bool isPull);

	/*
	 * Log Event
	 **/
	//Variant& CreateLogEventInvoke(BaseRTMPProtocol *pFrom, Variant &request);
};


#endif	/* _BASERTMPAPPPROTOCOLHANDLER_H */

#endif /* HAS_PROTOCOL_RTMP */

