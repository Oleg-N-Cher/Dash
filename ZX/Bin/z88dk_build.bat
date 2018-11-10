@ECHO OFF
IF NOT "%ZCCCFG%"=="" GOTO z88dk
ECHO Please install z88dk and set up the system variable ZCCCFG
PAUSE
EXIT

:z88dk

SET Mod=Dash.c Labirint.c ..\C\Console.c ..\C\Control.c ..\C\GrApp.c ..\C\GrTiles.c ..\C\Rsrc.c ..\C\Sound.c ..\C\Strings.c ..\C\Timer.c
IF EXIST .debug SET Opt=--max-allocs-per-node 20
IF NOT EXIST .debug SET Opt=-SO3 --opt-code-size --max-allocs-per-node200000
SET Lib=%XDev%\ZXDev\Lib
SET CC=zcc +zx -vn -startup=31 -clib=sdcc_iy -create-app %Opt% -pragma-include:zpragma.inc -Cs"--disable-warning 59" -Cs"--disable-warning 85"

CD ..\Obj
IF EXIST Config.h DEL Config.h
IF EXIST Config.c DEL Config.c

%CC% %Mod% -oDash -I..\C -I..\Lib -I%Lib%\C -I%Lib%\Obj -L%Lib%
IF errorlevel 1 PAUSE
