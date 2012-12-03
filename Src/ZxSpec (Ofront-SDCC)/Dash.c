/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"
#include "GrScr.h"
#include "Platform.h"
#include "Rsrc.h"
#include "GrTiles.h"


static INTEGER Dash_title;
static SHORTINT Dash_titleSize, Dash_x, Dash_y;





export void *Dash__init(void)
{
	__DEFMOD;
	__IMPORT(GrScr);
	__IMPORT(Platform);
	__IMPORT(Rsrc);
	__IMPORT(GrTiles);
	__REGMOD("Dash", 0);
/* BEGIN */
	Dash_title = Rsrc_Open(Rsrc_Title);
	Dash_titleSize = 114;
	do {
		Dash_x = Rsrc_ReadByte(Dash_title);
		Dash_y = Rsrc_ReadByte(Dash_title);
		GrTiles_PutTile(Dash_x, Dash_y, Rsrc_GetTileByNum(Rsrc_ReadByte(Dash_title)));
		Dash_titleSize -= 1;
	} while (!(Dash_titleSize == 0));
	Rsrc_Close(Dash_title);
	GrScr_Repaint();
	GrScr_Close();
	__ENDMOD;
}
