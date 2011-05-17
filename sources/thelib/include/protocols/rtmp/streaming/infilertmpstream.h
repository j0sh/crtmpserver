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
#ifndef _INFILERTMPSTREAM_H
#define	_INFILERTMPSTREAM_H

#include "streaming/baseinfilestream.h"
#include "protocols/rtmp/header.h"
#include "protocols/rtmp/amf0serializer.h"


class BaseRTMPProtocol;
class StreamsManager;

class DLLEXP InFileRTMPStream
: public BaseInFileStream {
private:

	class BaseBuilder {
	public:
		BaseBuilder();
		virtual ~BaseBuilder();
		virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
				IOBuffer &buffer) = 0;
	};

	class AVCBuilder : public BaseBuilder {
	private:
		uint8_t _videoCodecHeaderInit[5];
		uint8_t _videoCodecHeaderKeyFrame[2];
		uint8_t _videoCodecHeader[2];
	public:
		AVCBuilder();
		virtual ~AVCBuilder();
		virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
				IOBuffer &buffer);
	};

	class AACBuilder : public BaseBuilder {
	private:
		uint8_t _audioCodecHeaderInit[2];
		uint8_t _audioCodecHeader[2];
	public:
		AACBuilder();
		virtual ~AACBuilder();
		virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
				IOBuffer &buffer);
	};

	class MP3Builder : public BaseBuilder {
	private:

	public:
		MP3Builder();
		virtual ~MP3Builder();
		virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
				IOBuffer &buffer);
	};

	class PassThroughBuilder : public BaseBuilder {
	public:
		PassThroughBuilder();
		virtual ~PassThroughBuilder();
		virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
				IOBuffer &buffer);
	};
private:
	BaseBuilder *_pAudioBuilder;
	BaseBuilder *_pVideoBuilder;

	IOBuffer _metadataBuffer;
	AMF0Serializer _amfSerializer;
	string _metadataName;
	Variant _metadataParameters;
	Variant _tempVariant;
protected:
	Variant _completeMetadata;
	uint32_t _chunkSize;
public:
	InFileRTMPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name);
	virtual ~InFileRTMPStream();

	virtual bool Initialize(int32_t clientSideBufferLength);

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);

	virtual bool IsCompatibleWithType(uint64_t type);

	uint32_t GetChunkSize();

	static InFileRTMPStream *GetInstance(BaseRTMPProtocol *pRTMPProtocol,
			StreamsManager *pStreamsManager, Variant &metadata);

	void SetCompleteMetadata(Variant &completeMetadata);
	Variant GetCompleteMetadata();
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream);
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream);

	virtual bool BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
			IOBuffer &buffer);
	virtual bool FeedMetaData(FileClass *pFile, MediaFrame &mediaFrame);

};


#endif	/* _INFILERTMPSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

