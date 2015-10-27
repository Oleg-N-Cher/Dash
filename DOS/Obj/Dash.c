/*  Ofront 1.2 -xtspkaem */
#include "SYSTEM.h"
#include "GrApp.h"
#include "Timer.h"
#include "GrTiles.h"
#include "Rsrc.h"






/*============================================================================*/


export main(int argc, char **argv)
{
	__INIT(argc, argv);
	__IMPORT(GrApp__init);
	__IMPORT(Timer__init);
	__IMPORT(GrTiles__init);
	__IMPORT(Rsrc__init);
	__REGMAIN("Dash", 0);
/* BEGIN */
	GrTiles_DrawTile(1, 6, Rsrc_Grass);
	GrTiles_DrawTile(3, 6, Rsrc_Wall);
	GrTiles_DrawTile(5, 6, Rsrc_UpMan);
	GrTiles_DrawTile(7, 6, Rsrc_Wall);
	GrTiles_DrawTile(9, 6, Rsrc_UpMan1);
	GrTiles_DrawTile(11, 6, Rsrc_Wall);
	GrTiles_DrawTile(1, 9, Rsrc_Wall);
	GrTiles_DrawTile(3, 9, Rsrc_DownMan);
	GrTiles_DrawTile(5, 9, Rsrc_Wall);
	GrTiles_DrawTile(7, 9, Rsrc_DownMan1);
	GrTiles_DrawTile(9, 9, Rsrc_Wall);
	GrTiles_DrawTile(11, 9, Rsrc_None);
	GrTiles_DrawTile(1, 12, Rsrc_Wall);
	GrTiles_DrawTile(3, 12, Rsrc_Mina1);
	GrTiles_DrawTile(5, 12, Rsrc_Wall);
	GrTiles_DrawTile(7, 12, Rsrc_Babo1);
	GrTiles_DrawTile(9, 12, Rsrc_Wall);
	GrTiles_DrawTile(11, 12, Rsrc_None);
	Timer_Start(100);
	Timer_Until();
	GrApp_Close();
	__FINI;
}
