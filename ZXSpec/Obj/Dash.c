/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "Control.h"
#include "GrTiles.h"
#include "Rsrc.h"
#include "GrApp.h"
#include "Sound.h"
#include "Timer.h"




static CHAR Dash_ReadKeyBeep (void);
static void Dash_ShowTitle (void);


/*============================================================================*/

static void Dash_ShowTitle (void)
{
	INTEGER title;
	SHORTINT titleSize, x, y;
	title = Rsrc_Open(Rsrc_Title);
	titleSize = 114;
	for (;;) {
		Timer_Start(3);
		if (titleSize != 0) {
			titleSize -= 1;
			x = Rsrc_ReadByte(title);
			y = Rsrc_ReadByte(title);
			GrTiles_PutTile(x, y, Rsrc_GetTileByNum(Rsrc_ReadByte(title)));
			GrApp_Redraw();
		} else {
			Rsrc_Close(title);
		}
		Sound_TitleNext();
		if (Control_PressedAnyKey()) {
			GrApp_Cls();
			return;
		}
		Timer_Until();
	}
}

static CHAR Dash_ReadKeyBeep (void)
{
	CHAR key;
	key = Control_ReadKey();
	Sound_KeyPressed();
	return key;
}


export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(Control__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrApp__init);
	__IMPORT(Sound__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	Dash_ShowTitle();
	GrApp_Close();
	__FINI;
}
