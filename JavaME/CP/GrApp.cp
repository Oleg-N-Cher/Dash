(* Universal graphic library for Oberon (GPCP) for J2ME.  *)
(* Copyright (c) 2012, Oleg N. Cher . *)
(* VEDAsoft Oberon Club - http://zx.oberon2.ru *)

(*============================================================================*)
(*                            CP.GrApp.GrApp.class                            *)
(*============================================================================*)
MODULE GrApp; (** non-portable *)
(* Thanks to Raydac (Igor A. Maznitsa) for consultations. *)

IMPORT
  lcdui := javax_microedition_lcdui,  (* J2ME display. *)
  midlet := javax_microedition_midlet, (* J2ME midlet. *)
  lang := java_lang,    (* Base Java language library. *)
  Config; (* Configuration module. Don't add it to JAR. *)

CONST
  KeyBufSize = 32; NoKey = 0X;

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

  Color* = INTEGER;
  Colors* = LONGINT;

  Key = CHAR;

VAR
  Main-: Midlet;
  Width-, Height-: INTEGER;
  display: lcdui.Display;
  form: lcdui.Form;
  command: lcdui.Command;
  canvas: lcdui.Canvas;
  keysAvailable-, keyIn, keyOut: INTEGER;
  keyBuf: ARRAY KeyBufSize OF Key;

(*============================================================================*)
(*                         CP.GrApp.GrApp_Screen.class                        *)
(*============================================================================*)
PROCEDURE (screen: Screen) paint* (g: lcdui.Graphics);
(** ��������� Runnable - ���������� ����� VM, ����� ����� ������������ �����. *)
BEGIN
  g.drawImage(screen.i, 0, 0, 20);
END paint;

(*----------------------------------------------------------------------------*)
PROCEDURE (screen: Screen) run* , NEW;
(** ������� ������� ����� ���������� � ����� ��� ���������� (� ����� BEGIN). *)
VAR
  mainClass: lang.Class;
BEGIN
  (* ������� ������ �� ������� ����� � �������� ���. *)
  mainClass := lang.Class.forName(Config.MainClass);
  (* ���� ����������� ����������� ���������� ����������: *)
  (* "Java application has thrown Exception and will be closed". *)
  (* RESCUE (classnotfoundexception); *)
END run;

PROCEDURE (screen: Screen) keyPressed* (keyCode: INTEGER);
BEGIN
  (* Add a key: *)
  keyBuf[keyIn] := CHR(keyCode);
  keyIn := (keyIn+1) MOD KeyBufSize;
  INC(keysAvailable);
END keyPressed;
    
(*============================================================================*)
(*                         CP.GrApp.GrApp_Midlet.class                        *)
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
(** ����� ����� - ���������� ����� JVM ��� ������� ������ ����������. *)
BEGIN
  (* ��������� ������ �� ������ Display ������ javax.microedition.lcdui
     ��� ������ � �������, ������� (javax.microedition.lcdui.Form) ��� ��
     ������� (javax.microedition.lcdui.Canvas) � ������������ ����� ����. *)
  display := lcdui.Display.getDisplay(midlet);
  command := NIL;
  form := lcdui.Form.Init(Config.AppTitle);
  
  IF midlet.screen = NIL THEN (* ���� ����� ��� �� ����������, ������� ���. *)
    NEW(midlet.screen);
    Width := midlet.screen.getWidth();
    Height := midlet.screen.getHeight();
    midlet.screen.i := lcdui.Image.createImage(Width, Height); (* ������� �����. *)
    midlet.screen.g := midlet.screen.i.getGraphics();
    display.setCurrent(midlet.screen);
    keysAvailable := 0; keyIn := 0; keyOut := 0;
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
  IF ~midlet.threadStarted THEN
    midlet.thread := lang.Thread.Init(midlet.screen);
    midlet.thread.start; (* �������� ���������� Runnable.run � ����� ������. *)
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
BEGIN
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
  display := NIL;
  form := NIL;
  command := NIL;
  canvas := NIL;
  Main.screen.i := NIL;
  Main.screen.g := NIL;
  Main.screen := NIL;
  Main.thread := NIL;
  Main := NIL;
  midlet.notifyDestroyed();
END destroyApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
  (*self.*)command := c;
END commandAction;
(*----------------------------------------------------------------------------*)

PROCEDURE Clear* (color: INTEGER); (* �������� ���� ����� ������ color. *)
VAR
  oldcolor: INTEGER;
BEGIN
  oldcolor := Main.screen.g.getColor();
  Main.screen.g.setColor(color);
  Main.screen.g.fillRect(0, 0, Width, Height);
  Main.screen.g.setColor(oldcolor);
END Clear;

PROCEDURE Redraw* ; (** ������������ ����� �� ������. *)
BEGIN
  Main.screen.repaint;
  Main.screen.serviceRepaints;
END Redraw;

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
  
PROCEDURE Close* ; (** ������� ����������. *)
BEGIN
  Main.destroyApp(TRUE)
END Close;

END GrApp.