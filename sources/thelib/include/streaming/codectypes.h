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


#ifndef _CODECTYPES_H
#define	_CODECTYPES_H

#include "common.h"

#define CODEC_VIDEO_UNKNOWN			MAKE_TAG4('V','U','N','K')
#define CODEC_VIDEO_PASS_THROUGH	MAKE_TAG3('V','P','T')
#define CODEC_VIDEO_JPEG			MAKE_TAG5('V','J','P','E','G')
#define CODEC_VIDEO_SORENSON_H263	MAKE_TAG6('V','S','H','2','6','3')
#define CODEC_VIDEO_SCREEN_VIDEO	MAKE_TAG5('V','S','V','I','D')
#define CODEC_VIDEO_VP6				MAKE_TAG4('V','V','P','6')
#define CODEC_VIDEO_VP6_ALPHA		MAKE_TAG5('V','V','P','6','A')
#define CODEC_VIDEO_AVC				MAKE_TAG4('V','A','V','C')

#define CODEC_AUDIO_UNKNOWN			MAKE_TAG4('A','U','N','K')
#define CODEC_AUDIO_PASS_THROUGH	MAKE_TAG3('A','P','T')
#define CODEC_AUDIO_AAC				MAKE_TAG4('A','A','A','C')
#define CODEC_AUDIO_ADTS			MAKE_TAG5('A','A','D','T','S')
#define CODEC_AUDIO_MP3				MAKE_TAG4('A','M','P','3')

#endif	/* _CODECTYPES_H */


