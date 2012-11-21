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
(** ����� ����� - ���������� ����� JVM ��� ������� ������ ����������. *)
VAR
  MainObj: lang.Object;
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
  (* ���� ������� ����������, �� �����������; ����� �������� ���. *)
  IF ~midlet.threadStarted THEN
    midlet.screen.run;
    (* ������� ������ �� ������� ������ � �������� ��� *)
    MainObj := lang.Class.forName("CP.Dash.Dash").newInstance();
    IF MainObj # NIL THEN lang.Thread.sleep(15000); END;
    (* MainObj := Main.newInstance(); *)
    midlet.threadStarted := TRUE;
  END;
END startApp;

PROCEDURE (midlet: Midlet) pauseApp* ;
(** ���������� ����� �� ���� ���������� ��������� ������, SMS � �.�. *)
END pauseApp;

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

PROCEDURE (midlet: Midlet) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
BEGIN
  (*self.*)command := c;
END commandAction;

(*
   g.setColor(0x000000);
     //�������� ���� ����� ������ ������
      g.fillRect(0, 0, getWidth(), getHeight());
      midlet.screenWidth=getWidth();
      midlet.screenHeight=getHeight();
     }
*)

END GrScr.
