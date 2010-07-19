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

	/*
	 * pOutStream - the out-stream to be linked to this in stream.
	 * reverseLink - if true, pOutStream::Link will be called internally
	 *               this is used to break the infinite calls.
	 * */
	virtual bool Link(BaseOutStream *pOutStream, bool reverseLink = true);

	/*
	 * pOutStream - the out-stream to be unlinked from this in stream.
	 * reverseUnLink - if true, pOutStream::UnLink will be called internally
	 *               this is used to break the infinite calls
	 * */
	virtual bool UnLink(BaseOutStream *pOutStream, bool reverseUnLink = true);

	/*
	 * This will start the feeding process
	 * absoluteTimestamp - the timestamp where we want to seek
	 *                     before start the feeding process
	 * */
	virtual bool Play(double absoluteTimestamp, double length);

	/*
	 * This will pause the feeding process
	 * */
	virtual bool Pause();

	/*
	 * This will resume the feeding process
	 * */
	virtual bool Resume();

	/*
	 * This will seek to the specified point in time.
	 * */
	virtual bool Seek(double absoluteTimestamp);

	/*
	 * This will stop the feeding process
	 * */
	virtual bool Stop();

	/*
	 * Called after the link is complete
	 * pOutStream - the newly added stream
	 * */
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream) = 0;

	/*
	 * Called after the link is broken
	 * pOutStream - the removed stream
	 * */
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream) = 0;
};

#endif	/* _BASEINSTREAM_H */

