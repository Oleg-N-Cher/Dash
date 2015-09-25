@IF /%XDev%==/ GOTO noxdev

@SET Name=Dash
@SET CodeAddr=32768
@SET DataAddr=63488
@SET Bin=%XDev%\ZXDev\Bin
@SET Lib=%XDev%\ZXDev\Lib

@IF /%1==%Name%/ GOTO build
@CALL ..\Bin\compile %1

:build

%Bin%\sdcc %Name%.c -mz80 --code-loc %CodeAddr% --data-loc %DataAddr% --opt-code-size --disable-warning 59 --disable-warning 85 -I ..\C -I %Lib%\C -I "." -L %Lib% Rsrc.rel GrApp.rel GrTiles.rel Timer.rel
@IF errorlevel 1 PAUSE

%Bin%\hex2bin Dash.ihx
%Bin%\bin2tap -c 24999 -a %CodeAddr% -r %CodeAddr% -b -o ..\%Name%.tap %Name%.bin
@START ..\%Name%.tap
@GOTO exit

:noxdev
@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
