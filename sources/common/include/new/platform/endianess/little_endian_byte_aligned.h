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



#ifndef _LITTLE_ENDIAN_H
#define	_LITTLE_ENDIAN_H

#ifndef DONT_DEFINE_HTONLL
#define htonll(x) \
                    ((((x) & 0xff00000000000000LL) >> 56) | \
                    (((x) & 0x00ff000000000000LL) >> 40) | \
                    (((x) & 0x0000ff0000000000LL) >> 24) | \
                    (((x) & 0x000000ff00000000LL) >> 8) | \
                    (((x) & 0x00000000ff000000LL) << 8) | \
                    (((x) & 0x0000000000ff0000LL) << 24) | \
                    (((x) & 0x000000000000ff00LL) << 40) | \
                    (((x) & 0x00000000000000ffLL) << 56))
#define ntohll(x)   htonll(x)
#endif /* DONT_DEFINE_HTONLL */

//adobe shit. Sometimes, Adobe's reprezentation of 0x0a0b0c0d
//in "network-order" is like this: 0b 0c 0d 0a
#define htona(x)    ((htonl(x)>>8)|((x) & 0xff000000))
#define ntoha(x)    htona(x)

#define ntohsp(x) ntohs(*((uint16_t *)(x)))
#define ntohlp(x) ntohl(*((uint32_t *)(x)))
#define ntohap(x) ntoha(*((uint32_t *)(x)))
#define ntohllp(x) ntohll(*((uint64_t *)(x)))

#define put_htons(x,v) (*((uint16_t*)(x)) = htons(v))
#define put_htonl(x,v) (*((uint32_t*)(x)) = htonl(v))
#define put_htonll(x,v) (*((uint64_t*)(x)) = htonll(v))
/* TODO: Do we need put_value methods for aligned access when not swapping bytes */
/* TODO: Do we need put_htonll() or put_htona() */

#endif	/* _LITTLE_ENDIAN_H */

