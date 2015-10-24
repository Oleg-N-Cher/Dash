#ifndef __Config
#define __Config


/* High byte-1 of the fast-screen-access table generated into memory by GrApp.BEGIN */

#define Config_ScreenTable  0xFC  /* This memory #FC00; #FD00 used for screen table. */

//#define Config_CheckTileCoords
#define Config_FastTiles
#define Config_UseSound


#define Config__init()


#endif
