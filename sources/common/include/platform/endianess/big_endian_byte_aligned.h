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

//64 bit
#ifndef DONT_DEFINE_HTONLL
#define htonll(x)	(x)
#define ntohll(x)	(x)
#endif /* DONT_DEFINE_HTONLL */

//64 bit
#define EHTONLL(x) htonll(x)
#define ENTOHLL(x) ntohll(x)

//double
#define EHTOND(hostDoubleVal,networkUI64Val) networkUI64Val=EHTONLL((*((uint64_t *)(&(hostDoubleVal)))))
#define ENTOHD(networkUI64Val,hostDoubleVal) \
do {\
	uint64_t ___tempHostENTOHD=ENTOHLL(networkUI64Val); \
	hostDoubleVal=(double)(*((double *)&___tempHostENTOHD)); \
} while(0)

//32 bit
#define EHTONL(x) htonl(x)
#define ENTOHL(x) ntohl(x)

//16 bit
#define EHTONS(x) htons(x)
#define ENTOHS(x) ntohs(x)

//adobe
#define EHTONA(x)    (((x)<<8)|((x)>>24))
#define ENTOHA(x)    (((x)>>8)|((x)<<24))

//64 bit pointer
#define EHTONLLP(pNetworkPointer,hostLongLongValue) (*((uint64_t*)(pNetworkPointer)) = EHTONLL(hostLongLongValue))
#define ENTOHLLP(pNetworkPointer) ENTOHLL(*((uint64_t *)(pNetworkPointer)))

//double pointer
#define EHTONDP(hostDoubleVal,pNetworkPointer) EHTOND(hostDoubleVal,(*((uint64_t *)(pNetworkPointer))))
#define ENTOHDP(pNetworkPointer,hostDoubleVal) ENTOHD((*((uint64_t *)(pNetworkPointer))),hostDoubleVal)

//32 bit pointer
#define EHTONLP(pNetworkPointer,hostLongValue) (*((uint32_t*)(pNetworkPointer)) = EHTONL(hostLongValue))
#define ENTOHLP(pNetworkPointer) ENTOHL(*((uint32_t *)(pNetworkPointer)))

//16 bit pointer
#define EHTONSP(pNetworkPointer,hostShortValue) (*((uint16_t*)(pNetworkPointer)) = EHTONS(hostShortValue))
#define ENTOHSP(pNetworkPointer) ENTOHS(*((uint16_t *)(pNetworkPointer)))

//adobe pointer
#define EHTONAP(pNetworkPointer,hostAdobeValue) (*((uint32_t*)(pNetworkPointer)) = EHTONA(hostAdobeValue))
#define ENTOHAP(pNetworkPointer) ENTOHA(*((uint32_t *)(pNetworkPointer)))

#endif	/* _BIG_ENDIAN_BYTE_ALIGNED_H */

