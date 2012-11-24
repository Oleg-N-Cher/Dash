MODULE Rsrc; (** non-portable: J2ME *)
IMPORT
  lcdui := javax_microedition_lcdui,
  lang := java_lang,
  io := java_io,
  GrScr;

TYPE
  Resource = io.InputStream;
  Ident* = lang.String;
  Tile = lcdui.Image;

CONST
  Title* = "/Rsrc/Title.bin";
  TitleSize* = 114; (** Cells, a cell occupies 3 bytes. *)
  
VAR
  None-, Grass-, Stone-, Almas-, StopMan-, Wall-, Mina-, Babo-,
  LeftMan-, LeftMan1-, RightMan-, RightMan1-, UpMan-, UpMan1-,
  DownMan-, DownMan1-, Mina1-, Babo1-, LastTile-: Tile;

PROCEDURE Open* (IN name: Ident): Resource;
BEGIN
  RETURN GrScr.Main.getClass().getResourceAsStream(name);
  RESCUE (exception);
  RETURN NIL;
END Open;

PROCEDURE ReadByte* (rsrc: Resource): BYTE;
VAR
  abyte: ARRAY 2 OF BYTE;
BEGIN
  IF rsrc.read(abyte, 0, 1) = -1 THEN RETURN 0 END;
  RETURN abyte[0];
  RESCUE (exception);
  RETURN 0;
END ReadByte;

PROCEDURE Close* (rsrc: Resource);
BEGIN
  rsrc.close;
  RESCUE (exception);
END Close;

PROCEDURE GetTileByNum* (num: INTEGER): Tile;
BEGIN
  CASE num OF
  |  0: RETURN None;
  |  1: RETURN Grass;
  |  2: RETURN Stone;
  |  3: RETURN Almas;
  |  4: RETURN StopMan;
  |  5: RETURN Wall;
  |  6: RETURN Mina;
  |  7: RETURN Babo;
  |  8: RETURN LeftMan;
  |  9: RETURN LeftMan1;
  | 10: RETURN RightMan;
  | 11: RETURN RightMan1;
  | 12: RETURN UpMan;
  | 13: RETURN UpMan1;
  | 14: RETURN DownMan;
  | 15: RETURN DownMan1;
  | 16: RETURN Mina1;
  | 17: RETURN Babo1;
  END;
  RETURN NIL;
END GetTileByNum;

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

