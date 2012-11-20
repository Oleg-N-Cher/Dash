MODULE GrScr;
IMPORT
  lcdui := javax_microedition_lcdui,
  midlet := javax_microedition_midlet,
  lang := java_lang,
  GrCfg;
  
TYPE
  Main* = POINTER TO RECORD (midlet.MIDlet + lcdui.CommandListener)
  END;
  
VAR
  display: lcdui.Display;
  form: lcdui.Form;
  graphics*: lcdui.Graphics;
  command: lcdui.Command;
  
PROCEDURE (self: Main) startApp* ;
(** Точка входа - вызывается самой JVM при запуске нашего приложения. *)
VAR
  Main: lang.Class; MainObj: lang.Object;
BEGIN
  (* Получение ссылки на объект Display пакета javax.microedition.lcdui
     для работы с экраном, формами (javax.microedition.lcdui.Form) или же
     холстом (javax.microedition.lcdui.Canvas) и переключения между ними. *)
  display := lcdui.Display.getDisplay(self);
  command := NIL;
  form := lcdui.Form.Init(GrCfg.AppTitle);
  
  (* Получим ссылку на главный модуль и запустим его *)
  Main := lang.Class.forName("CP.Dash");
  MainObj := Main.newInstance();
END startApp;

PROCEDURE (self: Main) pauseApp* ;
(** вызывается когда на наше устройство поступает звонок, SMS и т.д. *)
END pauseApp;

PROCEDURE (self: Main) destroyApp* (c: BOOLEAN);
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

PROCEDURE (self: Main) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
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
