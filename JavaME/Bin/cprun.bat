@echo off
REM run the class file
java -DCPSYM=.;%JRoot%\libs;%JRoot%\libs\JvmSystem;"%CD%\Libs" -cp .;%JRoot% CP.%1.%1 %2 %3 %4 %5 %6 %7 %8 %9
