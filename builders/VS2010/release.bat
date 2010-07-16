
FOR /F "tokens=*" %%G IN ('DIR /B /S Release\*.pdb') DO echo "%%G" && del /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /S Release\*.lib') DO echo "%%G" && del /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /S Release\*.exp') DO echo "%%G" && del /Q "%%G"
xcopy /E /I /Y ..\..\sources\applications\flvplayback\flex\bin-debug Release\applications\flvplayback\flex
xcopy /E /I /Y ..\..\sources\applications\mpegtsplayback\flex\bin-debug Release\applications\mpegtsplayback\flex
xcopy /E /I /Y Release %TEMP%\rtmpd
del rtmpd.zip
..\..\3rdparty\7zip\7za.exe a rtmpd.zip %TEMP%\rtmpd
rd /Q /S %TEMP%\rtmpd
