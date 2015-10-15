MODULE Dash; (*$MAIN*)
IMPORT
  Scr := GrApp, Rsrc, Gr := GrTiles, Ctrl := Control, Timer, Sound;

(*
static void game_title (void)             // PROCEDURE ShowTitle;
{{                                        // BEGIN
 struct title_item * i;                   //   -> Rsrc
 unsigned p = 0;                          //   -> Sound.BEGIN
 static unsigned pal = 0, bgr = 0;        //   -> GrApp
                                          //
 i = title_list;                          // title := Rsrc.Open(Rsrc.Title);
 while (1) {                              // LOOP
  unsigned m = p, k;                      //   -> Sound.TitleNext
                                          //
  run_delay (1);                          // Timer.Start(3);
  if (i < title_list + numtl)             // IF titleSize # 0 THEN
   sprite (i -> x, i -> y, i -> z), i ++; // Gr.PutTile(..); DEC(titleSize);
  m = ++ p;                               //   -> Sound.TitleNext
  for (k = 0; m; m >>= 1)                 //   -> Sound.TitleNext
   if (m & 1)                             //   -> Sound.TitleNext
    k ++;                                 //   -> Sound.TitleNext
  snd (30 + (k << 4), 30);                // Sound.TitleNext;
  switch (pressed (ANY_KEY)) {            // IF Keys.PressedAnyKey() THEN
   case NO_KEY :                          //
    break;                                //
   case KEYF1 :                           //   -> Control.PressedAnyKey
    palcolor (++ pal);                    //   -> Control.PressedAnyKey
    press_off (KEYF1);                    //   -> Control.PressedAnyKey
    break;                                //
   case KEYF2 :                           //   -> Control.PressedAnyKey
    backcolor (++ bgr);                   //   -> Control.PressedAnyKey
    press_off (KEYF2);                    //   -> Control.PressedAnyKey
    break;                                //
   default :                              //
    cls ();                               // Scr.Cls;
    return;                               // RETURN
    }                                     //
  wait_delay ();                          // Timer.Until;
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
        DEC(titleSize);
        x := Rsrc.ReadByte(title);
        y := Rsrc.ReadByte(title); (* x, y used for correct order. *)
        Gr.PutTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
        Scr.Redraw;
      ELSE
        Rsrc.Close(title);
      END;
      Sound.TitleNext;
      IF Ctrl.PressedAnyKey() THEN Scr.Cls; RETURN END;
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
  (*REPEAT UNTIL ReadKeyBeep() = " ";*)
  ShowTitle;
  IF ~ (Ctrl.Esc IN Ctrl.Get()) THEN (* Play game: *)
  END;

  Scr.ScrollUp(100); Scr.ScrollDown(100); LOOP END;

  Ctrl.Close;
  Scr.Close;
END Dash.
