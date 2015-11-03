/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "GrTiles.h"
#include "Labirint.h"
#include "Rsrc.h"
#include "GrApp.h"
#include "Timer.h"


static SHORTINT Dash_level;


static void Dash_ShowTitle (void);


/*============================================================================*/

static void Dash_ShowTitle (void)
{
	LONGINT title;
	SHORTINT titleSize, x, y;
	title = Rsrc_Open(Rsrc_Title);
	titleSize = 114;
	for (;;) {
		Timer_Start(1);
		if (titleSize != 0) {
			titleSize -= 1;
			x = Rsrc_ReadByte(title);
			y = Rsrc_ReadByte(title);
			GrTiles_DrawTile(x, y, Rsrc_GetTileByNum(Rsrc_ReadByte(title)));
			GrApp_Redraw();
		} else {
			Rsrc_Close(title);
			return;
		}
		Timer_Until();
	}
}


export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(GrTiles__init);
	__IMPORT(Labirint__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrApp__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	Dash_level = 36;
	for (;;) {
		Labirint_Play(Dash_level);
		Dash_level += 1;
	}
	GrApp_Redraw();
	__FINI;
}
