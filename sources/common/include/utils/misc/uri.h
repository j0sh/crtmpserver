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

#ifndef _URI_H
#define	_URI_H

#include "platform/platform.h"
#include "utils/misc/variant.h"

class DLLEXP URI
: public Variant {
private:
	//static Variant _dummy;
public:
	VARIANT_GETSET(string, originalUri, "");
	VARIANT_GETSET(string, fullUri, "");
	VARIANT_GETSET(string, fullUriWithAuth, "");
	VARIANT_GETSET(string, scheme, "");
	VARIANT_GETSET(string, userName, "");
	VARIANT_GETSET(string, password, "");
	VARIANT_GETSET(string, host, "");
	VARIANT_GETSET(string, ip, "");
	VARIANT_GETSET(uint16_t, port, 0);
	VARIANT_GETSET(bool, portSpecified, false);
	VARIANT_GETSET(string, fullDocumentPathWithParameters, "");
	VARIANT_GETSET(string, fullDocumentPath, "");
	VARIANT_GETSET(string, fullParameters, "");
	VARIANT_GETSET(string, documentPath, "");
	VARIANT_GETSET(string, document, "");
	VARIANT_GETSET(string, documentWithFullParameters, "");
	VARIANT_GETSET(Variant, parameters, Variant());

	static bool FromVariant(Variant & variant, URI &uri);
	static bool FromString(string stringUri, bool resolveHost, URI &uri);
};

#endif	/* _URI_H */

