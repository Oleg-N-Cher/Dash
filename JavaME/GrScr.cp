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
  mainClass := lang.Class.forName(GrCfg.MainClass);
  (* ���� ����������� ����������� ���������� ����������: *)
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
(** ����� ����� - ���������� ����� JVM ��� ������� ������ ����������. *)
BEGIN
  (* ��������� ������ �� ������ Display ������ javax.microedition.lcdui
     ��� ������ � �������, ������� (javax.microedition.lcdui.Form) ��� ��
     ������� (javax.microedition.lcdui.Canvas) � ������������ ����� ����. *)
  display := lcdui.Display.getDisplay(midlet);
  command := NIL;
  form := lcdui.Form.Init(GrCfg.AppTitle);
  
  IF midlet.screen = NIL THEN (* ���� ����� ��� �� ����������, ������� ���. *)
    NEW(midlet.screen);
    midlet.screen.i := lcdui.Image.createImage(
      midlet.screen.getWidth(), midlet.screen.getHeight()); (* ������� �����. *)
    midlet.screen.g := midlet.screen.i.getGraphics();
    display.setCurrent(midlet.screen);
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
END destroyApp;

(*----------------------------------------------------------------------------*)
PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
  (*self.*)command := c;
END commandAction;
(*----------------------------------------------------------------------------*)

(*
   g.setColor(0x000000);
     //�������� ���� ����� ������ ������
      g.fillRect(0, 0, getWidth(), getHeight());
      midlet.screenWidth=getWidth();
      midlet.screenHeight=getHeight();
     }
*)

PROCEDURE Repaint* ; (** ������������ ����� �� ������. *)
BEGIN
  Main.screen.repaint;
  Main.screen.serviceRepaints;
END Repaint;

END GrScr.
