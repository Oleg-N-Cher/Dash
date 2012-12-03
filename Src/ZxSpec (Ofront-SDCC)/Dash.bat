@SET Bin=..\..\..\..\ZXDev\Bin
@SET Libs=..\..\..\..\ZXDev\Libs
@SET Obj=..\\..\..\..\Ofront\Obj

@IF EXIST %Obj%\Dash.c MOVE %Obj%\Dash.c
%Bin%\sdcc -mz80 --code-loc 26000 --data-loc 0xF000 --no-std-crt0 --opt-code-size --funsigned-char --disable-warning 59 -I %Libs% -L %Libs%\z80 Dash.c Rsrc.rel GrScr.rel GrTiles.rel
@PAUSE
%Bin%\Hex2bin Dash.ihx
@REM %Bin%\stripbin Dash.bin
%Bin%\bin2trd.exe Dash.trl
@MOVE Dash.TRD ..\Dash.trd
