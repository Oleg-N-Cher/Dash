@IF /%XDev%==/ GOTO noxdev

@SET Bin=%XDev%\ZXDev\Bin
@SET Lib=%XDev%\ZXDev\Lib
@SET Main=Dash
@SET sdcc=%Bin%\sdcc --disable-warning 59 --disable-warning 85

@IF EXIST ..\C\%1.h GOTO modC

:modOberon

%sdcc% -c %1.c -mz80 --opt-code-size -I ..\C -I "." -I %Lib%\C -I %Lib%
@IF errorlevel 1 PAUSE
@IF %1==%Main% GOTO exit
@IF EXIST %1.c DEL %1.c
@GOTO exit

:modC

@DEL %1.h
@DEL %1.c
@IF NOT EXIST ..\C\%1.c GOTO exit
%sdcc% -c ..\C\%1.c -mz80 --opt-code-size -I ..\C -I "." -I %Lib%\C -I %Lib%
@IF errorlevel 1 PAUSE
@GOTO exit

:noxdev
@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
