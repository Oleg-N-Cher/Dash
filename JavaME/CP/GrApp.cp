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
(** ����� ����� - ���������� ����� JVM ��� ������� ������ ����������. *)
BEGIN
  (* ��������� ������ �� ������ Display ������ javax.microedition.lcdui
     ��� ������ � �������, ������� (javax.microedition.lcdui.Form) ��� ��
     ������� (javax.microedition.lcdui.Canvas) � ������������ ����� ����. *)
  midlet.display := lcdui.Display.getDisplay(midlet);

  IF midlet.screen = NIL THEN (* ���� ����� ��� �� ����������, ������� ���. *)
    NEW(midlet.screen);
    midlet.screen.midlet := midlet;
  ELSE (* ����� ���������� � ������; ������ ���������� ���. *)
    midlet.screen.repaint;
    midlet.screen.serviceRepaints;
  END;
  (* ��������! startApp ������ ����������� ����������� ������, ��� ���
     ��� ��������� ����������� �� ���������� ����������� ������ �����������,
     ��������� ������� ���� ���������� ����� ������, ��������,
     ���� 20 ������ ����� ������, �� �����, ��� �������, � �����.
     ������ � ���� ����� �� ������ ��������� ���������� � ������ �����. *)

  (* ���� ����� ����������, �� ��������� ���� ������; ����� �������� ���. *)
  IF ~midlet.threadStarted THEN (* �������� ���������� Runnable.run *)
    lang.Thread.Init(midlet.screen).start; (* � ����� ������. *)
    midlet.threadStarted := TRUE;
  END;
END startApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) pauseApp* ;
(** ���������� ����� VM ����� �� ���� ���������� ��������� ������, SMS � �.�. *)
END pauseApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) destroyApp* (c: BOOLEAN);
(** ���������� �����, ����� �� ����������. *)
(* ������ ����� ������, ��� ����������� � ����������, � �� �����
   ��� ������������ ������ �� �� ����. ������ startApp() � pauseApp()
   ������������ ����� AMS, � destroyApp() - ���. �� ����������� ��������
   ������ ���� ������ ������� �������� ��������� �������:

   public void destroyApp(boolean unconditional) {
     notifyDestroyed(); // ������ ����� ��������� ���� ����������
   }

   ��� �� ������ ������, ������������ ����� ����� ���������� ���������
   ������� notifyDestroyed(), � �� destroyApp(). �� �� �����,
   �������������� ��������� ��� � ��������� ������... *)

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
(** ��������� Runnable - ���������� ����� VM, ����� ����� ������������ �����. *)
BEGIN
  graphics.drawImage(img, 0, 0, 20);
END paint;

(*----------------------------------------------------------------------------*)
PROCEDURE (self: Screen) run* , NEW;
(** ������� ������� ����� ���������� � ����� ��� ���������� (� ����� BEGIN). *)
BEGIN
  IF screen = NIL THEN
    screen := self;
    IF img = NIL THEN (* ������ � ������ ��� ���, ������ ��� ������ ��� *)
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
  (* ������� ������ �� ������� ����� � �������� ���. *)
  IF lang.Class.forName(Config.MainClass) # NIL THEN END;
  (*screen.midlet.notifyDestroyed();*)
  (* �� ����������� ����������� ���������� ����������: *)
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
PROCEDURE ReadKey* (): Key; (** ������ ��� ������� �� ������. *)
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

PROCEDURE Redraw* ; (** ������������ ����� �� ������. *)
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

PROCEDURE Close* ; (** ������� ����������. *)
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
