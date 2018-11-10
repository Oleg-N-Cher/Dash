{$APPTYPE CONSOLE}
PROGRAM GenExoLoader;
USES Windows, Kol;
{$R *.RES}

CONST
  CodeStartAdr = 32832;
  LoaderStartAdr = 65118;
VAR
  bin, loader: AnsiString; packedAdr: INTEGER;
BEGIN
  bin := Kol.StrLoadFromFile(ParamStr(1));
  loader := Kol.StrLoadFromFile(ParamStr(2));
  IF (LENGTH(bin) = 0) OR (LENGTH(loader) = 0) THEN BEGIN
    WriteLn('Couldn''t load a file'); HALT(0)
  END;
  // Packed data + Loader
  packedAdr := LoaderStartAdr - LENGTH(bin);
  // LD   HL, #52490  ; compressed data start
  loader[2] := CHR(packedAdr MOD 256); loader[3] := CHR(packedAdr DIV 256);
  // LD   DE, #32832  ; uncompressed destination start
  loader[5] := CHR(CodeStartAdr MOD 256); loader[6] := CHR(CodeStartAdr DIV 256);

  StrSaveToFile(ParamStr(1), bin + loader);
  WriteLn('Start adr: ', packedAdr);
  WriteLn('File size: ', LENGTH(bin) + LENGTH(loader));
  WriteLn('Entry point: ', LoaderStartAdr);
  HALT(packedAdr)
END {GenExoLoader}.
