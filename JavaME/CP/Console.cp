MODULE Console;
IMPORT GrApp, GrFonts, GrTiles;

CONST
  Black*        = 0000000H;
  Blue*         = 00000AAH;
  Green*        = 000AA00H;
  Cyan*         = 000AAAAH;
  Red*          = 0AA0000H;
  Magenta*      = 0AA00AAH;
  Olive*        = 0AAAA00H;
  White*        = 0AAAAAAH;
  Gray*         = 0555555H;
  LightBlue*    = 00000FFH;
  LightGreen*   = 055FF55H;
  LightCyan*    = 000FFFFH;
  LightRed*     = 0FF0000H;
  LightMagenta* = 0FF00FFH;
  Yellow*       = 0FFFF00H;
  LightWhite*   = 0FFFFFFH;
  Orange*       = 0FFAA00H;

  Paper = 100000000L;
  BlackOnBlack*    = Black * Paper + Black;
  BlueOnBlack*     = Black * Paper + Blue;
  GreenOnBlack*    = Black * Paper + Green;
  CyanOnBlack*     = Black * Paper + Cyan;
  RedOnBlack*      = Black * Paper + Red;
  MagentaOnBlack*  = Black * Paper + Magenta;
  OliveOnBlack*    = Black * Paper + Olive;
  WhiteOnBlack*    = Black * Paper + White;
  GrayOnBlack*     = Black * Paper + Gray;
  LBlueOnBlack*    = Black * Paper + LightBlue;
  LGreenOnBlack*   = Black * Paper + LightGreen;
  LCyanOnBlack*    = Black * Paper + LightCyan;
  LRedOnBlack*     = Black * Paper + LightRed;
  LMagentaOnBlack* = Black * Paper + LightMagenta;
  YellowOnBlack*   = Black * Paper + Yellow;
  LWhiteOnBlack*   = Black * Paper + LightWhite;
  OrangeOnBlack*   = Black * Paper + Orange;

  BlackOnBlue*    = Blue * Paper + Black;
  BlueOnBlue*     = Blue * Paper + Blue;
  GreenOnBlue*    = Blue * Paper + Green;
  CyanOnBlue*     = Blue * Paper + Cyan;
  RedOnBlue*      = Blue * Paper + Red;
  MagentaOnBlue*  = Blue * Paper + Magenta;
  OliveOnBlue*    = Blue * Paper + Olive;
  WhiteOnBlue*    = Blue * Paper + White;
  GrayOnBlue*     = Blue * Paper + Gray;
  LBlueOnBlue*    = Blue * Paper + LightBlue;
  LGreenOnBlue*   = Blue * Paper + LightGreen;
  LCyanOnBlue*    = Blue * Paper + LightCyan;
  LRedOnBlue*     = Blue * Paper + LightRed;
  LMagentaOnBlue* = Blue * Paper + LightMagenta;
  YellowOnBlue*   = Blue * Paper + Yellow;
  LWhiteOnBlue*   = Blue * Paper + LightWhite;
  OrangeOnBlue*   = Blue * Paper + Orange;

  BlackOnGreen*    = Green * Paper + Black;
  BlueOnGreen*     = Green * Paper + Blue;
  GreenOnGreen*    = Green * Paper + Green;
  CyanOnGreen*     = Green * Paper + Cyan;
  RedOnGreen*      = Green * Paper + Red;
  MagentaOnGreen*  = Green * Paper + Magenta;
  OliveOnGreen*    = Green * Paper + Olive;
  WhiteOnGreen*    = Green * Paper + White;
  GrayOnGreen*     = Green * Paper + Gray;
  LBlueOnGreen*    = Green * Paper + LightBlue;
  LGreenOnGreen*   = Green * Paper + LightGreen;
  LCyanOnGreen*    = Green * Paper + LightCyan;
  LRedOnGreen*     = Green * Paper + LightRed;
  LMagentaOnGreen* = Green * Paper + LightMagenta;
  YellowOnGreen*   = Green * Paper + Yellow;
  LWhiteOnGreen*   = Green * Paper + LightWhite;
  OrangeOnGreen*   = Green * Paper + Orange;

  BlackOnCyan*    = Cyan * Paper + Black;
  BlueOnCyan*     = Cyan * Paper + Blue;
  GreenOnCyan*    = Cyan * Paper + Green;
  CyanOnCyan*     = Cyan * Paper + Cyan;
  RedOnCyan*      = Cyan * Paper + Red;
  MagentaOnCyan*  = Cyan * Paper + Magenta;
  OliveOnCyan*    = Cyan * Paper + Olive;
  WhiteOnCyan*    = Cyan * Paper + White;
  GrayOnCyan*     = Cyan * Paper + Gray;
  LBlueOnCyan*    = Cyan * Paper + LightBlue;
  LGreenOnCyan*   = Cyan * Paper + LightGreen;
  LCyanOnCyan*    = Cyan * Paper + LightCyan;
  LRedOnCyan*     = Cyan * Paper + LightRed;
  LMagentaOnCyan* = Cyan * Paper + LightMagenta;
  YellowOnCyan*   = Cyan * Paper + Yellow;
  LWhiteOnCyan*   = Cyan * Paper + LightWhite;
  OrangeOnCyan*   = Cyan * Paper + Orange;

  BlackOnRed*    = Red * Paper + Black;
  BlueOnRed*     = Red * Paper + Blue;
  GreenOnRed*    = Red * Paper + Green;
  CyanOnRed*     = Red * Paper + Cyan;
  RedOnRed*      = Red * Paper + Red;
  MagentaOnRed*  = Red * Paper + Magenta;
  OliveOnRed*    = Red * Paper + Olive;
  WhiteOnRed*    = Red * Paper + White;
  GrayOnRed*     = Red * Paper + Gray;
  LBlueOnRed*    = Red * Paper + LightBlue;
  LGreenOnRed*   = Red * Paper + LightGreen;
  LCyanOnRed*    = Red * Paper + LightCyan;
  LRedOnRed*     = Red * Paper + LightRed;
  LMagentaOnRed* = Red * Paper + LightMagenta;
  YellowOnRed*   = Red * Paper + Yellow;
  LWhiteOnRed*   = Red * Paper + LightWhite;
  OrangeOnRed*   = Red * Paper + Orange;

  BlackOnMagenta*    = Magenta * Paper + Black;
  BlueOnMagenta*     = Magenta * Paper + Blue;
  GreenOnMagenta*    = Magenta * Paper + Green;
  CyanOnMagenta*     = Magenta * Paper + Cyan;
  RedOnMagenta*      = Magenta * Paper + Red;
  MagentaOnMagenta*  = Magenta * Paper + Magenta;
  OliveOnMagenta*    = Magenta * Paper + Olive;
  WhiteOnMagenta*    = Magenta * Paper + White;
  GrayOnMagenta*     = Magenta * Paper + Gray;
  LBlueOnMagenta*    = Magenta * Paper + LightBlue;
  LGreenOnMagenta*   = Magenta * Paper + LightGreen;
  LCyanOnMagenta*    = Magenta * Paper + LightCyan;
  LRedOnMagenta*     = Magenta * Paper + LightRed;
  LMagentaOnMagenta* = Magenta * Paper + LightMagenta;
  YellowOnMagenta*   = Magenta * Paper + Yellow;
  LWhiteOnMagenta*   = Magenta * Paper + LightWhite;
  OrangeOnMagenta*   = Magenta * Paper + Orange;

  BlackOnOlive*    = Olive * Paper + Black;
  BlueOnOlive*     = Olive * Paper + Blue;
  GreenOnOlive*    = Olive * Paper + Green;
  CyanOnOlive*     = Olive * Paper + Cyan;
  RedOnOlive*      = Olive * Paper + Red;
  MagentaOnOlive*  = Olive * Paper + Magenta;
  OliveOnOlive*    = Olive * Paper + Olive;
  WhiteOnOlive*    = Olive * Paper + White;
  GrayOnOlive*     = Olive * Paper + Gray;
  LBlueOnOlive*    = Olive * Paper + LightBlue;
  LGreenOnOlive*   = Olive * Paper + LightGreen;
  LCyanOnOlive*    = Olive * Paper + LightCyan;
  LRedOnOlive*     = Olive * Paper + LightRed;
  LMagentaOnOlive* = Olive * Paper + LightMagenta;
  YellowOnOlive*   = Olive * Paper + Yellow;
  LWhiteOnOlive*   = Olive * Paper + LightWhite;
  OrangeOnOlive*   = Olive * Paper + Orange;

  BlackOnWhite*    = White * Paper + Black;
  BlueOnWhite*     = White * Paper + Blue;
  GreenOnWhite*    = White * Paper + Green;
  CyanOnWhite*     = White * Paper + Cyan;
  RedOnWhite*      = White * Paper + Red;
  MagentaOnWhite*  = White * Paper + Magenta;
  OliveOnWhite*    = White * Paper + Olive;
  WhiteOnWhite*    = White * Paper + White;
  GrayOnWhite*     = White * Paper + Gray;
  LBlueOnWhite*    = White * Paper + LightBlue;
  LGreenOnWhite*   = White * Paper + LightGreen;
  LCyanOnWhite*    = White * Paper + LightCyan;
  LRedOnWhite*     = White * Paper + LightRed;
  LMagentaOnWhite* = White * Paper + LightMagenta;
  YellowOnWhite*   = White * Paper + Yellow;
  LWhiteOnWhite*   = White * Paper + LightWhite;
  OrangeOnWhite*   = White * Paper + Orange;

  BlackOnGray*    = Gray * Paper + Black;
  BlueOnGray*     = Gray * Paper + Blue;
  GreenOnGray*    = Gray * Paper + Green;
  CyanOnGray*     = Gray * Paper + Cyan;
  RedOnGray*      = Gray * Paper + Red;
  MagentaOnGray*  = Gray * Paper + Magenta;
  OliveOnGray*    = Gray * Paper + Olive;
  WhiteOnGray*    = Gray * Paper + White;
  GrayOnGray*     = Gray * Paper + Gray;
  LBlueOnGray*    = Gray * Paper + LightBlue;
  LGreenOnGray*   = Gray * Paper + LightGreen;
  LCyanOnGray*    = Gray * Paper + LightCyan;
  LRedOnGray*     = Gray * Paper + LightRed;
  LMagentaOnGray* = Gray * Paper + LightMagenta;
  YellowOnGray*   = Gray * Paper + Yellow;
  LWhiteOnGray*   = Gray * Paper + LightWhite;
  OrangeOnGray*   = Gray * Paper + Orange;

  BlackOnLBlue*    = LightBlue * Paper + Black;
  BlueOnLBlue*     = LightBlue * Paper + Blue;
  GreenOnLBlue*    = LightBlue * Paper + Green;
  CyanOnLBlue*     = LightBlue * Paper + Cyan;
  RedOnLBlue*      = LightBlue * Paper + Red;
  MagentaOnLBlue*  = LightBlue * Paper + Magenta;
  OliveOnLBlue*    = LightBlue * Paper + Olive;
  WhiteOnLBlue*    = LightBlue * Paper + White;
  GrayOnLBlue*     = LightBlue * Paper + Gray;
  LBlueOnLBlue*    = LightBlue * Paper + LightBlue;
  LGreenOnLBlue*   = LightBlue * Paper + LightGreen;
  LCyanOnLBlue*    = LightBlue * Paper + LightCyan;
  LRedOnLBlue*     = LightBlue * Paper + LightRed;
  LMagentaOnLBlue* = LightBlue * Paper + LightMagenta;
  YellowOnLBlue*   = LightBlue * Paper + Yellow;
  LWhiteOnLBlue*   = LightBlue * Paper + LightWhite;
  OrangeOnLBlue*   = LightBlue * Paper + Orange;

  BlackOnLGreen*    = LightGreen * Paper + Black;
  BlueOnLGreen*     = LightGreen * Paper + Blue;
  GreenOnLGreen*    = LightGreen * Paper + Green;
  CyanOnLGreen*     = LightGreen * Paper + Cyan;
  RedOnLGreen*      = LightGreen * Paper + Red;
  MagentaOnLGreen*  = LightGreen * Paper + Magenta;
  OliveOnLGreen*    = LightGreen * Paper + Olive;
  WhiteOnLGreen*    = LightGreen * Paper + White;
  GrayOnLGreen*     = LightGreen * Paper + Gray;
  LBlueOnLGreen*    = LightGreen * Paper + LightBlue;
  LGreenOnLGreen*   = LightGreen * Paper + LightGreen;
  LCyanOnLGreen*    = LightGreen * Paper + LightCyan;
  LRedOnLGreen*     = LightGreen * Paper + LightRed;
  LMagentaOnLGreen* = LightGreen * Paper + LightMagenta;
  YellowOnLGreen*   = LightGreen * Paper + Yellow;
  LWhiteOnLGreen*   = LightGreen * Paper + LightWhite;
  OrangeOnLGreen*   = LightGreen * Paper + Orange;

  BlackOnLCyan*    = LightCyan * Paper + Black;
  BlueOnLCyan*     = LightCyan * Paper + Blue;
  GreenOnLCyan*    = LightCyan * Paper + Green;
  CyanOnLCyan*     = LightCyan * Paper + Cyan;
  RedOnLCyan*      = LightCyan * Paper + Red;
  MagentaOnLCyan*  = LightCyan * Paper + Magenta;
  OliveOnLCyan*    = LightCyan * Paper + Olive;
  WhiteOnLCyan*    = LightCyan * Paper + White;
  GrayOnLCyan*     = LightCyan * Paper + Gray;
  LBlueOnLCyan*    = LightCyan * Paper + LightBlue;
  LGreenOnLCyan*   = LightCyan * Paper + LightGreen;
  LCyanOnLCyan*    = LightCyan * Paper + LightCyan;
  LRedOnLCyan*     = LightCyan * Paper + LightRed;
  LMagentaOnLCyan* = LightCyan * Paper + LightMagenta;
  YellowOnLCyan*   = LightCyan * Paper + Yellow;
  LWhiteOnLCyan*   = LightCyan * Paper + LightWhite;
  OrangeOnLCyan*   = LightCyan * Paper + Orange;

  BlackOnLRed*    = LightRed * Paper + Black;
  BlueOnLRed*     = LightRed * Paper + Blue;
  GreenOnLRed*    = LightRed * Paper + Green;
  CyanOnLRed*     = LightRed * Paper + Cyan;
  RedOnLRed*      = LightRed * Paper + Red;
  MagentaOnLRed*  = LightRed * Paper + Magenta;
  OliveOnLRed*    = LightRed * Paper + Olive;
  WhiteOnLRed*    = LightRed * Paper + White;
  GrayOnLRed*     = LightRed * Paper + Gray;
  LBlueOnLRed*    = LightRed * Paper + LightBlue;
  LGreenOnLRed*   = LightRed * Paper + LightGreen;
  LCyanOnLRed*    = LightRed * Paper + LightCyan;
  LRedOnLRed*     = LightRed * Paper + LightRed;
  LMagentaOnLRed* = LightRed * Paper + LightMagenta;
  YellowOnLRed*   = LightRed * Paper + Yellow;
  LWhiteOnLRed*   = LightRed * Paper + LightWhite;
  OrangeOnLRed*   = LightRed * Paper + Orange;

  BlackOnLMagenta*   = LightMagenta * Paper + Black;
  BlueOnLMagenta*    = LightMagenta * Paper + Blue;
  GreenOnLMagenta*   = LightMagenta * Paper + Green;
  CyanOnLMagenta*    = LightMagenta * Paper + Cyan;
  RedOnLMagenta*     = LightMagenta * Paper + Red;
  MagentaOnLMagenta* = LightMagenta * Paper + Magenta;
  OliveOnLMagenta*   = LightMagenta * Paper + Olive;
  WhiteOnLMagenta*   = LightMagenta * Paper + White;
  GrayOnLMagenta*    = LightMagenta * Paper + Gray;
  LBlueOnLMagenta*   = LightMagenta * Paper + LightBlue;
  LGreenOnLMagenta*  = LightMagenta * Paper + LightGreen;
  LCyanOnLMagenta*   = LightMagenta * Paper + LightCyan;
  LRedOnLMagenta*    = LightMagenta * Paper + LightRed;
  LMagentaOnLMagenta*= LightMagenta * Paper + LightMagenta;
  YellowOnLMagenta*  = LightMagenta * Paper + Yellow;
  LWhiteOnLMagenta*  = LightMagenta * Paper + LightWhite;
  OrangeOnLMagenta*  = LightMagenta * Paper + Orange;

  BlackOnYellow*    = Yellow * Paper + Black;
  BlueOnYellow*     = Yellow * Paper + Blue;
  GreenOnYellow*    = Yellow * Paper + Green;
  CyanOnYellow*     = Yellow * Paper + Cyan;
  RedOnYellow*      = Yellow * Paper + Red;
  MagentaOnYellow*  = Yellow * Paper + Magenta;
  OliveOnYellow*    = Yellow * Paper + Olive;
  WhiteOnYellow*    = Yellow * Paper + White;
  GrayOnYellow*     = Yellow * Paper + Gray;
  LBlueOnYellow*    = Yellow * Paper + LightBlue;
  LGreenOnYellow*   = Yellow * Paper + LightGreen;
  LCyanOnYellow*    = Yellow * Paper + LightCyan;
  LRedOnYellow*     = Yellow * Paper + LightRed;
  LMagentaOnYellow* = Yellow * Paper + LightMagenta;
  YellowOnYellow*   = Yellow * Paper + Yellow;
  LWhiteOnYellow*   = Yellow * Paper + LightWhite;
  OrangeOnYellow*   = Yellow * Paper + Orange;

  BlackOnLWhite*    = LightWhite * Paper + Black;
  BlueOnLWhite*     = LightWhite * Paper + Blue;
  GreenOnLWhite*    = LightWhite * Paper + Green;
  CyanOnLWhite*     = LightWhite * Paper + Cyan;
  RedOnLWhite*      = LightWhite * Paper + Red;
  MagentaOnLWhite*  = LightWhite * Paper + Magenta;
  OliveOnLWhite*    = LightWhite * Paper + Olive;
  WhiteOnLWhite*    = LightWhite * Paper + White;
  GrayOnLWhite*     = LightWhite * Paper + Gray;
  LBlueOnLWhite*    = LightWhite * Paper + LightBlue;
  LGreenOnLWhite*   = LightWhite * Paper + LightGreen;
  LCyanOnLWhite*    = LightWhite * Paper + LightCyan;
  LRedOnLWhite*     = LightWhite * Paper + LightRed;
  LMagentaOnLWhite* = LightWhite * Paper + LightMagenta;
  YellowOnLWhite*   = LightWhite * Paper + Yellow;
  LWhiteOnLWhite*   = LightWhite * Paper + LightWhite;
  OrangeOnLWhite*   = LightWhite * Paper + Orange;

  CharSizeInBytes = 8;

TYPE
  Colors* = GrApp.Colors;
  Coords* = GrTiles.Coords;

VAR
  curX, curY: Coords; curColors: Colors;
  curFont: GrFonts.Font; curFontDis: INTEGER;

PROCEDURE At* (x, y: Coords);
BEGIN
  curX := x; curY := y;
END At;

PROCEDURE Clear* (color: GrApp.Color);
BEGIN
  curX := 0; curY := 0;
  GrApp.Clear(color);
END Clear;

PROCEDURE SetColors* (colors: Colors);
BEGIN
  curColors := colors;
END SetColors;

PROCEDURE SetFont* (font: GrFonts.Font);
BEGIN
  IF font = GrFonts.ZxSpecRom8x8 THEN
    curFont := font; curFontDis := 32;
  ELSE
    curFont := font; curFontDis := 0;
  END;
END SetFont;

PROCEDURE WriteCh* (ch: CHAR);
VAR
  dis, n: INTEGER; tile: GrTiles.Tile8x8;
BEGIN
  dis := (ORD(ch) - curFontDis) * CharSizeInBytes;
  FOR n := 0 TO 7 DO
    tile[n] := curFont[n + dis];
  END;
  GrTiles.DrawMonoTile8x8(curX, curY, tile, curColors);
  INC(curX);
END WriteCh;

PROCEDURE WriteLn* ;
BEGIN
  curX := 0; INC(curY);
END WriteLn;

PROCEDURE WriteStr* (IN str: ARRAY OF CHAR);
VAR
  n: LONGINT;
BEGIN
  n := 0;
  WHILE (* (n < LEN(str)) & *) (str[n] # 0X) DO WriteCh(str[n]); INC(n) END;
END WriteStr;

PROCEDURE WriteStrLn* (IN str: ARRAY OF CHAR);
BEGIN
  WriteStr(str); WriteLn;
END WriteStrLn;

PROCEDURE WriteInt* (x: INTEGER); (* Write integer x to Console. *)
VAR
  i: INTEGER; buf: ARRAY 10 OF CHAR;
BEGIN
  IF x < 0 THEN
    IF x = MIN(INTEGER) THEN WriteStr("-2147483648"); RETURN END;
    WriteCh("-"); x := -x;
  END;
  i := 0;
  REPEAT
    buf[i] := CHR(x MOD 10 + ORD("0")); x := x DIV 10; INC(i);
  UNTIL x = 0;
  REPEAT DEC(i); WriteCh(buf[i]) UNTIL i = 0;
END WriteInt;

BEGIN
  curX := 0; curY := 0; curColors := 00000000000808080L;
  (*SetFont(GrFonts.ZxSpecRom8x8);*)
END Console.
