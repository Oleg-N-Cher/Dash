@SET Bin=..\..\..\..\ZXDev\Bin
@SET Lib=..\..\..\..\ZXDev\Lib
%Bin%\sdcc -mz80 --no-std-crt0 --opt-code-speed --funsigned-char --disable-warning 59 --disable-warning 85 -I Obj -I %Lib% -L %Lib%\z80 GrTiles.c
