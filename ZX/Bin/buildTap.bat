@SET Name=Dash
@SET CodeAddr=55497
@SET EntryPoint=65118
@SET Bin=%XDev%\ZXDev\Bin

CD ..\Obj
%Bin%\bin2tap -c 24999 -a %CodeAddr% -r %EntryPoint% -b -o ..\%Name%.tap %Name%.bin
PAUSE
