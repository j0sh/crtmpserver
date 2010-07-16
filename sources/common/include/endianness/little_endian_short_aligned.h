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


#ifndef _LITTLE_ENDIAN_SHORT_ALIGNED_H
#define	_LITTLE_ENDIAN_SHORT_ALIGNED_H

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

//adobe shit. Sometimes, Adobe's reprezentation of 0x0a0b0c0d
//in "network-order" is like this: 0b 0c 0d 0a
#define htona(x)    ((htonl(x)>>8)|((x) & 0xff000000))
#define ntoha(x)    htona(x)

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

#define put_htonll(x,v) {\
    (*(((uint8_t*)(x))) = (uint8_t)((v) >> 56));\
    (*(((uint8_t*)(x))+1) = (uint8_t)((v) >> 48));\
    (*(((uint8_t*)(x))+2) = (uint8_t)((v) >> 40));\
    (*(((uint8_t*)(x))+3) = (uint8_t)((v) >> 32));\
    (*(((uint8_t*)(x))+4) = (uint8_t)((v) >> 24));\
    (*(((uint8_t*)(x))+5) = (uint8_t)((v) >> 16));\
    (*(((uint8_t*)(x))+6) = (uint8_t)((v) >> 8));\
    (*(((uint8_t*)(x))+7) = (uint8_t)(v));\
}
/* TODO: Do we need put_value methods for aligned access when not swapping bytes */
/* TODO: Do we need put_htonll() or put_htona() */

#endif	/* _LITTLE_ENDIAN_SHORT_ALIGNED_H */


