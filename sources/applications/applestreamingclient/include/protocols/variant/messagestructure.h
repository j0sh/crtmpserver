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

#define ASC_REQ_TYPE_CONTEXT_CREATE			"contextCreate"
#define ASC_REQ_TYPE_CONTEXT_LIST			"contextList"
#define ASC_REQ_TYPE_CONTEXT_CLOSE			"contextClose"
#define ASC_REQ_TYPE_CONTEXT_CLOSE_ALL		"contextCloseAll"
#define ASC_REQ_TYPE_COMMAND_PLAY			"commandPlay"
#define ASC_REQ_TYPE_COMMAND_PAUSE			"commandPause"
#define ASC_REQ_TYPE_COMMAND_RESUME			"commandResume"
#define ASC_REQ_TYPE_INFO_LIST_STREAMS		"listStreams"
#define ASC_REQ_TYPE_INFO_LIST_ALL_STREAMS	"listAllStreams"

#define ASC_REQ_COMMAND_PLAY_URI_KEY			"uri"
#define ASC_REQ_COMMAND_PLAY_SESSION_ID_KEY		"sessionId"
#define ASC_REQ_COMMAND_PLAY_PASSWORD_KEY	"keyPassword"

#define ASC_RES_CONTEXT_CREATE_CONTEXT_ID_KEY	"contextId"

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
#define ASC_REQ_COMMAND_PLAY_URI(v)			ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_URI_KEY)
#define ASC_REQ_COMMAND_PLAY_SESSION_ID(v)	ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_SESSION_ID_KEY)
#define ASC_REQ_COMMAND_PLAY_PASSWORD(v)	ASC_REQ_PARAM(v,ASC_REQ_COMMAND_PLAY_PASSWORD_KEY)

#define ASC_RES(v)						(v)["response"]
#ifdef INCLUDE_DEBUG_LOCATIONS
#define ASC_RES_DEBUG_FILE(v)			ASC_RES(v)["debug"]["fileName"]=__FILE__
#define ASC_RES_DEBUG_LINE_NUMBER(v)	ASC_RES(v)["debug"]["lineNumber"]=__LINE__
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

#define ASC_REQ_BUILD_INFO_LIST_STREAMS(v,contextId) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_LIST_STREAMS,contextId,Variant())

#define ASC_REQ_BUILD_INFO_LIST_ALL_STREAMS(v) \
	ASC_REQ_BUILD(v,ASC_REQ_TYPE_INFO_LIST_ALL_STREAMS,0,Variant())


#define ASC_RES_BUILD(r,status,params) \
do {\
	ASC_RES_DEBUG_FILE(r); \
	ASC_RES_DEBUG_LINE_NUMBER(r); \
	ASC_RES_STATUS(r)=(status); \
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
			ASSERT("Unknown status code: %d",(status)); \
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
	for(uint32_t contextIdsIndex;contextIdsIndex<(contextIds).size();contextIdsIndex++) \
		responseParameters.PushToArray((contextIds)[contextIdsIndex]); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,responseParameters); \
} while(0)

#define ASC_RES_BUILD_UNKNOWN_REQUEST_TYPE(r) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_UNKNOWN_REQUEST_TYPE,Variant())

#define ASC_RES_BUILD_NYI(r) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_NYI,Variant())

#define ASC_RES_BUILD_CONTEXT_CREATE_FAILED(r) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_CONTEXT_CREATE_FAILED,Variant())

#define ASC_RES_BUILD_CONTEXT_NOT_FOUND(r) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_CONTEXT_NOT_FOUND,Variant())

#define ASC_RES_BUILD_COMMAND_PLAY_FAILED(r) \
	ASC_RES_BUILD(r,ASC_RES_STATUS_COMMAND_PLAY_FAILED,Variant())

#define ASC_RES_BUILD_OK_INFO_LIST_STREAMS(r,streamNames) \
do { \
	Variant responseParameters; \
	responseParameters.IsArray(true); \
	for(uint32_t streamNamesIndex;streamNamesIndex<(streamNames).size();streamNamesIndex++) \
		responseParameters.PushToArray((streamNames)[streamNamesIndex]); \
	ASC_RES_BUILD(r,ASC_RES_STATUS_OK,responseParameters); \
} while(0)

#define ASC_RES_BUILD_OK_INFO_LIST_ALL_STREAMS(r,streamNames) \
	ASC_RES_BUILD_OK_INFO_LIST_STREAMS(r,streamNames)

#endif	/* _MESSAGESTRUCTURE_H */

