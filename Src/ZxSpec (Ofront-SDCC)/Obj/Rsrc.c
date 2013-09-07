/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"


export INTEGER Rsrc_Title, Rsrc_Sprites, Rsrc_Levels, Rsrc_Font;


export void Rsrc_Close (INTEGER rsrc);
export SHORTINT Rsrc_GetTileByNum (SHORTINT num);
export INTEGER Rsrc_Open (INTEGER name);
export SHORTINT Rsrc_ReadByte (INTEGER rsrc);


INTEGER Rsrc_Open (INTEGER name)
{
	return 0;
}

SHORTINT Rsrc_ReadByte (INTEGER rsrc)
{
	return 0;
}

void Rsrc_Close (INTEGER rsrc)
{
}

SHORTINT Rsrc_GetTileByNum (SHORTINT num)
{
	return 0;
}


export void *Rsrc__init(void)
{
	__DEFMOD;
	__REGMOD("Rsrc", 0);
/* BEGIN */
	__ENDMOD;
}
