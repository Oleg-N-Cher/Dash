SET CodeAddr=65118

%XDev%\ZXDev\Bin\sdasz80.exe -o deexo_simple.rel deexo_simple.asm
%XDev%\ZXDev\Bin\sdcc.exe -mz80 --no-std-crt0 --code-loc %CodeAddr% deexo_simple.rel -o Exomizer.ihx
%XDev%\ZXDev\Bin\hex2bin.exe Exomizer.ihx

DEL /Q *.ihx *.map *.rel *.lk *.noi
