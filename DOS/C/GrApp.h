#ifndef GrApp__h
#define GrApp__h


extern unsigned char GrApp_ink, GrApp_paper;

extern void GrApp_Cls (void);
extern void GrApp_SetPalette (void);
extern void GrApp_ScrollDown (unsigned char lines);
extern void GrApp_ScrollUp (unsigned char lines);
#define GrApp_Redraw()
extern void GrApp_Close (void);

extern void *GrApp__init(void);


#endif

