MODULE GrTiles;
IMPORT
  lcdui := javax_microedition_lcdui,
  GrApp;

TYPE
  Tile* = lcdui.Image;
  Coords* = INTEGER;

VAR
  tileWidth, tileHeight, tileStepX, tileStepY: INTEGER;
  
PROCEDURE DrawTile* (x, y: Coords; tile: Tile);
BEGIN
  GrApp.Main.screen.g.drawImage(tile, x * tileStepX, y * tileStepY, 20);
END DrawTile;

PROCEDURE SetSize* (width, height, stepX, stepY: INTEGER);
BEGIN
  tileWidth := width; tileHeight := height;
  tileStepX := stepX; tileStepY := stepY;
END SetSize;

BEGIN
  SetSize(10, 10, 5, 5);
END GrTiles.
