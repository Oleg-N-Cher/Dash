MODULE Rsrc; (** non-portable: J2ME *)
IMPORT
  lcdui := javax_microedition_lcdui;

TYPE
  Tile = lcdui.Image;

VAR
  None-, Grass-, Stone-, Almas-, StopMan-, Wall-, Mina-, Babo-,
  LeftMan-, LeftMan1-, RightMan-, RightMan1-, UpMan-, UpMan1-,
  DownMan-, DownMan1-, Mina1-, Babo1-, LastTile-: Tile;

BEGIN
  None      := lcdui.Image.createImage("/Rsrc/None.png");
  Grass     := lcdui.Image.createImage("/Rsrc/Grass.png");
  Stone     := lcdui.Image.createImage("/Rsrc/Stone.png");
  Almas     := lcdui.Image.createImage("/Rsrc/Almas.png");
  StopMan   := lcdui.Image.createImage("/Rsrc/StopMan.png");
  Wall      := lcdui.Image.createImage("/Rsrc/Wall.png");
  Mina      := lcdui.Image.createImage("/Rsrc/Mina.png");
  Babo      := lcdui.Image.createImage("/Rsrc/Babo.png");
  LeftMan   := lcdui.Image.createImage("/Rsrc/LeftMan.png");
  LeftMan1  := lcdui.Image.createImage("/Rsrc/LeftMan1.png");
  RightMan  := lcdui.Image.createImage("/Rsrc/RightMan.png");
  RightMan1 := lcdui.Image.createImage("/Rsrc/RightMan1.png");
  UpMan     := lcdui.Image.createImage("/Rsrc/UpMan.png");
  UpMan1    := lcdui.Image.createImage("/Rsrc/UpMan1.png");
  DownMan   := lcdui.Image.createImage("/Rsrc/DownMan.png");
  DownMan1  := lcdui.Image.createImage("/Rsrc/DownMan1.png");
  Mina1     := lcdui.Image.createImage("/Rsrc/Mina1.png");
  Babo1     := lcdui.Image.createImage("/Rsrc/Babo1.png");
  LastTile  := Babo1;
(*
    try {
      displayIm = Image.createImage("/num/display.PNG");
      tochki = Image.createImage("/num/tochki.PNG");
    } catch (IOException ex1) {
      System.out.println("Image not loading");
    }
*)

END Rsrc.

(* 1. Need to check integrity of the resource pack with exception if broken *)
