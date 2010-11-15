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

#ifdef HAS_MEDIA_MKV

#include "mediaformats/mkv/baseelement.h"
#include "mediaformats/mkv/mkvdocument.h"
#include "mediaformats/mkv/elementdummy.h"
#include "mediaformats/mkv/boxelement.h"
#include "mediaformats/mkv/intelement.h"
#include "mediaformats/mkv/stringelement.h"
#include "mediaformats/mkv/binaryelement.h"
#include "mediaformats/mkv/floatelement.h"

//#define DEBUG_READ_CUI(...) FINEST(__VA_ARGS__)
#define DEBUG_READ_CUI(...)

BaseElement::BaseElement() {
	_pDoc = NULL;
	_elementStart = 0;
	_elementSize = 0;
	_payloadStart = 0;
	_payloadSize = 0;
	_id = 0;
}

BaseElement::~BaseElement() {
	_pDoc = NULL;
	_elementStart = 0;
	_elementSize = 0;
	_payloadStart = 0;
	_payloadSize = 0;
	_id = 0;
}

BoxElement *BaseElement::Parent() {
	return _pParent;
}

uint64_t BaseElement::Id() {
	return _id;
}

uint64_t BaseElement::ElementStart() {
	return _elementStart;
}

uint64_t BaseElement::ElementSize() {
	return _elementSize;
}

uint64_t BaseElement::PayloadStart() {
	return _payloadStart;
}

uint64_t BaseElement::PayloadSize() {
	return _payloadSize;
}

BaseElement::operator string() {
	string result = "";
	result += format("id: %llx\n", _id);
	result += format("elementStart: %llx\n", _elementStart);
	result += format("elementSize: %llx\n", _elementSize);
	result += format("payloadStart: %llx\n", _payloadStart);
	result += format("payloadSize: %llx", _payloadSize);
	return result;
}

vector<BaseElement *> BaseElement::GetPath(uint32_t depth, ...) {
	vector<uint64_t> path;
	va_list arguments;
	va_start(arguments, depth);
	//string strPath = "";
	for (uint8_t i = 0; i < depth; i++) {
		uint64_t pathElement = va_arg(arguments, uint64_t);
		//strPath += format("%s->", STR(MKVElemToString(pathElement)));
		ADD_VECTOR_END(path, pathElement);
	}
	va_end(arguments);
	//FINEST("Path: %s", STR(strPath));
	if (path.size() == 0)
		return vector<BaseElement *>();
	return GetPath(path);
}

vector<BaseElement *> BaseElement::GetPath(vector<uint64_t> path) {
	return vector<BaseElement *>();
}

int ___ii = 0;

BaseElement *BaseElement::ReadElement(MKVDocument *pDoc, BoxElement *pParent, uint64_t start, uint64_t max) {
	uint64_t elementStart = 0;
	uint64_t elementSize = 0;
	uint64_t payloadStart = 0;
	uint64_t payloadSize = 0;
	uint64_t id = 0;

	if (((___ii++) % 1000) == 0) {
		printf("\r%.2f", (double) start / (double) pDoc->GetMediaFile().Size()*100.00);
		fflush(stdout);
	}

	if (pDoc->GetMediaFile().Size() < max) {
		FATAL("Max is greater than file size");
		return false;
	}

	pDoc->GetMediaFile().SeekTo(start);

	elementStart = pDoc->GetMediaFile().Cursor();

	if (!ReadCUI(pDoc->GetMediaFile(), id, false)) {
		FATAL("Unable to read id");
		return NULL;
	}
	if (pDoc->GetMediaFile().Cursor() > max) {
		FATAL("Element exceeds bounds");
		return false;
	}


	if (!ReadCUI(pDoc->GetMediaFile(), payloadSize, true)) {
		FATAL("Unable to read id");
		return NULL;
	}
	if (pDoc->GetMediaFile().Cursor() > max) {
		FATAL("Element exceeds bounds");
		return false;
	}

	payloadStart = pDoc->GetMediaFile().Cursor();
	if (payloadStart + payloadSize > max) {
		FATAL("payload exceeds bounds: payloadStart: %d; payloadSize: %d; max: %d",
				payloadStart, payloadSize, max);
		return false;
	}

	elementSize = payloadStart - elementStart + payloadSize;
	if (elementStart + elementSize > max) {
		FATAL("Element exceeds bounds");
		return false;
	}

	BaseElement *pResult = NULL;
	switch (id) {
		case MKV_ID_ChapString:
		case MKV_ID_CodecName:
		case MKV_ID_CodecSettings:
		case MKV_ID_FileDescription:
		case MKV_ID_FileName:
		case MKV_ID_MuxingApp:
		case MKV_ID_Name:
		case MKV_ID_NextFilename:
		case MKV_ID_PrevFilename:
		case MKV_ID_SegmentFilename:
		case MKV_ID_TagName:
		case MKV_ID_TagString:
		case MKV_ID_Title:
		case MKV_ID_WritingApp:
		{
			//TODO: unicode
			pResult = new StringElement();
			break;
		}
		case MKV_ID_Block:
		case MKV_ID_BlockAdditional:
		case MKV_ID_BlockVirtual:
		case MKV_ID_CRC_32:
		case MKV_ID_ChannelPositions:
		case MKV_ID_ChapProcessData:
		case MKV_ID_ChapProcessPrivate:
		case MKV_ID_ChapterSegmentEditionUID:
		case MKV_ID_ChapterSegmentUID:
		case MKV_ID_ChapterTranslateID:
		case MKV_ID_CodecPrivate:
		case MKV_ID_CodecState:
		case MKV_ID_ColourSpace:
		case MKV_ID_ContentCompSettings:
		case MKV_ID_ContentEncKeyID:
		case MKV_ID_ContentSigKeyID:
		case MKV_ID_ContentSignature:
		case MKV_ID_EncryptedBlock:
		case MKV_ID_FileData:
		case MKV_ID_FileReferral:
		case MKV_ID_NextUID:
		case MKV_ID_PrevUID:
		case MKV_ID_SeekID:
		case MKV_ID_SegmentFamily:
		case MKV_ID_SegmentUID:
		case MKV_ID_Signature:
		case MKV_ID_SignaturePublicKey:
		case MKV_ID_SignedElement:
		case MKV_ID_SimpleBlock:
		case MKV_ID_TagBinary:
		case MKV_ID_TrackTranslateTrackID:
		case MKV_ID_Void:
		{
			pResult = new BinaryElement();
			break;
		}
		case MKV_ID_DateUTC:
		{
			//TODO: Date
			pResult = new ElementDummy();
			break;
		}
		case MKV_ID_DurationInSegmentInfo:
		case MKV_ID_FrameRate:
		case MKV_ID_GammaValue:
		case MKV_ID_OutputSamplingFrequency:
		case MKV_ID_SamplingFrequency:
		case MKV_ID_TrackTimecodeScale:
		{
			//TODO: Float
			pResult = new FloatElement();
			break;
		}
		case MKV_ID_ReferenceBlock:
		case MKV_ID_ReferenceVirtual:
		case MKV_ID_TrackOffset:
		{
			//TODO: Signed
			pResult = new IntElement();
			break;
		}
		case MKV_ID_AttachedFile:
		case MKV_ID_Attachments:
		case MKV_ID_Audio:
		case MKV_ID_BlockAdditions:
		case MKV_ID_BlockGroup:
		case MKV_ID_BlockMore:
		case MKV_ID_ChapProcess:
		case MKV_ID_ChapProcessCommand:
		case MKV_ID_ChapterAtom:
		case MKV_ID_ChapterDisplay:
		case MKV_ID_ChapterTrack:
		case MKV_ID_ChapterTranslate:
		case MKV_ID_Chapters:
		case MKV_ID_ContentCompression:
		case MKV_ID_ContentEncoding:
		case MKV_ID_ContentEncodings:
		case MKV_ID_ContentEncryption:
		case MKV_ID_CuePoint:
		case MKV_ID_CueReference:
		case MKV_ID_CueTrackPositions:
		case MKV_ID_EBML:
		case MKV_ID_EditionEntry:
		case MKV_ID_Info:
		case MKV_ID_Seek:
		case MKV_ID_SeekHead:
		case MKV_ID_Segment:
		case MKV_ID_SignatureElementList:
		case MKV_ID_SignatureElements:
		case MKV_ID_SignatureSlot:
		case MKV_ID_SilentTracks:
		case MKV_ID_SimpleTag:
		case MKV_ID_Slices:
		case MKV_ID_Tag:
		case MKV_ID_Tags:
		case MKV_ID_Targets:
		case MKV_ID_TimeSlice:
		case MKV_ID_TrackCombinePlanes:
		case MKV_ID_TrackEntry:
		case MKV_ID_TrackJoinBlocks:
		case MKV_ID_TrackOperation:
		case MKV_ID_TrackPlane:
		case MKV_ID_TrackTranslate:
		case MKV_ID_Tracks:
		case MKV_ID_Video:
		case MKV_ID_Cues:
		{
			pResult = new BoxElement();
			break;
		}
		case MKV_ID_Cluster:
		{
			pResult = new ClusterBoxElement();
			break;
		}
			//		case MKV_ID_Cues: //this is actually a box element
			//		{
			//			pResult = new ElementDummy();
			//			break;
			//		}
		case MKV_ID_ChapCountry:
		case MKV_ID_ChapLanguage:
		case MKV_ID_CodecDownloadURL:
		case MKV_ID_CodecID:
		case MKV_ID_CodecInfoURL:
		case MKV_ID_DocType:
		case MKV_ID_FileMimeType:
		case MKV_ID_Language:
		case MKV_ID_TagLanguage:
		case MKV_ID_TargetType:
		{
			//TODO: non-unicode
			pResult = new StringElement();
			break;
		}
		case MKV_ID_AspectRatioType:
		case MKV_ID_AttachmentLink:
		case MKV_ID_AttachmentUID:
		case MKV_ID_BitDepth:
		case MKV_ID_BlockAddID:
		case MKV_ID_BlockAdditionID:
		case MKV_ID_BlockDuration:
		case MKV_ID_Channels:
		case MKV_ID_ChapProcessCodecID:
		case MKV_ID_ChapProcessTime:
		case MKV_ID_ChapterFlagEnabled:
		case MKV_ID_ChapterFlagHidden:
		case MKV_ID_ChapterPhysicalEquiv:
		case MKV_ID_ChapterTimeEnd:
		case MKV_ID_ChapterTimeStart:
		case MKV_ID_ChapterTrackNumber:
		case MKV_ID_ChapterTranslateCodec:
		case MKV_ID_ChapterTranslateEditionUID:
		case MKV_ID_ChapterUIDInChapters:
		case MKV_ID_ChapterUIDInTagging:
		case MKV_ID_CodecDecodeAll:
		case MKV_ID_ContentCompAlgo:
		case MKV_ID_ContentEncAlgo:
		case MKV_ID_ContentEncodingOrder:
		case MKV_ID_ContentEncodingScope:
		case MKV_ID_ContentEncodingType:
		case MKV_ID_ContentSigAlgo:
		case MKV_ID_ContentSigHashAlgo:
		case MKV_ID_CueBlockNumber:
		case MKV_ID_CueClusterPosition:
		case MKV_ID_CueCodecState:
		case MKV_ID_CueRefCluster:
		case MKV_ID_CueRefCodecState:
		case MKV_ID_CueRefNumber:
		case MKV_ID_CueRefTime:
		case MKV_ID_CueTime:
		case MKV_ID_CueTrack:
		case MKV_ID_DefaultDuration:
		case MKV_ID_Delay:
		case MKV_ID_DisplayHeight:
		case MKV_ID_DisplayUnit:
		case MKV_ID_DisplayWidth:
		case MKV_ID_DocTypeReadVersion:
		case MKV_ID_DocTypeVersion:
		case MKV_ID_DurationInCluster:
		case MKV_ID_EBMLMaxIDLength:
		case MKV_ID_EBMLMaxSizeLength:
		case MKV_ID_EBMLReadVersion:
		case MKV_ID_EBMLVersion:
		case MKV_ID_EditionFlagDefault:
		case MKV_ID_EditionFlagHidden:
		case MKV_ID_EditionFlagOrdered:
		case MKV_ID_EditionUIDInTagging:
		case MKV_ID_EditionUIDInChapters:
		case MKV_ID_FileUID:
		case MKV_ID_FlagDefault:
		case MKV_ID_FlagEnabled:
		case MKV_ID_FlagForced:
		case MKV_ID_FlagInterlaced:
		case MKV_ID_FlagLacing:
		case MKV_ID_FrameNumber:
		case MKV_ID_LaceNumber:
		case MKV_ID_MaxBlockAdditionID:
		case MKV_ID_MaxCache:
		case MKV_ID_MinCache:
		case MKV_ID_PixelCropBottom:
		case MKV_ID_PixelCropLeft:
		case MKV_ID_PixelCropRight:
		case MKV_ID_PixelCropTop:
		case MKV_ID_PixelHeight:
		case MKV_ID_PixelWidth:
		case MKV_ID_Position:
		case MKV_ID_PrevSize:
		case MKV_ID_ReferencePriority:
		case MKV_ID_SeekPosition:
		case MKV_ID_SignatureAlgo:
		case MKV_ID_SignatureHash:
		case MKV_ID_SilentTrackNumber:
		case MKV_ID_StereoMode:
		case MKV_ID_TagDefault:
		case MKV_ID_TargetTypeValue:
		case MKV_ID_Timecode:
		case MKV_ID_TimecodeScale:
		case MKV_ID_TrackJoinUID:
		case MKV_ID_TrackNumber:
		case MKV_ID_TrackOverlay:
		case MKV_ID_TrackPlaneType:
		case MKV_ID_TrackPlaneUID:
		case MKV_ID_TrackTranslateCodec:
		case MKV_ID_TrackTranslateEditionUID:
		case MKV_ID_TrackType:
		case MKV_ID_TrackUIDInTagging:
		case MKV_ID_TrackUIDInTrack:
		{
			//TODO: Unsigned int
			pResult = new IntElement();
			break;
		}
		default:
		{
			pResult = new ElementDummy();
			break;
		}
	}

	if (pResult != NULL) {
		pResult->_pDoc = pDoc;
		pResult->_pParent = pParent;
		pResult->_id = id;
		pResult->_elementStart = elementStart;
		pResult->_elementSize = elementSize;
		pResult->_payloadStart = payloadStart;
		pResult->_payloadSize = payloadSize;
		if (!pResult->Read()) {
			FATAL("Unable to read element");
			delete pResult;
			pResult = NULL;
		}
		pDoc->_counters[id] = pDoc->_counters[id] + 1;
	}


	return pResult;
}

bool BaseElement::ReadCUI(MediaFile &mf, uint64_t &value, bool removeLeadingBit) {
	//1. Build the fast search array for computing the number of leading zeros
	static const uint8_t fastSearch[] = {
		8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	//2. Read the leading byte
	uint8_t bytes[8] = {0};
	if (!mf.ReadBuffer(&bytes[7], 1)) {
		FATAL("Unable to read the leading byte");
		return false;
	}
	DEBUG_READ_CUI("bytes[7]: %02x", bytes[7]);

	//3. Compute the number of leading zeros
	uint8_t leadingZerosCount = fastSearch[bytes[7]];
	DEBUG_READ_CUI("leadingZerosCount: %d", leadingZerosCount);
	if (leadingZerosCount == 8) {
		FATAL("Invalid compacted number. First byte is 0");
		return false;
	}
	bytes[7 - leadingZerosCount] = bytes[7];

	//4. Read the rest of the bytes
	if (leadingZerosCount > 0) {
		if (!mf.ReadBuffer(bytes + 8 - leadingZerosCount, leadingZerosCount)) {
			FATAL("Unable to read the rest of the bytes");
			return false;
		}
	}

	if (removeLeadingBit) {
		DEBUG_READ_CUI("BEFORE %02x %02x %02x %02x %02x %02x %02x %02x",
				bytes[0], bytes[1], bytes[2], bytes[3],
				bytes[4], bytes[5], bytes[6], bytes[7]);
		bytes[7 - leadingZerosCount] = bytes[7 - leadingZerosCount]&(0xff >> (leadingZerosCount + 1));
	}
	DEBUG_READ_CUI(" AFTER %02x %02x %02x %02x %02x %02x %02x %02x",
			bytes[0], bytes[1], bytes[2], bytes[3],
			bytes[4], bytes[5], bytes[6], bytes[7]);

	value = ENTOHLLP(bytes);
	DEBUG_READ_CUI("value: %016llx", value);

	return true;
}

#endif /* HAS_MEDIA_MKV */
