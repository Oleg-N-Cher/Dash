@CD "ZxSpec (Ofront-SDCC)"
@DEL *.bin
@IF NOT EXIST GrScr.ihx   CALL GrScr.bat
@IF NOT EXIST GrTiles.ihx CALL GrTiles.bat
@IF NOT EXIST Rsrc.ihx    CALL Rsrc.bat
CALL Dash.bat
