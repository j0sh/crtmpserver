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


#ifndef _GENERICPROTOCOL_H
#define	_GENERICPROTOCOL_H

#include "protocols/baseprotocol.h"

namespace app_applestreamingclient {
	class ClientContext;

	class GenericProtocol
	: public BaseProtocol {
	private:
		uint32_t _contextId;
	public:
		GenericProtocol(uint64_t type);
		virtual ~GenericProtocol();

		virtual bool Initialize(Variant &parameters);
		ClientContext *GetContext();
		bool DoHTTPRequest();
	};
}

#endif	/* _GENERICPROTOCOL_H */

