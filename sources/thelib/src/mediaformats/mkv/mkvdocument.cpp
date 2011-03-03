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

#include "mediaformats/mkv/mkvdocument.h"
#include "mediaformats/mkv/baseelement.h"
#include "mediaformats/mp4/boxatom.h"
#include "mediaformats/mkv/boxelement.h"
#include "mediaformats/mkv/stringelement.h"
#include "mediaformats/mkv/intelement.h"

MKVDocument::MKVDocument(Variant &metadata)
: BaseMediaDocument(metadata) {
	FINEST("metadata:\n%s", STR(metadata.ToString()));
}

MKVDocument::~MKVDocument() {

	FOR_VECTOR(_elements, i) {
		delete _elements[i];
	}
	_elements.clear();
}

bool MKVDocument::ParseDocument() {
	//1. Seek to begin
	if (!_mediaFile.SeekBegin()) {
		FATAL("Unable to seek to the begining of file");
		return false;
	}

	uint64_t start = 0;

	//2. Read the elements
	while (start < _mediaFile.Size()) {
		BaseElement *pElement = BaseElement::ReadElement(this, NULL, start, _mediaFile.Size());
		if (pElement == NULL) {
			FATAL("Unable to read element");
			return false;
		}
		ADD_VECTOR_END(_elements, pElement);
		start = pElement->ElementStart() + pElement->ElementSize();
	}

	return true;
}

bool MKVDocument::BuildFrames() {
	//1. Test to see if we have at least 2 elements
	if (_elements.size() < 2) {
		FATAL("Invalid MKV structure");
		return false;
	}

	//2. First element must be MKV_ID_EBML and the second must be MKV_ID_Segment
	if ((_elements[0]->Id() != MKV_ID_EBML)
			|| (_elements[1]->Id() != MKV_ID_Segment)) {
		FATAL("Invalid MKV structure");
		return false;
	}

	BoxElement *pVideoTrack = GetTrack(_elements[1], false);
	BoxElement *pAudioTrack = GetTrack(_elements[1], true);
	FINEST("pVideoTrack: %p; pAudioTrack: %p", pVideoTrack, pAudioTrack);

	GetHierarchyString();



	//	FINEST("Printing hierarchy...");
	//	FINEST("Hierarchy:\n%s", STR(GetHierarchyString()));
	//
	//	FOR_MAP(_counters, uint64_t, uint32_t, i) {
	//		FINEST("%llx: %d", MAP_KEY(i), MAP_VAL(i));
	//	}
	//	NYIR;

	//1. Get the
	return false;
}

Variant MKVDocument::GetRTMPMeta() {
	return Variant();
}

string MKVDocument::GetHierarchyString() {
	string result = "<?xml version=\"1.0\"?>\n<mkv>";

	FOR_VECTOR(_elements, i) {
		result += "\n" + _elements[i]->GetHierarchyString(1);
	}
	result += "\n</mkv>";
	File f;
	f.Initialize("/tmp/test.xml",FILE_OPEN_MODE_TRUNCATE);
	f.WriteBuffer((uint8_t *) STR(result), result.length());
	return result;
}

BoxElement *MKVDocument::GetTrack(BaseElement *pSegment, bool audio) {
	vector<BaseElement *> temp;
	BoxElement *pResult = NULL;

	//1. get the track
	temp = pSegment->GetPath(3, MKV_ID_Tracks, MKV_ID_TrackEntry,
			(audio ? MKV_ID_Audio : MKV_ID_Video));
	if (temp.size() == 0)
		return NULL;

	//2. Is it alone?
	if (temp.size() > 1) {
		WARN("Multiple %s tracks detected: %d. Picking up the first one...",
				audio ? "audio" : "video",
				temp.size());
	}

	//3. Get the track entry
	pResult = temp[0]->Parent();

	//4. Examine it's codec ID
	temp = ((BaseElement *) pResult)->GetPath(1, MKV_ID_CodecID);
	if (temp.size() != 1) {
		WARN("%s track doesn't contain codec ID",
				audio ? "audio" : "video");
		return NULL;
	}

	string codecName = ((StringElement *) temp[0])->Value();
	if (audio) {
		if (codecName != "A_AAC") {
			WARN("Audio codec %s not supported", STR(codecName));
			return NULL;
		}
	} else {
		if (codecName != "V_MPEG4/ISO/AVC") {
			WARN("Video codec %s not supported", STR(codecName));
			return NULL;
		}
	}

	//5. Examine lacing
	temp = ((BaseElement *) pResult)->GetPath(1, MKV_ID_FlagLacing);
	if (temp.size() != 1) {
		WARN("%s track doesn't contain lacing flag",
				audio ? "audio" : "video");
		return NULL;
	}
	int32_t lacing = ((IntElement *) temp[0])->SignedValue();
	if (lacing != 0) {
		WARN("Lacing is not supported yet");
		return NULL;
	}

	return pResult;
}

#endif /* HAS_MEDIA_MKV */
