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


#ifndef _BASESTREAM_H
#define	_BASESTREAM_H

#include "common.h"
#include "streaming/streamcapabilities.h"

class StreamsManager;
class BaseProtocol;

/*!
	@class BaseStream
*/
class DLLEXP BaseStream {
protected:
	StreamsManager *_pStreamsManager;
	uint64_t _type;
	uint32_t _uniqueId;
	BaseProtocol *_pProtocol;
	string _name;
	double _creationTimestamp;
public:
	BaseStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name);
	virtual ~BaseStream();

	/*!
		@brief Returns the stream manager. This is read-only
	*/
	StreamsManager * GetStreamsManager();

	/*!
		@brief Returns the stream capabilities. Specifically, codec and codec related info
	*/
	virtual StreamCapabilities * GetCapabilities() = 0;

	/*!
		@brief Returns the type of this stream. This is read-only
	*/
	uint64_t GetType();

	/*!
		@brief Returns the unique id of this stream. This is read-only
	*/
	uint32_t GetUniqueId();

	/*!
		@brief returns the creation timestamp expressed in milliseconds for 1970 epoch
	*/
	double GetSpawnTimestamp();

	/*!
		@brief Returns the name of this stream. This is setup-once
	*/
	string GetName();

	/*!
		@brief Sets the name of the stream
		@param name - Name of stream in string format
	*/
	void SetName(string name);

	/*!
		@brief This will return information about the stream
		@param info
	*/
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	/*!
		@brief Returns the protocol that owns this stream.
	*/
	BaseProtocol * GetProtocol();

	/*!
		@brief Tells if this stream is enqueued for delete or not based on the pProtocol
	*/
	virtual bool IsEnqueueForDelete();

	/*!
		@brief Will enqueue this stream for delete along with his protocol
	*/
	virtual void EnqueueForDelete();

	/*!
		@brief This will start the feeding process
		@param absoluteTimestamp - the timestamp where we want to see before start the feeding process
		@param length - time limit
	*/
	virtual bool Play(double absoluteTimestamp, double length) = 0;

	/*!
		@brief This will pause the feeding process
	*/
	virtual bool Pause() = 0;

	/*!
		@brief This will resume the feeding process
	*/
	virtual bool Resume() = 0;

	/*!
		@brief  will seek to the specified point in time.
		@param absoluteTimestamp
	*/
	virtual bool Seek(double absoluteTimestamp) = 0;

	/*!
		 @brief This will stop the feeding process
	*/
	virtual bool Stop() = 0;


	/*!
		@brief Called when a play command was issued
		@param absoluteTimestamp - the timestamp where we want to seek before start the feeding process
		@param length
	*/
	virtual bool SignalPlay(double &absoluteTimestamp, double &length) = 0;

	/*!
		@brief Called when a pasue command was issued
	*/
	virtual bool SignalPause() = 0;

	/*!
		@brief Called when a resume command was issued
	*/
	virtual bool SignalResume() = 0;

	/*!
		@brief Called when a seek command was issued
		@param absoluteTimestamp
	*/
	virtual bool SignalSeek(double &absoluteTimestamp) = 0;

	/*!
		@brief Called when a stop command was issued
	*/
	virtual bool SignalStop() = 0;

	/*!
		@param pData - the buffer containing the data
		@param dataLength - the size of pData in bytes
		@param processedLength - if pData is only partial data, this shows the numbers of bytes processed so far, excluding pData
		@param totalLength - if pData is only partial data, this shows the total number of bytes inside the current packet
		@param isAudio - true if pData is audio data, false if pData is video data
		@discussion Rules:
			dataLength+processedLength<=totalLength
			dataLength<=totalLength
			processedLength<=totalLength
			dataLength!=0
	*/
	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio) = 0;

	/*!
		@brief The networking layer signaled the availability for sending data
	*/
	virtual void ReadyForSend() = 0;

	/*!
		@brief This is called to ensure that the linking process can be done
		@param type - the target type to which this strem must be linked against
	*/
	virtual bool IsCompatibleWithType(uint64_t type) = 0;
};

#endif	/* _BASESTREAM_H */

