MODULE TestTiles; (*$MAIN*)

IMPORT Scr := GrApp, GrTiles, Rsrc;

VAR
  i: BYTE;

BEGIN
  FOR i := 0 TO 17 DO
    IF i < 16 THEN
      GrTiles.DrawTile(2*i, 0, Rsrc.GetTileByNum(i));
    ELSE
      GrTiles.DrawTile(2*(i-16), 2, Rsrc.GetTileByNum(i));
    END;
  END;
  (*Scr.Close*)
END TestTiles.
