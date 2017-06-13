@FOR %%i IN (*.cps) DO CALL cprun Browse -html -sort %%i
@PAUSE
@DEL *.html
