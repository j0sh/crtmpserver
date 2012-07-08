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

#ifdef HAS_MEDIA_MP4
#include "mediaformats/mp4/ignoredatom.h"
#include "mediaformats/mp4/mp4document.h"

IgnoredAtom::IgnoredAtom(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BaseAtom(pDocument, type, size, start) {

}

IgnoredAtom::~IgnoredAtom() {
}

bool IgnoredAtom::IsIgnored() {
	return true;
}

bool IgnoredAtom::Read() {
	return SkipRead(
			(_type != A_SKIP)
			&& (_type != A_FREE)
			&& (_type != A_MDAT)
			&& (_type != A_IODS)
			&& (_type != A_WIDE)
			&& (_type != A_TREF)
			&& (_type != A_TMCD)
			&& (_type != A_TAPT)
			&& (_type != A_STPS)
			&& (_type != A_SDTP)
			&& (_type != A_RTP)
			&& (_type != A_PASP)
			&& (_type != A_LOAD)
			&& (_type != A_HNTI)
			&& (_type != A_HINV)
			&& (_type != A_HINF)
			&& (_type != A_GMHD)
			&& (_type != A_GSHH)
			&& (_type != A_GSPM)
			&& (_type != A_GSPU)
			&& (_type != A_GSSD)
			&& (_type != A_GSST)
			&& (_type != A_GSTD)
			&& (_type != A_EDTS)
			&& (_type != A_ALLF)
			&& (_type != A_SELO)
			&& (_type != A_WLOC)
			&& (_type != A_ALIS)
			&& (_type != A_BTRT)
			&& (_type != A_BTRT)
			&& (_type != A_CHAN)
			&& (_type != A_COLR)
			&& (_type != A_CSLG)
			&& (_type != A_____)
			&& (_type != A_UUID)
			);
}

string IgnoredAtom::Hierarchy(uint32_t indent) {
	return string(4 * indent, ' ') + GetTypeString();
}

#endif /* HAS_MEDIA_MP4 */
