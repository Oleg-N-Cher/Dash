@SET Name=Dash
@SET CodeAddr=32832
@SET Bin=%XDev%\ZXDev\Bin

CD ..\Obj
%Bin%\bin2tap -c 24999 -a %CodeAddr% -r %CodeAddr% -b -o ..\%Name%.tap %Name%.bin
pause