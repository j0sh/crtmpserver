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

#ifdef HAS_LUA
#ifndef _LUAAPPVIRTUALMACHINE_H
#define	_LUAAPPVIRTUALMACHINE_H

#include "vm/baseappvirtualmachine.h"
class BaseVMLua;

namespace app_vmapp {

	class LuaAppVirtualMachine
	: public BaseAppVirtualMachine {
		friend class BaseAppVirtualMachine;
	private:
		BaseVMLua *_pVM;
		int _functionsMappingIds[APIFunctionID_size + 1];
	protected:
		LuaAppVirtualMachine();
	public:
		virtual ~LuaAppVirtualMachine();
		virtual bool HasFunction(APIFunctionID functionId);
		virtual bool Call(APIFunctionID functionId);
		virtual bool Call(APIFunctionID functionId, Variant &variant);
		virtual bool Call(APIFunctionID functionId, BaseStream *pBaseStream);
		virtual bool Call(APIFunctionID functionId, InNetTSStream *pInNetTSStream);
		virtual bool Call(APIFunctionID functionId, BaseProtocol *pProtocol);
		virtual bool Call(APIFunctionID functionId, OutboundRTMPProtocol *pProtocol);
		virtual bool Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
				Variant &variant);
		virtual bool Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
				Variant &variant1, Variant &variant2);
		virtual bool Call(APIFunctionID functionId, URI &uri, Variant &variant);
		virtual bool Call(APIFunctionID functionId, BaseInStream *pInStream,
				Variant &variant);
	protected:
		virtual bool BindAPI();
		virtual bool Initialize(string script);
	private:
		bool BindAPIGenerics();
		bool BindAPIApplication();
		bool BindAPIProtocols();
		bool BindAPIHandlers();
		bool BindAPIHandlersRTMP();
		bool BindAPIHandlersRTSP();
		bool BindAPIHandlersMPEGTS();
		bool BindAPIHandlersLiveFLV();
		bool GetFunctionsMappings();
		bool ValidateHandler(Variant &handler);
		void Shutdown();
	};
}

#endif	/* _LUAAPPVIRTUALMACHINE_H */
#endif	/* HAS_LUA */
