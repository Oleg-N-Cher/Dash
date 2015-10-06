MODULE Dash; (*$MAIN*)
IMPORT
  Scr := GrApp, Rsrc, Gr := GrTiles, Ctrl := Control, Timer, Sound;

(*
static void game_title (void)             // PROCEDURE ShowTitle;
{{                                        // BEGIN
 struct title_item * i;                   //
 unsigned p = 0;                          //
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
  switch (pressed (ANY_KEY)) {            // IF Keys.PressedAnyKey() THEN
   case NO_KEY :                          //
    break;                                //
   case KEYF1 :                           //
    palcolor (++ pal);                    //
    press_off (KEYF1);                    //
    break;                                //
   case KEYF2 :                           //
    backcolor (++ bgr);                   //
    press_off (KEYF2);                    //
    break;                                //
   default :                              //
    cls ();                               //  GrApp.FillArea(0)
    return;                               //  RETURN
    }                                     //
  wait_delay ();                          // Timer.Until(3)
   }                                      //
}}                                        // END ShowTitle; *)

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
      x := Rsrc.ReadByte(title);
      y := Rsrc.ReadByte(title); (* x, y used for correct order. *)
      Gr.PutTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
      Scr.Redraw; DEC(titleSize);
    ELSE
      Rsrc.Close(title);
    END;
(*
    INC(p); k := 0; m := p;
    WHILE m # 0 DO
      IF ODD(m) THEN INC(k) END;
      m := m DIV 2;
    END;
    Sound.Beep(24858 DIV (k*8 + 15) * 4, 8(*?*));
*)
    Sound.TitleNext;
    IF Ctrl.PressedAnyKey() THEN Scr.FillArea(0); END;

    Timer.Until;
  END;
END ShowTitle;

(*
scan key_i_beep (void)                    // PROCEDURE ReadKeyBeep (): Ctrl.Key;
{{                                        // BEGIN
 scan k;                                  // VAR key: Ctrl.Key;
                                          //
 while ((k = pressed (ANY_KEY)) == 0)	;  // key := Ctrl.ReadKey();
 press_off (k);                           //   (* in Control.ReadKey() *)
 snd (64, 64);                            // Sound.Beep(64, 64);
 return k;                                // RETURN key
}}                                        // END ReadKeyBeep; *)

PROCEDURE ReadKeyBeep (): Ctrl.Key;
VAR
  key: Ctrl.Key;
BEGIN
  key := Ctrl.ReadKey(); Sound.KeyPressed; RETURN key
END ReadKeyBeep;

BEGIN
  REPEAT UNTIL ReadKeyBeep() = " ";
  ShowTitle;
  Scr.Close;
END Dash.
