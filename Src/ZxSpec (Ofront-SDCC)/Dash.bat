@SET Bin=..\..\..\..\ZXDev\Bin
@SET Lib=..\..\..\..\ZXDev\Lib

@IF EXIST Obj\Dash.c MOVE Obj\Dash.c
%Bin%\sdcc -mz80 --code-loc 26000 --data-loc 0xF000 --no-std-crt0 --opt-code-size --funsigned-char --disable-warning 59 -I Obj -I %Lib% -L %Lib%\z80 Dash.c Rsrc.rel GrScr.rel GrTiles.rel
@IF errorlevel 1 PAUSE

%Bin%\Hex2bin Dash.ihx
@REM %Bin%\stripbin Dash.bin
%Bin%\bin2trd.exe Dash.trl
@MOVE Dash.TRD ..\Dash.trd
