@SET Bin=..\..\..\..\ZXDev\Bin
@SET Lib=..\..\..\..\ZXDev\Lib

@IF EXIST ..\%1.c GOTO levelup

..\%Bin%\sdcc -c %1.c -mz80 --opt-code-size -I "." -I ..\%Lib%
@GOTO exit

:levelup
@CD ..
%Bin%\sdcc -c %1.c -mz80 --opt-code-size -I ".." -I Obj -I %Lib%

:exit
@IF errorlevel 1 PAUSE
