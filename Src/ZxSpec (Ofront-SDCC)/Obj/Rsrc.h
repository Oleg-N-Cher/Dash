/*  Ofront 1.2 -xtspkae */

#ifndef Rsrc__h
#define Rsrc__h

#include "SYSTEM.h"


import INTEGER Rsrc_Title, Rsrc_Sprites, Rsrc_Levels, Rsrc_Font;


import void Rsrc_Close (INTEGER rsrc);
import SHORTINT Rsrc_GetTileByNum (SHORTINT num);
import INTEGER Rsrc_Open (INTEGER name);
import SHORTINT Rsrc_ReadByte (INTEGER rsrc);
import void *Rsrc__init(void);


#endif
