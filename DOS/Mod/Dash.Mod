MODULE Dash; (* portable *)

IMPORT GrApp, Timer, g:=GrTiles, p:=Rsrc;

BEGIN (*$MAIN*)
  g.DrawTile(1,6,p.Grass);
 g.DrawTile(3,6,p.Wall);
  g.DrawTile(5,6,p.UpMan);
  g.DrawTile(7,6,p.Wall);
  g.DrawTile(9,6,p.UpMan1);
  g.DrawTile(11,6,p.Wall);

  g.DrawTile(1,9,p.Wall);
  g.DrawTile(3,9,p.DownMan);
  g.DrawTile(5,9,p.Wall);
  g.DrawTile(7,9,p.DownMan1);
  g.DrawTile(9,9,p.Wall);
  g.DrawTile(11,9,p.None);

  g.DrawTile(1,12,p.Wall);
  g.DrawTile(3,12,p.Mina1);
  g.DrawTile(5,12,p.Wall);
  g.DrawTile(7,12,p.Babo1);
  g.DrawTile(9,12,p.Wall);
  g.DrawTile(11,12,p.None);

  Timer.Start(100);
  Timer.Until;
  GrApp.Close
END Dash.