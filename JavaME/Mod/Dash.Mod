MODULE Dash;
IMPORT
  Scr := GrApp, Rsrc, Gr := GrTiles, Timer, Sound, Ctrl := Control, Labirint;

PROCEDURE ShowTitle;
VAR
  title: Rsrc.Resource;
  titleSize: SHORTINT; (* 0..127 is enough here. *)
  x, y: Gr.Coords;
BEGIN
  title := Rsrc.Open(Rsrc.Title);
  titleSize := Rsrc.TitleSize; (* Must be > 0. *)
  LOOP
    Timer.Start(3);
      IF titleSize # 0 THEN
        DEC(titleSize);
        x := Rsrc.ReadByte(title);
        y := Rsrc.ReadByte(title); (* x, y used for correct order. *)
        Gr.DrawTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
        Scr.Redraw;
      ELSE
        Rsrc.Close(title);RETURN
      END;
      Sound.TitleNext;
      IF Ctrl.PressedAnyKey() THEN RETURN END;
    Timer.Until;
  END;
END ShowTitle;

BEGIN
  ShowTitle;
  Labirint.Play(0);
  Scr.Redraw;
  (* Scr.Close *)
END Dash.
