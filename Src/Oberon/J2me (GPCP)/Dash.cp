MODULE Dash;
IMPORT
  GrScr, Rsrc, Til := GrTiles;

VAR
  title: Rsrc.Resource;
  titleSize: INTEGER;
BEGIN
  Til.SetSize(10, 10, 5, 5);
  title := Rsrc.Open(Rsrc.Title);
  titleSize := Rsrc.TitleSize;
  WHILE titleSize # 0 DO
    GrTiles.PutTile(Rsrc.ReadByte(title), Rsrc.ReadByte(title),
      Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
    DEC(titleSize);
  END;
  Rsrc.Close(title);
  GrScr.Repaint;
END Dash.
