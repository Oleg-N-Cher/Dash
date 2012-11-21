MODULE GrScr;
IMPORT
  lcdui := javax_microedition_lcdui,
  midlet := javax_microedition_midlet,
  lang := java_lang,
  GrCfg;
  
TYPE
  Screen* = POINTER TO RECORD (lcdui.Canvas + lang.Runnable)
    g: lcdui.Graphics;
    i: lcdui.Image;
  END;

  Midlet* = POINTER TO RECORD (midlet.MIDlet + lcdui.CommandListener)
    screen: Screen;
    threadStarted: BOOLEAN;
  END;

VAR
  display: lcdui.Display;
  form: lcdui.Form;
  graphics*: lcdui.Graphics;
  command: lcdui.Command;
  canvas: lcdui.Canvas;

PROCEDURE (self: Screen) paint* (g: lcdui.Graphics);
BEGIN
  g.drawImage(self.i, 0, 0, 20);
END paint;

PROCEDURE (self: Screen) run* , NEW;
BEGIN
  self.g.setColor(99, 99, 99);
  self.g.fillRect(0, 0, 40, 39);
  self.g.setColor(255, 255, 255);
  self.g.fillRect(0, 16, 40, 40 - 16);
  self.repaint();
  self.serviceRepaints();
END run;

PROCEDURE Repaint* ;
BEGIN
  canvas.repaint;
  canvas.serviceRepaints;
END Repaint;

PROCEDURE Init* (): Midlet, BASE (); (* Midlet's CONSTRUCTOR *)
BEGIN
  SELF.screen := NIL;
  SELF.threadStarted := FALSE;
  RETURN SELF;
END Init;

PROCEDURE (midlet: Midlet) startApp* ;
(** Точка входа - вызывается самой JVM при запуске нашего приложения. *)
VAR
  MainObj: lang.Object;
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
    midlet.screen.run;
    (* Получим ссылку на главный модуль и запустим его *)
    MainObj := lang.Class.forName("CP.Dash.Dash").newInstance();
    IF MainObj # NIL THEN lang.Thread.sleep(15000); END;
    (* MainObj := Main.newInstance(); *)
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

END GrScr.
