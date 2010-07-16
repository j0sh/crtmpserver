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


#ifndef _STRAMCAPABILITIES_H
#define	_STRAMCAPABILITIES_H

#include "streaming/codectypes.h"

typedef struct _StreamCapabilities {
	uint64_t videoCodecId;
	uint64_t audioCodecId;

	union {

		struct {
			uint8_t *pSPS;
			uint16_t SPSLength;
			uint8_t *pPPS;
			uint16_t PPSLength;
		} avc;
	} videoCodecInfo;

	union {

		struct {
			uint8_t *pAAC;
			uint32_t AACLength;
			uint32_t sampleRate;
		} aac;
	} audioCodecInfo;

	_StreamCapabilities() {
		videoCodecId = CODEC_VIDEO_UNKNOWN;
		memset(&videoCodecInfo, 0, sizeof (videoCodecInfo));
		audioCodecId = CODEC_AUDIO_UNKNOWN;
		memset(&audioCodecInfo, 0, sizeof (audioCodecInfo));
	}

	void ClearVideo() {
		switch (videoCodecId) {
			case CODEC_VIDEO_AVC:
			{
				if (videoCodecInfo.avc.pSPS != NULL) {
					delete[] videoCodecInfo.avc.pSPS;
				}
				if (videoCodecInfo.avc.pPPS != NULL) {
					delete[] videoCodecInfo.avc.pPPS;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		memset(&videoCodecInfo, 0, sizeof (videoCodecInfo));
		videoCodecId = 0;
	}

	void ClearAudio() {
		switch (audioCodecId) {
			case CODEC_AUDIO_AAC:
			{
				if (audioCodecInfo.aac.pAAC != NULL) {
					delete[] audioCodecInfo.aac.pAAC;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		memset(&audioCodecInfo, 0, sizeof (audioCodecInfo));
		audioCodecId = 0;
	}

	void Clear() {
		ClearVideo();
		ClearAudio();
	}

	virtual ~_StreamCapabilities() {
		Clear();
	}
} StreamCapabilities;


#endif	/* _STRAMCAPABILITIES_H */


