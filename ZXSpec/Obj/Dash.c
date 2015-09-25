/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "GrApp.h"
#include "Rsrc.h"
#include "GrTiles.h"
#include "Timer.h"




static void Dash_ShowTitle (void);


/*============================================================================*/

static void Dash_ShowTitle (void)
{
	INTEGER title;
	SHORTINT titleSize, x, y;
	title = Rsrc_Open(Rsrc_Title);
	titleSize = 114;
	for (;;) {
		Timer_Start();
		if (titleSize != 0) {
			x = Rsrc_ReadByte(title);
			y = Rsrc_ReadByte(title);
			GrTiles_PutTile(x, y, Rsrc_GetTileByNum(Rsrc_ReadByte(title)));
			GrApp_Redraw();
			titleSize -= 1;
		} else {
			Rsrc_Close(title);
		}
		Timer_Until(1);
	}
}


export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(GrApp__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	Dash_ShowTitle();
	GrApp_Close();
	__FINI;
}
