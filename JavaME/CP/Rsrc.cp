MODULE Rsrc; (** non-portable: J2ME *)
IMPORT
  lcdui := javax_microedition_lcdui,
  lang := java_lang,
  io := java_io,
  GrApp;

CONST
  Title*  = "/Rsrc/Title.bin";
  TitleSize* = 114; (** Number of cells, a cell occupies 3 bytes. Must be > 0 *)
  Levels* = "/Rsrc/Levels.bin";
  LevelSize* =  96; (* bytes *)
  MaxTileNum =  17;

TYPE
  NATINT* = INTEGER; (* Native integer *)
  Resource* = io.InputStream;
  Ident* = lang.String;
  Tile = lcdui.Image;

VAR
  None-, Grass-, Stone-, Almas-, StopMan-, Wall-, Mina-, Babo-,
  LeftMan-, LeftMan1-, RightMan-, RightMan1-, UpMan-, UpMan1-,
  DownMan-, DownMan1-, Mina1-, Babo1-, LastTile-: Tile;

  tileByNum: ARRAY MaxTileNum + 1 OF Tile; (* For returning the tiles by a number. *)

(*============================================================================*)
(*                            Work with resources                             *)
(*============================================================================*)
PROCEDURE Open* (IN name: Ident): Resource;
BEGIN
  RETURN GrApp.Main.getClass().getResourceAsStream(name);
RESCUE (exception);
  RETURN NIL
END Open;

PROCEDURE OpenAt* (pos: INTEGER; IN name: Ident): Resource;
VAR
  rsrc: Resource;
BEGIN
  rsrc := GrApp.Main.getClass().getResourceAsStream(name);
  IF rsrc.skip(pos) = pos THEN RETURN rsrc END;
  RETURN NIL
RESCUE (exception);
  RETURN NIL
END OpenAt;

PROCEDURE ReadByte* (rsrc: Resource): BYTE;
VAR
  abyte: ARRAY 2 OF BYTE;
BEGIN
  IF rsrc.read(abyte, 0, 1) = -1 THEN RETURN 0 END;
  RETURN abyte[0];
RESCUE (exception);
  RETURN 0
END ReadByte;

PROCEDURE Close* (rsrc: Resource);
BEGIN
  rsrc.close;
RESCUE (exception);
END Close;

PROCEDURE GetTileByNum* (num: INTEGER): Tile;
BEGIN
  RETURN tileByNum[num]
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
  (* For returning the tiles by a number. *)
  tileByNum[ 0] := None;     tileByNum[ 1] := Grass;    tileByNum[ 2] := Stone;
  tileByNum[ 3] := Almas;    tileByNum[ 4] := StopMan;  tileByNum[ 5] := Wall;
  tileByNum[ 6] := Mina;     tileByNum[ 7] := Babo;     tileByNum[ 8] := LeftMan;
  tileByNum[ 9] := LeftMan1; tileByNum[10] := RightMan; tileByNum[11] := RightMan1;
  tileByNum[12] := UpMan;    tileByNum[13] := UpMan1;   tileByNum[14] := DownMan;
  tileByNum[15] := DownMan1; tileByNum[16] := Mina1;    tileByNum[17] := Babo1;

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
