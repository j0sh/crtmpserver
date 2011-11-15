constants={
--configuration,
CONF_DAEMON="daemon",
CONF_PATH_SEPARATOR="pathSeparator",
CONF_ACCEPTORS="acceptors",
CONF_IP="ip",
CONF_PORT="port",
CONF_DNSRESOLVER="dnsResolver",
CONF_DNSRESOLVER_SELF="self",
CONF_SSL_KEY="sslKey",
CONF_SSL_CERT="sslCert",
CONF_PROTOCOL="protocol",
CONF_PROTOCOL_DYNAMICLINKLIBRARY="dynamiclinklibrary",

-- RTMP protocols
CONF_PROTOCOL_INBOUND_RTMP="inboundRtmp",
CONF_PROTOCOL_OUTBOUND_RTMP="outboundRtmp",
CONF_PROTOCOL_INBOUND_RTMPT="inboundRtmpt",
CONF_PROTOCOL_OUTBOUND_RTMPT="outboundRtmpt",
CONF_PROTOCOL_OUTBOUND_RTMPE="outboundRtmpe",
CONF_PROTOCOL_INBOUND_RTMPS="inboundRtmps",

-- Async DNS protocols
CONF_PROTOCOL_INBOUND_DNS="inboundDns",
CONF_PROTOCOL_OUTBOUND_DNS="outboundDns",

-- MPEG-TS protocols
CONF_PROTOCOL_INBOUND_TCP_TS="inboundTcpTs",
CONF_PROTOCOL_INBOUND_UDP_TS="inboundUdpTs",

-- RTSP/RTP/RTCP protocols
CONF_PROTOCOL_INBOUND_RTSP_RTP="inboundRtspRtp",
CONF_PROTOCOL_INBOUND_UDP_RTP="inboundUdpRtp",
CONF_PROTOCOL_RTSP_RTCP="inboundRtspRtcp",
CONF_PROTOCOL_UDP_RTCP="inboundUdpRtcp",
CONF_PROTOCOL_INBOUND_RTSP="inboundRtsp",
CONF_PROTOCOL_RTP_NAT_TRAVERSAL="rtpNatTraversal",

-- HTTP protocols
CONF_PROTOCOL_INBOUND_HTTP="inboundHttp",
CONF_PROTOCOL_OUTBOUND_HTTP="outboundHttp",

-- Live FLV protocols
CONF_PROTOCOL_INBOUND_LIVE_FLV="inboundLiveFlv",
CONF_PROTOCOL_OUTBOUND_LIVE_FLV="outboundLiveFlv",

-- NSV protocols
CONF_PROTOCOL_INBOUND_NSV="inboundNsv",
CONF_PROTOCOL_OUTBOUND_NSV="outboundNsv",

-- MMS protocols
CONF_PROTOCOL_OUTBOUND_MMS="outboundMms",

-- Variant protocols
CONF_PROTOCOL_INBOUND_XML_VARIANT="inboundXmlVariant",
CONF_PROTOCOL_INBOUND_BIN_VARIANT="inboundBinVariant",
CONF_PROTOCOL_INBOUND_HTTP_XML_VARIANT="inboundHttpXmlVariant",
CONF_PROTOCOL_INBOUND_HTTP_BIN_VARIANT="inboundHttpBinVariant",
CONF_PROTOCOL_OUTBOUND_XML_VARIANT="outboundXmlVariant",
CONF_PROTOCOL_OUTBOUND_BIN_VARIANT="outboundBinVariant",
CONF_PROTOCOL_OUTBOUND_HTTP_XML_VARIANT="outboundHttpXmlVariant",
CONF_PROTOCOL_OUTBOUND_HTTP_BIN_VARIANT="outboundHttpBinVariant",

-- CLI protocols
CONF_PROTOCOL_INBOUND_CLI_JSON="inboundJsonCli",
CONF_PROTOCOL_INBOUND_HTTP_CLI_JSON="inboundHttpJsonCli",

-- Raw HTTP streams
CONF_PROTOCOL_INBOUND_RAW_HTTP_STREAM="inboundRawHttpStream",
CONF_PROTOCOL_INBOUND_RAW_HTTPS_STREAM="inboundRawHttpsStream",

CONF_NUMBER_OF_THREADS="numberOfThreads",
CONF_APPLICATIONS="applications",
CONF_APPLICATIONS_ROOTDIRECTORY="rootDirectory",
CONF_APPLICATION_DIRECTORY="appDir",
CONF_APPLICATION_NAME="name",
CONF_APPLICATION_ALIASES="aliases",
CONF_APPLICATION_DESCRIPTION="description",
CONF_APPLICATION_DEFAULT="default",
CONF_APPLICATION_GENERATE_META_FILES="generateMetaFiles",
CONF_APPLICATION_LIBRARY="library",
CONF_APPLICATION_MEDIAFOLDER="mediaFolder",
CONF_APPLICATION_KEYFRAMESEEK="keyframeSeek",
CONF_APPLICATION_SEEKGRANULARITY="seekGranularity",
CONF_APPLICATION_CLIENTSIDEBUFFER="clientSideBuffer",
CONF_APPLICATION_RTCPDETECTIONINTERVAL="rtcpDetectionInterval",
CONF_APPLICATION_VALIDATEHANDSHAKE="validateHandshake",
CONF_APPLICATION_AUTH="authentication",
CONF_APPLICATION_AUTH_TYPE="type",
CONF_APPLICATION_AUTH_TYPE_ADOBE="adobe",
CONF_APPLICATION_AUTH_ENCODER_AGENTS="encoderAgents",
CONF_APPLICATION_AUTH_USERS_FILE="usersFile",
CONF_APPLICATION_RENAMEBADFILES="renameBadFiles",
CONF_APPLICATION_EXTERNSEEKGENERATOR="externSeekGenerator",
CONF_APPLICATION_INIT_APPLICATION_FUNCTION="initApplicationFunction",
CONF_APPLICATION_DELETE_APPLICATION_FUNCTION="deleteApplicationFunction",
CONF_APPLICATION_INIT_FACTORY_FUNCTION="initFactoryFunction",
CONF_APPLICATION_DELETE_FACTORY_FUNCTION="deleteFactoryFunction",
CONF_APPLICATION_ALLOW_DUPLICATE_INBOUND_NETWORK_STREAMS="allowDuplicateInboundNetworkStreams",
CONF_ADDRESS="address",
CONF_CONFIGURATION="configuration",
CONF_BIND_ADDRESSES="bindAddresses",
CONF_STORAGE_PATH="storagePath",
CONF_LOG_APPENDERS="logAppenders",
CONF_LOG_APPENDER_NAME="name",
CONF_LOG_APPENDER_TYPE="type",
CONF_LOG_APPENDER_TYPE_CONSOLE="console",
CONF_LOG_APPENDER_TYPE_COLORED_CONSOLE="coloredConsole",
CONF_LOG_APPENDER_TYPE_FILE="file",
CONF_LOG_APPENDER_TYPE_SYSLOG="syslog",
CONF_LOG_APPENDER_FILE_NAME="fileName",
CONF_LOG_APPENDER_LEVEL="level",
CONF_LOG_APPENDER_SPECIFIC_LEVEL="specificLevel",
CONF_LOG_APPENDER_COLORED="colored",
CONF_LOG_APPENDER_FORMAT="format",
CONF_LOG_APPENDER_SINGLE_LINE="singleLine",
CONF_LOG_APPENDER_NEW_LINE_CHARACTERS="newLineCharacters",
CONF_LOG_APPENDER_FILE_HISTORY_SIZE="fileHistorySize",
CONF_LOG_APPENDER_FILE_LENGTH="fileLength",

MEDIA_TYPE_LIVE_OR_FLV="liveOrFlv",
MEDIA_TYPE_FLV="flv",
MEDIA_TYPE_MP3="mp3",
MEDIA_TYPE_MP4="mp4",
MEDIA_TYPE_M4A="m4a",
MEDIA_TYPE_M4V="m4v",
MEDIA_TYPE_MOV="mov",
MEDIA_TYPE_F4V="f4v",
MEDIA_TYPE_NSV="nsv",
MEDIA_TYPE_TS="ts",
MEDIA_TYPE_SEEK="seek",
MEDIA_TYPE_META="meta",

DNS_RESOLVER_HOST="host",
DNS_RESOLVER_CONSUMER_PROTOCOL_ID="consumerProtocolId",
DNS_RESOLVER_CALLBACK="callback",
DNS_RESOLVER_CUSTOM_DATA="customData",
DNS_RESOLVER_REQUEST="request",
DNS_RESOLVER_REQUEST_ID="id",
DNS_RESOLVER_IP="ip",

URL_HOST="host",
URL_PORT="port",
URL_USER="user",
URL_PASSWORD="password",
URL_DOCUMENT="document",

HTTP_FULL_URL="fullUrl",
HTTP_URL="url",
HTTP_METHOD="method",
HTTP_METHOD_POST="POST",
HTTP_METHOD_GET="GET",
HTTP_PARAMETERS="parameters",
HTTP_VERSION="version",
HTTP_VERSION_1_1="HTTP/1.1",
HTTP_VERSION_1_0="HTTP/1.0",
HTTP_HEADERS="headers",
HTTP_HEADERS_USER_AGENT="User-Agent",
HTTP_HEADERS_CONTENT_TYPE="Content-Type",
HTTP_HEADERS_CONTENT_TYPE_XFCS="application/x-fcs",
HTTP_HEADERS_CONTENT_LENGTH="Content-Length",
HTTP_HEADERS_CONNECTION="Connection",
HTTP_HEADERS_CONNECTION_CLOSE="close",
HTTP_HEADERS_CONNECTION_KEEP_ALIVE="Keep-Alive",
HTTP_HEADERS_HOST="Host",
HTTP_HEADERS_CACHE_CONTROL="Cache-Control",
HTTP_HEADERS_CACHE_CONTROL_NO_CACHE="no-cache",
HTTP_HEADERS_SERVER="Server",
HTTP_HEADERS_SERVER_US="C++ RTMP Server (http://www.rtmpd.com)",
HTTP_HEADERS_X_POWERED_BY="X-Powered-By",
HTTP_HEADERS_X_POWERED_BY_US="C++ RTMP Server (http://www.rtmpd.com)",
HTTP_HEADERS_TRANSFER_ENCODING="Transfer-Encoding",
HTTP_HEADERS_TRANSFER_ENCODING_CHUNKED="chunked",
HTTP_HEADERS_AUTORIZATION="Authorization",
HTTP_HEADERS_WWWAUTHENTICATE="WWW-Authenticate",
HTTP_STATUS_CODE="statusCode",
HTTP_STATUS_CODE_200="200",
HTTP_STATUS_CODE_REASON="reason",
HTTP_FIRST_LINE="firstLine",

RTSP_VERSION="version",
RTSP_STATUS_CODE="statusCode",
RTSP_STATUS_CODE_REASON="reason",
RTSP_URL="url",
RTSP_VERSION_1_0="RTSP/1.0",
RTSP_HEADERS="headers",
RTSP_HEADERS_CSEQ="CSeq",
RTSP_HEADERS_ACCEPT="Accept",
RTSP_HEADERS_ACCEPT_APPLICATIONSDP="application/sdp",
RTSP_HEADERS_CONTENT_TYPE="Content-Type",
RTSP_HEADERS_TRANSPORT="Transport",
RTSP_HEADERS_PUBLIC="Public",
RTSP_HEADERS_SESSION="Session",
RTSP_HEADERS_CONTENT_LENGTH="Content-Length",
RTSP_HEADERS_SERVER="Server",
RTSP_HEADERS_SERVER_US="C++ RTMP Server (http://www.rtmpd.com)",
RTSP_HEADERS_X_POWERED_BY="X-Powered-By",
RTSP_HEADERS_X_POWERED_BY_US="C++ RTMP Server (http://www.rtmpd.com)",
RTSP_HEADERS_RANGE="Range",
RTSP_HEADERS_RANGE_NOW="now",
RTSP_HEADERS_RTP_INFO="RTP-Info",
RTSP_HEADERS_AUTHORIZATION="Authorization",
RTSP_FIRST_LINE="firstLine",
RTSP_METHOD="method",
RTSP_METHOD_OPTIONS="OPTIONS",
RTSP_METHOD_DESCRIBE="DESCRIBE",
RTSP_METHOD_SETUP="SETUP",
RTSP_METHOD_PLAY="PLAY",
RTSP_METHOD_PAUSE="PAUSE",
RTSP_METHOD_RECORD="RECORD",
RTSP_METHOD_TEARDOWN="TEARDOWN",
RTSP_METHOD_ANNOUNCE="ANNOUNCE",

PROTOCOL_EVENT_TYPE="type",
PROTOCOL_EVENT_PAYLOAD="payload",
PROTOCOL_EVENT_TYPE_DNS_RESPONSE="dnsResponse",

AMF3_TRAITS="____traits____",
AMF3_TRAITS_CLASSNAME="____class_name____",
AMF3_TRAITS_DYNAMIC="____isDynamic____",


META_REQUESTED_STREAM_NAME="requestedStreamName",
META_MEDIA_TYPE="streamType",
META_SERVER_FULL_PATH="fullPath",
META_SERVER_MEDIA_DIR="mediaDir",
META_SERVER_FILE_NAME="fileName",
META_RTMP_META="rtmpMeta",
META_FILE_SIZE="fileSize",
META_FILE_DURATION="fileDuration",
META_FILE_BANDWIDTH="bandwidth",
META_AUDIO_FRAMES_COUNT="audioFramesCount",
META_VIDEO_FRAMES_COUNT="videoFramesCount",
META_TOTAL_FRAMES_COUNT="totalFrames",
META_MP4_HIERARCHY="mp4Hierarchy",

--Application message structure,
AM="applicationMessage",
	AM_MESSAGEID="messageId",
	AM_SERVERID="serverId",
	AM_CONNECTIONID="connectionId",
	AM_PAYLOADTYPE="payloadType",
	AM_PAYLOAD="payload",

--RTMP message structure,
RM="rtmpMessage",
	RM_HEADER="header",
		RM_HEADER_ORIGINALHEADERSIZE="originalHeaderSize",
		RM_HEADER_HEADERTYPE="headerType",
		RM_HEADER_CHANNELID="channelId",
		RM_HEADER_TIMESTAMP="timestamp",
		RM_HEADER_MESSAGELENGTH="messageLength",
		RM_HEADER_MESSAGETYPE="messageType",
			RM_HEADER_MESSAGETYPE_CHUNKSIZE=0x01,
			RM_HEADER_MESSAGETYPE_ABORTMESSAGE=0x02,
			RM_HEADER_MESSAGETYPE_ACK=0x03,
			RM_HEADER_MESSAGETYPE_USRCTRL=0x04,
			RM_HEADER_MESSAGETYPE_WINACKSIZE=0x05,
			RM_HEADER_MESSAGETYPE_PEERBW=0x06,
			RM_HEADER_MESSAGETYPE_AUDIODATA=0x08,
			RM_HEADER_MESSAGETYPE_VIDEODATA=0x09,
			RM_HEADER_MESSAGETYPE_FLEX=0x11,
			RM_HEADER_MESSAGETYPE_NOTIFY=0x12,
			RM_HEADER_MESSAGETYPE_INVOKE=0x14,
			RM_HEADER_MESSAGETYPE_FLEXSHAREDOBJECT=0x10,
			RM_HEADER_MESSAGETYPE_SHAREDOBJECT=0x13,
			RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND=0x0f,
			RM_HEADER_MESSAGETYPE_AGGREGATE=0x16,
		RM_HEADER_STREAMID="streamId",
		RM_HEADER_ISABSOLUTE="isAbsolute",
	RM_NOTIFY="notify",
		RM_NOTIFY_PARAMS="params",
	RM_FLEXSTREAMSEND="flexStreamSend",
		RM_FLEXSTREAMSEND_UNKNOWNBYTE="unknownByte",
		RM_FLEXSTREAMSEND_PARAMS="params",
	RM_FLEXSHAREDOBJECT="flexSharedObject",
		RM_FLEXSHAREDOBJECT_NAME="fsoName",
		RM_FLEXSHAREDOBJECT_VERSION="fsoVersion",
		RM_FLEXSHAREDOBJECT_PERSISTENCE="fsoPersistence",
		RM_FLEXSHAREDOBJECT_PRIMITIVES="fsoPrimitives",
			RM_FLEXSHAREDOBJECTPRIMITIVE_TYPE="fsoType",
			RM_FLEXSHAREDOBJECTPRIMITIVE_RAWLENGTH="fsoRawLength",
			RM_FLEXSHAREDOBJECTPRIMITIVE_PAYLOAD="fsoPayload",
	RM_SHAREDOBJECT="sharedObject",
		RM_SHAREDOBJECT_NAME="name",
		RM_SHAREDOBJECT_VERSION="version",
		RM_SHAREDOBJECT_PERSISTENCE="persistence",
		RM_SHAREDOBJECT_PRIMITIVES="primitives",
			RM_SHAREDOBJECTPRIMITIVE_TYPE="type",
			RM_SHAREDOBJECTPRIMITIVE_STRTYPE="strType",
			RM_SHAREDOBJECTPRIMITIVE_RAWLENGTH="rawLength",
			RM_SHAREDOBJECTPRIMITIVE_PAYLOAD="payload",
	RM_INVOKE="invoke",
		RM_INVOKE_IS_FLEX="isFlex",
		RM_INVOKE_FUNCTION="functionName",
			RM_INVOKE_FUNCTION_RESULT="_result",
			RM_INVOKE_FUNCTION_ERROR="_error",
			RM_INVOKE_FUNCTION_ONSTATUS="onStatus",
			RM_INVOKE_FUNCTION_CONNECT="connect",
			RM_INVOKE_FUNCTION_CREATESTREAM="createStream",
			RM_INVOKE_FUNCTION_PUBLISH="publish",
			RM_INVOKE_FUNCTION_CLOSESTREAM="closeStream",
			RM_INVOKE_FUNCTION_RELEASESTREAM="releaseStream",
			RM_INVOKE_FUNCTION_DELETESTREAM="deleteStream",
			RM_INVOKE_FUNCTION_PLAY="play",
			RM_INVOKE_FUNCTION_PAUSE="pause",
			RM_INVOKE_FUNCTION_PAUSERAW="pauseRaw",
			RM_INVOKE_FUNCTION_CLOSE="close",
			RM_INVOKE_FUNCTION_SEEK="seek",
			RM_INVOKE_FUNCTION_FCPUBLISH="FCPublish",
			RM_INVOKE_FUNCTION_FCSUBSCRIBE="FCSubscribe",
			RM_INVOKE_FUNCTION_ONBWDONE="onBWDone",
			RM_INVOKE_FUNCTION_GETSTREAMLENGTH="getStreamLength",
			RM_INVOKE_FUNCTION_CHECKBANDWIDTH="checkBandwidth",
			RM_INVOKE_FUNCTION_ONBWCHECK="onBWCheck",
		RM_INVOKE_ID="id",
		RM_INVOKE_PARAMS="parameters",

			RM_INVOKE_PARAMS_RESULT_LEVEL="level",
				RM_INVOKE_PARAMS_RESULT_LEVEL_STATUS="status",
				RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR="error",
			RM_INVOKE_PARAMS_RESULT_CODE="code",
				RM_INVOKE_PARAMS_RESULT_CODE_NETCONNECTIONCONNECTSUCCESS="NetConnection.Connect.Success",
				RM_INVOKE_PARAMS_RESULT_CODE_NETCONNECTIONCONNECTREJECTED="NetConnection.Connect.Rejected",
			RM_INVOKE_PARAMS_RESULT_DESCRIPTION="description",
				RM_INVOKE_PARAMS_RESULT_DESCRIPTION_CONNECTIONSUCCEEDED="Connection succeeded",
			RM_INVOKE_PARAMS_RESULT_OBJECTENCODING="objectEncoding",

			RM_INVOKE_PARAMS_ONSTATUS_LEVEL="level",
				RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS="status",
			RM_INVOKE_PARAMS_ONSTATUS_CODE="code",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPUBLISHSTART="NetStream.Publish.Start",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPLAYRESET="NetStream.Play.Reset",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPLAYSTART="NetStream.Play.Start",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPAUSENOTIFY="NetStream.Pause.Notify",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMUNPAUSENOTIFY="NetStream.Unpause.Notify",
				RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMSEEKNOTIFY="NetStream.Seek.Notify",
			RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION="description",
			RM_INVOKE_PARAMS_ONSTATUS_DETAILS="details",
			RM_INVOKE_PARAMS_ONSTATUS_CLIENTID="clientid",

			RM_INVOKE_PARAMS_CONNECT_APP="app",
			RM_INVOKE_PARAMS_CONNECT_AUDIOCODECS="audioCodecs",
			RM_INVOKE_PARAMS_CONNECT_CAPABILITIES="capabilities",
			RM_INVOKE_PARAMS_CONNECT_FLASHVER="flashVer",
			RM_INVOKE_PARAMS_CONNECT_FPAD="fpad",
			RM_INVOKE_PARAMS_CONNECT_PAGEURL="pageUrl",
			RM_INVOKE_PARAMS_CONNECT_SWFURL="swfUrl",
			RM_INVOKE_PARAMS_CONNECT_TCURL="tcUrl",
			RM_INVOKE_PARAMS_CONNECT_VIDEOCODECS="videoCodecs",
			RM_INVOKE_PARAMS_CONNECT_VIDEOFUNCTION="videoFunction",
			RM_INVOKE_PARAMS_PUBLISH_TYPERECORD="record",
			RM_INVOKE_PARAMS_PUBLISH_TYPEAPPEND="append",
			
	RM_USRCTRL="usrCtrl",
		RM_USRCTRL_TYPE="type",
			RM_USRCTRL_TYPE_STREAM_BEGIN=0,
			RM_USRCTRL_TYPE_STREAM_EOF=1,
			RM_USRCTRL_TYPE_STREAM_DRY=2,
			RM_USRCTRL_TYPE_STREAM_SET_BUFFER_LENGTH=3,
			RM_USRCTRL_TYPE_STREAM_IS_RECORDED=4,
			RM_USRCTRL_TYPE_PING_REQUEST=6,
			RM_USRCTRL_TYPE_PING_RESPONSE=7,
			RM_USRCTRL_TYPE_UNKNOWN1=31,
			RM_USRCTRL_TYPE_UNKNOWN2=32,
		RM_USRCTRL_TYPE_STRING="typeString",
		RM_USRCTRL_STREAMID="streamId",
		RM_USRCTRL_BUFFLEN="buffLen",
		RM_USRCTRL_TIMESTAMP="timestamp",
		RM_USRCTRL_PING="ping",
		RM_USRCTRL_PONG="pong",
		RM_USRCTRL_UNKNOWN_U32="unknownU32",
	RM_ABORTMESSAGE="abortMessage",
	RM_CHUNKSIZE="chunkSize",
	RM_ACK="ack",
	RM_WINACKSIZE="winAckSize",
	RM_PEERBW="peerBw",
		RM_PEERBW_VALUE="value",
		RM_PEERBW_TYPE="kind",
			RM_PEERBW_TYPE_HARD=0,
			RM_PEERBW_TYPE_SOFT=1,
			RM_PEERBW_TYPE_DYNAMIC=2,

--Control message structure,
CM="controlMessage",
	CM_OP="operation",
		CM_OP_HELLOREQUEST="helloRequest",
		CM_OP_HELLORESPONSE="helloResponse",

		CM_OP_DISCONNECTCLIENT="disconnectClient",

		CM_OP_CLIENTDISCONNECTED="clientDisconnected",

		CM_OP_ERROR="error",

		CM_OP_CREATESTREAM="createStream",
		CM_OP_STREAMCREATED="streamCreated",

		CM_OP_CLOSESTREAM="closeStream",
		CM_OP_STREAMCLOSED="streamClosed",

		CM_OP_CREATEINS="createInboundNetworkStream",
		CM_OP_INSCREATED="inboundNetworkStreamCreated",

		CM_OP_CREATEOFS="createOutboundFileStream",
		CM_OP_OFSCREATED="outboundFileStreamCreated",

		CM_OP_CREATEONS="createOutboundNetworkStream",
		CM_OP_ONSCREATED="outboundNetworkStreamCreated",

		CM_OP_ATTACHONSTOINS="attachONSToINS",
		CM_OP_ONSTOINSATTACHED="ONSToINSAttached",

	CM_PARAMS="parameters",
		CM_PARAMS_ERROR_CODE="code",
			CM_PARAMS_ERROR_CODE_NYI=1,
			CM_PARAMS_ERROR_CODE_INVALIDRTMPHEADER=2,
			CM_PARAMS_ERROR_CODE_NOMOREFREESTREAMS=3,
			CM_PARAMS_ERROR_CODE_NEUTRALSTREAMINITFAILED=4,
			CM_PARAMS_ERROR_CODE_INSINITFAILED=5,
			CM_PARAMS_ERROR_CODE_INVALIDSTREAMID=6,
			CM_PARAMS_ERROR_CODE_OFSINITFAILED=7,
			CM_PARAMS_ERROR_CODE_ONSINITFAILED=8,
		CM_PARAMS_ERROR_DESCRIPTION="description",
		CM_PARAMS_ERROR_EXTRAPARAMS="extraParams",

		CM_PARAMS_CREATESTREAM_REQUESTCHANNELID="requestChannelId",
		CM_PARAMS_CREATESTREAM_REQUESTSTREAMID="requestStreamId",
		CM_PARAMS_CREATESTREAM_REQUESTID="requestId",

		CM_PARAMS_STREAMCREATED_STREAMID="streamId",

		CM_PARAMS_CLOSESTREAM_REQUESTCHANNELID="requestChannelId",
		CM_PARAMS_CLOSESTREAM_REQUESTSTREAMID="requestStreamId",
		CM_PARAMS_CLOSESTREAM_REQUESTID="requestId",

		CM_PARAMS_STREAMCLOSED_STREAMID="streamId",

		CM_PARAMS_CREATEINS_REQUESTCHANNELID="requestChannelId",
		CM_PARAMS_CREATEINS_REQUESTSTREAMID="requestStreamId",
		CM_PARAMS_CREATEINS_REQUESTID="requestId",
		CM_PARAMS_CREATEINS_STREAMNAME="streamName",

		CM_PARAMS_INSCREATED_STREAMID="streamId",
		CM_PARAMS_INSCREATED_STREAMNAME="streamName",

		CM_PARAMS_CREATEOFS_STREAMID="streamId",
		CM_PARAMS_CREATEOFS_APPEND="append",
		CM_PARAMS_CREATEOFS_FILENAME="fileName",

		CM_PARAMS_OFSCREATED_STREAMID="streamId",
		CM_PARAMS_OFSCREATED_APPEND="append",
		CM_PARAMS_OFSCREATED_FILENAME="filename",

		CM_PARAMS_CREATEONS_REQUESTCHANNELID="requestChannelId",
        CM_PARAMS_CREATEONS_REQUESTSTREAMID="requestStreamId",
        CM_PARAMS_CREATEONS_REQUESTID="requestId",
        CM_PARAMS_CREATEONS_STREAMNAME="streamName",

		CM_PARAMS_ONSCREATED_STREAMID="streamId",
        CM_PARAMS_ONSCREATED_STREAMNAME="streamName",

		CM_PARAMS_ATTACHONSTOINS_ONSCONNECTIONID="ONSConnectionId",
		CM_PARAMS_ATTACHONSTOINS_ONSSTREAMID="ONSStreamId",
		CM_PARAMS_ATTACHONSTOINS_INSCONNECTIONID="INSConnectionId",
		CM_PARAMS_ATTACHONSTOINS_INSSTREAMID="INSStreamId",

		CM_PARAMS_ONSTOINSATTACHED_ONSCONNECTIONID="ONSConnectionId",
        CM_PARAMS_ONSTOINSATTACHED_ONSSTREAMID="ONSStreamId",
        CM_PARAMS_ONSTOINSATTACHED_INSCONNECTIONID="INSConnectionId",
        CM_PARAMS_ONSTOINSATTACHED_INSSTREAMID="INSStreamId",

--Variant,
VAR_NULL_VALUE="__null__value__",
VAR_TYPE="type",
VAR_TIMESTAMP="timestamp",
VAR_DATE="date",
VAR_TIME="time",
VAR_YEAR="year",
VAR_MONTH="month",
VAR_DAY="day",
VAR_HOUR="hour",
VAR_MIN="min",
VAR_SEC="sec",
VAR_ISDST="isdst",
VAR_INDEX_VALUE="__index__value__",
VAR_MAP_NAME="__map__name__",
VAR_ENUM_VALUE_NULL=1,
VAR_ENUM_VALUE_UNDEFINED=2,
VAR_ENUM_VALUE_BOOL=3,
VAR_ENUM_VALUE_INT8=4,
VAR_ENUM_VALUE_INT16=5,
VAR_ENUM_VALUE_INT32=6,
VAR_ENUM_VALUE_INT64=7,
VAR_ENUM_VALUE_UINT8=8,
VAR_ENUM_VALUE_UINT16=9,
VAR_ENUM_VALUE_UINT32=10,
VAR_ENUM_VALUE_UINT64=11,
VAR_ENUM_VALUE_DOUBLE=12,
VAR_ENUM_VALUE_NUMERIC=13,
VAR_ENUM_VALUE_TIMESTAMP=14,
VAR_ENUM_VALUE_DATE=15,
VAR_ENUM_VALUE_TIME=16,
VAR_ENUM_VALUE_STRING=17,
VAR_ENUM_VALUE_TYPED_MAP=18,
VAR_ENUM_VALUE_MAP=19,
VAR_ENUM_VALUE_BYTEARRAY=20,

--publish mode,
PM_UNKNOWN=0,
PM_UNKNOWN_STRING="unknown",
PM_RECORD=1,
PM_RECORD_STRING="record",
PM_APPEND=2,
PM_APPEND_STRING="append",
PM_LIVE=3,
PM_LIVE_STRING="live",

--log levels
_FATAL_=0,
_ERROR_=1,
_WARNING_=2,
_INFO_=3,
_DEBUG_=4,
_FINE_=5,
_FINEST_=6,

--keyframes per second in dot-seek file
KEYFRAMES_PER_SECOND=2,

SOT_UNKNOWN=0,
SOT_CS_CONNECT=1,
SOT_CS_DISCONNECT=2,
SOT_CS_SET_ATTRIBUTE=3,
SOT_SC_UPDATE_DATA=4, 
SOT_SC_UPDATE_DATA_ACK=5,
SOT_BW_SEND_MESSAGE=6,
SOT_SC_STATUS=7,
SOT_SC_CLEAR_DATA=8,
SOT_SC_DELETE_DATA=9,
SOT_CSC_DELETE_DATA=10,
SOT_SC_INITIAL_DATA=11,

}

function __genOrderedIndex( t )
    local orderedIndex = {}
    for key in pairs(t) do
        table.insert( orderedIndex, key )
    end
    table.sort( orderedIndex )
    return orderedIndex
end

function orderedNext(t, state)
    -- Equivalent of the next function, but returns the keys in the alphabetic
    -- order. We use a temporary ordered key table that is stored in the
    -- table being iterated.

    --print("orderedNext: state = "..tostring(state) )
    if state == nil then
        -- the first time, generate the index
        t.__orderedIndex = __genOrderedIndex( t )
        key = t.__orderedIndex[1]
        return key, t[key]
    end
    -- fetch the next value
    key = nil
    for i = 1,table.getn(t.__orderedIndex) do
        if t.__orderedIndex[i] == state then
            key = t.__orderedIndex[i+1]
        end
    end

    if key then
        return key, t[key]
    end

    -- no more value to return, cleanup
    t.__orderedIndex = nil
    return
end

function orderedPairs(t)
    -- Equivalent of the pairs() function on tables. Allows to iterate
    -- in order
    return orderedNext, t, nil
end


if arg[1]=="cpp" then
	file=io.open(arg[2],"w")
	file:write("//DO NOT EDIT THIS FILE\n")
	file:write("//THIS FILE IS GENERATED\n")
	file:write("#ifndef DEFINES_H_\n")
	file:write("#define DEFINES_H_\n")
	for k,v in orderedPairs(constants) do
		if(type(v)=="string") then
			file:write("#define "..k.." \""..v.."\"\n")
			file:write("#define "..k.."_LEN "..string.len(v).."\n")
		else
			file:write("#define "..k.." "..v.."\n")
		end
	end
	file:write("#endif /*DEFINES_H_*/\n")
	file:write("\n")
	file:close()
end

if arg[1]=="cs" then
	file=io.open(arg[2],"w")
	file:write("//DO NOT EDIT THIS FILE\n")
	file:write("//THIS FILE IS GENERATED\n")
	file:write("namespace "..arg[3].."\r\n{\r\n")
	file:write("public static class "..arg[4].."\r\n{\r\n")
	for k,v in orderedPairs(constants) do
		print (k,v)
		if(type(v)=="string") then
			file:write("public const string "..k.."=\""..v.."\";\n")
		else
			file:write("public const int "..k.."="..v..";\n")
		end
	end
	file:write("}\r\n}\r\n")
	file:close()
end


if arg[1]=="php" then
	file=io.open(arg[2],"w")
	file:write("<?php\n");
	file:write("//DO NOT EDIT THIS FILE\n")
	file:write("//THIS FILE IS GENERATED\n")
	file:write("class Constants\n")
	file:write("{\n")
	for k,v in orderedPairs(constants) do
                if(type(v)=="string") then
                        file:write("    const "..k.."=\""..v.."\";\n")
                else
                        file:write("    const "..k.."="..v..";\n")
                end
        end
        file:write("}\n")
end

