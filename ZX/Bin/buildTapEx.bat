@SET Name=Dash
@IF "%CodeAddr%"=="" SET CodeAddr=55507
@SET EntryPoint=65118
@SET Bin=%XDev%\ZXDev\Bin

CD ..\Obj
%Bin%\bin2tap -c 24999 -a %CodeAddr% -r %EntryPoint% -b -o ..\Dash.tap ..\Dash.bin.exo
PAUSE
