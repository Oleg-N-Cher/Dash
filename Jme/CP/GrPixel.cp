MODULE GrPixel; (** non-portable *)
IMPORT
  lcdui := javax_microedition_lcdui,
  GrApp, Cfg := GrCfg;

CONST
  DisActualY = Cfg.DisActualY;
TYPE
  Color* = GrApp.Color;
  Coords* = INTEGER;

PROCEDURE Ink* (color: Color);
BEGIN
  GrApp.Main.screen.g.setColor(color);
END Ink;

(*
PROCEDURE PutActualPixel (x, y: Coords);
BEGIN
  GrApp.Main.screen.g.drawLine(x, y, x, y);
END PutActualPixel;
*)
PROCEDURE Line* (x1, y1, x2, y2: Coords);
BEGIN
  GrApp.Main.screen.g.drawLine(x1, y1 + DisActualY, x2, y2 + DisActualY);
END Line;

PROCEDURE PutPixel* (x, y: Coords); (* PutVirtualPixel *)
VAR
  xActual, yActual, xNext, yNext: Coords;
BEGIN
  xActual := x * GrApp.Width DIV0 Cfg.VirtualWidth;
  yActual := y * GrApp.Height DIV0 Cfg.VirtualHeight;
  xNext := (x + 1) * GrApp.Width DIV0 Cfg.VirtualWidth;
  yNext := (y + 1) * GrApp.Height DIV0 Cfg.VirtualHeight;
  GrApp.Main.screen.g.fillRect(xActual, yActual + DisActualY,
    xNext - xActual, yNext - yActual);
END PutPixel;

END GrPixel.
