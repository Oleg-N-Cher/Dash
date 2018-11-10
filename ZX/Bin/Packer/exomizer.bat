exomizer.exe raw -P0 -c ..\..\Obj\Dash.bin -o ..\..\Dash.bin.exo
:: Dash.bin.exo + Exomizer.bin => Dash.bin.exo
GenExoLoader.exe ..\..\Dash.bin.exo Exomizer\Exomizer.bin
@SET CodeAddr=%errorlevel%
