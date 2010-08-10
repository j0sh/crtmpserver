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


#ifndef _ENDIANESS_H
#define	_ENDIANESS_H

//LITTLE ENDIAN SYSTEMS
#ifdef LITTLE_ENDIAN_BYTE_ALIGNED
#include "platform/endianess/little_endian_byte_aligned.h"
#endif

#ifdef LITTLE_ENDIAN_SHORT_ALIGNED
#include "platform/endianess/little_endian_short_aligned.h"
#endif

#ifdef BIG_ENDIAN_BYTE_ALIGNED
#include "platform/endianess/big_endian_byte_aligned.h"
#endif

#ifdef BIG_ENDIAN_SHORT_ALIGNED
#include "platform/endianess/big_endian_short_aligned.h"
#endif

#endif	/* _ENDIANESS_H */


