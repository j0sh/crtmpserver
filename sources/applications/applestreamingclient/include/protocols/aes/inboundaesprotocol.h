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


#ifndef _INBOUNDAESPROTOCOL_H
#define	_INBOUNDAESPROTOCOL_H

#include "protocols/genericprotocol.h"

namespace app_applestreamingclient {

	class InboundAESProtocol
	: public GenericProtocol {
	private:
		IOBuffer _tempBuffer;
		IOBuffer _inputBuffer;
		EVP_CIPHER_CTX _decContex;
		bool _lastChunk;
		uint8_t *_pIV;
		uint8_t *_pKey;
		uint32_t _totalDecrypted;
	public:
		InboundAESProtocol();
		virtual ~InboundAESProtocol();

		virtual IOBuffer * GetInputBuffer();

		virtual bool Initialize(Variant &parameters);
		virtual bool AllowFarProtocol(uint64_t type);
		virtual bool AllowNearProtocol(uint64_t type);
		virtual bool SignalInputData(int32_t recvAmount);
		virtual bool SignalInputData(IOBuffer &buffer);
	};
}
#endif	/* _INBOUNDAESPROTOCOL_H */

