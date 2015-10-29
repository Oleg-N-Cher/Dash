MODULE Timer; (** Java ME *)

IMPORT
  lang := java_lang;

CONST
  Mul* = 10000; Div* = 182;
  
VAR
  wantedTime: LONGINT;

PROCEDURE Start* (ms: INTEGER);
BEGIN
  wantedTime := lang.System.currentTimeMillis() + ms;
END Start;

PROCEDURE Until* ;
VAR
  sleepTime: LONGINT;
BEGIN
  sleepTime := wantedTime - lang.System.currentTimeMillis();
  IF sleepTime > 0 THEN lang.Thread.sleep(sleepTime) END;
RESCUE(
  interruptedException)
END Until;

END Timer.
