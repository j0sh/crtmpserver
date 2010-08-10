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


#ifndef _BIG_ENDIAN_BYTE_ALIGNED_H
#define	_BIG_ENDIAN_BYTE_ALIGNED_H

#define htonll(x)   (x)
#define ntohll(x)   (x)

//adobe shit. Sometimes, Adobe's reprezentation of 0x0a0b0c0d
//in "network-order" is like this: 0b 0c 0d 0a
#define htona(x)    (((x)<<8)|((x)>>24))
#define ntoha(x)    (((x)>>8)|((x)<<24))

#define ntohsp(x) ntohs(*((uint16_t *)(x)))
#define ntohlp(x) ntohl(*((uint32_t *)(x)))
#define ntohap(x) ntoha(*((uint32_t *)(x)))
#define ntohllp(x) ntohll(*((uint64_t *)(x)))

#define put_htons(x,v) (*((uint16_t*)(x)) = htons(v))
#define put_htonl(x,v) (*((uint32_t*)(x)) = htonl(v))
#define put_htonll(x,v) (*((uint64_t*)(x)) = htonll(v))
/* TODO: Do we need put_value methods for aligned access when not swapping bytes */
/* TODO: Do we need put_htonll() or put_htona() */

#endif	/* _BIG_ENDIAN_BYTE_ALIGNED_H */
