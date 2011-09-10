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


#ifndef _SAMPLEFACTORYAPPLICATION_H
#define _SAMPLEFACTORYAPPLICATION_H

#include "application/baseclientapplication.h"

namespace app_samplefactory {
	class ProtocolFactory;
	class EchoAppProtocolHandler;
	
	class DLLEXP SampleFactoryApplication
	: public BaseClientApplication {
	private:
		EchoAppProtocolHandler *_pEchoHandler;
	public:
		SampleFactoryApplication(Variant &configuration);
		virtual ~SampleFactoryApplication();

		virtual bool Initialize();
	};
}

#endif	/* _SAMPLEFACTORYAPPLICATION_H */


