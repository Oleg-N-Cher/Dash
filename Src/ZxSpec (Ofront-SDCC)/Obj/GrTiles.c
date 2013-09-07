/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"
#include "GrScr.h"
#include "Platform.h"




export void GrTiles_PutTile (INTEGER x, INTEGER y, INTEGER tile);


void GrTiles_PutTile (INTEGER x, INTEGER y, INTEGER tile)
{
}


export void *GrTiles__init(void)
{
	__DEFMOD;
	__IMPORT(GrScr__init);
	__IMPORT(Platform__init);
	__REGMOD("GrTiles", 0);
/* BEGIN */
	__ENDMOD;
}
