@ECHO OFF
IF NOT "%XDev%"=="" GOTO XDev
ECHO Please set system variable XDev=X:\Path\To\XDev
PAUSE
EXIT

:XDev
SET Modules=..\C\Rsrc.c ..\C\GrApp.c ..\C\GrTiles.c ..\C\Control.c ..\C\Timer.c ..\C\Sound.c ..\C\Console.c ..\C\Strings.c ..\C\Labirint.c ..\C\Dash.c
SET Opt=--max-allocs-per-node200000
SET Opt=--oldralloc
SET ZCCCFG=%XDev%\ZXDev\Bin\z88dk\lib\config
SET PATH=%XDev%\ZXDev\Bin\z88dk\bin
SET Lib=%XDev%\ZXDev\Lib
CD ..\Obj

zcc +zx -vn -startup=31 -clib=sdcc_iy -oDash -create-app -compiler=sdcc -SO3 %Opt% -pragma-include:zpragma.inc -I..\C -I%Lib%\C -I%Lib%\Obj %Modules%
IF errorlevel 1 PAUSE
