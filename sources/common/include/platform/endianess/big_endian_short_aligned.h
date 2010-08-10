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


#ifndef _BIG_ENDIAN_SHORT_ALIGNED_H
#define	_BIG_ENDIAN_SHORT_ALIGNED_H

// For A Big Endian ARM system this should be htonll(x) (x)
#define htonll(x)   (x)
#define ntohll(x)   (x)

//adobe shit. Sometimes, Adobe's reprezentation of 0x0a0b0c0d
//in "network-order" is like this: 0b 0c 0d 0a
#define htona(x)    (((x)<<8)|((x)>>24))
#define ntoha(x)    (((x)>>8)|((x)<<24))

// Pointer to value defines
#define ptovh(x) ((uint16_t)(*((uint8_t*)(x))))
#define ptovl(x) ((uint32_t)(*((uint8_t*)(x))))
#define ptovll(x) ((uint64_t)(*((uint8_t*)(x))))

// These will work with both BE and LE systems
#define ntohsp(x) ((ptovh(x) << 8) | (ptovh(x + 1)))
#define ntohlp(x) ((ptovl(x) << 24) | (ptovl(x + 1) << 16) | (ptovl(x + 2) << 8) | (ptovl(x + 3)))
#define ntohap(x) ((ptovl(x) << 16) | (ptovl(x + 1) << 8) | (ptovl(x + 2)) | (ptovl(x + 3) << 24))
#define ntohllp(x) \
		((ptovll(x) << 56) | (ptovll(x + 1) << 48) | (ptovll(x + 2) << 40) | (ptovll(x + 3) << 32) | \
		(ptovll(x + 4) << 24) | (ptovll(x + 5) << 16) | (ptovll(x + 6) << 8) | (ptovll(x + 7)))

#error put_htons in big_endian_short_aligned.h needs testing
/* These should work as-is for both BE and LE systems */

#define put_htons(x,v) {\
    (*(((uint8_t*)(x))) = (uint8_t)((v) >> 8));\
    (*(((uint8_t*)(x))+1) = (uint8_t)(v));\
    }

#define put_htonl(x,v) {\
    (*(((uint8_t*)(x))) = (uint8_t)((v) >> 24));\
    (*(((uint8_t*)(x))+1) = (uint8_t)((v) >> 16));\
    (*(((uint8_t*)(x))+2) = (uint8_t)((v) >> 8));\
    (*(((uint8_t*)(x))+3) = (uint8_t)(v));\
    }

/* TODO: Do we need put_value methods for aligned access when not swapping bytes */
/* TODO: Do we need put_htonll() or put_htona() */

#endif	/* _BIG_ENDIAN_SHORT_ALIGNED_H */


