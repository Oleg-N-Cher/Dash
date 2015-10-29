@IF /%XDev%==/ GOTO noXDev

@%XDev%\Bin\upx ..\Dash.exe
@IF errorlevel 1 PAUSE
@GOTO exit

:noXDev

@ECHO Please set system variable XDev=X:\Path\To\XDev
@PAUSE

:exit
