MODULE Dash; (*$MAIN*)
IMPORT
  Scr := GrApp, IO := Console, Gr := GrTiles, Labirint,
  Msg, Rsrc, Ctrl := Control, Timer, Sound, Strings, Cfg := Config;

  CONST
    Abort = -1;

  VAR
    controlMethodN: (* UNSIGNED *) SHORTINT;

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
        Gr.DrawTile(x, y, Rsrc.GetTileByNum(Rsrc.ReadByte(title)));
        Scr.Redraw;
      ELSE
        Rsrc.Close(title);
      END;
      Sound.TitleNext;
      IF Ctrl.PressedAnyKey() THEN RETURN END;
    Timer.Until;
  END;
END ShowTitle;

(*
scan key_i_beep (void)                    // PROCEDURE ReadKeyBeep (): Ctrl.Key;
{{                                        // BEGIN
 scan k;                                  // VAR key: Ctrl.Key;
                                          //
 while ((k = pressed (ANY_KEY)) == 0) ;   // key := Ctrl.ReadKey();
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

PROCEDURE Center (y: (*UNSIGNED*) SHORTINT; IN str: ARRAY OF CHAR);
(*
void center (unsigned line, char * s)
{{
 int i = 0, j = strlen (s), k;

 locate (line, 0);
 k = 16 - (j >> 1);
 while (i ++ < k)
  emt16 (' ');
 print_asciz (s);
 k += j;
 while (++ k < 32)
  emt16 (' ');
}}
*)
VAR
  i, x, len: (*UNSIGNED*) SHORTINT;
BEGIN
  len := SHORT( Strings.Length(str) );
  x := 16 - (len DIV 2);
  IO.At(0, y);
  FOR i := x TO 1 BY -1 DO IO.WriteCh(" ") END;
  IO.WriteStr(str);
  (* INC(x, len);
  WHILE x < 31 DO IO.WriteCh(" "); INC(x) END; *)
  (* IF (x = 31) & (y # 15) THEN IO.WriteCh(" ") END; *)
END Center;

PROCEDURE SetJoystickCtrl;
BEGIN
  IF Cfg.TargetZX THEN
    IO.At(Msg.MenuX, Msg.MenuY+1);
    controlMethodN := (controlMethodN + 1) MOD 4;
    CASE controlMethodN OF
    | 0: IO.WriteStr(Msg.MenuKeyboard);
    | 1: IO.WriteStr(Msg.MenuKempston);
    | 2: IO.WriteStr(Msg.MenuSinclair);
    ELSE IO.WriteStr(Msg.MenuCursor);
    END;
    Ctrl.Select(controlMethodN);
  END;
END SetJoystickCtrl;

(*
static unsigned menu_select (void)
{{
 struct menu_item * z;

 cls ();
 for (z = menu_text; z < menu_text + NMENU; z ++) {
  locate (z -> str, z -> col);
  print_asciz (_M[z -> msg]);
   }
 z = menu_text;
 while (1) {
  locate (z -> str, z -> col - 3);
  print_asciz ("=>");
  switch (key_i_beep ()) {
   case UP_ARROW : case LEFT_ARROW :
    if (z > menu_text) {
     locate (z -> str, z -> col - 3);
     print_asciz ("  ");
     z --;
      }
    break;
   case DOWN_ARROW : case RIGHT_ARROW :
    if (z < menu_text + NMENU - 1) {
     locate (z -> str, z -> col - 3);
     print_asciz ("  ");
     z ++;
      }
    break;
   case ENTER : case SPACE_BAR :
    return z - menu_text;
    }
   }
}}
*)

PROCEDURE MenuSelect (): (* UNSIGNED *) SHORTINT;
VAR
  item: (* UNSIGNED *) SHORTINT; key: Ctrl.Key; keys: Ctrl.Keys;
BEGIN
  Scr.Cls;
  IO.Ink(IO.Cyan); Center(15, Msg.VEDAsoft);
  IO.Ink(IO.LWhite);
  IO.At(Msg.MenuX, Msg.MenuY  ); IO.WriteStr(Msg.MenuPlay);
  IF Cfg.TargetZX THEN
    DEC(controlMethodN); SetJoystickCtrl;
  ELSE
    IO.At(Msg.MenuX, Msg.MenuY+2); IO.WriteStr(Msg.MenuKeyboard);
  END;
  IO.At(Msg.MenuX, Msg.MenuY+2); IO.WriteStr(Msg.MenuHelp);
  IO.At(Msg.MenuX, Msg.MenuY+3); IO.WriteStr(Msg.MenuExit);
  item := 0;
  LOOP
    IO.At(Msg.MenuX - 3, Msg.MenuY + item); IO.WriteStr("=>");
    key := ReadKeyBeep();
    keys := Ctrl.Get();
    IF (keys = {Ctrl.Up}) OR (keys = {Ctrl.Left}) THEN
      IF item # 0 THEN
        IO.At(Msg.MenuX - 3, Msg.MenuY + item); IO.WriteStr("  ");
        DEC(item);
      END;
    END;
    IF (keys = {Ctrl.Down}) OR (keys = {Ctrl.Right}) THEN
      IF item # 3 THEN
        IO.At(Msg.MenuX - 3, Msg.MenuY + item); IO.WriteStr("  ");
        INC(item);
      END;
    END;
    IF (keys = {Ctrl.Fire}) OR (key = " ") OR (key = 0DX) THEN
      IF (~ Cfg.TargetZX) OR (item # 1) THEN RETURN item END;
      IF Cfg.TargetZX THEN SetJoystickCtrl END;
    END;
    IF keys = {Ctrl.Esc} THEN RETURN Abort END;
  END
END MenuSelect;

PROCEDURE PressAnyKey;
(*
static void press_any_key (void)
{{
 center (15, _M[PRESS_ANY_KEY]);
 key_i_beep ();
}}
*)
BEGIN
  Center(15, Msg.PressAnyKey); IF ReadKeyBeep() = 0X THEN END;
END PressAnyKey;

PROCEDURE Instruction (IN text: ARRAY OF CHAR): BOOLEAN;
(*
static void instruction (void)
{{
 int z = aopen ("BOLDER.TXR");
 unsigned Z = alength (z), k;
 static char c[2] = {0, 0};

 cls ();
 for (k = 0; k < Z; k ++) {
  aread (z, c, 1);
  switch ( *c) {
   case 0 :
    cr ();
    break;
   case '@' :
    press_any_key ();
    locate (15, 0);
    break;
   default :
    print_asciz (c);
    }
   }
}}
*)
VAR
  i: INTEGER; ch: CHAR;
BEGIN
  i := 0; ch := text[i];
  WHILE ch # 0X DO
    CASE ch OF
    | "#": Scr.Cls; IO.At(1, 0);
    | "^": IO.WriteLn; IO.WriteCh(" ");
    | "@": IO.WriteLn; PressAnyKey;
           IF Ctrl.Get() = {Ctrl.Esc} THEN RETURN FALSE END;
           IO.At(1, 15);
    ELSE IO.WriteCh(ch);
    END;
    INC(i); ch := text[i];
  END;
  RETURN TRUE
END Instruction;

PROCEDURE PlayGame;
BEGIN
  ShowTitle;
  IF Ctrl.Get() # {Ctrl.Esc} THEN
    LOOP
      CASE MenuSelect() OF
      | 0: Labirint.Play(0);LOOP END;
      | 2: (*HELP*)
        IF ~ Instruction(Msg.Instruction1) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction2) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction3) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction4) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction5) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction6) THEN RETURN END;
        IF ~ Instruction(Msg.Instruction7) THEN RETURN END;
      | Abort: RETURN
      ELSE (*EXIT*)
        Scr.Cls; Center(7, Msg.IfSureY);
        CASE ReadKeyBeep() OF "Y", "y": RETURN ELSE END;
      END;
    END;
  END;
END PlayGame;

BEGIN
  IF Cfg.TargetZX THEN controlMethodN := 0 END;
  PlayGame;
  Ctrl.Close; Scr.Close
END Dash.
