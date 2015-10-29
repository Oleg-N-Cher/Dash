MODULE Dash; (* portable *)

IMPORT Scr := GrApp, Timer, Gr := GrTiles, Rsrc;

PROCEDURE ShowTitle;
VAR
  title: Rsrc.Resource;
  titleSize: SHORTINT; (* 0..127 is enough here. *)
  x, y: Gr.Coords;
BEGIN
  title := Rsrc.Open(Rsrc.Title);
  titleSize := Rsrc.TitleSize; (* Must be > 0. *)
  LOOP
    Timer.Start(SHORT(ENTIER(1*Timer.Mul/Timer.Div + 0.5)));
      IF titleSize # 0 THEN
        DEC(titleSize);
        x := Rsrc.ReadByte(title);
        y := Rsrc.ReadByte(title); (* x, y used for correct order. *)
        Gr.DrawTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
        Scr.Redraw;
      ELSE
        Rsrc.Close(title);RETURN
      END;
      (*Sound.TitleNext;
      IF Ctrl.PressedAnyKey() THEN RETURN END;*)
    Timer.Until;
  END;
END ShowTitle;

BEGIN (*$MAIN*)
  ShowTitle;
  Timer.Start(100);
  Timer.Until;
  Scr.Close
END Dash.