@SET Bin=..\..\..\..\ZXDev\Bin
@SET Libs=..\..\..\..\ZXDev\Libs
%Bin%\sdcc -mz80 --no-std-crt0 --opt-code-size --disable-warning 59 --disable-warning 85 -I %Libs% -L %Libs%\z80 Rsrc.c
