MODULE GrTiles;
IMPORT
  lcdui := javax_microedition_lcdui,
  GrScr;

TYPE
  Tile* = lcdui.Image;
  
PROCEDURE PutTile* (x, y: INTEGER; tile: Tile);
BEGIN
  GrScr.Main.screen.g.drawImage(tile, x, y, 20);
END PutTile;

END GrTiles.

