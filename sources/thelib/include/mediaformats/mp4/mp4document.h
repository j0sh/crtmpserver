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
#ifndef _MP4DOCUMENT_H
#define	_MP4DOCUMENT_H

#include "common.h"
#include "mediaformats/basemediadocument.h"

#define A_NULL (0x00000000)
#define A_FTYP (0x66747970)
#define A_MOOV (0x6d6f6f76)
#define A_MOOF (0x6d6f6f66)
#define A_MVHD (0x6d766864)
#define A_MFHD (0x6d666864)
#define A_MVEX (0x6d766578)
#define A_TRAK (0x7472616b)
#define A_TRAF (0x74726166)
#define A_TREX (0x74726578)
#define A_TRUN (0x7472756e)
#define A_TKHD (0x746b6864)
#define A_TFHD (0x74666864)
#define A_MDIA (0x6d646961)
#define A_MDHD (0x6d646864)
#define A_HDLR (0x68646c72)
#define A_MINF (0x6d696e66)
#define A_SMHD (0x736d6864)
#define A_DINF (0x64696e66)
#define A_STBL (0x7374626c)
#define A_VMHD (0x766d6864)
#define A_DREF (0x64726566)
#define A_STSD (0x73747364)
#define A_STTS (0x73747473)
#define A_STSC (0x73747363)
#define A_STSZ (0x7374737a)
#define A_STCO (0x7374636f)
#define A_CTTS (0x63747473)
#define A_STSS (0x73747373)
#define A_URL (0x75726c20)
#define A_MP4A (0x6d703461)
#define A_MP3 (0x2e6d7033)
#define A_AVC1 (0x61766331)
#define A_ESDS (0x65736473)
#define A_VIDE (0x76696465)
#define A_SOUN (0x736f756e)
#define A_AVCC (0x61766343)
#define A_UDTA (0x75647461)
#define A_WAVE (0x77617665)
#define A_CHAN (0x6368616e)
#define A_META (0x6d657461)
#define A_ILST (0x696c7374)
#define A__NAM (0xa96e616d)
#define A_CPIL (0x6370696c)
#define A_PGAP (0x70676170)
#define A_TMPO (0x746d706f)
#define A__TOO (0xa9746f6f)
#define A__ART1 (0xa9415254)
#define A__ART2 (0xa9617274)
#define A__PRT (0xa9707274)
#define A__ALB (0xa9616c62)
#define A_GNRE (0x676e7265)
#define A_TRKN (0x74726b6e)
#define A__DAY (0xa9646179)
#define A_DISK (0x6469736b)
#define A__CMT (0xa9636d74)
#define A__CPY (0xa9637079)
#define A__DES (0xa9646573)
#define A_DATA (0x64617461)
#define A_COVR (0x636f7672)
#define A_AART (0x61415254)
#define A__WRT (0xa9777274)
#define A__GRP (0xa9677270)
#define A__LYR (0xa96c7972)
#define A_NAME (0x6e616d65)
#define A__COM (0xa9636f6d)
#define A__GEN (0xa967656e)
#define A_DESC (0x64657363)
#define A_TVSH (0x74767368)
#define A_TVEN (0x7476656e)
#define A_TVSN (0x7476736e)
#define A_TVES (0x74766573)
#define A_CO64 (0x636f3634)

class BaseAtom;
class AtomFTYP;
class AtomMOOV;
class AtomMOOF;
class AtomTRAK;
class AtomTRAF;

class MP4Document
: public BaseMediaDocument {
private:
	vector<BaseAtom *> _allAtoms;
	vector<BaseAtom *> _topAtoms;
	AtomFTYP *_pFTYP;
	AtomMOOV *_pMOOV;
	vector<AtomMOOF*> _moof;
public:
	MP4Document(Variant &metadata);
	virtual ~MP4Document();

	void AddAtom(BaseAtom *pAtom);
	BaseAtom * ReadAtom(BaseAtom *pParentAtom);
protected:
	virtual bool ParseDocument();
	virtual bool BuildFrames();
	virtual Variant GetRTMPMeta();
private:
	string Hierarchy();
	AtomTRAK * GetTRAK(bool audio);
	AtomTRAF * GetTRAF(AtomMOOF *pMOOF, bool audio);
	bool BuildMOOVFrames(bool audio);
	bool BuildMOOFFrames(AtomMOOF *pMOOF, bool audio);
};

#endif	/* _MP4DOCUMENT_H */


#endif /* HAS_MEDIA_MP4 */
