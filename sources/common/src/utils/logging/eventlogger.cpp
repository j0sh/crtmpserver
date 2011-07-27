/* 
 *  Copyright (c) 2010,
 *  Evostream (contact@evostream.com)
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

#include "common.h"
#include "utils/logging/eventlogger.h"

Variant EventLogger::_result;

Variant& EventLogger::CreateLE(Variant &stats, string operation, uint32_t statusCode, string statusCodeDesc, Variant &fields) {
	if (stats.HasKeyChain(V_MAP, false, 1, "carrier"))
		_result["carrier"] = stats["carrier"];
	else
		_result["carrier"] = Variant();

	_result["operation"] = operation;
	_result["statusCode"] = statusCode;
	_result["statusCodeDesc"] = statusCodeDesc;
	_result["fields"] = fields;

	return _result;
}
