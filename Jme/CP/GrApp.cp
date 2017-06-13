(*  Graphic library in Oberon-2 (GPCP) for Java ME *)
(*      Copyright (c) 2012-2015, Oleg N. Cher      *)
(*   VEDAsoft Oberon Club - http://zx.oberon2.ru   *)
(*  Thanks to Igor A. Maznitsa (Raydac) for help   *)

(*============================================================================*)
(*                            CP.GrApp.GrApp.class                            *)
(*============================================================================*)
MODULE GrApp; (** non-portable *)

IMPORT
  lcdui := javax_microedition_lcdui,   (* J2ME display. *)
  midlet := javax_microedition_midlet,  (* J2ME midlet. *)
  lang := java_lang,     (* Base Java language library. *)
  Config; (* Configuration module. Don't add it to JAR. *)

CONST
  KeyBufSize = 32; NoKey = 0X;
  Black = 000000H;
  WTKfix = TRUE;

TYPE
  Midlet* = POINTER TO RECORD (midlet.MIDlet + lcdui.CommandListener)
    screen-: Screen;
    display-: lcdui.Display;
    threadStarted: BOOLEAN;
  END;
  
  Screen* = POINTER TO RECORD (lcdui.Canvas + lang.Runnable)
    Width-, Height-: INTEGER;
    gfx-: lcdui.Graphics;
    midlet-: Midlet;
    exit: lcdui.Command;
  END;

  Coords* = SHORTINT; Color* = INTEGER;
  Key = CHAR;

VAR
  screen-: Screen;
  img: lcdui.Image;
  keysAvailable-, keyIn, keyOut: INTEGER;
  keyBuf: ARRAY KeyBufSize OF Key;

(*============================================================================*)
(*                         CP.GrApp.GrApp_Midlet.class                        *)
(*============================================================================*)
PROCEDURE Init* (): Midlet, BASE (); (* Midlet's CONSTRUCTOR *)
BEGIN
  SELF.screen := NIL;
  SELF.threadStarted := FALSE;
  RETURN SELF
END Init;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) startApp* ;
(** Точка входа - вызывается самой JVM при запуске нашего приложения. *)
BEGIN
  (* Получение ссылки на объект Display пакета javax.microedition.lcdui
     для работы с экраном, формами (javax.microedition.lcdui.Form) или же
     холстом (javax.microedition.lcdui.Canvas) и переключения между ними. *)
  midlet.display := lcdui.Display.getDisplay(midlet);

  IF midlet.screen = NIL THEN (* Если экран ещё не существует, создать его. *)
    NEW(midlet.screen);
    midlet.screen.midlet := midlet;
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
  IF ~midlet.threadStarted THEN (* Запустим наследника Runnable.run *)
    lang.Thread.Init(midlet.screen).start; (* в новом потоке. *)
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

(*
    public void destroyApp(boolean flag)
    {
        m = null;
        M.I = null; // lcdui.Image
        M.G = null; // lcdui.Graphics
        CD = null;
        TB = null;
        F = null;
        A = null;
        L = null;
        fw = null;
        LC = null;
        notifyDestroyed();
    }
*)
END destroyApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
(*  display := NIL;
  exit := NIL;*)(*
  screen.g := NIL;
  screen.i := NIL;
  screen := NIL;*)
  (*thread := NIL;*)
  midlet.notifyDestroyed()
END commandAction;

(*============================================================================*)
(*                         CP.GrApp.GrApp_Screen.class                        *)
(*============================================================================*)
PROCEDURE (screen: Screen) paint* (graphics: lcdui.Graphics);
(** Наследник Runnable - вызывается самой VM, когда нужно перерисовать экран. *)
BEGIN
  graphics.drawImage(img, 0, 0, 20);
END paint;

(*----------------------------------------------------------------------------*)
PROCEDURE (self: Screen) run* , NEW;
(** Находим главный класс приложения и отдаём ему управление (с точки BEGIN). *)
BEGIN
  IF screen = NIL THEN
    screen := self;
    IF img = NIL THEN (* Экрана в памяти ещё нет, создаём его первый раз *)
      screen.Width := screen.getWidth();
      screen.Height := screen.getHeight();
      (* Fix for models ~ 128x127 *)
      WHILE screen.Height MOD 16 # 0 DO INC(screen.Height) END;
      img := lcdui.Image.createImage(screen.Width, screen.Height);
      screen.gfx := img.getGraphics();
      screen.gfx.setColor(Black);
      screen.gfx.fillRect(0, 0, screen.Width, screen.Height);
    END;
  ELSE
    screen.midlet := self.midlet;
    IF img = NIL THEN
      screen.midlet.display := lcdui.Display.getDisplay(screen.midlet);
      screen.Width := screen.getWidth();
      screen.Height := screen.getHeight();
      (* Fix for models ~ 128x127 *)
      WHILE screen.Height MOD 16 # 0 DO INC(screen.Height) END;

      img := lcdui.Image.createImage(screen.Width, screen.Height);
      IF screen.gfx = NIL THEN screen.gfx := img.getGraphics() END;
      screen.midlet.display.setCurrent(screen);
    END;
  END;
  
  IF screen.gfx = NIL THEN screen.gfx := img.getGraphics() END;
  screen.exit := lcdui.Command.Init("Exit", 7, 0);
  screen.addCommand(screen.exit);
  screen.setCommandListener(screen.midlet);
  screen.midlet.display.setCurrent(screen);
  keysAvailable := 0; keyIn := 0; keyOut := 0;
  (* Получим ссылку на главный класс и запустим его. *)
  IF lang.Class.forName(Config.MainClass) # NIL THEN END;
  (*screen.midlet.notifyDestroyed();*)
  (* Не ограничимся стандартной обработкой исключения: *)
  (* "Java application has thrown Exception and will be closed". *)

RESCUE(exception);
  screen.midlet.notifyDestroyed()
END run;

PROCEDURE (screen: Screen) keyPressed* (keyCode: INTEGER);
BEGIN
  (* Add a key: *)
  keyBuf[keyIn] := CHR(keyCode);
  keyIn := (keyIn+1) MOD KeyBufSize;
  INC(keysAvailable);
END keyPressed;

(*----------------------------------------------------------------------------*)
PROCEDURE ReadKey* (): Key; (** Читать код клавиши из буфера. *)
VAR
  key: Key;
BEGIN
  IF keysAvailable = 0 THEN RETURN NoKey END;
  DEC(keysAvailable);
  key := keyBuf[keyOut];
  keyOut := (keyOut+1) MOD KeyBufSize;
  RETURN key
END ReadKey;

PROCEDURE Cls* ;
VAR
  oldcolor: INTEGER;
BEGIN
  oldcolor := screen.gfx.getColor();
  screen.gfx.setColor(Black);
  screen.gfx.fillRect(0, 0, screen.Width, screen.Height);
  screen.gfx.setColor(oldcolor);
END Cls;

PROCEDURE Redraw* ; (** Перерисовать экран из буфера. *)
BEGIN
  screen.repaint;
  screen.serviceRepaints;
END Redraw;

PROCEDURE ScrollDown* (lines: (* UNSIGNED *) INTEGER);
VAR
  newscr: lcdui.Image; gfx: lcdui.Graphics;
BEGIN
  newscr := lcdui.Image.createImage(screen.Width, screen.Height);
  gfx := newscr.getGraphics();
  gfx.setColor(Black); gfx.fillRect(0, 0, screen.Width, lines);
  gfx.drawImage(img, 0, lines, 20);
  img := newscr; screen.gfx := gfx;
END ScrollDown;

PROCEDURE ScrollUp* (lines: (* UNSIGNED *) INTEGER);
VAR
  newscr: lcdui.Image; gfx: lcdui.Graphics;
BEGIN
  newscr := lcdui.Image.createImage(screen.Width, screen.Height);
  gfx := newscr.getGraphics();
  gfx.setColor(Black); gfx.fillRect(0, screen.Height-lines, screen.Width, lines);
  gfx.drawImage(img, 0, -lines, 20);
  img := newscr; screen.gfx := gfx;
END ScrollUp;

PROCEDURE Close* ; (** Закрыть приложение. *)
VAR
  midlet: Midlet;
BEGIN
  midlet := screen.midlet;
  IF ~WTKfix THEN
    screen.midlet.screen := NIL;
    screen.midlet := NIL;
    screen.gfx := NIL;
    screen := NIL;
    img := NIL;
  END;
  midlet.notifyDestroyed()
END Close;

END GrApp.
