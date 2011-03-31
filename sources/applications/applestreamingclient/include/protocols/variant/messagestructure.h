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

#ifndef _MESSAGESTRUCTURE_H
#define	_MESSAGESTRUCTURE_H

#define INCLUDE_DEBUG_LOCATIONS

#define ASC_REQ_TYPE_CONTEXT_CREATE				"contextCreate"
#define ASC_REQ_TYPE_CONTEXT_LIST				"contextList"
#define ASC_REQ_TYPE_CONTEXT_CLOSE				"contextClose"
#define ASC_REQ_TYPE_CONTEXT_CLOSE_ALL			"contextCloseAll"

#define ASC_REQ_TYPE_COMMAND_PLAY				"commandPlay"
#define ASC_REQ_TYPE_COMMAND_SET_BITRATES		"commandSetBitrates"
#define ASC_REQ_TYPE_COMMAND_PAUSE				"commandPause"
#define ASC_REQ_TYPE_COMMAND_RESUME				"commandResume"
#define ASC_REQ_TYPE_COMMAND_SELECT_BANDWIDTH	"commandSelectBandwidth"
#define ASC_REQ_TYPE_COMMAND_SEEK				"commandSeek"
#define	ASC_REQ_TYPE_COMMAND_SELECT_AV_CHANNELS	"commandSelectAVChannels"

#define ASC_REQ_TYPE_INFO_LIST_STREAMS			"infoListStreams"
#define ASC_REQ_TYPE_INFO_LIST_ALL_STREAMS		"infoListAllStreams"
#define	ASC_REQ_TYPE_INFO_BANDWIDTH				"infoBandwidth"
#define	ASC_REQ_TYPE_INFO_PLAYBACK				"infoPlayback"

#define ASC_REQ_COMMAND_SET_BITRATES_BITRATES		"bitrates"
#define ASC_REQ_COMMAND_PLAY_URI_KEY				"uri"
#define ASC_REQ_COMMAND_PLAY_SESSION_ID_KEY			"sessionId"
#define ASC_REQ_COMMAND_PLAY_PASSWORD_KEY			"keyPassword"
#define ASC_REQ_COMMAND_SELECT_BANDWIDTH_BANDWIDTH	"bandwidth"
#define ASC_REQ_COMMAND_SEEK_POINT					"point"
#define	ASC_REQ_COMMAND_SELECT_AV_CHANNELS_AUDIO	"audio"
#define	ASC_REQ_COMMAND_SELECT_AV_CHANNELS_VIDEO	"video"

#define ASC_RES_CONTEXT_CREATE_CONTEXT_ID_KEY			"contextId"
#define ASC_RES_INFO_BANDWIDTH_AVAILABLE_BWS			"availableBws"
#define ASC_RES_INFO_BANDWIDTH_DETECTED_BW				"detectedBw"
#define ASC_RES_INFO_BANDWIDTH_SELECTED_BW				"selectedBw"
#define ASC_RES_INFO_BANDWIDTH_BUFFER_LEVEL				"bufferLevel"
#define ASC_RES_INFO_BANDWIDTH_MAX_BUFFER_LEVEL			"bufferMaxLevel"
#define ASC_RES_INFO_BANDWIDTH_MAX_BUFFER_LEVEL_PERCENT	"bufferLevelPercent"
#define ASC_RES_INFO_PLAYBACK_MIN_TS					"minTs"
#define ASC_RES_INFO_PLAYBACK_MAX_TS					"maxTs"
#define ASC_RES_INFO_PLAYBACK_CHUNKS_COUNT				"chunksCount"
#define ASC_RES_INFO_PLAYBACK_CURRENT_CHUNK				"currentChunk"
#define ASC_RES_INFO_PLAYBACK_CURRENT_TS				"currentTs"

#define ASC_RES_STATUS_OK							0
#define ASC_RES_STATUS_OK_DESC						"OK"
#define ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE			1
#define ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE_DESC	"Unknwon request type"
#define ASC_RES_STATUS_NYI							2
#define ASC_RES_STATUS_NYI_DESC						"Not yet implemented"
#define ASC_RES_STATUS_CONTEXT_CREATE_FAILED		3
#define ASC_RES_STATUS_CONTEXT_CREATE_FAILED_DESC	"Unable to create context"
#define ASC_RES_STATUS_CONTEXT_NOT_FOUND			4
#define ASC_RES_STATUS_CONTEXT_NOT_FOUND_DESC		"Context not found"
#define ASC_RES_STATUS_COMMAND_PLAY_FAILED			5
#define ASC_RES_STATUS_COMMAND_PLAY_FAILED_DESC		"Play command failed"

#define ASC_RES_STATUS_UNKNOWN	"Unknown status code"

#define ASC_REQ(v)							(v)["request"]
#ifdef INCLUDE_DEBUG_LOCATIONS
#define ASC_REQ_DEBUG_FILE(v)				ASC_REQ(v)["debug"]["fileName"]=__FILE__
#define ASC_REQ_DEBUG_LINE_NUMBER(v)		ASC_REQ(v)["debug"]["lineNumber"]=__LINE__
#else
#define ASC_REQ_DEBUG_FILE(v)
#define ASC_REQ_DEBUG_LINE_NUMBER(v)
#endif /* INCLUDE_DEBUG_LOCATIONS */
#define ASC_REQ_TYPE(v)						ASC_REQ(v)["type"]
#define ASC_REQ_CONTEXT_ID(v)				ASC_REQ(v)["contextId"]
#define ASC_REQ_PARAMS(v)					ASC_REQ(v)["parameters"]
#define ASC_REQ_PARAM(v,key)				ASC_REQ_PARAMS(v)[(key)]
#define ASC_REQ_COMMAND_SET_BITRATES_BWS(v)	ASC_REQ_PARAM(v,ASC_REQ_COMMAND_SET_BITRATES_BITRATES)
#define ASC_REQ_COMMAND_PLAY_URI(v)			ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_URI_KEY)
#define ASC_REQ_COMMAND_PLAY_SESSION_ID(v)	ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_SESSION_ID_KEY)
#define ASC_REQ_COMMAND_PLAY_PASSWORD(v)	ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_PASSWORD_KEY)

#define ASC_RES(v)						(v)["response"]
#ifdef INCLUDE_DEBUG_LOCATIONS
#define ASC_RES_DEBUG_FILE(v)			ASC_RES(v)["debug"]["fileName"]=__FILE__
#define ASC_RES_DEBUG_LINE_NUMBER(v)	ASC_RES(v)["debug"]["lineNumber"]=(uint32_t)__LINE__
#else
#define ASC_RES_DEBUG_FILE(v)
#define ASC_RES_DEBUG_LINE_NUMBER(v)
#endif /* INCLUDE_DEBUG_LOCATIONS */
#define ASC_RES_STATUS(v)				ASC_RES(v)["status"]
#define ASC_RES_STATUS_DESC(v)			ASC_RES(v)["statusDescription"]
#define ASC_RES_PARAMS(v)				ASC_RES(v)["parameters"]
#define ASC_RES_PARAM(v,key)			ASC_RES_PARAMS(v)[(key)]

#define ASC_REQ_BUILD(v,type,contextId,params) \
do {\
	ASC_REQ_DEBUG_FILE(v); \
	ASC_REQ_DEBUG_LINE_NUMBER(v); \
	ASC_REQ_TYPE(v)=(type); \
	ASC_REQ_CONTEXT_ID(v)=(uint32_t)(contextId); \
	ASC_REQ_PARAMS(v)=(params); \
} while(0)

#define ASC_REQ_BUILD_CONTEXT_CREATE(v) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_CONTEXT_CREATE,0,Variant())

#define ASC_REQ_BUILD_CONTEXT_LIST(v) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_CONTEXT_LIST,0,Variant())

#define ASC_REQ_BUILD_CONTEXT_CLOSE(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_CONTEXT_CLOSE,contextId,Variant())

#define ASC_REQ_BUILD_CONTEXT_CLOSE_ALL(v) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_CONTEXT_CLOSE_ALL,0,Variant())

#define ASC_REQ_BUILD_COMMAND_SET_BITRATES(v,contextId,bws) \
do { \
	Variant parameters; \
	parameters[ASC_REQ_COMMAND_SET_BITRATES_BITRATES].IsArray(true); \
	for(uint32_t i=0;i<bws.size();i++) { \
		parameters[ASC_REQ_COMMAND_SET_BITRATES_BITRATES].PushToArray((uint32_t)bws[i]); \
	} \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_SET_BITRATES,contextId,parameters); \
} while(0)

#define ASC_REQ_BUILD_COMMAND_PLAY(v,contextId,uri,sessionId,keyPassword) \
do { \
	Variant playParameters; \
	playParameters[ASC_REQ_COMMAND_PLAY_URI_KEY]=uri; \
	playParameters[ASC_REQ_COMMAND_PLAY_SESSION_ID_KEY]=sessionId; \
	playParameters[ASC_REQ_COMMAND_PLAY_PASSWORD_KEY]=keyPassword; \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_PLAY,contextId,playParameters); \
} while(0)

#define ASC_REQ_BUILD_COMMAND_PAUSE(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_PAUSE,contextId,Variant())

#define ASC_REQ_BUILD_COMMAND_RESUME(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_RESUME,contextId,Variant())

#define ASC_REQ_BUILD_COMMAND_SELECT_BANDWIDTH(v,contextId,value) \
do { \
	Variant params; \
	params[ASC_REQ_COMMAND_SELECT_BANDWIDTH_BANDWIDTH]=(uint32_t)value; \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_SELECT_BANDWIDTH,contextId,params); \
} while(0)

#define ASC_REQ_BUILD_COMMAND_SEEK(v,contextId,value) \
do { \
	Variant params; \
	params[ASC_REQ_COMMAND_SEEK_POINT]=(double)value; \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_SEEK,contextId,params); \
} while(0)

#define ASC_REQ_BUILD_COMMAND_SELECT_AV_CHANNELS(v,contextId,audio,video) \
do { \
	Variant params; \
	params[ASC_REQ_COMMAND_SELECT_AV_CHANNELS_AUDIO]=(uint32_t)audio; \
	params[ASC_REQ_COMMAND_SELECT_AV_CHANNELS_VIDEO]=(uint32_t)video; \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_COMMAND_SELECT_AV_CHANNELS,contextId,params); \
} while(0)

#define ASC_REQ_BUILD_INFO_LIST_STREAMS(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_LIST_STREAMS,contextId,Variant())

#define ASC_REQ_BUILD_INFO_LIST_ALL_STREAMS(v) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_LIST_ALL_STREAMS,0,Variant())

#define ASC_REQ_BUILD_INFO_BANDWIDTH(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_BANDWIDTH,contextId,Variant())

#define ASC_REQ_BUILD_INFO_PLAYBACK(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_PLAYBACK,contextId,Variant())

#define ASC_RES_BUILD(r,status,params) \
do {\
	ASC_RES_DEBUG_FILE(r); \
	ASC_RES_DEBUG_LINE_NUMBER(r); \
	ASC_RES_STATUS(r)=(uint32_t) (status); \
	switch ((status)) { \
		case ASC_RES_STATUS_OK: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_OK_DESC; \
			break; \
		} \
		case ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE_DESC; \
			break; \
		} \
		case ASC_RES_STATUS_NYI: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_NYI_DESC; \
			break; \
		} \
		case ASC_RES_STATUS_CONTEXT_CREATE_FAILED: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_CONTEXT_CREATE_FAILED_DESC; \
			break; \
		} \
		case ASC_RES_STATUS_CONTEXT_NOT_FOUND: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_CONTEXT_NOT_FOUND_DESC; \
			break; \
		} \
		case ASC_RES_STATUS_COMMAND_PLAY_FAILED: \
		{ \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_COMMAND_PLAY_FAILED_DESC; \
			break; \
		} \
		default: \
		{ \
			ASSERT("Unknown status code: %u",(uint32_t)(status)); \
			ASC_RES_STATUS_DESC(r)=ASC_RES_STATUS_UNKNOWN; \
			break;\
		} \
	} \
	ASC_RES_PARAMS(r)=(params); \
} while (0)

#define ASC_RES_BUILD_OK(r,parameters) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,parameters)

#define ASC_RES_BUILD_OK_CONTEXT_CREATE(r,contextId) \
do { \
	Variant responseParameters; \
	responseParameters[ASC_RES_CONTEXT_CREATE_CONTEXT_ID_KEY]=(contextId); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,responseParameters); \
} while(0)

#define ASC_RES_BUILD_OK_CONTEXT_LIST(r,contextIds) \
do { \
	Variant responseParameters; \
	responseParameters.IsArray(true); \
	for(uint32_t contextIdsIndex=0;contextIdsIndex<(contextIds).size();contextIdsIndex++) \
		responseParameters.PushToArray((contextIds)[contextIdsIndex]); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,responseParameters); \
} while(0)

#define ASC_RES_BUILD_UNKNOWN_REQUEST_TYPE(r) \
do { \
	Variant __tempVariant; \
	ASC_RES_BUILD(r,ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE,__tempVariant); \
} while(0)

#define ASC_RES_BUILD_NYI(r) \
do { \
    Variant __tempVariant; \
	ASC_RES_BUILD(r,ASC_RES_STATUS_NYI, __tempVariant); \
} while(0)

#define ASC_RES_BUILD_CONTEXT_CREATE_FAILED(r) \
do { \
    Variant __tempVariant; \
	ASC_RES_BUILD(r,ASC_RES_STATUS_CONTEXT_CREATE_FAILED, __tempVariant); \
} while(0)

#define ASC_RES_BUILD_CONTEXT_NOT_FOUND(r) \
do { \
	Variant __tempVariant; \
	ASC_RES_BUILD(r,ASC_RES_STATUS_CONTEXT_NOT_FOUND,__tempVariant); \
} while(0)

#define ASC_RES_BUILD_COMMAND_PLAY_FAILED(r) \
do { \
    Variant __tempVariant; \
	ASC_RES_BUILD(r,ASC_RES_STATUS_COMMAND_PLAY_FAILED, __tempVariant); \
} while(0)

#define ASC_RES_BUILD_OK_INFO_LIST_STREAMS(r,streamNames) \
do { \
	Variant responseParameters; \
	responseParameters.IsArray(true); \
	for(uint32_t streamNamesIndex=0;streamNamesIndex<(streamNames).size();streamNamesIndex++) \
		responseParameters.PushToArray((streamNames)[streamNamesIndex]); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,responseParameters); \
} while(0)

#define ASC_RES_BUILD_OK_INFO_LIST_ALL_STREAMS(r,streamNames) \
	ASC_RES_BUILD_OK_INFO_LIST_STREAMS(r,streamNames)

#define ASC_RES_BUILD_OK_INFO_BANDWIDTH(r,available, detected, selected, bufferLevel, maxBufferLevel, bufferLevelPercent) \
do { \
	Variant params; \
	params[ASC_RES_INFO_BANDWIDTH_AVAILABLE_BWS].IsArray(true); \
	for(uint32_t i=0;i<available.size();i++) \
		params[ASC_RES_INFO_BANDWIDTH_AVAILABLE_BWS].PushToArray((double)available[i]); \
	params[ASC_RES_INFO_BANDWIDTH_DETECTED_BW]=(double)(detected); \
	params[ASC_RES_INFO_BANDWIDTH_SELECTED_BW]=(double)(selected); \
	params[ASC_RES_INFO_BANDWIDTH_BUFFER_LEVEL]=(uint32_t)(bufferLevel); \
	params[ASC_RES_INFO_BANDWIDTH_MAX_BUFFER_LEVEL]=(uint32_t)(maxBufferLevel); \
	params[ASC_RES_INFO_BANDWIDTH_MAX_BUFFER_LEVEL_PERCENT]=(double)(bufferLevelPercent); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,params); \
} while(0)

#define ASC_RES_BUILD_OK_INFO_PLAYBACK(r,minTs, maxTs, chunksCount, currentTs, currentChunk) \
do { \
	Variant params; \
	params[ASC_RES_INFO_PLAYBACK_MIN_TS]=(double)(minTs); \
	params[ASC_RES_INFO_PLAYBACK_MAX_TS]=(double)(maxTs); \
	params[ASC_RES_INFO_PLAYBACK_CHUNKS_COUNT]=(uint32_t)(chunksCount); \
	params[ASC_RES_INFO_PLAYBACK_CURRENT_CHUNK]=(uint32_t)(currentChunk); \
	params[ASC_RES_INFO_PLAYBACK_CURRENT_TS]=(double)(currentTs); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,params); \
} while(0)

#endif	/* _MESSAGESTRUCTURE_H */

