MODULE Dash;
IMPORT
  GrScr, Rsrc, Til := GrTiles;
  
BEGIN
  Til.PutTile(0, 5, Rsrc.StopMan);
  GrScr.Repaint;
END Dash.
