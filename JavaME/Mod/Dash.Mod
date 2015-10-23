MODULE Dash;
IMPORT
  GrApp, Rsrc, Til := GrTiles;

VAR
  title: Rsrc.Resource;
  titleSize: INTEGER;

BEGIN
  title := Rsrc.Open(Rsrc.Title);
  titleSize := Rsrc.TitleSize; (* Must be > 0 *)
  REPEAT
    Til.DrawTile(Rsrc.ReadByte(title), Rsrc.ReadByte(title),
      Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
    DEC(titleSize);
  UNTIL titleSize = 0;
  Rsrc.Close(title);
  GrApp.Redraw;
END Dash.
