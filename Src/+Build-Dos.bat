@CD "MsDos (Oberon-M)"

@REM ========== Standard libraries ==========
IF NOT EXIST CmdLine.obj COPY /Y /B ..\..\..\..\..\DosDev\Libs\CMDLINE.OBJ
IF NOT EXIST CmdLine.dfn COPY /Y /B ..\..\..\..\..\DosDev\Libs\CMDLINE.DFN
IF NOT EXIST CmdLine.ref COPY /Y /B ..\..\..\..\..\DosDev\Libs\CMDLINE.REF

@REM ============ Dash libraries ============
IF NOT EXIST Config.obj CALL Config.bat
IF NOT EXIST Rsrc.obj CALL Rsrc.bat

IF EXIST ..\Dash.mod move "..\Dash.mod"
..\..\..\..\..\DosDev\Bin\OC\oc Dash def
..\..\..\..\..\DosDev\Bin\OC\xdef Dash
@CD..
@PAUSE