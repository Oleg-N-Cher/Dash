(* Universal graphic library for Oberon (GPCP) for J2ME.  *)
(* Copyright (c) 2012, Oleg N. Cher . *)
(* VEDAsoft Oberon Club - http://zx.oberon2.ru *)

(*============================================================================*)
(*                            CP.GrScr.GrScr.class                            *)
(*============================================================================*)
MODULE GrScr; (** non-portable *)
(* Thanks to Raydac (Igor A. Maznitsa) for consultations. *)

IMPORT
  lcdui := javax_microedition_lcdui,  (* J2ME display. *)
  midlet := javax_microedition_midlet, (* J2ME midlet. *)
  lang := java_lang,    (* Base Java language library. *)
  GrCfg; (* Configuration module. Don't add it to JAR. *)
  
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

(*============================================================================*)
(*                         CP.GrScr.GrScr_Screen.class                        *)
(*============================================================================*)
PROCEDURE (screen: Screen) paint* (g: lcdui.Graphics);
(** Наследник Runnable - вызывается самой VM, когда нужно перерисовать экран. *)
BEGIN
  g.drawImage(screen.i, 0, 0, 20);
END paint;

(*----------------------------------------------------------------------------*)
PROCEDURE (screen: Screen) run* , NEW;
(** Находим главный класс приложения и отдаём ему управление (с точки BEGIN). *)
VAR
  mainClass: lang.Class;
BEGIN
  (* Получим ссылку на главный класс и запустим его. *)
  mainClass := lang.Class.forName(GrCfg.MainClass);
  (* Пока ограничимся стандартной обработкой исключения: *)
  (* "Java application has thrown Exception and will be closed". *)
  (* RESCUE (classnotfoundexception); *)
END run;

(*============================================================================*)
(*                         CP.GrScr.GrScr_Midlet.class                        *)
(*============================================================================*)
PROCEDURE Init* (): Midlet, BASE (); (* Midlet's CONSTRUCTOR *)
BEGIN
  Main := SELF;
  SELF.screen := NIL;
  SELF.threadStarted := FALSE;
  RETURN SELF;
END Init;

(*----------------------------------------------------------------------------*)
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
  (* ВНИМАНИЕ! startApp должен выполниться максимально быстро, так как
     эта процедура запускается во внутреннем управляющем потоке операционки,
     некоторые девайсы даже лимитируют время вызова, например,
     если 20 секунд будет висеть, то решат, что зависло, и убьют.
     Именно с этой целью мы желаем перевести исполнение в другой поток. *)

  (* Если поток существует, он продолжит свою работу; иначе создадим его. *)
  IF ~midlet.threadStarted THEN
    midlet.thread := lang.Thread.Init(midlet.screen);
    midlet.thread.start; (* Запустим наследника Runnable.run в новом потоке. *)
    midlet.threadStarted := TRUE;
  END;
END startApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) pauseApp* ;
(** Вызывается самой VM когда на наше устройство поступает звонок, SMS и т.д. *)
END pauseApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) destroyApp* (c: BOOLEAN);
(** Вызывается тогда, когда он вызывается. *)
(* Данный метод указан, как необходимый к реализации, в то время
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

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
  (*self.*)command := c;
END commandAction;
(*----------------------------------------------------------------------------*)

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
