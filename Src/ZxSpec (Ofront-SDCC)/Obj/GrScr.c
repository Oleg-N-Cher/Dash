/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"




export void GrScr_Close (void);
export void GrScr_ClrScr (void);
export void GrScr_Repaint (void);


void GrScr_ClrScr (void)
{
}

void GrScr_Close (void)
{
}

void GrScr_Repaint (void)
{
}


export void *GrScr__init(void)
{
	__DEFMOD;
	__REGMOD("GrScr", 0);
	__REGCMD("Close", GrScr_Close);
	__REGCMD("ClrScr", GrScr_ClrScr);
	__REGCMD("Repaint", GrScr_Repaint);
/* BEGIN */
	__ENDMOD;
}
