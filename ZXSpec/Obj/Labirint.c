/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"
#include "Control.h"
#include "GrTiles.h"
#include "Rsrc.h"
#include "GrApp.h"
#include "Sound.h"
#include "Timer.h"

typedef
	struct Labirint_MONSTER {
		SHORTINT coordX, coordY, dirX, dirY;
		INTEGER idx;
	} Labirint_MONSTER;


static SHORTINT Labirint_Field[256];
static Labirint_MONSTER Labirint_Monsters[128];
static INTEGER Labirint_monstrsNum;
static SHORTINT Labirint_objX, Labirint_objY;
static INTEGER Labirint_objIdx;
static SHORTINT Labirint_objCell, Labirint_manX, Labirint_manY, Labirint_manDirX, Labirint_manDirY;
static INTEGER Labirint_manIdx;
static BOOLEAN Labirint_manPresent;
static INTEGER Labirint_lives, Labirint_score;

export LONGINT *Labirint_MONSTER__typ;

static void Labirint_Activate (void);
static BOOLEAN Labirint_CanFlyThru (void);
static BOOLEAN Labirint_CanRollFrom (SHORTINT side);
static BOOLEAN Labirint_CanStartFall (void);
static BOOLEAN Labirint_CheckMan (void);
static void Labirint_Deactivate (void);
static void Labirint_Display (void);
static void Labirint_DisplayInf (void);
static void Labirint_ExcludeMonster (INTEGER i);
static SHORTINT Labirint_GetCell (SHORTINT x, SHORTINT y);
static INTEGER Labirint_GetIndex (SHORTINT x, SHORTINT y);
static SHORTINT Labirint_GetManSprite (void);
static SHORTINT Labirint_GetMonsterSprite (SHORTINT sprite);
static void Labirint_LookForObjects (void);
static BOOLEAN Labirint_ManCanGo (void);
static void Labirint_ManContinuesGoing (void);
static BOOLEAN Labirint_ManGoing (SHORTINT x, SHORTINT y);
static void Labirint_MonsterContinuesGoing (Labirint_MONSTER *monstr, LONGINT *monstr__typ);
static void Labirint_NextFall (void);
export void Labirint_Play (SHORTINT room);
static void Labirint_RollFrom (SHORTINT side);
static void Labirint_SceneOfDeath (SHORTINT x, SHORTINT y);
static void Labirint_SetCell (SHORTINT x, SHORTINT y, SHORTINT cell);
static void Labirint_TryMoveMan (void);
static void Labirint_TryMoveMonster (Labirint_MONSTER *monstr, LONGINT *monstr__typ);
static void Labirint_TwoStep (INTEGER dy);
static void Labirint_Unpack (SHORTINT room);


/*============================================================================*/

static INTEGER Labirint_GetIndex (SHORTINT x, SHORTINT y)
{
	return __ASHL(__ASHR(y, 1), 4) + __ASHR(x, 1);
}

static void Labirint_SetCell (SHORTINT x, SHORTINT y, SHORTINT cell)
{
	Labirint_Field[__X(__ASHL(__ASHR(y, 1), 4) + __ASHR(x, 1), 256)] = cell;
}

static SHORTINT Labirint_GetCell (SHORTINT x, SHORTINT y)
{
	return Labirint_Field[__X(__ASHL(__ASHR(y, 1), 4) + __ASHR(x, 1), 256)];
}

static void Labirint_TwoStep (INTEGER dy)
{
	Timer_Start(1);
	dy = dy * 6;
	if (dy >= 0) {
		GrApp_ScrollUp(dy);
	} else {
		GrApp_ScrollDown(-dy);
	}
	Timer_Until();
	Timer_Start(1);
	if (dy >= 0) {
		GrApp_ScrollUp(dy);
	} else {
		GrApp_ScrollDown(-dy);
	}
}

static void Labirint_Display (void)
{
	SHORTINT x, y;
	y = 0;
	while (y < 32) {
		Labirint_TwoStep(1);
		x = 0;
		while (x < 32) {
			GrTiles_DrawTile(x, 30, Rsrc_GetTileByNum(Labirint_GetCell(x, y)));
			x += 2;
		}
		y += 2;
		Timer_Until();
	}
}

static void Labirint_Unpack (SHORTINT room)
{
	INTEGER level;
	SHORTINT shiftCount;
	INTEGER idx, val;
	level = Rsrc_OpenAt(room * 96, Rsrc_Levels);
	shiftCount = 0;
	idx = 0;
	val = Rsrc_ReadByte(level);
	do {
		Labirint_Field[__X(idx, 256)] = __MASK((int)__ASH((LONGINT)val, shiftCount), -8);
		if (shiftCount <= -5) {
			shiftCount += 5;
			val = val + __ASHL((int)Rsrc_ReadByte(level), 8);
			Labirint_Display();
			for (;;) {
			}
		} else {
			shiftCount -= 3;
		}
		idx += 1;
	} while (!(idx == 256));
	Rsrc_Close(level);
}

static void Labirint_DisplayInf (void)
{
	INTEGER i;
	Labirint_TwoStep(-1);
	Timer_Until();
	i = 1;
	while (i < 16) {
		Labirint_TwoStep(-1);
		Timer_Until();
		i += 1;
	}
	Timer_Start(18);
	Timer_Until();
}

static void Labirint_LookForObjects (void)
{
	SHORTINT x, y;
	INTEGER idx;
	Labirint_monstrsNum = 0;
	Labirint_manPresent = 0;
	idx = 256;
	y = 32;
	do {
		y -= 2;
		x = 32;
		do {
			x -= 2;
			idx -= 1;
			switch (Labirint_Field[__X(idx, 256)]) {
				case 6: case 7: 
					Labirint_Monsters[__X(Labirint_monstrsNum, 128)].coordX = x;
					Labirint_Monsters[__X(Labirint_monstrsNum, 128)].coordY = y;
					Labirint_Monsters[__X(Labirint_monstrsNum, 128)].idx = idx;
					Labirint_Monsters[__X(Labirint_monstrsNum, 128)].dirX = 0;
					Labirint_Monsters[__X(Labirint_monstrsNum, 128)].dirY = 0;
					Labirint_monstrsNum += 1;
					break;
				case 4: 
					Labirint_manPresent = 1;
					Labirint_manX = x;
					Labirint_manY = y;
					Labirint_manIdx = idx;
					break;
				default: 
					break;
			}
		} while (!(x == 0));
	} while (!(y == 0));
}

static void Labirint_SceneOfDeath (SHORTINT x, SHORTINT y)
{
	SHORTINT plusX, minusX, plusY, minusY, tX, tY;
	INTEGER ctr, lim;
	if (x > 2) {
		minusX = x - 2;
	} else {
		minusX = 0;
	}
	if (y > 2) {
		minusY = y - 2;
	} else {
		minusY = 0;
	}
	if (x > 28) {
		plusX = 30;
	} else {
		plusX = x + 2;
	}
	if (y > 28) {
		plusY = 30;
	} else {
		plusY = y + 2;
	}
	tY = minusY;
	while (tY <= plusY) {
		tX = minusX;
		while (tX <= plusX) {
			GrTiles_DrawTile(tX, tY, Rsrc_Almas);
			Labirint_SetCell(tX, tY, 3);
			tX += 2;
		}
		tY += 2;
	}
}

static void Labirint_Deactivate (void)
{
	if (Labirint_objCell == 19) {
		GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_Stone);
		Labirint_Field[__X(Labirint_objIdx, 256)] = 2;
	} else {
		GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_Almas);
		Labirint_Field[__X(Labirint_objIdx, 256)] = 3;
	}
}

static BOOLEAN Labirint_CanFlyThru (void)
{
	if (Labirint_objY == 30) {
		return 0;
	}
	switch (Labirint_Field[__X(Labirint_objIdx + 16, 256)]) {
		case 5: case 1: case 2: case 3: case 18: 
		case 20: case 19: case 21: 
			return 0;
			break;
		default: 
			break;
	}
	return 1;
}

static void Labirint_NextFall (void)
{
	switch (Labirint_Field[__X(Labirint_objIdx + 16, 256)]) {
		case 0: 
			GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_None);
			if (Labirint_objCell == 19) {
				GrTiles_DrawTile(Labirint_objX, Labirint_objY + 1, Rsrc_Stone);
				Labirint_Field[__X(Labirint_objIdx, 256)] = 18;
				Labirint_Field[__X(Labirint_objIdx + 16, 256)] = 18;
			} else {
				GrTiles_DrawTile(Labirint_objX, Labirint_objY + 1, Rsrc_Almas);
				Labirint_Field[__X(Labirint_objIdx, 256)] = 20;
				Labirint_Field[__X(Labirint_objIdx + 16, 256)] = 20;
			}
			break;
		case 7: case 6: case 17: case 16: case 4: 
		case 12: case 13: case 14: case 15: case 10: 
		case 11: case 8: case 9: 
			Labirint_SceneOfDeath(Labirint_objX, Labirint_objY + 2);
			Labirint_objX += 2;
			Labirint_objIdx += 1;
			break;
		default: 
			break;
	}
}

static BOOLEAN Labirint_CanStartFall (void)
{
	if (Labirint_objY < 30) {
		if (Labirint_Field[__X(Labirint_objIdx + 16, 256)] == 0) {
			return 1;
		}
	}
	return 0;
}

static BOOLEAN Labirint_CanRollFrom (SHORTINT side)
{
	SHORTINT x;
	INTEGER idx;
	x = Labirint_objX + side;
	idx = Labirint_objIdx + (int)__ASHR(side, 1);
	if (((x >= 0 && x <= 30) && Labirint_objY >= 0) && Labirint_objY < 30) {
		if (Labirint_GetCell(x, Labirint_objY) == 0 && Labirint_GetCell(x, Labirint_objY + 2) == 0) {
			return 1;
		}
	}
	return 0;
}

static void Labirint_RollFrom (SHORTINT side)
{
	Labirint_Field[__X(Labirint_objIdx, 256)] = 0;
	GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_None);
	Labirint_Field[__X(Labirint_objIdx + (int)__ASHR(side, 1), 256)] = Labirint_objCell;
	GrTiles_DrawTile(Labirint_objX + side, Labirint_objY, Rsrc_GetTileByNum(Labirint_objCell));
	if (side == 2) {
		Labirint_objX += 2;
		Labirint_objIdx += 1;
	}
}

static void Labirint_Activate (void)
{
	if (Labirint_objCell == 2) {
		GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_None);
		GrTiles_DrawTile(Labirint_objX, Labirint_objY + 1, Rsrc_Stone);
		Labirint_Field[__X(Labirint_objIdx, 256)] = 18;
		Labirint_Field[__X(Labirint_objIdx + 16, 256)] = 18;
	} else {
		GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_None);
		GrTiles_DrawTile(Labirint_objX, Labirint_objY + 1, Rsrc_Almas);
		Labirint_Field[__X(Labirint_objIdx, 256)] = 20;
		Labirint_Field[__X(Labirint_objIdx + 16, 256)] = 20;
	}
}

static SHORTINT Labirint_GetManSprite (void)
{
	if (Labirint_manDirX == 0) {
		switch (Labirint_manDirY) {
			case 1: 
				return 14;
				break;
			case -1: 
				return 12;
				break;
			default: 
				return 4;
				break;
		}
	}
	if (Labirint_manDirY == 0) {
		switch (Labirint_manDirX) {
			case 1: 
				return 10;
				break;
			case -1: 
				return 8;
				break;
			default: 
				return 4;
				break;
		}
	}
	return 0;
}

static BOOLEAN Labirint_ManCanGo (void)
{
	SHORTINT x, y, tX, tY, cell;
	x = (Labirint_manX + Labirint_manDirX) + Labirint_manDirX;
	y = (Labirint_manY + Labirint_manDirY) + Labirint_manDirY;
	if (((x >= 0 && x < 32) && y >= 0) && y < 32) {
		switch (Labirint_GetCell(x, y)) {
			case 5: case 18: case 20: 
				return 0;
				break;
			case 2: case 19: 
				tX = x;
				tY = y;
				for (;;) {
					if (((tX >= 0 && tX < 32) && tY >= 0) && tY < 32) {
						cell = Labirint_GetCell(tX, tY);
						if (cell != 2 && cell != 19) {
							if (cell == 0) {
								break;
							}
							return 0;
						}
						tX += Labirint_manDirX + Labirint_manDirX;
						tY += Labirint_manDirY + Labirint_manDirY;
					} else {
						return 0;
					}
				}
				Labirint_SetCell(tX, tY, 2);
				Labirint_SetCell(x, y, 0);
				GrTiles_DrawTile(tX, tY, Rsrc_Stone);
				GrTiles_DrawTile(x, y, Rsrc_None);
				break;
			default: 
				break;
		}
		return 1;
	}
	return 0;
}

static BOOLEAN Labirint_ManGoing (SHORTINT x, SHORTINT y)
{
	switch (Labirint_GetCell(x, y)) {
		case 6: case 7: case 16: case 17: 
			Labirint_SceneOfDeath(x, y);
			return 0;
			break;
		case 3: case 21: 
			Labirint_score += 1;
			break;
		default: 
			break;
	}
	return 1;
}

static void Labirint_TryMoveMan (void)
{
	SHORTINT x, y, manSprite;
	if (Labirint_ManCanGo()) {
		x = (Labirint_manX + Labirint_manDirX) + Labirint_manDirX;
		y = (Labirint_manY + Labirint_manDirY) + Labirint_manDirY;
		Labirint_manPresent = Labirint_ManGoing(x, y);
		if (!Labirint_manPresent) {
			return;
		}
		manSprite = Labirint_GetManSprite();
		Labirint_SetCell(x, y, manSprite);
		GrTiles_DrawTile(Labirint_manX, Labirint_manY, Rsrc_None);
		Labirint_manX += Labirint_manDirX;
		Labirint_manY += Labirint_manDirY;
	} else {
		manSprite = 4;
	}
	GrTiles_DrawTile(Labirint_manX, Labirint_manY, Rsrc_GetTileByNum(manSprite));
}

static void Labirint_ManContinuesGoing (void)
{
	SHORTINT plusX, minusX, plusY, minusY;
	plusX = Labirint_manX + Labirint_manDirX;
	minusX = Labirint_manX - Labirint_manDirX;
	plusY = Labirint_manY + Labirint_manDirY;
	minusY = Labirint_manY - Labirint_manDirY;
	GrTiles_DrawTile(minusX, minusY, Rsrc_None);
	if (Labirint_GetCell(minusX, minusY) == 4) {
		Labirint_SetCell(minusX, minusY, 0);
	}
	GrTiles_DrawTile(plusX, plusY, Rsrc_GetTileByNum(Labirint_GetCell(plusX, plusY)) + 1);
	Labirint_SetCell(plusX, plusY, 4);
	Labirint_manX = plusX;
	Labirint_manY = plusY;
}

static BOOLEAN Labirint_CheckMan (void)
{
	switch (Labirint_GetCell(Labirint_manX, Labirint_manY)) {
		case 4: case 12: case 14: case 8: case 10: 
		case 13: case 15: case 9: case 11: 
			return 1;
			break;
		default: 
			break;
	}
	return 0;
}

static SHORTINT Labirint_GetMonsterSprite (SHORTINT sprite)
{
	switch (sprite) {
		case 6: 
			return 16;
			break;
		case 16: 
			return 6;
			break;
		case 7: 
			return 17;
			break;
		case 17: 
			return 7;
			break;
		default: 
			break;
	}
	return 0;
}

static void Labirint_MonsterContinuesGoing (Labirint_MONSTER *monstr, LONGINT *monstr__typ)
{
	SHORTINT newX, newY, oldX, oldY, sprite;
	newX = (*monstr).coordX + (*monstr).dirX;
	oldX = (*monstr).coordX - (*monstr).dirX;
	newY = (*monstr).coordY + (*monstr).dirY;
	oldY = (*monstr).coordY - (*monstr).dirY;
	sprite = Labirint_GetMonsterSprite(Labirint_GetCell(oldX, oldY));
	Labirint_SetCell(oldX, oldY, 0);
	Labirint_SetCell(newX, newY, sprite);
	GrTiles_DrawTile(oldX, oldY, Rsrc_None);
	GrTiles_DrawTile(newX, newY, Rsrc_GetTileByNum(sprite));
	(*monstr).coordX = newX;
	(*monstr).coordY = newY;
}

static void Labirint_ExcludeMonster (INTEGER i)
{
	SHORTINT tX, tY;
	tX = Labirint_Monsters[__X(i, 128)].coordX;
	tY = Labirint_Monsters[__X(i, 128)].coordY;
	if (__ODD(tX) || __ODD(tY)) {
		tX += Labirint_Monsters[__X(i, 128)].dirX;
		tY += Labirint_Monsters[__X(i, 128)].dirY;
		Labirint_SetCell(tX, tY, 0);
		GrTiles_DrawTile(tX, tY, Rsrc_None);
	}
	Labirint_monstrsNum -= 1;
	while (i < Labirint_monstrsNum) {
		Labirint_Monsters[__X(i, 128)].coordX = Labirint_Monsters[__X(i + 1, 128)].coordX;
		Labirint_Monsters[__X(i, 128)].coordY = Labirint_Monsters[__X(i + 1, 128)].coordY;
		Labirint_Monsters[__X(i, 128)].idx = Labirint_Monsters[__X(i + 1, 128)].idx;
		Labirint_Monsters[__X(i, 128)].dirX = Labirint_Monsters[__X(i + 1, 128)].dirX;
		Labirint_Monsters[__X(i, 128)].dirY = Labirint_Monsters[__X(i + 1, 128)].dirY;
		i += 1;
	}
}

static struct TryMoveMonster__25 {
	struct TryMoveMonster__25 *lnk;
} *TryMoveMonster__25_s;

static SHORTINT Sign__26 (SHORTINT x);

static SHORTINT Sign__26 (SHORTINT x)
{
	if (x >= 0) {
		return 1;
	}
	return -1;
}

static void Labirint_TryMoveMonster (Labirint_MONSTER *monstr, LONGINT *monstr__typ)
{
	SHORTINT solutionDirX, solutionDirY, negDirX, negDirY, delta, ddX, ddY, max, mark, x, y, tX, tY, vh, sprite;
	SHORTINT DirX[4], DirY[4];
	INTEGER movesNum, i;
	struct TryMoveMonster__25 _s;
	_s.lnk = TryMoveMonster__25_s;
	TryMoveMonster__25_s = &_s;
	movesNum = 0;
	solutionDirX = 0;
	solutionDirY = 0;
	x = (*monstr).coordX;
	y = (*monstr).coordY;
	sprite = Labirint_GetMonsterSprite(Labirint_GetCell(x, y));
	delta = -1;
	do {
		vh = 0;
		do {
			tX = x;
			tY = y;
			if (vh != 0) {
				tX += delta + delta;
			} else {
				tY += delta + delta;
			}
			if (((tX >= 0 && tX <= 30) && tY >= 0) && tY <= 30) {
				switch (Labirint_GetCell(tX, tY)) {
					case 4: case 0: 
						if (vh != 0) {
							DirX[__X(movesNum, 4)] = delta;
							DirY[__X(movesNum, 4)] = 0;
						} else {
							DirX[__X(movesNum, 4)] = 0;
							DirY[__X(movesNum, 4)] = delta;
						}
						movesNum += 1;
						break;
					default: 
						break;
				}
			}
			vh += 1;
		} while (!(vh > 1));
		delta += 2;
	} while (!(delta > 1));
	if (movesNum == 0) {
		Labirint_SetCell(x, y, sprite);
		GrTiles_DrawTile(x, y, Rsrc_GetTileByNum(sprite));
	} else {
		if (movesNum == 1) {
			solutionDirX = DirX[0];
			solutionDirY = DirY[0];
		} else {
			negDirX = -(*monstr).dirX;
			negDirY = -(*monstr).dirY;
			i = 0;
			for (;;) {
				if (DirX[__X(i, 4)] == negDirX && DirY[__X(i, 4)] == negDirY) {
					movesNum -= 1;
					while (i < movesNum) {
						DirX[__X(i, 4)] = DirX[__X(i + 1, 4)];
						DirY[__X(i, 4)] = DirY[__X(i + 1, 4)];
						i += 1;
					}
					break;
				}
				i += 1;
				if (i >= movesNum) {
					break;
				}
			}
			if (movesNum == 1) {
				solutionDirX = DirX[0];
				solutionDirY = DirY[0];
			} else {
				ddX = Sign__26(Labirint_manX - x);
				ddY = Sign__26(Labirint_manY - y);
				max = 0;
				i = 0;
				do {
					mark = __ABS(DirX[__X(i, 4)] + ddX) + __ABS(DirY[__X(i, 4)] + ddY);
					if (mark > max) {
						max = mark;
						solutionDirX = DirX[__X(i, 4)];
						solutionDirY = DirY[__X(i, 4)];
					}
					i += 1;
				} while (!(i >= movesNum));
			}
		}
		(*monstr).dirX = solutionDirX;
		(*monstr).coordX += solutionDirX;
		(*monstr).dirY = solutionDirY;
		(*monstr).coordY += solutionDirY;
		GrTiles_DrawTile(x, y, Rsrc_None);
		GrTiles_DrawTile((*monstr).coordX, (*monstr).coordY, Rsrc_GetTileByNum(sprite));
		Labirint_SetCell((*monstr).coordX + (*monstr).dirX, (*monstr).coordY + (*monstr).dirY, sprite);
	}
	TryMoveMonster__25_s = _s.lnk;
}

void Labirint_Play (SHORTINT room)
{
	SHORTINT x, y, side;
	INTEGER number;
	SET keys;
	GrApp_Cls();
	Labirint_Unpack(room);
	Labirint_Display();
	return;
	Labirint_LookForObjects();
	if (!Labirint_manPresent || Labirint_monstrsNum == 0) {
		return;
	}
	side = 2;
	for (;;) {
		Timer_Start(2);
		if (__ODD(Labirint_manX) || __ODD(Labirint_manY)) {
			Labirint_ManContinuesGoing();
		} else {
			keys = Control_Get();
			if (keys == 0x08) {
				Labirint_manDirX = 0;
				Labirint_manDirY = -1;
				Labirint_TryMoveMan();
			} else if (keys == 0x04) {
				Labirint_manDirX = 0;
				Labirint_manDirY = 1;
				Labirint_TryMoveMan();
			} else if (keys == 0x02) {
				Labirint_manDirX = -1;
				Labirint_manDirY = 0;
				Labirint_TryMoveMan();
			} else if (keys == 0x01) {
				Labirint_manDirX = 1;
				Labirint_manDirY = 0;
				Labirint_TryMoveMan();
			} else if (keys == 0x20) {
				Labirint_manPresent = 0;
				break;
			} else {
				Labirint_manDirX = 0;
				Labirint_manDirY = 0;
			}
			if (!Labirint_manPresent) {
				break;
			}
		}
		number = 0;
		Labirint_objIdx = 240;
		Labirint_objY = 32;
		while (Labirint_objY > 0) {
			Labirint_objY -= 2;
			Labirint_objX = 0;
			while (Labirint_objX < 32) {
				Labirint_objCell = Labirint_Field[__X(Labirint_objIdx, 256)];
				switch (Labirint_objCell) {
					case 3: case 20: case 21: 
						number += 1;
						break;
					default: 
						break;
				}
				switch (Labirint_objCell) {
					case 3: case 2: 
						if (Labirint_CanStartFall()) {
							Labirint_Activate();
						} else {
							side = -side;
							if (Labirint_CanRollFrom(side)) {
								Labirint_RollFrom(side);
							} else if (Labirint_CanRollFrom(-side)) {
								Labirint_RollFrom(-side);
							}
						}
						break;
					case 21: case 19: 
						if (Labirint_CanFlyThru()) {
							Labirint_NextFall();
						} else {
							Labirint_Deactivate();
						}
						break;
					case 18: 
						GrTiles_DrawTile(Labirint_objX, Labirint_objY - 2, Rsrc_None);
						GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_Stone);
						Labirint_Field[__X(Labirint_objIdx, 256)] = 19;
						Labirint_Field[__X(Labirint_objIdx - 16, 256)] = 0;
						break;
					case 20: 
						GrTiles_DrawTile(Labirint_objX, Labirint_objY - 2, Rsrc_None);
						GrTiles_DrawTile(Labirint_objX, Labirint_objY, Rsrc_Almas);
						Labirint_Field[__X(Labirint_objIdx, 256)] = 21;
						Labirint_Field[__X(Labirint_objIdx - 16, 256)] = 0;
						break;
					default: 
						break;
				}
				Labirint_objX += 2;
				Labirint_objIdx += 1;
			}
			Labirint_objIdx -= 32;
		}
		Labirint_manPresent = Labirint_CheckMan();
		if (!Labirint_manPresent) {
			break;
		}
		if (number == 0) {
			Timer_Until();
			Labirint_ManContinuesGoing();
			break;
		}
		number = 0;
		while (number < Labirint_monstrsNum) {
			x = Labirint_Monsters[__X(number, 128)].coordX;
			y = Labirint_Monsters[__X(number, 128)].coordY;
			if (__ODD(x)) {
				x -= Labirint_Monsters[__X(number, 128)].dirX;
			}
			if (__ODD(y)) {
				y -= Labirint_Monsters[__X(number, 128)].dirY;
			}
			switch (Labirint_GetCell(x, y)) {
				case 6: case 7: case 16: case 17: 
					if (__ODD(Labirint_Monsters[__X(number, 128)].coordX) || __ODD(Labirint_Monsters[__X(number, 128)].coordY)) {
						Labirint_MonsterContinuesGoing(&Labirint_Monsters[__X(number, 128)], Labirint_MONSTER__typ);
					} else {
						Labirint_TryMoveMonster(&Labirint_Monsters[__X(number, 128)], Labirint_MONSTER__typ);
					}
					number += 1;
					break;
				default: 
					Labirint_ExcludeMonster(number);
					break;
			}
		}
		Labirint_manPresent = Labirint_CheckMan();
		if (!Labirint_manPresent) {
			Labirint_SceneOfDeath(Labirint_manX, Labirint_manY);
		}
		Timer_Until();
		if (!Labirint_manPresent) {
			break;
		}
	}
	Labirint_DisplayInf();
}

/*----------------------------------------------------------------------------*/
__TDESC(Labirint_MONSTER__desc, 1, 0) = {__TDFLDS("MONSTER", 6), {-4}};

export void *Labirint__init(void)
{
	__DEFMOD;
	__IMPORT(Control__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Rsrc__init);
	__IMPORT(GrApp__init);
	__IMPORT(Sound__init);
	__IMPORT(Timer__init);
	__REGMOD("Labirint", 0);
	__INITYP(Labirint_MONSTER, Labirint_MONSTER, 0);
/* BEGIN */
	__ENDMOD;
}
