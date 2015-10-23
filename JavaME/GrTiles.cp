MODULE GrTiles;
IMPORT
  lcdui := javax_microedition_lcdui,
  GrScr;

TYPE
  Tile* = lcdui.Image;
  Coords* = INTEGER;

VAR
  tileWidth, tileHeight, tileStepX, tileStepY: INTEGER;
  
PROCEDURE PutTile* (x, y: Coords; tile: Tile);
BEGIN
  GrScr.Main.screen.g.drawImage(tile, x * tileStepX, y * tileStepY, 20);
END PutTile;

PROCEDURE SetSize* (width, height, stepX, stepY: INTEGER);
BEGIN
  tileWidth := width; tileHeight := height;
  tileStepX := stepX; tileStepY := stepY;
END SetSize;

END GrTiles.

