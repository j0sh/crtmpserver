#define RTMPLOGFILE "C:\\rtmplogs.txt"
int WriteToRtmpLog(char* str);

void QuitSignalHandler(void);
void ConfRereadSignalHandler(void);
bool Initialize();
void Run();
void Cleanup();
void rtmpserver(char * config_filePath);