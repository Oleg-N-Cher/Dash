MODULE Timer; (** Java ME *)

IMPORT
  lang := java_lang;

CONST
  Mul* = 1000; Div* = 18.2;
  
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
