MODULE GrScr;
(* Thanks to Raydac (Igor A. Maznitsa) for consultations. *)

IMPORT
  lcdui := javax_microedition_lcdui,
  midlet := javax_microedition_midlet,
  lang := java_lang,
  GrCfg;
  
TYPE
  Screen* = POINTER TO RECORD (lcdui.Canvas + lang.Runnable)
    g-: lcdui.Graphics;
    i: lcdui.Image;
  END;

  Midlet* = POINTER TO RECORD (midlet.MIDlet + lcdui.CommandListener)
    screen-: Screen;
    thread: lang.Thread;
    threadStarted: BOOLEAN;
  END;

VAR
  Main-: Midlet;
  display: lcdui.Display;
  form: lcdui.Form;
  command: lcdui.Command;
  canvas: lcdui.Canvas;

PROCEDURE (screen: Screen) paint* (g: lcdui.Graphics);
BEGIN
  g.drawImage(screen.i, 0, 0, 20);
END paint;

PROCEDURE (screen: Screen) run* , NEW;
VAR
  mainClass: lang.Class;
BEGIN
  (* Получим ссылку на главный класс и запустим его *)
  mainClass := lang.Class.forName(GrCfg.MainClass);
  (* Пока ограничимся стандартной обработкой исключения: *)
  (* "Java application has thrown Exception and will be closed". *)
  (* RESCUE (classnotfoundexception); *)
END run;

PROCEDURE Init* (): Midlet, BASE (); (* Midlet's CONSTRUCTOR *)
BEGIN
  Main := SELF;
  SELF.screen := NIL;
  SELF.threadStarted := FALSE;
  RETURN SELF;
END Init;

PROCEDURE (midlet: Midlet) startApp* ;
(** Точка входа - вызывается самой JVM при запуске нашего приложения. *)
BEGIN
  (* Получение ссылки на объект Display пакета javax.microedition.lcdui
     для работы с экраном, формами (javax.microedition.lcdui.Form) или же
     холстом (javax.microedition.lcdui.Canvas) и переключения между ними. *)
  display := lcdui.Display.getDisplay(midlet);
  command := NIL;
  form := lcdui.Form.Init(GrCfg.AppTitle);
  
  IF midlet.screen = NIL THEN (* Если экран ещё не существует, создать его. *)
    NEW(midlet.screen);
    midlet.screen.i := lcdui.Image.createImage(
      midlet.screen.getWidth(), midlet.screen.getHeight()); (* Теневой экран. *)
    midlet.screen.g := midlet.screen.i.getGraphics();
    display.setCurrent(midlet.screen);
  ELSE (* Экран существует в памяти; просто перерисуем его. *)
    midlet.screen.repaint;
    midlet.screen.serviceRepaints;
  END;
  (* Если процесс существует, он продолжится; иначе создадим его. *)
  IF ~midlet.threadStarted THEN
    midlet.thread := lang.Thread.Init(midlet.screen);
    midlet.thread.start;
    midlet.threadStarted := TRUE;
  END;
END startApp;

PROCEDURE (midlet: Midlet) pauseApp* ;
(** вызывается когда на наше устройство поступает звонок, SMS и т.д. *)
END pauseApp;

PROCEDURE (midlet: Midlet) destroyApp* (c: BOOLEAN);
(** вызывается тогда, когда он вызывается. *)
(* данный метод указан, как необходимый к реализации, в то время
   как практической пользы он не несёт. Методы startApp() и pauseApp()
   используются самой AMS, а destroyApp() - нет. По сложившейся традиции
   обычно блок данной функции выглядит следующим образом:

   public void destroyApp(boolean unconditional) {
     notifyDestroyed(); // данный метод закрывает наше приложение
   }

   Как вы можете видеть, прекращением цикла жизни приложений руководит
   функция notifyDestroyed(), а не destroyApp(). Ну да ладно,
   предназначение последней так и останется тайной... *)
END destroyApp;

PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
  (*self.*)command := c;
END commandAction;

(*
   g.setColor(0x000000);
     //Заливаем весь экран черным цветом
      g.fillRect(0, 0, getWidth(), getHeight());
      midlet.screenWidth=getWidth();
      midlet.screenHeight=getHeight();
     }
*)

PROCEDURE Repaint* ; (** Перерисовать экран из буфера. *)
BEGIN
  Main.screen.repaint;
  Main.screen.serviceRepaints;
END Repaint;

END GrScr.
