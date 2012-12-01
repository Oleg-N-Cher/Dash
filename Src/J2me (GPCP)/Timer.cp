MODULE Timer;

IMPORT
  lang := java_lang;

(*
private void sleep(){
      repaint();
      long startTime=System.currentTimeMillis();
      long timeTaken=System.currentTimeMillis()-startTime;
      while (timeTaken<50) {
         timeTaken=System.currentTimeMillis()-startTime;
      }
   }
*)

PROCEDURE Delay* (msec: INTEGER);
BEGIN
  lang.Thread.sleep(msec);
END Delay;

END Timer.
