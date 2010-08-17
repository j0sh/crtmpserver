#define TEXT_SIZE 80
//VOID __stdcall DisplayUsage(void);

bool __stdcall DoQuerySvc(void);
VOID __stdcall DoUpdateSvcDesc(void);
VOID __stdcall DoDisableSvc(void);
VOID __stdcall DoEnableSvc(void);
VOID __stdcall DoDeleteSvc(void);

bool svcconfig (TCHAR * argv[]);