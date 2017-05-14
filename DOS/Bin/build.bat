@IF /%XDev%==/ GOTO noXDev

@SET Name=Dash
@SET Modules=Rsrc.obj GrApp.obj GrTiles.obj Control.obj Timer.obj Sound.obj Labirint.obj
::Rsrc.rel GrApp.rel GrTiles.rel Control.rel Timer.rel Sound.rel Console.rel Strings.rel Labirint.rel

@SET path=%XDev%\DosDev\BIN\TC201\BIN

@IF %1==%Name% GOTO build
@CALL ..\Bin\compile %1

:build
:: -S
tcc %Name%.c DOSDEV.LIB %Modules%
@IF errorlevel 1 PAUSE
@IF EXIST %Name%.exe MOVE %Name%.exe ..\%Name%.exe
@GOTO exit

:noXDev

@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
::..\%Name%.exe
