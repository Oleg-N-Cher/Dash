MODULE Config;
(*=======================================================================
  32832       8040      Code start address (end of contended mem + Stack)
  56488       DCA8      Data start address
  64512-64959 FC00-FDBF Table of screen address {loByteScr; hiByteScr)
  65024-65280 FE00-FF00 Table of interrupt vectors for IM 2 mode
=======================================================================*)

CONST
  TargetJme * = FALSE;
  TargetDOS * = FALSE;
  TargetZX  * = TRUE;

END Config.
