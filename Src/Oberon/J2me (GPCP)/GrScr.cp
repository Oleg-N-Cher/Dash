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
(** ����� ����� - ���������� ����� JVM ��� ������� ������ ����������. *)
VAR
  Main: lang.Class; MainObj: lang.Object;
BEGIN
  (* ��������� ������ �� ������ Display ������ javax.microedition.lcdui
     ��� ������ � �������, ������� (javax.microedition.lcdui.Form) ��� ��
     ������� (javax.microedition.lcdui.Canvas) � ������������ ����� ����. *)
  display := lcdui.Display.getDisplay(self);
  command := NIL;
  form := lcdui.Form.Init(GrCfg.AppTitle);
  
  (* ������� ������ �� ������� ������ � �������� ��� *)
  Main := lang.Class.forName("CP.Dash");
  MainObj := Main.newInstance();
END startApp;

PROCEDURE (self: Main) pauseApp* ;
(** ���������� ����� �� ���� ���������� ��������� ������, SMS � �.�. *)
END pauseApp;

PROCEDURE (self: Main) destroyApp* (c: BOOLEAN);
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

PROCEDURE (self: Main) commandAction* (c: lcdui.Command; d: lcdui.Displayable), NEW;
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
