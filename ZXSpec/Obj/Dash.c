/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "Control.h"
#include "GrTiles.h"
#include "Console.h"
#include "Msg.h"
#include "Rsrc.h"
#include "GrApp.h"
#include "Sound.h"
#include "Strings.h"
#include "Timer.h"




static void Dash_Center (SHORTINT y, CHAR *str, LONGINT str__len);
static void Dash_Instruction (CHAR *text, LONGINT text__len);
static SHORTINT Dash_MenuSelect (void);
static void Dash_PlayGame (void);
static void Dash_PressAnyKey (void);
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
	CHAR key;
	SET keys;
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
		key = Dash_ReadKeyBeep();
		keys = Control_Get();
		if (keys == 0x08 || keys == 0x02) {
			if (item != 0) {
				Console_At(8, 6 + item);
				Console_WriteStr((void*)&"  ", (LONGINT)3);
				item -= 1;
			}
		}
		if (keys == 0x04 || keys == 0x01) {
			if (item != 3) {
				Console_At(8, 6 + item);
				Console_WriteStr((void*)&"  ", (LONGINT)3);
				item += 1;
			}
		}
		if ((keys == 0x10 || key == ' ') || key == 0x0d) {
			return item;
		}
	}
	__RETCHK;
}

static void Dash_Center (SHORTINT y, CHAR *str, LONGINT str__len)
{
	SHORTINT i, x, len;
	len = (int)Strings_Length((void*)str, str__len);
	x = 16 - __ASHR(len, 1);
	Console_At(0, y);
	i = x;
	while (i >= 1) {
		Console_WriteCh(' ');
		i += -1;
	}
	Console_WriteStr((void*)str, str__len);
	x += len;
	while (x < 31) {
		Console_WriteCh(' ');
		x += 1;
	}
}

static void Dash_PressAnyKey (void)
{
	Dash_Center(15, (void*)&"Press any key ...", (LONGINT)18);
	if (Dash_ReadKeyBeep() == 0x00) {
	}
}

static void Dash_Instruction (CHAR *text, LONGINT text__len)
{
	INTEGER i;
	CHAR ch;
	i = 0;
	ch = text[__X(i, text__len)];
	while (ch != 0x00) {
		switch (ch) {
			case '#': 
				GrApp_Cls();
				Console_At(1, 0);
				break;
			case '^': 
				Console_WriteLn();
				Console_WriteCh(' ');
				break;
			case '@': 
				Console_WriteLn();
				Dash_PressAnyKey();
				Console_At(1, 15);
				break;
			default: 
				Console_WriteCh(ch);
				break;
		}
		i += 1;
		ch = text[__X(i, text__len)];
	}
}

static void Dash_PlayGame (void)
{
	for (;;) {
		switch (Dash_MenuSelect()) {
			case 0: 
				break;
			case 1: 
				break;
			case 2: 
				Dash_Instruction((void*)&"#  Bolder (boulder) Dash  is  a^chain of  puzzle-caverns.  The^cavern   consists   of   16x16^cells.  Each cell may be  of 8^cases:^  1.  Empty cell.  The man and^other  moving objects  can  go^thru this cells.", (LONGINT)211);
				Dash_Instruction((void*)&"^  2.  Land  cell   (usually  a^grass or a dirt). Only man can^dig thru this cells  and makes^its empty.^  3.     Falling    invaluable^objects  (usually  a  stones).^Can fall down and roll to side@of  falling.  Can  kill living^objects.", (LONGINT)238);
				Dash_Instruction((void*)&"  The man can push any^chains only of this ones.^  4.  Falling valuable objects^  (usually   a   diamonds   or^emeralds).  Can fall, roll and^kill (see 3).  The man can NOT^push them,  but can  and  MUST@eat   ALL   of  this  ones  to^perform   his", (LONGINT)249);
				Dash_Instruction((void*)&"   task   in  the^cavern.^  5.  The MAN  (in form of ant^or  real  man).   He  is  user^ (player) - controlled  living^object.   Must  collect  (eat)^valuable     objects,     push@invaluable  objects  (to  kill^other  living  objects),   and^escape", (LONGINT)250);
				Dash_Instruction((void*)&"  away  from all objects^falling   over   him  and  all^living  objects  chasing   for^him.^  6.  The  wall  (of  cemented^stones, bricks, wood  or other@hard stuff).  All  living  and^falling objects (including the^man) can\'t go thru it.", (LONGINT)239);
				Dash_Instruction((void*)&"^  7.   Living    and   chasing^objects  (two types, usually a^batterflies     and    pulsing^squares, or other pairs).  Can^kill the man. Can be killed by@falling object.  Can move only^in empty cells.^-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_", (LONGINT)234);
				Dash_Instruction((void*)&"^  Standard Bolder Dash set has^76 (0..75) caverns ...@", (LONGINT)56);
				break;
			default: 
				GrApp_Cls();
				Dash_Center(7, (void*)&"Press Y if You sure", (LONGINT)20);
				switch (Dash_ReadKeyBeep()) {
					case 'Y': case 'y': 
						return;
						break;
					default: 
						break;
				}
				break;
		}
	}
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
	__IMPORT(Strings__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	Dash_ShowTitle();
	Dash_PlayGame();
	Control_Close();
	GrApp_Close();
	__FINI;
}
