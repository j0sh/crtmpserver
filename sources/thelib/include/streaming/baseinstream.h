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


#ifndef _BASEINSTREAM_H
#define	_BASEINSTREAM_H

#include "streaming/basestream.h"

class BaseOutStream;

/*!
	@class BaseInStream
 */
class DLLEXP BaseInStream
: public BaseStream {
private:
	bool _canCallOutStreamDetached;
protected:
	map<uint32_t, BaseOutStream *> _linkedStreams;
	LinkedListNode<BaseOutStream *> *_pOutStreams;
public:
	BaseInStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name);
	virtual ~BaseInStream();

	vector<BaseOutStream *> GetOutStreams();

	/*!
		@brief Returns information about the stream
		@param info
	 */
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	/*!
		@brief Links an out-stream to this stream
		@param pOutStream - the out-stream to be linked to this in stream.
		@param reverseLink - if true, pOutStream::Link will be called internally this is used to break the infinite calls.
	 */
	virtual bool Link(BaseOutStream *pOutStream, bool reverseLink = true);

	/*!
		@brief Unlinks an out-stream tot his stream
		@param pOutStream - the out-stream to be unlinked from this in stream.
		@param reverseUnLink - if true, pOutStream::UnLink will be called internally this is used to break the infinite calls
	 */
	virtual bool UnLink(BaseOutStream *pOutStream, bool reverseUnLink = true);

	/*!
		@brief This will start the feeding process
		@param absoluteTimestamp - the timestamp where we want to seek before start the feeding process
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
		@param pOutStream - the newly added stream
	 */
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream) = 0;

	/*!
		@brief Called after the link is broken
		@param pOutStream - the removed stream
	 */
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream) = 0;
};

#endif	/* _BASEINSTREAM_H */

