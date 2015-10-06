#ifndef GrApp__h
#define GrApp__h


extern unsigned char GrApp_ink, GrApp_paper;

extern void GrApp_FillArea (unsigned char color);
extern void GrApp_SetPalette (void);
#define GrApp_Redraw()
extern void GrApp_Close (void);

extern void *GrApp__init(void);


#endif

