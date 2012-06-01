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

#ifdef HAS_VOD_MANAGER
#ifndef _BASEVODSOURCE_H
#define	_BASEVODSOURCE_H

#include "common.h"

class BaseVODSource
: public Variant {
private:
	Variant _metadata;
public:
	BaseVODSource();
	virtual ~BaseVODSource();

	VARIANT_GETSET(string, OriginMediaFilePath, "");
	VARIANT_GETSET(string, OptimizedMediaFilePath, "");
	VARIANT_GETSET(string, SeekFilePath, "");
	VARIANT_GETSET(string, MetadataFilePath, "");
	VARIANT_GETSET(Variant &, Metadata, _metadata);
};

#endif	/* _BASEVODSOURCE_H */
#endif /* HAS_VOD_MANAGER */

