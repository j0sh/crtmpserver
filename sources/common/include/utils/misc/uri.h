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

typedef struct _URI {
	string fullUri;
	string scheme;
	string host;
	string ip;
	uint16_t port;
	string userName;
	string password;
	string fullDocumentPath;
	string documentPath;
	string document;
	map<string, string> parameters;

	DLLEXP void Reset();

	DLLEXP Variant ToVariant();
	DLLEXP static bool FromVariant(Variant & variant, _URI & uri);

	DLLEXP string ToString();
	DLLEXP static bool FromString(string stringUri, bool resolveHost, _URI & uri);
} URI;

#endif	/* _URI_H */

