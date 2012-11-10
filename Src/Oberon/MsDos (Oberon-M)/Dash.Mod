MODULE Dash; (* portable *)
IMPORT
  Cfg := Config, Rsrc,
  CmdLine;


BEGIN


  IF ~Cfg.PlrNameInCmdLine OR (CmdLine.ParamCount = 0) THEN
    Title;
END Dash.

Compile:
DevCompiler.CompileThis
DashSettings DashLowlevel DashResource DashKeyboard
DashTimer DashSound DashGrax DashLabirint DashDash 
Link:
DevLinker.LinkExe Dash.exe :=
Lowlevel Resource Settings Keyboard Timer Sound Grax Labirint
Dash$ 1 "Dash\Rsrc\dash.res" 2 Applogo.ico 