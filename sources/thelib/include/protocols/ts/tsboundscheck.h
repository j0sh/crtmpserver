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


#ifdef HAS_PROTOCOL_TS
#ifndef _TSBOUNDSCHECK_H
#define	_TSBOUNDSCHECK_H

#define CHECK_BOUNDS(size) if((cursor+(size))>(maxCursor)){ FATAL("Bounds error: cursor: %u; size: %u; maxCursor: %u",(uint32_t)cursor,(uint32_t)(size),(uint32_t)maxCursor);return false;}

#endif	/* _TSBOUNDSCHECK_H */
#endif	/* HAS_PROTOCOL_TS */

