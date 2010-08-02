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


#ifndef _GENERICFUNCTIONALITY_H
#define	_GENERICFUNCTIONALITY_H

#include "utils/core.h"

//1. define all the STL helpers
#define STR(x) (((string)(x)).c_str())
#define MAP_HAS1(m,k) ((bool)((m).find((k))!=(m).end()))
#define MAP_HAS2(m,k1,k2) ((MAP_HAS1((m),(k1))==true)?MAP_HAS1((m)[(k1)],(k2)):false)
#define MAP_HAS3(m,k1,k2,k3) ((MAP_HAS1((m),(k1)))?MAP_HAS2((m)[(k1)],(k2),(k3)):false)
#define FOR_MAP(m,k,v,i) for(map< k , v >::iterator i=(m).begin();i!=(m).end();i++)
#define MAP_KEY(i) ((i)->first)
#define MAP_VAL(i) ((i)->second)
#define MAP_ERASE1(m,k) if(MAP_HAS1((m),(k))) (m).erase((k));
#define MAP_ERASE2(m,k1,k2) \
if(MAP_HAS1((m),(k1))){ \
    MAP_ERASE1((m)[(k1)],(k2)); \
    if((m)[(k1)].size()==0) \
        MAP_ERASE1((m),(k1)); \
}
#define MAP_ERASE3(m,k1,k2,k3) \
if(MAP_HAS1((m),(k1))){ \
    MAP_ERASE2((m)[(k1)],(k2),(k3)); \
    if((m)[(k1)].size()==0) \
        MAP_ERASE1((m),(k1)); \
}

#define FOR_VECTOR(v,i) for(uint32_t i=0;i<(v).size();i++)
#define FOR_VECTOR_ITERATOR(e,v,i) for(vector<e>::iterator i=(v).begin();i!=(v).end();i++)
#define FOR_VECTOR_WITH_START(v,i,s) for(uint32_t i=s;i<(v).size();i++)
#define ADD_VECTOR_END(v,i) (v).push_back((i))
#define ADD_VECTOR_BEGIN(v,i) (v).insert((v).begin(),(i))
#define VECTOR_VAL(i) (*(i))

#define MAKE_TAG8(a,b,c,d,e,f,g,h) ((uint64_t)(((uint64_t)(a))<<56)|(((uint64_t)(b))<<48)|(((uint64_t)(c))<<40)|(((uint64_t)(d))<<32)|(((uint64_t)(e))<<24)|(((uint64_t)(f))<<16)|(((uint64_t)(g))<<8)|((uint64_t)(h)))
#define MAKE_TAG7(a,b,c,d,e,f,g) MAKE_TAG8(a,b,c,d,e,f,g,0)
#define MAKE_TAG6(a,b,c,d,e,f) MAKE_TAG7(a,b,c,d,e,f,0)
#define MAKE_TAG5(a,b,c,d,e) MAKE_TAG6(a,b,c,d,e,0)
#define MAKE_TAG4(a,b,c,d) MAKE_TAG5(a,b,c,d,0)
#define MAKE_TAG3(a,b,c) MAKE_TAG4(a,b,c,0)
#define MAKE_TAG2(a,b) MAKE_TAG3(a,b,0)
#define MAKE_TAG1(a) MAKE_TAG2(a,0)

#define TAG_KIND_OF(tag,kind) ((bool)(((tag)&getTagMask((kind)))==(kind)))

//2. define the fetch related things...
#define URL_SCHEMELEN 16
#define URL_USERLEN 256
#define URL_PWDLEN 256
#define SCHEME_FTP      "ftp"
#define SCHEME_HTTP     "http"
#define SCHEME_HTTPS    "https"
#define SCHEME_FILE     "file"

struct url {
	char scheme[URL_SCHEMELEN + 1];
	char user[URL_USERLEN + 1];
	char pwd[URL_PWDLEN + 1];
	char host[MAXHOSTNAMELEN + 1];
	int port;
	char *doc;
	off_t offset;
	size_t length;
};

typedef void (*SignalFnc)(void);

//2. the fine the utilities
DLLEXP string tagToString(uint64_t tag);
DLLEXP uint64_t getTagMask(uint64_t tag);
DLLEXP bool isNumeric(string value);
DLLEXP string lowercase(string value);
DLLEXP string uppercase(string value);
DLLEXP void ltrim(string &value);
DLLEXP void rtrim(string &value);
DLLEXP void trim(string &value);
DLLEXP void replace(string &target, string search, string replacement);
DLLEXP void split(string str, string separator, vector<string> &result);
//DLLEXP bool nameValue(string line, string separator,
//        string &name, string &value);
DLLEXP map<string, string> mapping(string str,
		string separator1, string separator2, bool trimStrings);
//DLLEXP string unmapping(map<string, string>& _map,
//        char c1, char c2);
DLLEXP string vformat(string format, va_list args);
DLLEXP string format(string format, ...);
DLLEXP void splitFileName(string fileName, string &name,
		string &extension, char separator = '.');
DLLEXP string generateRandomString(uint32_t length);
DLLEXP string GetHostByName(string name);
DLLEXP bool ParseURL(string stringUrl, string &host, uint16_t &port, string &user,
		string &pwd, string &doc);
DLLEXP void InstallQuitSignal(SignalFnc pQuitSignalFnc);
DLLEXP void InstallConfRereadSignal(SignalFnc pConfRereadSignalFnc);
DLLEXP bool MoveFile(string src, string dst);
DLLEXP bool DeleteFile(string path);
DLLEXP double GetFileModificationDate(string path);
DLLEXP void InitNetworking();


#endif	/* _GENERICFUNCTIONALITY_H */


