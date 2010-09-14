//#define CONFIGFILE_PATH TEXT("C:\\rtmpd-clean\\builders\\VS2010\\Debug\\rtmpserver.lua")
//#define STR(x) (((wstring)(x)).c_str())
#define TOTALBYTES    8192
#define BYTEINCREMENT 4096

using namespace std;
char * QueryKey(HKEY hKey, char * key);
HKEY AddKey();
DWORD SetKeyValue(HKEY hKey);
char * AppendDoubleSlashSeparator(char * filePath);
char * SvcRegistry(int action, char * key);
void FormatWindowsError(int nError);
