//#define CONFIGFILE_PATH TEXT("C:\\rtmpd-clean\\builders\\VS2010\\Debug\\rtmpserver.lua")
#define CONFIGFILE_PATH TEXT("C:\\rtmpserver.lua")
#define STR(x) (((wstring)(x)).c_str())

char * QueryKey(HKEY hKey);
HKEY AddKey();
void SetKeyValue(HKEY hKey);

char * GetSvcRegistry();
char * FormatWindowsError(int nError);
