@IF /%JROOT%==/ GOTO noGPCP

@SET WtkBin=d:\WIN32APP\Dev\WTK2.5.2_01\bin
@SET WtkLib=d:\WIN32APP\Dev\WTK2.5.2_01\lib
@SET JavaLib=d:\Program Files\Java\jre6\lib
@SET MidpLib="%JavaLib%\rt.jar";%WtkLib%\cldcapi10.jar;%WtkLib%\midpapi10.jar;%WtkLib%\mmapi.jar;%WtkLib%\jsr75.jar

@IF EXIST %1.h DEL %1.h
@IF EXIST %1.c DEL %1.c

@IF EXIST ..\CP\%1.cp GOTO modCP

:modOberon

@CALL ..\Bin\cprun gpcp ..\Mod\%1.Mod
@IF errorlevel 1 PAUSE

@IF EXIST CP\Config\Config.class DEL CP\Config\Config.class
@IF EXIST CP\Config RMDIR CP\Config
@IF EXIST CP\Msg\Msg.class DEL CP\Msg\Msg.class
@IF EXIST CP\Msg RMDIR CP\Msg

@GOTO exit

:modCP

@CALL ..\Bin\cprun gpcp ..\CP\%1.cp
@IF errorlevel 1 PAUSE
@GOTO exit

:noGPCP
@ECHO Please set system variable JROOT (for GPCP)
@PAUSE
@GOTO exit

:exit
