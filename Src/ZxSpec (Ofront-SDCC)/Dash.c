/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "GrScr.h"
#include "Platform.h"
#include "Rsrc.h"
#include "GrTiles.h"


static INTEGER Dash_title;
static SHORTINT Dash_titleSize, Dash_x, Dash_y;





export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(GrScr__init);
	__IMPORT(Platform__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrTiles__init);
	__REGMAIN("Dash", 0);
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
	__FINI;
}
