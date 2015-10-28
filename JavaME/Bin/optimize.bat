@IF /%JROOT%==/ GOTO noGPCP

@SET Name=Dash
@SET WtkBin=d:\WIN32APP\Dev\WTK2.5.2_01\bin
@SET WtkLib=d:\WIN32APP\Dev\WTK2.5.2_01\lib
@SET JavaLib=c:\Program Files\Java\jre7\lib
@SET MidpLib="%JavaLib%\rt.jar";%WtkLib%\cldcapi10.jar;%WtkLib%\midpapi10.jar;%WtkLib%\mmapi.jar;%WtkLib%\jsr75.jar

@CD ..\Obj
@IF NOT EXIST ${build}\classes MKDIR ${build}\classes
@XCOPY /I /S /Y CP ${build}\classes\CP > NUL

::@SET PROGUARD_HOME=%CD%\..\Bin\proguard.jar
@SET CLASSPATH=%CD%\..\Bin
java -jar ..\Bin\proguard.jar @Dash.pro
@IF errorlevel 1 PAUSE

@%WtkBin%\preverify -classpath '%MidpLib%' -d "${build}\preverify" "%CD%\${build}\obfuscate" -cldc
@CALL ant
@IF errorlevel 1 PAUSE
@goto exit

:noGPCP
@ECHO Please set system variable JROOT (for GPCP)
@PAUSE

:exit
