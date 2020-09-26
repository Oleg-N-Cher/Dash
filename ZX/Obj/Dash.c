/* Ofront+ 1.0 -mx3 -21 */
#include "SYSTEM.oh"
#include "GrApp.oh"
#include "Console.oh"
#include "GrTiles.oh"
#include "Labirint.oh"
#include "Msg.oh"
#include "Rsrc.oh"
#include "Control.oh"
#include "Timer.oh"
#include "Sound.oh"
#include "Strings.oh"
#include "Config.oh"


static BYTE Dash_controlMethodN, Dash_level;


static void Dash_Center (BYTE y, CHAR *str, SHORTINT str__len);
static BOOLEAN Dash_Instruction (CHAR *text, SHORTINT text__len);
static BYTE Dash_MenuSelect (void);
static void Dash_PlayGame (void);
static void Dash_PressAnyKey (void);
static CHAR Dash_ReadKeyBeep (void);
static void Dash_SetJoystickCtrl (void);
static void Dash_ShowTitle (void);


/*============================================================================*/

static void Dash_ShowTitle (void)
{
	SHORTINT title;
	BYTE titleSize, x, y;
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

static void Dash_Center (BYTE y, CHAR *str, SHORTINT str__len)
{
	BYTE i, x, len, _for__2;
	len = (BYTE)Strings_Length((void*)str, str__len);
	x = 16 - __ASHR(len, 1, BYTE);
	Console_At(0, y);
	i = x;
	if (i >= 1) {
		_for__2 = (__U_BYTE)(i - 1) + 1;
		do {
			Console_WriteCh(' ');
			i += -1;
		} while (--_for__2);
	}
	Console_WriteStr((void*)str, str__len);
}

static void Dash_SetJoystickCtrl (void)
{
	Console_At(11, 7);
	Dash_controlMethodN = __MASK(Dash_controlMethodN + 1, -4);
	switch (Dash_controlMethodN) {
		case 0: 
			Console_WriteStr((CHAR*)"KEYBOARD", 9);
			break;
		case 1: 
			Console_WriteStr((CHAR*)"KEMPSTON", 9);
			break;
		case 2: 
			Console_WriteStr((CHAR*)"SINCLAIR", 9);
			break;
		default: 
			Console_WriteStr((CHAR*)"CURSOR  ", 9);
			break;
	}
	Control_Select(Dash_controlMethodN);
}

static BYTE Dash_MenuSelect (void)
{
	BYTE item;
	CHAR key;
	SET keys;
	GrApp_Cls();
	Console_Ink(5);
	Dash_Center(15, (CHAR*)"VEDAsoft Oberon Club \'2019", 27);
	Console_Ink(71);
	Console_At(11, 6);
	Console_WriteStr((CHAR*)"PLAY", 5);
	Dash_controlMethodN -= 1;
	Dash_SetJoystickCtrl();
	Console_At(11, 8);
	Console_WriteStr((CHAR*)"HELP", 5);
	Console_At(11, 9);
	Console_WriteStr((CHAR*)"EXIT TO BASIC", 14);
	item = 0;
	for (;;) {
		Console_At(8, 6 + item);
		Console_WriteStr((CHAR*)"=>", 3);
		key = Dash_ReadKeyBeep();
		keys = Control_Get();
		if (keys == 0x08 || keys == 0x02) {
			if (item != 0) {
				Console_At(8, 6 + item);
				Console_WriteStr((CHAR*)"  ", 3);
				item -= 1;
			}
		}
		if (keys == 0x04 || keys == 0x01) {
			if (item != 3) {
				Console_At(8, 6 + item);
				Console_WriteStr((CHAR*)"  ", 3);
				item += 1;
			}
		}
		if ((keys == 0x10 || key == ' ') || key == 0x0d) {
			if (item != 1) {
				return item;
			}
			Dash_SetJoystickCtrl();
		}
		if (keys == 0x20) {
			return -1;
		}
	}
	__RETCHK((CHAR*)"Dash", -6763);
}

static void Dash_PressAnyKey (void)
{
	Dash_Center(15, (CHAR*)"Press any key ...", 18);
	if (Dash_ReadKeyBeep() == 0x00) {
	}
}

static BOOLEAN Dash_Instruction (CHAR *text, SHORTINT text__len)
{
	SHORTINT i;
	CHAR ch;
	i = 0;
	ch = text[i];
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
				if (Control_Get() == 0x20) {
					return 0;
				}
				Console_At(1, 15);
				break;
			default: 
				Console_WriteCh(ch);
				break;
		}
		i += 1;
		ch = text[i];
	}
	return 1;
}

static void Dash_PlayGame (void)
{
	Dash_ShowTitle();
	if (Control_Get() != 0x20) {
		for (;;) {
			switch (Dash_MenuSelect()) {
				case 0: 
					Dash_level = 0;
					for (;;) {
						Labirint_Play(Dash_level);
						Dash_level += 1;
					}
					break;
				case 2: 
					if (!Dash_Instruction((CHAR*)"#  Bolder (boulder) Dash  is  a^chain of  puzzle-caverns.  The^cavern   consists   of   16x16^cells.  Each cell may be  of 8^cases:^  1.  Empty cell.  The man and^other  moving objects  can  go^thru this cells.", 211)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"^  2.  Land  cell   (usually  a^grass or a dirt). Only man can^dig thru this cells  and makes^its empty.^  3.     Falling    invaluable^objects  (usually  a  stones).^Can fall down and roll to side@of  falling.  Can  kill living^objects.", 238)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"  The man can push any^chains only of this ones.^  4.  Falling valuable objects^  (usually   a   diamonds   or^emeralds).  Can fall, roll and^kill (see 3).  The man can NOT^push them,  but can  and  MUST@eat   ALL   of  this  ones  to^perform   his", 249)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"   task   in  the^cavern.^  5.  The MAN  (in form of ant^or  real  man).   He  is  user^ (player) - controlled  living^object.   Must  collect  (eat)^valuable     objects,     push@invaluable  objects  (to  kill^other  living  objects),   and^escape", 250)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"  away  from all objects^falling   over   him  and  all^living  objects  chasing   for^him.^  6.  The  wall  (of  cemented^stones, bricks, wood  or other@hard stuff).  All  living  and^falling objects (including the^man) can\'t go thru it.", 239)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"^  7.   Living    and   chasing^objects  (two types, usually a^batterflies     and    pulsing^squares, or other pairs).  Can^kill the man. Can be killed by@falling object.  Can move only^in empty cells.^-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_", 234)) {
						return;
					}
					if (!Dash_Instruction((CHAR*)"^  Standard Bolder Dash set has^76 (0..75) caverns ...@", 56)) {
						return;
					}
					break;
				case -1: 
					return;
					break;
				default: 
					GrApp_Cls();
					Dash_Center(7, (CHAR*)"Press Y if You sure", 20);
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
}


int main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(Config__init);
	__IMPORT(Control__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Console__init);
	__IMPORT(Labirint__init);
	__IMPORT(Msg__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrApp__init);
	__IMPORT(Sound__init);
	__IMPORT(Strings__init);
	__IMPORT(Timer__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	Dash_controlMethodN = 3;
	Dash_PlayGame();
	Control_Close();
	GrApp_Close();
	__FINI;
}
