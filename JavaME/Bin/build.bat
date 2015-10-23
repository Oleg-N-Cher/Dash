@IF /%JROOT%==/ GOTO noGPCP

@SET Name=Dash
@SET WtkBin=d:\WIN32APP\Dev\WTK2.5.2_01\bin
@SET WtkLib=d:\WIN32APP\Dev\WTK2.5.2_01\lib
@SET JavaLib=d:\Program Files\Java\jre6\lib
@SET MidpLib="%JavaLib%\rt.jar";%WtkLib%\cldcapi10.jar;%WtkLib%\midpapi10.jar;%WtkLib%\mmapi.jar;%WtkLib%\jsr75.jar

@IF %1==%Name% GOTO build
@CALL ..\Bin\compile %1

:build

@IF NOT EXIST ${build}\obfuscate MKDIR ${build}\obfuscate
@XCOPY /I /S /Y CP ${build}\obfuscate\CP > NUL

@%WtkBin%\preverify -classpath '%MidpLib%' -d "${build}\preverified" "%CD%\${build}\obfuscate" -cldc
@CALL ant
@IF errorlevel 1 PAUSE

goto exit

:noGPCP
@ECHO Please set system variable JROOT (for GPCP)
@PAUSE

:exit
