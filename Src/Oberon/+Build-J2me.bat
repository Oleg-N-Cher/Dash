@SET WtkBin=d:\WIN32APP\Dev\WTK2.5.2_01\bin
@SET WtkLib=d:\WIN32APP\Dev\WTK2.5.2_01\lib
@SET JavaLib=d:\Program Files\Java\jre6\lib
@SET MidpLib="%JavaLib%\rt.jar";%WtkLib%\cldcapi10.jar;%WtkLib%\midpapi10.jar;%WtkLib%\mmapi.jar;%WtkLib%\jsr75.jar

@CD "J2me (GPCP)"
@DEL *.cps

@IF NOT EXIST GrCfg.cps   CALL Bin\cprun gpcp GrCfg.cp
@IF NOT EXIST Timer.cps   CALL Bin\cprun gpcp Timer.cp
@IF NOT EXIST GrScr.cps   CALL Bin\cprun gpcp GrScr.cp
@IF NOT EXIST Rsrc.cps    CALL Bin\cprun gpcp Rsrc.cp
@IF NOT EXIST GrTiles.cps CALL Bin\cprun gpcp GrTiles.cp
@IF NOT EXIST Dash.cps    CALL Bin\cprun gpcp Dash.cp

@IF NOT EXIST ${build}\obfuscate MKDIR ${build}\obfuscate
@XCOPY /I /S /Y CP ${build}\obfuscate\CP > NUL

@%WtkBin%\preverify -classpath '%MidpLib%' -d "${build}\preverified" "%CD%\${build}\obfuscate" -cldc
@CALL ant

@CD..
@PAUSE