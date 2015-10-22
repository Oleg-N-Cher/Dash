#ifndef Rsrc__h
#define Rsrc__h

#include "SYSTEM.h"


#define Rsrc_TitleSize     114 /* cells, a cell occupies 3 bytes */
#define Rsrc_TileSize       24 /* bytes */
#define Rsrc_TileWidth      16 /* pixels */
#define Rsrc_TileHeight     12 /* pixels */
#define Rsrc_MaxLevelNumber 75
#define Rsrc_LevelSize      96 /* bytes */
#define Rsrc_CharSize       12 /* bytes */
#define Rsrc_FontSize       (96 * Rsrc_CharSize) // bytes
#define _LastTitleNum       17

extern const unsigned char _Rsrc_Title [Rsrc_TitleSize * 3];
extern const unsigned char _Rsrc_Levels [7296];
extern const unsigned char _Rsrc_Font [Rsrc_FontSize];
extern const unsigned char _Rsrc_Tiles [Rsrc_TileSize * (_LastTitleNum + 1)];

typedef BYTE* Rsrc_Resource;
typedef unsigned int ADDRESS;

/* Set of game tiles: */
#define Rsrc_None      ((Rsrc_Resource)( 0 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Grass     ((Rsrc_Resource)( 1 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Stone     ((Rsrc_Resource)( 2 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Almas     ((Rsrc_Resource)( 3 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_StopMan   ((Rsrc_Resource)( 4 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Wall      ((Rsrc_Resource)( 5 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Mina      ((Rsrc_Resource)( 6 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Babo      ((Rsrc_Resource)( 7 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_LeftMan   ((Rsrc_Resource)( 8 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_LeftMan1  ((Rsrc_Resource)( 9 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_RightMan  ((Rsrc_Resource)(10 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_RightMan1 ((Rsrc_Resource)(11 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_UpMan     ((Rsrc_Resource)(12 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_UpMan1    ((Rsrc_Resource)(13 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_DownMan   ((Rsrc_Resource)(14 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_DownMan1  ((Rsrc_Resource)(15 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Mina1     ((Rsrc_Resource)(16 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_Babo1     ((Rsrc_Resource)(17 * Rsrc_TileSize + &_Rsrc_Tiles))
#define Rsrc_LastTile  Rsrc_Babo1

#define Rsrc_Title  ((int)_Rsrc_Title)
#define Rsrc_Tiles   __Rsrc_Tiles
#define Rsrc_Levels ((int)_Rsrc_Levels)
#define Rsrc_Font    __Rsrc_Font

#define Rsrc_Close(rsrc)
extern Rsrc_Resource Rsrc_GetTileByNum (unsigned char num);
#define Rsrc_Open(name) ((ADDRESS)name)
#define Rsrc_OpenAt(pos,name) ((ADDRESS)name + pos)
#define Rsrc_ReadByte(rsrc) (*(BYTE*)rsrc++)

#define Rsrc__init()


#endif
