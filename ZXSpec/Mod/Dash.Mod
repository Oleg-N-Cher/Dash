MODULE Dash; (*$MAIN*)
IMPORT
  GrApp, Rsrc, Tile := GrTiles, Timer;

(*
static void game_title (void)             // PROCEDURE ShowTitle;
{{                                        //
 struct title_item * i;                   //
 unsigned p = 0;
 static unsigned pal = 0, bgr = 0;        //

 i = title_list;                          // title := Rsrc.Open(Rsrc.Title);
 while (1) {                              // LOOP
  unsigned m = p, k;

  run_delay (1);                          // Timer.Start;
  if (i < title_list + numtl)             // IF titleSize # 0 THEN
   sprite (i -> x, i -> y, i -> z), i ++; // Tile.PutTile(..); DEC(titleSize);
  m = ++ p;
  for (k = 0; m; m >>= 1)
   if (m & 1)
    k ++;
  snd (30 + (k << 4), 30);
  switch (pressed (ANY_KEY)) {
   case NO_KEY :
    break;
   case KEYF1 :                           //
    palcolor (++ pal);                    //
    press_off (KEYF1);                    //
    break;                                //
   case KEYF2 :                           //
    backcolor (++ bgr);                   //
    press_off (KEYF2);                    //
    break;                                //
   default :
    cls ();
    return;
    }
  wait_delay ();
   }
}}
*)

PROCEDURE ShowTitle;
VAR
  title: Rsrc.Resource;
  titleSize: SHORTINT; (* 0..127 is enough here. *)
  x, y: Tile.Coords;
BEGIN
  title := Rsrc.Open(Rsrc.Title);
  titleSize := Rsrc.TitleSize; (* Must be > 0. *)

  LOOP
    Timer.Start;

    IF titleSize # 0 THEN
      x := Rsrc.ReadByte(title);
      y := Rsrc.ReadByte(title); (* x, y used for correct order. *)
      Tile.PutTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
      GrApp.Redraw; DEC(titleSize);
    ELSE
      Rsrc.Close(title);
    END;

    Timer.Until(1);
  END;
END ShowTitle;

BEGIN
  ShowTitle;
  GrApp.Close;
END Dash.
