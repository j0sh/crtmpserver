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

#ifdef HAS_PROTOCOL_RTMP
#ifdef LITTLE_ENDIAN_SHORT_ALIGNED

#include "protocols/rtmp/header_le_sa.h"
#include "protocols/rtmp/channel.h"

bool Header::Read(uint32_t channelId, uint8_t type, IOBuffer &buffer,
		uint32_t availableBytes) {
	ht = type;
	ci = channelId;

	switch (ht) {
		case HT_FULL:
		{
			isAbsolute = true;
			if (availableBytes < 11) {
				readCompleted = false;
				return true;
			}
			memcpy(hf.datac + 1, GETIBPOINTER(buffer), 11);
			hf.s.ts = ntohl(hf.s.ts)&0x00ffffff; //----MARKED-LONG---
			hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---

			if (hf.s.ts == 0x00ffffff) {
				if (availableBytes < 15) {
					readCompleted = false;
					return true;
				}
				hf.s.ts = ntohlp(GETIBPOINTER(buffer) + 11);
				readCompleted = true;
				return buffer.Ignore(15);
			} else {
				readCompleted = true;
				return buffer.Ignore(11);
			}
		}
		case HT_SAME_STREAM:
		{
			isAbsolute = false;
			if (availableBytes < 7) {
				readCompleted = false;
				return true;
			}
			memcpy(hf.datac + 1, GETIBPOINTER(buffer), 7);
			hf.s.ts = ntohl(hf.s.ts)&0x00ffffff; //----MARKED-LONG---
			hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---

			if (hf.s.ts == 0x00ffffff) {
				if (availableBytes < 11) {
					readCompleted = false;
					return true;
				}
				hf.s.ts = ntohlp(GETIBPOINTER(buffer) + 7);
				readCompleted = true;
				return buffer.Ignore(11);
			} else {
				readCompleted = true;
				return buffer.Ignore(7);
			}
		}
		case HT_SAME_LENGTH_AND_STREAM:
		{
			isAbsolute = false;
			if (availableBytes < 3) {
				readCompleted = false;
				return true;
			}
			memcpy(hf.datac + 1, GETIBPOINTER(buffer), 3);
			hf.s.ts = ntohl(hf.s.ts)&0x00ffffff; //----MARKED-LONG---

			if (hf.s.ts == 0x00ffffff) {
				if (availableBytes < 7) {
					readCompleted = false;
					return true;
				}
				hf.s.ts = ntohlp(GETIBPOINTER(buffer) + 3);
				readCompleted = true;
				return buffer.Ignore(7);
			} else {
				readCompleted = true;
				return buffer.Ignore(3);
			}
		}
		case HT_CONTINUATION:
		{
			isAbsolute = false;
			if (hf.s.ts == 0x00ffffff) {
				if (availableBytes < 4) {
					readCompleted = false;
					return true;
				}
				readCompleted = true;
				return buffer.Ignore(4);
			} else {
				readCompleted = true;
				return true;
			}
		}
		default:
		{
			FATAL("Invalid header type: %d", ht);
			return false;
		}
	}
}

Variant Header::GetVariant() {

	Variant result;


	result[RM_HEADER_HEADERTYPE] = (uint8_t) ht;
	result[RM_HEADER_CHANNELID] = (uint32_t) ci;
	result[RM_HEADER_TIMESTAMP] = (uint32_t) hf.s.ts;
	result[RM_HEADER_MESSAGELENGTH] = (uint32_t) hf.s.ml;
	result[RM_HEADER_MESSAGETYPE] = (uint8_t) hf.s.mt;
	result[RM_HEADER_STREAMID] = (uint32_t) hf.s.si;
	result[RM_HEADER_ISABSOLUTE] = isAbsolute;


	return result;
}

bool Header::GetFromVariant(Header &header, Variant &variant) {
	if (variant != V_MAP) {
		FATAL("Variant is not a map: %s", STR(variant.ToString()));
		return false;
	}

	if ((variant[RM_HEADER_HEADERTYPE] != _V_NUMERIC) ||
			(variant[RM_HEADER_CHANNELID] != _V_NUMERIC) ||
			(variant[RM_HEADER_TIMESTAMP] != _V_NUMERIC) ||
			(variant[RM_HEADER_MESSAGELENGTH] != _V_NUMERIC) ||
			(variant[RM_HEADER_MESSAGETYPE] != _V_NUMERIC) ||
			(variant[RM_HEADER_STREAMID] != _V_NUMERIC) ||
			(variant[RM_HEADER_ISABSOLUTE] != V_BOOL)
			) {
		FATAL("Variant is not a valid RTMP header: %s", STR(variant.ToString()));
		return false;
	}

	header.ht = (uint8_t) variant[RM_HEADER_HEADERTYPE];
	header.ci = (uint32_t) variant[RM_HEADER_CHANNELID];
	header.hf.s.ts = (uint32_t) variant[RM_HEADER_TIMESTAMP];
	header.hf.s.ml = (uint32_t) variant[RM_HEADER_MESSAGELENGTH];
	header.hf.s.mt = (uint8_t) variant[RM_HEADER_MESSAGETYPE];
	header.hf.s.si = (uint32_t) variant[RM_HEADER_STREAMID];
	header.readCompleted = true;
	header.isAbsolute = (bool) variant[RM_HEADER_ISABSOLUTE];

	return true;
}

bool Header::Write(Channel &channel, IOBuffer &buffer) {
	//FINEST("this: %p", this);
	//1. Compute the header size
	if (channel.lastOutStreamId == H_SI(*this)) {
		//FINEST("Same stream %d on channel %d", channel.lastOutStreamId, channel.id);
		if (H_IA(*this)) {
			//FINEST("This is absolute ts.");
			if (channel.lastOutProcBytes == 0) {
				//FINEST("This is the beginning of a packet");
				H_HT(*this) = HT_FULL;
				channel.lastOutAbsTs = H_TS(*this);
			} else {
				//FINEST("We are in the middle of the packet: %d", channel.lastOutProcBytes);
				H_HT(*this) = HT_CONTINUATION;
			}
		} else {
			//FINEST("This is relative ts.");
			if (channel.lastOutProcBytes == 0) {
				//FINEST("This is the beginning of a packet");
				H_HT(*this) = HT_SAME_STREAM;
				if ((H_MT(*this) == H_MT(channel.lastOutHeader)) &&
						(H_ML(*this) == H_ML(channel.lastOutHeader))) {
					//FINEST("Same stream and same length and type");
					H_HT(*this) = HT_SAME_LENGTH_AND_STREAM;
					if (H_TS(*this) == H_TS(channel.lastOutHeader)) {
						//FINEST("Exactly the same header");
						H_HT(*this) = HT_CONTINUATION;
					}
				}
				channel.lastOutAbsTs += H_TS(*this);
			} else {
				//FINEST("We are in the middle of the packet: %d", channel.lastOutProcBytes);
				H_HT(*this) = HT_CONTINUATION;
			}
		}
	} else {
		//        FINEST("Different stream on channel %d: channel: %d; header: %d",
		//                channel.id, channel.lastOutStreamId, H_SI(*this));
		H_HT(*this) = HT_FULL;
		H_IA(*this) = true;
		channel.lastOutAbsTs = H_TS(*this);
		channel.lastOutStreamId = H_SI(*this);
	}

	//2. Save the last sent header
	channel.lastOutHeader = *this;

	//    if (H_CI(*this) == 20)
	//        DEBUG("header: %s", STR(*this));
	//    else if (H_CI(*this) == 21)
	//        FINEST("header: %s", STR(*this));

	//3. do the write
	return Write(buffer);
}

bool Header::Write(IOBuffer &buffer) {
	//FINEST("header: %s", STR(*this));
	if (ci < 64) {
		buffer.ReadFromByte((ht << 6) | ci);
	} else if (ci < 319) {
		buffer.ReadFromByte(ht << 6);
		buffer.ReadFromByte(ci - 64);
	} else if (ci < 65599) {
		uint16_t temp = htons(ci - 64);
		buffer.ReadFromByte((ht << 6) | 0x01);
		buffer.ReadFromBuffer((uint8_t *) & temp, 2);
	} else {
		FATAL("Invalid channel index");
		return false;
	}

	switch (ht) {
		case HT_FULL:
		{
			if (hf.s.ts < 0x00ffffff) {
				hf.s.ts = htonl(hf.s.ts); //----MARKED-LONG---
				hf.s.ml = htonl(hf.s.ml << 8); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 11);
				hf.s.ts = ntohl(hf.s.ts); //----MARKED-LONG---
				hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			} else {
				uint32_t temp = htonl(hf.s.ts); //----MARKED-LONG---
				hf.s.ts = htonl(0x00ffffff); //----MARKED-LONG---
				hf.s.ml = htonl(hf.s.ml << 8); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 11);
				hf.s.ts = ntohl(temp); //----MARKED-LONG---
				hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---
				buffer.ReadFromBuffer((uint8_t *) & temp, 4);
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			}
		}
		case HT_SAME_STREAM:
		{
			if (hf.s.ts < 0x00ffffff) {
				hf.s.ts = htonl(hf.s.ts); //----MARKED-LONG---
				hf.s.ml = htonl(hf.s.ml << 8); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 7);
				hf.s.ts = ntohl(hf.s.ts); //----MARKED-LONG---
				hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			} else {
				uint32_t temp = htonl(hf.s.ts); //----MARKED-LONG---
				hf.s.ts = htonl(0x00ffffff); //----MARKED-LONG---
				hf.s.ml = htonl(hf.s.ml << 8); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 7);
				hf.s.ts = ntohl(temp); //----MARKED-LONG---
				hf.s.ml = ntohl(hf.s.ml) >> 8; //----MARKED-LONG---
				buffer.ReadFromBuffer((uint8_t *) & temp, 4);
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			}
		}
		case HT_SAME_LENGTH_AND_STREAM:
		{
			if (hf.s.ts < 0x00ffffff) {
				hf.s.ts = htonl(hf.s.ts); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 3);
				hf.s.ts = ntohl(hf.s.ts); //----MARKED-LONG---
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			} else {
				uint32_t temp = htonl(hf.s.ts); //----MARKED-LONG---
				hf.s.ts = htonl(0x00ffffff); //----MARKED-LONG---
				buffer.ReadFromBuffer(&hf.datac[1], 3);
				hf.s.ts = ntohl(temp); //----MARKED-LONG---
				buffer.ReadFromBuffer((uint8_t *) & temp, 4);
				//FINEST("Output buffer: %s", STR(buffer));
				return true;
			}
		}
		case HT_CONTINUATION:
		{
			//FINEST("Output buffer: %s", STR(buffer));
			return true;
		}
		default:
		{
			FATAL("Invalid header size: %d", ht);
			return false;
		}
	}
}

Header::operator string() {
	return format("(RC: %d; HT: %d; CI: %02u; T: % 9u; L: % 6u; MT: % 2u; SI: % 2u; IA: %d)",
			readCompleted, ht, ci, hf.s.ts, hf.s.ml, hf.s.mt, hf.s.si, isAbsolute);
}


#endif /* LITTLE_ENDIAN_SHORT_ALIGNED */
#endif /* HAS_PROTOCOL_RTMP */
