/*Version for CGA 320x200 4 colors*/

#include "Config.h"

void GrTiles_DrawTile (unsigned char x, unsigned char y, unsigned char* tile);
/*================================== Header ==================================*/

void GrTiles_DrawTile (unsigned char x, unsigned char y, unsigned char* tile) {
/* void sprite (unsigned x, unsigned y, spr_t z) */
  unsigned long * lsp;
  unsigned long _es * vbase;
  int i;

  if (x > 30 || y > 30) return;

  lsp = (unsigned long *)tile;
    /*sprite offset*/
  vbase = (unsigned long _es *)((x << 1) + (((y << 4) - y) << 4) + 168);
    /*video offset*/
  _ES = 0xB800;

/*access to rectangle 256x192 centered on screen*/
    /*Storing in even and odd banks*/
  for (i = 0; i < 12; i += 2, vbase += 20) {
    vbase[0] = *lsp ++; vbase[2048] = *lsp ++;
  }
} /*GrTiles_DrawTile*/
