MODULE GrTiles;
IMPORT
  lcdui := javax_microedition_lcdui,
  GrApp, Rsrc;

TYPE
  Coords* = INTEGER; Tile* = lcdui.Image;

PROCEDURE DrawTile* (x, y: Coords; tile: Tile);
BEGIN
  GrApp.Main.screen.g.drawImage(tile, x*Rsrc.TileStepX, y*Rsrc.TileStepY, 20);
END DrawTile;

END GrTiles.
