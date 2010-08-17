Application: win32service.

Source: http://msdn.microsoft.com/en-us/library/bb540476%28v=VS.85%29.aspx

BRIEF DESCRIPTION:
win32service is a win32 console application that runs the following command:
1. Installs a Windows service named "evo"
2. Uninstalls the service named "evo"
3. Starts the service
4. Stops the service
5. Starts the rtmpserver normally in the console.
In addition, the service can also be started and stopped using the Services Administrative Tool seen from the Control Panel.

MODULES:
1. svcconfig - contains the function that deletes/uninstalls the service. This is also where the application checks if a service is already installed.
2. svccontrol - contains the functions that starts and stops the service.
3. svc - contains the "main" function. When the service is created, the SvcMain acts as the main function of the thread when the service starts.
4. rtmpserver - is the application that runs the server.

Instruction (How to use):
To start/stop service:
1. Compile the application.
2. Open a command prompt as administrator (right click on command prompt, select "Run as Administrator").
3. cd to directory of win32service.
4. Type "win32service installService" and wait for "Service installed successfuly" to appear.
5. Check the Service admin tool and look for "evo" service.
6. Start/Stop evo service.

To start/stop rtmpserver
rtmpserver should start when evo service starts. To make sure that it starts successfuly:
- Put the exact path of rtmpserver.lua in line#-84 of rtmpserver.cpp.
- Assign a "mediaFolder" parameter in rtmpserver.lua
Start the server by starting the service using the 'startService' command.
Stop the server by stopping the service using the 'stopService' command.
If the service is not installed, start the server normally by running the application without any command parameter.

To uninstall the service, enter the 'uninstallService' command.

SOME TECHNICAL DETAILS:
- The application makes sure that when the service is installed, the user will not be able to start the server normally.
- To check if the service is installed, the application queries information about the service. If the query returns a value, the server will not be started normally.
- Running the application without any command assumes that the user is attempting to start the server in a normal way.
- svcconfig and svccontrol have other functions that can be useful during testing and in future implementations, hence a lot of codes remain intact.

NOTES:
- [as of commit in 08-17-2010] The config file is hard-coded in rtmpserver.cpp
- Currently, the rtmpserver logs is saved to C:\\RTMPServerlogs.txt.
- There are limited sample applications in the net that implements this model. 
- In most of the sample apps, they loop a "while()" during the service's RUNNING state, simulating a running thread (since SvcMain acts as a thread).