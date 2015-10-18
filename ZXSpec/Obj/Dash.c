/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "Control.h"
#include "GrTiles.h"
#include "Console.h"
#include "Msg.h"
#include "Rsrc.h"
#include "GrApp.h"
#include "Sound.h"
#include "Timer.h"




static SHORTINT Dash_MenuSelect (void);
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
			GrTiles_DrawTile(x, y, Rsrc_GetTileByNum(Rsrc_ReadByte(title)));
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

static SHORTINT Dash_MenuSelect (void)
{
	SHORTINT item;
	GrApp_Cls();
	Console_Ink(71);
	Console_At(11, 6);
	Console_WriteStr((void*)&"PLAY", (LONGINT)5);
	Console_At(11, 7);
	Console_WriteStr((void*)&"KEYBOARD", (LONGINT)9);
	Console_At(11, 8);
	Console_WriteStr((void*)&"HELP", (LONGINT)5);
	Console_At(11, 9);
	Console_WriteStr((void*)&"EXIT TO BASIC", (LONGINT)14);
	item = 0;
	for (;;) {
		Console_At(8, 6 + item);
		Console_WriteStr((void*)&"=>", (LONGINT)3);
		return 0;
	}
	__RETCHK;
}


export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(Control__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Console__init);
	__IMPORT(Msg__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrApp__init);
	__IMPORT(Sound__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	if (Dash_MenuSelect() == 0) {
	}
	for (;;) {
	}
	Control_Close();
	GrApp_Close();
	__FINI;
}
