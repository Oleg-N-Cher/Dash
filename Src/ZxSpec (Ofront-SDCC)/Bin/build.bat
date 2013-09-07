@SET CodeAddr=45056
@SET DataAddr=63488
@SET Bin=..\..\..\..\ZXDev\Bin
@SET Lib=..\..\..\..\ZXDev\Lib

@CD ..
@IF EXIST %1.c GOTO here

%Bin%\sdcc -c Obj\%1.c -mz80 --opt-code-size -I "." -I Obj -I %Lib%
@GOTO exit

:here
%Bin%\sdcc -c %1.c -mz80 --opt-code-size -I "." -I Obj -I %Lib%

:exit
@IF errorlevel 1 PAUSE

@IF EXIST Obj\Dash.c MOVE Obj\Dash.c
%Bin%\sdcc Dash.c -mz80 --code-loc %CodeAddr% --data-loc %DataAddr% --no-std-crt0 --opt-code-size -I "." -I Obj -I %Lib% Rsrc.rel GrScr.rel GrTiles.rel
@IF errorlevel 1 PAUSE

%Bin%\hex2bin Dash.ihx
%Bin%\bin2data.exe -rem -org %CodeAddr% Dash.bin ..\Dash.tap Dash
@START ..\Dash.tap