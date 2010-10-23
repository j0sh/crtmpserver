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
#ifndef _ELEMENTIDS_H
#define	_ELEMENTIDS_H

#include "common.h"

#define MKV_ID_ChapString	0x85LL
#define MKV_ID_CodecName	0x258688LL
#define MKV_ID_CodecSettings	0x3A9697LL
#define MKV_ID_FileDescription	0x467ELL
#define MKV_ID_FileName	0x466ELL
#define MKV_ID_MuxingApp	0x4D80LL
#define MKV_ID_Name	0x536ELL
#define MKV_ID_NextFilename	0x3E83BBLL
#define MKV_ID_PrevFilename	0x3C83ABLL
#define MKV_ID_SegmentFilename	0x7384LL
#define MKV_ID_TagName	0x45A3LL
#define MKV_ID_TagString	0x4487LL
#define MKV_ID_Title	0x7BA9LL
#define MKV_ID_WritingApp	0x5741LL
#define MKV_ID_Block	0xA1LL
#define MKV_ID_BlockAdditional	0xA5LL
#define MKV_ID_BlockVirtual	0xA2LL
#define MKV_ID_CRC_32	0xBFLL
#define MKV_ID_ChannelPositions	0x7D7BLL
#define MKV_ID_ChapProcessData	0x6933LL
#define MKV_ID_ChapProcessPrivate	0x450DLL
#define MKV_ID_ChapterSegmentEditionUID	0x6EBCLL
#define MKV_ID_ChapterSegmentUID	0x6E67LL
#define MKV_ID_ChapterTranslateID	0x69A5LL
#define MKV_ID_CodecPrivate	0x63A2LL
#define MKV_ID_CodecState	0xA4LL
#define MKV_ID_ColourSpace	0x2EB524LL
#define MKV_ID_ContentCompSettings	0x4255LL
#define MKV_ID_ContentEncKeyID	0x47e2LL
#define MKV_ID_ContentSigKeyID	0x47e4LL
#define MKV_ID_ContentSignature	0x47e3LL
#define MKV_ID_EncryptedBlock	0xAFLL
#define MKV_ID_FileData	0x465CLL
#define MKV_ID_FileReferral	0x4675LL
#define MKV_ID_NextUID	0x3EB923LL
#define MKV_ID_PrevUID	0x3CB923LL
#define MKV_ID_SeekID	0x53ABLL
#define MKV_ID_SegmentFamily	0x4444LL
#define MKV_ID_SegmentUID	0x73A4LL
#define MKV_ID_Signature	0x7EB5LL
#define MKV_ID_SignaturePublicKey	0x7EA5LL
#define MKV_ID_SignedElement	0x6532LL
#define MKV_ID_SimpleBlock	0xA3LL
#define MKV_ID_TagBinary	0x4485LL
#define MKV_ID_TrackTranslateTrackID	0x66A5LL
#define MKV_ID_Void	0xECLL
#define MKV_ID_DateUTC	0x4461LL
#define MKV_ID_DurationInSegmentInfo	0x4489LL
#define MKV_ID_FrameRate	0x2383E3LL
#define MKV_ID_GammaValue	0x2FB523LL
#define MKV_ID_OutputSamplingFrequency	0x78B5LL
#define MKV_ID_SamplingFrequency	0xB5LL
#define MKV_ID_TrackTimecodeScale	0x23314FLL
#define MKV_ID_ReferenceBlock	0xFBLL
#define MKV_ID_ReferenceVirtual	0xFDLL
#define MKV_ID_TrackOffset	0x537FLL
#define MKV_ID_AttachedFile	0x61A7LL
#define MKV_ID_Attachments	0x1941A469LL
#define MKV_ID_Audio	0xE1LL
#define MKV_ID_BlockAdditions	0x75A1LL
#define MKV_ID_BlockGroup	0xA0LL
#define MKV_ID_BlockMore	0xA6LL
#define MKV_ID_ChapProcess	0x6944LL
#define MKV_ID_ChapProcessCommand	0x6911LL
#define MKV_ID_ChapterAtom	0xB6LL
#define MKV_ID_ChapterDisplay	0x80LL
#define MKV_ID_ChapterTrack	0x8FLL
#define MKV_ID_ChapterTranslate	0x6924LL
#define MKV_ID_Chapters	0x1043A770LL
#define MKV_ID_Cluster	0x1F43B675LL
#define MKV_ID_ContentCompression	0x5034LL
#define MKV_ID_ContentEncoding	0x6240LL
#define MKV_ID_ContentEncodings	0x6d80LL
#define MKV_ID_ContentEncryption	0x5035LL
#define MKV_ID_CuePoint	0xBBLL
#define MKV_ID_CueReference	0xDBLL
#define MKV_ID_CueTrackPositions	0xB7LL
#define MKV_ID_Cues	0x1C53BB6BLL
#define MKV_ID_EBML	0x1A45DFA3LL
#define MKV_ID_EditionEntry	0x45B9LL
#define MKV_ID_Info	0x1549A966LL
#define MKV_ID_Seek	0x4DBBLL
#define MKV_ID_SeekHead	0x114D9B74LL
#define MKV_ID_Segment	0x18538067LL
#define MKV_ID_SignatureElementList	0x7E7BLL
#define MKV_ID_SignatureElements	0x7E5BLL
#define MKV_ID_SignatureSlot	0x1B538667LL
#define MKV_ID_SilentTracks	0x5854LL
#define MKV_ID_SimpleTag	0x67C8LL
#define MKV_ID_Slices	0x8ELL
#define MKV_ID_Tag	0x7373LL
#define MKV_ID_Tags	0x1254C367LL
#define MKV_ID_Targets	0x63C0LL
#define MKV_ID_TimeSlice	0xE8LL
#define MKV_ID_TrackCombinePlanes	0xE3LL
#define MKV_ID_TrackEntry	0xAELL
#define MKV_ID_TrackJoinBlocks	0xE9LL
#define MKV_ID_TrackOperation	0xE2LL
#define MKV_ID_TrackPlane	0xE4LL
#define MKV_ID_TrackTranslate	0x6624LL
#define MKV_ID_Tracks	0x1654AE6BLL
#define MKV_ID_Video	0xE0LL
#define MKV_ID_ChapCountry	0x437ELL
#define MKV_ID_ChapLanguage	0x437CLL
#define MKV_ID_CodecDownloadURL	0x26B240LL
#define MKV_ID_CodecID	0x86LL
#define MKV_ID_CodecInfoURL	0x3B4040LL
#define MKV_ID_DocType	0x4282LL
#define MKV_ID_FileMimeType	0x4660LL
#define MKV_ID_Language	0x22B59CLL
#define MKV_ID_TagLanguage	0x447ALL
#define MKV_ID_TargetType	0x63CALL
#define MKV_ID_AspectRatioType	0x54B3LL
#define MKV_ID_AttachmentLink	0x7446LL
#define MKV_ID_AttachmentUID	0x63C6LL
#define MKV_ID_BitDepth	0x6264LL
#define MKV_ID_BlockAddID	0xEELL
#define MKV_ID_BlockAdditionID	0xCBLL
#define MKV_ID_BlockDuration	0x9BLL
#define MKV_ID_Channels	0x9FLL
#define MKV_ID_ChapProcessCodecID	0x6955LL
#define MKV_ID_ChapProcessTime	0x6922LL
#define MKV_ID_ChapterFlagEnabled	0x4598LL
#define MKV_ID_ChapterFlagHidden	0x98LL
#define MKV_ID_ChapterPhysicalEquiv	0x63C3LL
#define MKV_ID_ChapterTimeEnd	0x92LL
#define MKV_ID_ChapterTimeStart	0x91LL
#define MKV_ID_ChapterTrackNumber	0x89LL
#define MKV_ID_ChapterTranslateCodec	0x69BFLL
#define MKV_ID_ChapterTranslateEditionUID	0x69FCLL
#define MKV_ID_ChapterUIDInTagging	0x63C4LL
#define MKV_ID_ChapterUIDInChapters	0x73C4LL
#define MKV_ID_CodecDecodeAll	0xAALL
#define MKV_ID_ContentCompAlgo	0x4254LL
#define MKV_ID_ContentEncAlgo	0x47e1LL
#define MKV_ID_ContentEncodingOrder	0x5031LL
#define MKV_ID_ContentEncodingScope	0x5032LL
#define MKV_ID_ContentEncodingType	0x5033LL
#define MKV_ID_ContentSigAlgo	0x47e5LL
#define MKV_ID_ContentSigHashAlgo	0x47e6LL
#define MKV_ID_CueBlockNumber	0x5378LL
#define MKV_ID_CueClusterPosition	0xF1LL
#define MKV_ID_CueCodecState	0xEALL
#define MKV_ID_CueRefCluster	0x97LL
#define MKV_ID_CueRefCodecState	0xEBLL
#define MKV_ID_CueRefNumber	0x535FLL
#define MKV_ID_CueRefTime	0x96LL
#define MKV_ID_CueTime	0xB3LL
#define MKV_ID_CueTrack	0xF7LL
#define MKV_ID_DefaultDuration	0x23E383LL
#define MKV_ID_Delay	0xCELL
#define MKV_ID_DisplayHeight	0x54BALL
#define MKV_ID_DisplayUnit	0x54B2LL
#define MKV_ID_DisplayWidth	0x54B0LL
#define MKV_ID_DocTypeReadVersion	0x4285LL
#define MKV_ID_DocTypeVersion	0x4287LL
#define MKV_ID_DurationInCluster	0xCFLL
#define MKV_ID_EBMLMaxIDLength	0x42F2LL
#define MKV_ID_EBMLMaxSizeLength	0x42F3LL
#define MKV_ID_EBMLReadVersion	0x42F7LL
#define MKV_ID_EBMLVersion	0x4286LL
#define MKV_ID_EditionFlagDefault	0x45DBLL
#define MKV_ID_EditionFlagHidden	0x45BDLL
#define MKV_ID_EditionFlagOrdered	0x45DDLL
#define MKV_ID_EditionUIDInChapters	0x45BCLL
#define MKV_ID_EditionUIDInTagging	0x63C9LL
#define MKV_ID_FileUID	0x46AELL
#define MKV_ID_FlagDefault	0x88LL
#define MKV_ID_FlagEnabled	0xB9LL
#define MKV_ID_FlagForced	0x55AALL
#define MKV_ID_FlagInterlaced	0x9ALL
#define MKV_ID_FlagLacing	0x9CLL
#define MKV_ID_FrameNumber	0xCDLL
#define MKV_ID_LaceNumber	0xCCLL
#define MKV_ID_MaxBlockAdditionID	0x55EELL
#define MKV_ID_MaxCache	0x6DF8LL
#define MKV_ID_MinCache	0x6DE7LL
#define MKV_ID_PixelCropBottom	0x54AALL
#define MKV_ID_PixelCropLeft	0x54CCLL
#define MKV_ID_PixelCropRight	0x54DDLL
#define MKV_ID_PixelCropTop	0x54BBLL
#define MKV_ID_PixelHeight	0xBALL
#define MKV_ID_PixelWidth	0xB0LL
#define MKV_ID_Position	0xA7LL
#define MKV_ID_PrevSize	0xABLL
#define MKV_ID_ReferencePriority	0xFALL
#define MKV_ID_SeekPosition	0x53ACLL
#define MKV_ID_SignatureAlgo	0x7E8ALL
#define MKV_ID_SignatureHash	0x7E9ALL
#define MKV_ID_SilentTrackNumber	0x58D7LL
#define MKV_ID_StereoMode	0x53B8LL
#define MKV_ID_TagDefault	0x4484LL
#define MKV_ID_TargetTypeValue	0x68CALL
#define MKV_ID_Timecode	0xE7LL
#define MKV_ID_TimecodeScale	0x2AD7B1LL
#define MKV_ID_TrackJoinUID	0xEDLL
#define MKV_ID_TrackNumber	0xD7LL
#define MKV_ID_TrackOverlay	0x6FABLL
#define MKV_ID_TrackPlaneType	0xE6LL
#define MKV_ID_TrackPlaneUID	0xE5LL
#define MKV_ID_TrackTranslateCodec	0x66BFLL
#define MKV_ID_TrackTranslateEditionUID	0x66FCLL
#define MKV_ID_TrackType	0x83LL
#define MKV_ID_TrackUIDInTagging	0x63C5LL
#define MKV_ID_TrackUIDInTrack	0x73C5LL

inline string MKVElemToString(uint64_t id) {
	switch (id) {
		case MKV_ID_ChapString: return "ChapString";
		case MKV_ID_CodecName: return "CodecName";
		case MKV_ID_CodecSettings: return "CodecSettings";
		case MKV_ID_FileDescription: return "FileDescription";
		case MKV_ID_FileName: return "FileName";
		case MKV_ID_MuxingApp: return "MuxingApp";
		case MKV_ID_Name: return "Name";
		case MKV_ID_NextFilename: return "NextFilename";
		case MKV_ID_PrevFilename: return "PrevFilename";
		case MKV_ID_SegmentFilename: return "SegmentFilename";
		case MKV_ID_TagName: return "TagName";
		case MKV_ID_TagString: return "TagString";
		case MKV_ID_Title: return "Title";
		case MKV_ID_WritingApp: return "WritingApp";
		case MKV_ID_Block: return "Block";
		case MKV_ID_BlockAdditional: return "BlockAdditional";
		case MKV_ID_BlockVirtual: return "BlockVirtual";
		case MKV_ID_CRC_32: return "CRC_32";
		case MKV_ID_ChannelPositions: return "ChannelPositions";
		case MKV_ID_ChapProcessData: return "ChapProcessData";
		case MKV_ID_ChapProcessPrivate: return "ChapProcessPrivate";
		case MKV_ID_ChapterSegmentEditionUID: return "ChapterSegmentEditionUID";
		case MKV_ID_ChapterSegmentUID: return "ChapterSegmentUID";
		case MKV_ID_ChapterTranslateID: return "ChapterTranslateID";
		case MKV_ID_CodecPrivate: return "CodecPrivate";
		case MKV_ID_CodecState: return "CodecState";
		case MKV_ID_ColourSpace: return "ColourSpace";
		case MKV_ID_ContentCompSettings: return "ContentCompSettings";
		case MKV_ID_ContentEncKeyID: return "ContentEncKeyID";
		case MKV_ID_ContentSigKeyID: return "ContentSigKeyID";
		case MKV_ID_ContentSignature: return "ContentSignature";
		case MKV_ID_EncryptedBlock: return "EncryptedBlock";
		case MKV_ID_FileData: return "FileData";
		case MKV_ID_FileReferral: return "FileReferral";
		case MKV_ID_NextUID: return "NextUID";
		case MKV_ID_PrevUID: return "PrevUID";
		case MKV_ID_SeekID: return "SeekID";
		case MKV_ID_SegmentFamily: return "SegmentFamily";
		case MKV_ID_SegmentUID: return "SegmentUID";
		case MKV_ID_Signature: return "Signature";
		case MKV_ID_SignaturePublicKey: return "SignaturePublicKey";
		case MKV_ID_SignedElement: return "SignedElement";
		case MKV_ID_SimpleBlock: return "SimpleBlock";
		case MKV_ID_TagBinary: return "TagBinary";
		case MKV_ID_TrackTranslateTrackID: return "TrackTranslateTrackID";
		case MKV_ID_Void: return "Void";
		case MKV_ID_DateUTC: return "DateUTC";
		case MKV_ID_DurationInSegmentInfo: return "DurationInSegmentInfo";
		case MKV_ID_FrameRate: return "FrameRate";
		case MKV_ID_GammaValue: return "GammaValue";
		case MKV_ID_OutputSamplingFrequency: return "OutputSamplingFrequency";
		case MKV_ID_SamplingFrequency: return "SamplingFrequency";
		case MKV_ID_TrackTimecodeScale: return "TrackTimecodeScale";
		case MKV_ID_ReferenceBlock: return "ReferenceBlock";
		case MKV_ID_ReferenceVirtual: return "ReferenceVirtual";
		case MKV_ID_TrackOffset: return "TrackOffset";
		case MKV_ID_AttachedFile: return "AttachedFile";
		case MKV_ID_Attachments: return "Attachments";
		case MKV_ID_Audio: return "Audio";
		case MKV_ID_BlockAdditions: return "BlockAdditions";
		case MKV_ID_BlockGroup: return "BlockGroup";
		case MKV_ID_BlockMore: return "BlockMore";
		case MKV_ID_ChapProcess: return "ChapProcess";
		case MKV_ID_ChapProcessCommand: return "ChapProcessCommand";
		case MKV_ID_ChapterAtom: return "ChapterAtom";
		case MKV_ID_ChapterDisplay: return "ChapterDisplay";
		case MKV_ID_ChapterTrack: return "ChapterTrack";
		case MKV_ID_ChapterTranslate: return "ChapterTranslate";
		case MKV_ID_Chapters: return "Chapters";
		case MKV_ID_Cluster: return "Cluster";
		case MKV_ID_ContentCompression: return "ContentCompression";
		case MKV_ID_ContentEncoding: return "ContentEncoding";
		case MKV_ID_ContentEncodings: return "ContentEncodings";
		case MKV_ID_ContentEncryption: return "ContentEncryption";
		case MKV_ID_CuePoint: return "CuePoint";
		case MKV_ID_CueReference: return "CueReference";
		case MKV_ID_CueTrackPositions: return "CueTrackPositions";
		case MKV_ID_Cues: return "Cues";
		case MKV_ID_EBML: return "EBML";
		case MKV_ID_EditionEntry: return "EditionEntry";
		case MKV_ID_Info: return "Info";
		case MKV_ID_Seek: return "Seek";
		case MKV_ID_SeekHead: return "SeekHead";
		case MKV_ID_Segment: return "Segment";
		case MKV_ID_SignatureElementList: return "SignatureElementList";
		case MKV_ID_SignatureElements: return "SignatureElements";
		case MKV_ID_SignatureSlot: return "SignatureSlot";
		case MKV_ID_SilentTracks: return "SilentTracks";
		case MKV_ID_SimpleTag: return "SimpleTag";
		case MKV_ID_Slices: return "Slices";
		case MKV_ID_Tag: return "Tag";
		case MKV_ID_Tags: return "Tags";
		case MKV_ID_Targets: return "Targets";
		case MKV_ID_TimeSlice: return "TimeSlice";
		case MKV_ID_TrackCombinePlanes: return "TrackCombinePlanes";
		case MKV_ID_TrackEntry: return "TrackEntry";
		case MKV_ID_TrackJoinBlocks: return "TrackJoinBlocks";
		case MKV_ID_TrackOperation: return "TrackOperation";
		case MKV_ID_TrackPlane: return "TrackPlane";
		case MKV_ID_TrackTranslate: return "TrackTranslate";
		case MKV_ID_Tracks: return "Tracks";
		case MKV_ID_Video: return "Video";
		case MKV_ID_ChapCountry: return "ChapCountry";
		case MKV_ID_ChapLanguage: return "ChapLanguage";
		case MKV_ID_CodecDownloadURL: return "CodecDownloadURL";
		case MKV_ID_CodecID: return "CodecID";
		case MKV_ID_CodecInfoURL: return "CodecInfoURL";
		case MKV_ID_DocType: return "DocType";
		case MKV_ID_FileMimeType: return "FileMimeType";
		case MKV_ID_Language: return "Language";
		case MKV_ID_TagLanguage: return "TagLanguage";
		case MKV_ID_TargetType: return "TargetType";
		case MKV_ID_AspectRatioType: return "AspectRatioType";
		case MKV_ID_AttachmentLink: return "AttachmentLink";
		case MKV_ID_AttachmentUID: return "AttachmentUID";
		case MKV_ID_BitDepth: return "BitDepth";
		case MKV_ID_BlockAddID: return "BlockAddID";
		case MKV_ID_BlockAdditionID: return "BlockAdditionID";
		case MKV_ID_BlockDuration: return "BlockDuration";
		case MKV_ID_Channels: return "Channels";
		case MKV_ID_ChapProcessCodecID: return "ChapProcessCodecID";
		case MKV_ID_ChapProcessTime: return "ChapProcessTime";
		case MKV_ID_ChapterFlagEnabled: return "ChapterFlagEnabled";
		case MKV_ID_ChapterFlagHidden: return "ChapterFlagHidden";
		case MKV_ID_ChapterPhysicalEquiv: return "ChapterPhysicalEquiv";
		case MKV_ID_ChapterTimeEnd: return "ChapterTimeEnd";
		case MKV_ID_ChapterTimeStart: return "ChapterTimeStart";
		case MKV_ID_ChapterTrackNumber: return "ChapterTrackNumber";
		case MKV_ID_ChapterTranslateCodec: return "ChapterTranslateCodec";
		case MKV_ID_ChapterTranslateEditionUID: return "ChapterTranslateEditionUID";
		case MKV_ID_ChapterUIDInTagging: return "ChapterUIDInTagging";
		case MKV_ID_ChapterUIDInChapters: return "ChapterUIDInChapters";
		case MKV_ID_CodecDecodeAll: return "CodecDecodeAll";
		case MKV_ID_ContentCompAlgo: return "ContentCompAlgo";
		case MKV_ID_ContentEncAlgo: return "ContentEncAlgo";
		case MKV_ID_ContentEncodingOrder: return "ContentEncodingOrder";
		case MKV_ID_ContentEncodingScope: return "ContentEncodingScope";
		case MKV_ID_ContentEncodingType: return "ContentEncodingType";
		case MKV_ID_ContentSigAlgo: return "ContentSigAlgo";
		case MKV_ID_ContentSigHashAlgo: return "ContentSigHashAlgo";
		case MKV_ID_CueBlockNumber: return "CueBlockNumber";
		case MKV_ID_CueClusterPosition: return "CueClusterPosition";
		case MKV_ID_CueCodecState: return "CueCodecState";
		case MKV_ID_CueRefCluster: return "CueRefCluster";
		case MKV_ID_CueRefCodecState: return "CueRefCodecState";
		case MKV_ID_CueRefNumber: return "CueRefNumber";
		case MKV_ID_CueRefTime: return "CueRefTime";
		case MKV_ID_CueTime: return "CueTime";
		case MKV_ID_CueTrack: return "CueTrack";
		case MKV_ID_DefaultDuration: return "DefaultDuration";
		case MKV_ID_Delay: return "Delay";
		case MKV_ID_DisplayHeight: return "DisplayHeight";
		case MKV_ID_DisplayUnit: return "DisplayUnit";
		case MKV_ID_DisplayWidth: return "DisplayWidth";
		case MKV_ID_DocTypeReadVersion: return "DocTypeReadVersion";
		case MKV_ID_DocTypeVersion: return "DocTypeVersion";
		case MKV_ID_DurationInCluster: return "DurationInCluster";
		case MKV_ID_EBMLMaxIDLength: return "EBMLMaxIDLength";
		case MKV_ID_EBMLMaxSizeLength: return "EBMLMaxSizeLength";
		case MKV_ID_EBMLReadVersion: return "EBMLReadVersion";
		case MKV_ID_EBMLVersion: return "EBMLVersion";
		case MKV_ID_EditionFlagDefault: return "EditionFlagDefault";
		case MKV_ID_EditionFlagHidden: return "EditionFlagHidden";
		case MKV_ID_EditionFlagOrdered: return "EditionFlagOrdered";
		case MKV_ID_EditionUIDInChapters: return "EditionUIDInChapters";
		case MKV_ID_EditionUIDInTagging: return "EditionUIDInTagging";
		case MKV_ID_FileUID: return "FileUID";
		case MKV_ID_FlagDefault: return "FlagDefault";
		case MKV_ID_FlagEnabled: return "FlagEnabled";
		case MKV_ID_FlagForced: return "FlagForced";
		case MKV_ID_FlagInterlaced: return "FlagInterlaced";
		case MKV_ID_FlagLacing: return "FlagLacing";
		case MKV_ID_FrameNumber: return "FrameNumber";
		case MKV_ID_LaceNumber: return "LaceNumber";
		case MKV_ID_MaxBlockAdditionID: return "MaxBlockAdditionID";
		case MKV_ID_MaxCache: return "MaxCache";
		case MKV_ID_MinCache: return "MinCache";
		case MKV_ID_PixelCropBottom: return "PixelCropBottom";
		case MKV_ID_PixelCropLeft: return "PixelCropLeft";
		case MKV_ID_PixelCropRight: return "PixelCropRight";
		case MKV_ID_PixelCropTop: return "PixelCropTop";
		case MKV_ID_PixelHeight: return "PixelHeight";
		case MKV_ID_PixelWidth: return "PixelWidth";
		case MKV_ID_Position: return "Position";
		case MKV_ID_PrevSize: return "PrevSize";
		case MKV_ID_ReferencePriority: return "ReferencePriority";
		case MKV_ID_SeekPosition: return "SeekPosition";
		case MKV_ID_SignatureAlgo: return "SignatureAlgo";
		case MKV_ID_SignatureHash: return "SignatureHash";
		case MKV_ID_SilentTrackNumber: return "SilentTrackNumber";
		case MKV_ID_StereoMode: return "StereoMode";
		case MKV_ID_TagDefault: return "TagDefault";
		case MKV_ID_TargetTypeValue: return "TargetTypeValue";
		case MKV_ID_Timecode: return "Timecode";
		case MKV_ID_TimecodeScale: return "TimecodeScale";
		case MKV_ID_TrackJoinUID: return "TrackJoinUID";
		case MKV_ID_TrackNumber: return "TrackNumber";
		case MKV_ID_TrackOverlay: return "TrackOverlay";
		case MKV_ID_TrackPlaneType: return "TrackPlaneType";
		case MKV_ID_TrackPlaneUID: return "TrackPlaneUID";
		case MKV_ID_TrackTranslateCodec: return "TrackTranslateCodec";
		case MKV_ID_TrackTranslateEditionUID: return "TrackTranslateEditionUID";
		case MKV_ID_TrackType: return "TrackType";
		case MKV_ID_TrackUIDInTagging: return "TrackUIDInTagging";
		case MKV_ID_TrackUIDInTrack: return "TrackUIDInTrack";
		default: return "#unknown#";
	}
}

#endif	/* _ELEMENTIDS_H */
#endif /* HAS_MEDIA_MKV */
