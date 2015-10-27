MODULE Timer; (** Java ME *)

IMPORT
  lang := java_lang;

CONST
  TickMs = 10000 DIV 182;
  
VAR
  wantedTime: LONGINT;

PROCEDURE Start* (ticks: INTEGER);
BEGIN
  wantedTime := lang.System.currentTimeMillis() + ticks*TickMs;
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
