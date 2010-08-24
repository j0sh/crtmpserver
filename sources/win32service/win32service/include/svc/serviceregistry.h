//#define CONFIGFILE_PATH TEXT("C:\\rtmpd-clean\\builders\\VS2010\\Debug\\rtmpserver.lua")
#define CONFIGFILE_PATH TEXT("C:\\rtmpserver.lua")
#define STR(x) (((wstring)(x)).c_str())

char * QueryKey(HKEY hKey);
HKEY AddKey();
DWORD SetKeyValue(HKEY hKey);

char * SvcRegistry(int action);
char * FormatWindowsError(int nError);
