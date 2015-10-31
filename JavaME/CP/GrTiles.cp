MODULE GrTiles;
IMPORT
  lcdui := javax_microedition_lcdui,
  GrApp, Rsrc;

TYPE
  Coords* = INTEGER; Tile* = lcdui.Image;

PROCEDURE DrawTile* (x, y: Coords; tile: Tile);
BEGIN
  GrApp.screen.gfx.drawImage(
    tile,
    x*Rsrc.TileWidth DIV0 2,
    y*Rsrc.TileHeight DIV0 2,
    20
  );
END DrawTile;

END GrTiles.
