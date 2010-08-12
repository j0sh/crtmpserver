Application: win32service.

Source: http://msdn.microsoft.com/en-us/library/bb540476%28v=VS.85%29.aspx

Brief Description:
win32service is a win32 console application that installs a service named "evo". 
This service can be started and stopped using the Services Administrative Tool seen from the Control Panel.

Additional Applications:
There are other applications outside win32service that can be useful in testing:
1. SvcControl - is a console application that can start and stop the service. More commands can be done here (refer to Source)
2. SvcConfig - is a consol application that can be used to delete the service. More commands can be done here (refer to Source)

Instruction (How to use?):
To start/stop service:
1. Compile the application.
2. Open a command prompt as administrator (right click on command prompt, select "Run as Administrator").
3. cd to directory of win32service.
4. Type "win32service install" and wait for "Service installed successfuly" to appear.
5. Check the Service admin tool and look for "evo" service.
6. Start/Stop evo service.

To delete the service: compile SvcConfig, cd to the dir of svcconfig.exe, type "svcconfig delete evo".

To start/stop rtmpserver (Will be improved soon)
rtmpserver should start when evo service starts. To make sure that it starts successfuly:
- Put the exact path of rtmpserver.lua in line#-84 of rtmpserver.cpp.
- Assign a "mediaFolder" parameter in rtmpserver.lua
Stop the evo service to stop rtmpserver.

NOTES:
- Currently, the rtmpserver logs is saved to C:\\RTMPServerlogs.txt.
- There are limited sample applications in the net that implements this model. 
- In most of the sample apps, they loop a "while()" during the service's RUNNING state, simulating a running thread (since SvcMain acts like a thread).