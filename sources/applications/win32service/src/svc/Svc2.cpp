#include "netio/netio.h"
#include "configuration/sample.h"
#include "configuration/configfile.h"
#include "protocols/protocolmanager.h"
#include "application/clientapplicationmanager.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/defaultprotocolfactory.h"

//#include <windows.h>
//#include <tchar.h>
//#include <strsafe.h>

#pragma comment(lib, "advapi32.lib")

#define SVCNAME TEXT("evostream")

SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;

VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler( DWORD ); 
VOID WINAPI SvcMain( DWORD, LPTSTR * ); 

VOID ReportSvcStatus( DWORD, DWORD, DWORD );
VOID SvcInit( DWORD, LPTSTR * ); 
VOID SvcReportEvent( LPTSTR );

//This is a structure that holds the state
//between config re-reads/re-runs

struct RunningStatus {
    // startup parameters
    int argc;
    char **argv;

    //Configuration file
    ConfigFile *pConfigFile;

    //default protocol factory
    DefaultProtocolFactory * pProtocolFactory;

    //should we run again?
    bool run;

    //is this a daemon already?
    bool daemon;
};

void QuitSignalHandler(void);
void ConfRereadSignalHandler(void);
bool Initialize();
void Run();
void Cleanup();

RunningStatus gRs = {0};

void StartRtmp();

//
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{ 
    // If command-line parameter is "install", install the service. 
    // Otherwise, the service is probably being started by the SCM.
	
    if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
        SvcInstall();
        return;
    }

    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.

    if (!StartServiceCtrlDispatcher( DispatchTable )) 
    { 
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); 
    } 
} 

//
// Purpose: 
//   Installs a service in the SCM database
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID SvcInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the SCM database. 
 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Create the service

    schService = CreateService( 
        schSCManager,              // SCM database 
        SVCNAME,                   // name of service 
        SVCNAME,                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) 
    {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n"); 

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}

//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI SvcMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
    // Register the handler function for the service

    gSvcStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM

    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.

    SvcInit( dwArgc, lpszArgv );
}

//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.

    ghSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.

    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

    // TO_DO: Perform work until service stops.
	printf("\nTO DO?\n");
	do
		{
			(void)("rtmpserver.exe");//StartRtmp();
		}while ((gSvcStatus.dwCurrentState = SERVICE_RUNNING));
	/*while(gSvcStatus.dwCurrentState = SERVICE_RUNNING)
	{
		StartRtmp();
	}*/
    while(1)
    {
		if(gSvcStatus.dwCurrentState == SERVICE_RUNNING)
		{
			(void)("rtmpserver.exe");
		}
		else
		{
        // Check whether to stop the service.

			WaitForSingleObject(ghSvcStopEvent, INFINITE);
			ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
		    return;
		}
    }
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
           (dwCurrentState == SERVICE_STOPPED) )
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus( gSvcStatusHandle, &gSvcStatus );
}

//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl )
{
   // Handle the requested control code. 

   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.

         SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
 
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 
   
}

//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction) 
{ 
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];

    hEventSource = RegisterEventSource(NULL, SVCNAME);

    if( NULL != hEventSource )
    {
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

        lpszStrings[0] = SVCNAME;
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
}
 void StartRtmp()
 {
	 InitNetworking();

    //1. Pick up the startup parameters and hold them inside the running status
   // gRs.argc = argc;
    //gRs.argv = argv;

    do {
        //2. Reset the run flag
        gRs.run = false;

        //3. Initialize the running status
        if (Initialize()) {
            Run();
        } else {
            gRs.run = false;
        }

        //5. Cleanup
       // Cleanup();
    } while (gRs.run);

    //6. We are done
   // return 0;
 }

 bool Initialize() {

	 gRs.argc = 2;
    gRs.argv[1] = "rtmpserver.lua";

    Logger::Init();

    if (gRs.argc < 2) {
        printf("Usage: %s [--use-implicit-console-appender] <config_file_path>\n", gRs.argv[0]);
        printf("\n--use-implicit-console-appender  use this switch to add a default console\n");
        printf("appender. Used when you are not sure about the logeAppenders section in the config file\n");
        return false;
    }

    for (int i = 1; i < gRs.argc - 1; i++) {
        if (string(gRs.argv[i]) == "--use-implicit-console-appender") {
            ConsoleLogLocation *pLogLocation = new ConsoleLogLocation(false);
            pLogLocation->SetLevel(_FINEST_);
            Logger::AddLogLocation(pLogLocation);
        }
    }

    INFO("Reading configuration from %s", gRs.argv[gRs.argc - 1]);
    gRs.pConfigFile = new ConfigFile();
    string configFilePath = gRs.argv[gRs.argc - 1];
    string fileName;
    string extension;
    splitFileName(configFilePath, fileName, extension);

    if (lowercase(extension) == "xml") {
        if (!gRs.pConfigFile->LoadXmlFile(configFilePath)) {
            FATAL("Unable to load file %s", STR(configFilePath));
            return false;
        }
    } else if (lowercase(extension) == "lua") {
#ifdef HAS_LUA
        if (!gRs.pConfigFile->LoadLuaFile(configFilePath)) {
            FATAL("Unable to load file %s", STR(configFilePath));
            return false;
        }
#else
        cout << "Lua is not supported by the current build of the server" << endl;
        ASSERT("Lua is not supported by the current build of the server");
        return false;
#endif /* HAS_LUA */
    } else {
        FATAL("Invalid file format: %s", STR(configFilePath));
        return false;
    }

#ifndef WIN32
    if (gRs.pConfigFile->IsDaemon()) {
        if (!gRs.daemon) {
            INFO("Daemonize...");
            pid_t pid = fork();
            if (pid < 0) {
                FATAL("Unable to start daemonize. fork() failed");
                return false;
            }

            if (pid > 0) {
                return false;
            }

            FINEST("Create a new SID for the daemon");
            pid_t sid = setsid();
            if (sid < 0) {
                FATAL("Unable to start daemonize. setsid() failed");
                return false;
            }

            gRs.daemon = true;
        }
    }
#endif /* WIN32 */

    INFO("Configure log appenders from configuration file");
    if (!gRs.pConfigFile->ConfigureLogAppenders()) {
        WARN("Unable to configure log appenders");
    }

    INFO("Plug in the default protocol factory");
    gRs.pProtocolFactory = new DefaultProtocolFactory();
    if (!ProtocolFactoryManager::RegisterProtocolFactory(gRs.pProtocolFactory)) {
        FATAL("Unable to register default protocols factory");
        return false;
    }

    INFO("Initialize I/O handlers manager: %s", NETWORK_REACTOR);
    IOHandlerManager::Initialize();

    INFO("Initialize DNS resolver");
    if (!gRs.pConfigFile->ConfigureDNSResolver()) {
        FATAL("Unable to configure DNS resolver");
        return false;
    }

    INFO("Initialize applications");
    if (!gRs.pConfigFile->ConfigureApplications()) {
        FATAL("Unable to configure applications");
        return false;
    }

    INFO("Install the quit signal");
    InstallQuitSignal(QuitSignalHandler);

    INFO("Install the conf re-read signal");
    InstallConfRereadSignal(ConfRereadSignalHandler);

    return true;
}

void Run() {
    INFO("\n%s", STR(gRs.pConfigFile->GetServicesInfo()));
    INFO("GO! GO! GO! (%d)", getpid());
    while (IOHandlerManager::Pulse()) {
        IOHandlerManager::DeleteDeadHandlers();
        ProtocolManager::CleanupDeadProtocols();
    }
}

void Cleanup() {
    WARN("Shutting down protocols manager");
    ProtocolManager::Shutdown();
    ProtocolManager::CleanupDeadProtocols();

    WARN("Shutting down I/O handlers manager");
    IOHandlerManager::ShutdownIOHandlers();
    IOHandlerManager::DeleteDeadHandlers();
    IOHandlerManager::Shutdown();

    WARN("Unregister and delete default protocol handler");
    ProtocolFactoryManager::UnRegisterProtocolFactory(gRs.pProtocolFactory);
    delete gRs.pProtocolFactory;
    gRs.pProtocolFactory = NULL;

    WARN("Shutting down applications");
    ClientApplicationManager::Shutdown();

    WARN("Delete the configuration");
    delete gRs.pConfigFile;
    gRs.pConfigFile = NULL;

    WARN("Shutting down the logger leaving you in the dark. Bye bye... :(");
    Logger::Free(true);
}

void QuitSignalHandler(void) {
    IOHandlerManager::SignalShutdown();
}

void ConfRereadSignalHandler(void) {
    gRs.run = true;
    IOHandlerManager::SignalShutdown();
}

