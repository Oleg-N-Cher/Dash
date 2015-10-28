:: - CLDC 1.1 и CDC 1.1 есть подмножества 1.4.2
::javac -source 1.4 -target 1.4 -d . CPJrts.java
javac -source 1.2 -target 1.1 -bootclasspath "c:\Program Files\Java\jre7\lib\rt.jar" -d . XHR.java
@IF errorlevel 1 PAUSE
