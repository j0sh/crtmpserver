
#include "svc/sample.h"

#define LOGFILE "C:\\ServiceLogs.txt"

SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;

VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler( DWORD ); 
VOID WINAPI SvcMain( DWORD, LPTSTR * ); 

VOID ReportSvcStatus( DWORD, DWORD, DWORD );
VOID SvcInit( DWORD, LPTSTR * ); 
VOID SvcReportEvent( LPTSTR );

DWORD WINAPI ThreadProc( __in  LPVOID lpParameter);

int WriteToLog(char* str); //used to log the service and save it in a file

HANDLE hThread = NULL;
