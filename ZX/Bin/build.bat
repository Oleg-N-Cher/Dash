@IF /%XDev%==/ GOTO noxdev

@SET Name=Dash
@SET Modules=Rsrc.rel GrApp.rel GrTiles.rel Control.rel Timer.rel Sound.rel Console.rel Strings.rel Labirint.rel
@SET CodeAddr=32832
@SET DataAddr=56488
@SET Bin=%XDev%\ZXDev\Bin
@SET Lib=%XDev%\ZXDev\Lib

@IF %1==%Name% GOTO build
@SET Name=TestTiles
@IF %1==%Name% GOTO build
@SET Name=Dash
@CALL ..\Bin\compile %1

:build

%Bin%\sdcc %Name%.c -mz80 --code-loc %CodeAddr% --data-loc %DataAddr% --opt-code-speed --reserve-regs-iy --disable-warning 59 --disable-warning 84 --disable-warning 85 -I ..\C -I %Lib%\C -I "." -L %Lib% %Modules%
@IF errorlevel 1 PAUSE

%Bin%\hex2bin %Name%.ihx
%Bin%\stripbin %Name%.bin
%Bin%\bin2tap -c 24999 -a %CodeAddr% -r %CodeAddr% -b -o ..\%Name%.tap %Name%.bin
@START ..\%Name%.tap
@GOTO exit

:noxdev
@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
