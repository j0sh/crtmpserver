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


#include "utils/core.h"
#include "application/baseclientapplication.h"
#include "application/baseappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "streaming/basestream.h"

uint32_t BaseClientApplication::_idGenerator = 0;

BaseClientApplication::BaseClientApplication(Variant &configuration)
: _streamsManager(this) {
    _id = _idGenerator++;
    _configuration = configuration;
    _name = (string) configuration[CONF_APPLICATION_NAME];
    if ((VariantType) configuration[CONF_APPLICATION_ALIASES] != V_NULL) {

        FOR_MAP((configuration[CONF_APPLICATION_ALIASES]), string, Variant, i) {
            ADD_VECTOR_END(_aliases, MAP_VAL(i));
        }
    }
    _isDefault = (VariantType) configuration[CONF_APPLICATION_DEFAULT] != V_NULL ?
            (bool)configuration[CONF_APPLICATION_DEFAULT] : false;
}

BaseClientApplication::~BaseClientApplication() {

}

uint32_t BaseClientApplication::GetId() {
    return _id;
}

string BaseClientApplication::GetName() {
    return _name;
}

Variant BaseClientApplication::GetConfiguration() {
    return _configuration;
}

vector<string> BaseClientApplication::GetAliases() {
    return _aliases;
}

bool BaseClientApplication::IsDefault() {
    return _isDefault;
}

StreamsManager *BaseClientApplication::GetStreamsManager() {
    return &_streamsManager;
}

bool BaseClientApplication::Initialize() {
    return true;
}

void BaseClientApplication::RegisterAppProtocolHandler(uint64_t protocolType,
        BaseAppProtocolHandler *pAppProtocolHandler) {
    if (MAP_HAS1(_protocolsHandlers, protocolType))
        ASSERT("Invalid protocol handler type. Already registered");
    _protocolsHandlers[protocolType] = pAppProtocolHandler;
    pAppProtocolHandler->SetApplication(this);
}

void BaseClientApplication::UnRegisterAppProtocolHandler(uint64_t protocolType) {
    if (MAP_HAS1(_protocolsHandlers, protocolType))
        _protocolsHandlers[protocolType]->SetApplication(NULL);
    _protocolsHandlers.erase(protocolType);
}

BaseAppProtocolHandler *BaseClientApplication::GetProtocolHandler(BaseProtocol *pProtocol) {
    return GetProtocolHandler(pProtocol->GetType());
}

BaseAppProtocolHandler *BaseClientApplication::GetProtocolHandler(uint64_t protocolType) {
    if (!MAP_HAS1(_protocolsHandlers, protocolType))
        ASSERT("Protocol handler not activated for protocol type %d in application %s",
            protocolType, STR(_name));
    return _protocolsHandlers[protocolType];
}

void BaseClientApplication::RegisterProtocol(BaseProtocol *pProtocol) {
    if (!MAP_HAS1(_protocolsHandlers, pProtocol->GetType()))
        ASSERT("Protocol handler not activated for protocol type %s in application %s",
            STR(tagToString(pProtocol->GetType())),
            STR(_name));
    _protocolsHandlers[pProtocol->GetType()]->RegisterProtocol(pProtocol);
}

void BaseClientApplication::UnRegisterProtocol(BaseProtocol *pProtocol) {
    if (!MAP_HAS1(_protocolsHandlers, pProtocol->GetType()))
        ASSERT("Protocol handler not activated for protocol type %d in application %s",
            pProtocol->GetType(), STR(_name));
    _streamsManager.UnRegisterStreams(pProtocol->GetId());
    _protocolsHandlers[pProtocol->GetType()]->UnRegisterProtocol(pProtocol);
    FINEST("Protocol %s unregistered from application: %s", STR(*pProtocol), STR(_name));
}

void BaseClientApplication::SignalStreamRegistered(BaseStream *pStream) {
    INFO("Stream %d of type %s with name `%s` registered to application `%s`",
            pStream->GetUniqueId(),
            STR(tagToString(pStream->GetType())),
            STR(pStream->GetName()),
            STR(_name));
}

void BaseClientApplication::SignalStreamUnRegistered(BaseStream *pStream) {
    INFO("Stream %d of type %s with name `%s` unregistered from application `%s`",
            pStream->GetUniqueId(),
            STR(tagToString(pStream->GetType())),
            STR(pStream->GetName()),
            STR(_name));
}

