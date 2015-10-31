MODULE Rsrc; (** non-portable: J2ME *)
IMPORT
  lcdui := javax_microedition_lcdui,
  lang := java_lang,
  io := java_io,
  GrApp;

CONST
  Title     * = "/Rsrc/Title.bin";
  TitleSize * = 114; (** Number of cells, a cell occupies 3 bytes. Must be > 0 *)
  Levels    * = "/Rsrc/Levels.bin";
  LevelSize * =  96; (* bytes *)
  MapWidth    =  16;
  MapHeight   =  16;
  MaxTileNum  =  17;

TYPE
  NATINT* = INTEGER; (* Native integer *)
  Resource* = io.InputStream;
  Ident* = lang.String;
  Tile = lcdui.Image;

VAR
  TileWidth-, TileHeight-, TileStepX-, TileStepY-: INTEGER;
  
  None-, Grass-, Stone-, Almas-, StopMan-, Wall-, Mina-, Babo-,
  LeftMan-, LeftMan1-, RightMan-, RightMan1-, UpMan-, UpMan1-,
  DownMan-, DownMan1-, Mina1-, Babo1-: Tile;

  tilePack: lcdui.Image; x: INTEGER;
  tileByNum: ARRAY MaxTileNum + 1 OF Tile; (* For returning a tile by number. *)

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

(*----------------------------------------------------------------------------*)

PROCEDURE GetTileByNum* (num: INTEGER): Tile;
BEGIN
  RETURN tileByNum[num]
END GetTileByNum;

PROCEDURE LoadTile (VAR tile: Tile);
VAR
  g: lcdui.Graphics;
BEGIN
  tile := lcdui.Image.createImage(TileWidth, TileHeight);
  g := tile.getGraphics();
  g.drawImage(tilePack, x, 0, 20);
  DEC(x, TileWidth);
END LoadTile;

BEGIN
  TileWidth  := GrApp.Width  DIV0 MapWidth;  TileStepX := TileWidth  DIV0 2;
  TileHeight := GrApp.Height DIV0 MapHeight; TileStepY := TileHeight DIV0 2;
  x := 0;
  IF TileWidth < 10 THEN
    TileWidth := 8;
    tilePack := lcdui.Image.createImage("/Rsrc/Tiles8.png");
  ELSE
    TileWidth := 10;
    tilePack := lcdui.Image.createImage("/Rsrc/Tiles10.png");
  END;

  (* Load tiles from a tile pack: *)
  LoadTile(None); LoadTile(Grass); LoadTile(Stone); LoadTile(Almas);
  LoadTile(StopMan); LoadTile(Wall); LoadTile(Mina); LoadTile(Babo);
  LoadTile(LeftMan); LoadTile(LeftMan1); LoadTile(RightMan);
  LoadTile(RightMan1); LoadTile(UpMan); LoadTile(UpMan1);
  LoadTile(DownMan); LoadTile(DownMan1); LoadTile(Mina1); LoadTile(Babo1);
  tilePack := NIL;
  
  (* For returning the tiles by a number: *)
  tileByNum[ 0] := None;     tileByNum[ 1] := Grass;    tileByNum[ 2] := Stone;
  tileByNum[ 3] := Almas;    tileByNum[ 4] := StopMan;  tileByNum[ 5] := Wall;
  tileByNum[ 6] := Mina;     tileByNum[ 7] := Babo;     tileByNum[ 8] := LeftMan;
  tileByNum[ 9] := LeftMan1; tileByNum[10] := RightMan; tileByNum[11] := RightMan1;
  tileByNum[12] := UpMan;    tileByNum[13] := UpMan1;   tileByNum[14] := DownMan;
  tileByNum[15] := DownMan1; tileByNum[16] := Mina1;    tileByNum[17] := Babo1;

END Rsrc.

(* 1. Need to check integrity of the resource pack with exception if broken *)
