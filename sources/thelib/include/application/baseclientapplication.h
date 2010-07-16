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


#ifndef _BASECLIENTAPPLICATION_H
#define	_BASECLIENTAPPLICATION_H

#include "utils/core.h"
#include "streaming/streamsmanager.h"

class BaseProtocol;
class BaseAppProtocolHandler;
class BaseStream;

class DLLEXP BaseClientApplication {
private:
    static uint32_t _idGenerator;
    uint32_t _id;
    string _name;
    vector<string> _aliases;
    map<uint64_t, BaseAppProtocolHandler *> _protocolsHandlers;
    StreamsManager _streamsManager;
protected:
    Variant _configuration;
    bool _isDefault;
public:
    BaseClientApplication(Variant &configuration);
    virtual ~BaseClientApplication();

    uint32_t GetId();
    string GetName();
    Variant GetConfiguration();
    vector<string> GetAliases();
    bool IsDefault();
    StreamsManager *GetStreamsManager();

    virtual bool Initialize();
    void RegisterAppProtocolHandler(uint64_t protocolType,
            BaseAppProtocolHandler *pAppProtocolHandler);
    void UnRegisterAppProtocolHandler(uint64_t protocolType);

    BaseAppProtocolHandler *GetProtocolHandler(BaseProtocol *pProtocol);
    BaseAppProtocolHandler *GetProtocolHandler(uint64_t protocolType);

    virtual void RegisterProtocol(BaseProtocol *pProtocol);
    virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

    virtual void SignalStreamRegistered(BaseStream *pStream);
    virtual void SignalStreamUnRegistered(BaseStream *pStream);
};


#endif	/* _BASECLIENTAPPLICATION_H */


