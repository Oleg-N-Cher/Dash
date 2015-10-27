@IF /%XDev%==/ GOTO noXDev

@SET Main=Dash
@SET path=%XDev%\DosDev\BIN\TC201\BIN

@IF EXIST ..\C\%1.h GOTO modC

:modOberon

tcc -I%XDev%\DosDev\Lib\C -c %1.c
@IF errorlevel 1 PAUSE
@IF %1==%Main% GOTO exit
@IF EXIST %1.c DEL %1.c
@GOTO exit

:modC

@DEL %1.h
@DEL %1.c
@IF NOT EXIST ..\C\%1.c GOTO exit
tcc -I%XDev%\DosDev\Lib\C -c ..\C\%1.c
@IF errorlevel 1 PAUSE
@GOTO exit

:nonoXDev
@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
