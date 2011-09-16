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


#ifndef _BASEOUTSTREAM_H
#define	_BASEOUTSTREAM_H

#include "streaming/basestream.h"

class BaseInStream;

class DLLEXP BaseOutStream
: public BaseStream {
private:
	bool _canCallDetachedFromInStream;
protected:
	BaseInStream *_pInStream;
public:
	BaseOutStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name);
	virtual ~BaseOutStream();

	/*!
		@brief Returns the stream capabilities. Specifically, codec and codec related info
	*/
	virtual StreamCapabilities * GetCapabilities();

	/*!
		@brief The networking layer signaled the availability for sending data
	*/
	virtual void ReadyForSend();

	/*!
		@brief Links an in-stream to this stream
		@param pInStream - the in-stream where we want to attach
		@param reverseLink - if true, pInStream::Link will be called internally this is used to break the infinite calls.
	*/
	virtual bool Link(BaseInStream *pInStream, bool reverseLink = true);

	/*!
		@brief Unlinks an in-stream to this stream
		@param reverseUnLink - if true, pInStream::UnLink will be called internally this is used to break the infinite calls
	*/
	virtual bool UnLink(bool reverseUnLink = true);

	/*!
		@brief Returns true if this stream is linked to an inbound stream. Otherwise, returns false
	*/
	bool IsLinked();

	/*!
		@brief Returns the feeder of this stream
	*/
	BaseInStream *GetInStream();

	/*!
		@brief  This will return information about the stream
		@param info
	*/
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	/*!
		@brief This will start the feeding process
		@param absoluteTimestamp - the timestamp where we want to seek before start the feeding process\
		@param length
	*/
	virtual bool Play(double absoluteTimestamp, double length);

	/*!
		@brief This will pause the feeding process
	*/
	virtual bool Pause();

	/*!
		@brief This will resume the feeding process
	*/
	virtual bool Resume();

	/*!
		@brief This will seek to the specified point in time.
		@param absoluteTimestamp
	*/
	virtual bool Seek(double absoluteTimestamp);

	/*!
		@brief This will stop the feeding process
	*/
	virtual bool Stop();

	/*!
		@brief Called after the link is complete
	*/
	virtual void SignalAttachedToInStream() = 0;

	/*!
		@brief Called after the link is broken
	*/
	virtual void SignalDetachedFromInStream() = 0;

	/*!
		@brief Called when the feeder finished the work
	*/
	virtual void SignalStreamCompleted() = 0;
};

#endif	/* _BASEOUTBOUNDSTREAM_H */

