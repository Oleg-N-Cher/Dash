#ifndef Control__h
#define Control__h


extern unsigned char Control_Get (void);
extern unsigned char Control_ReadKey (void); // With auto-repeat
extern unsigned char Control_PressedAnyKey (void);
#define Control_Close()

void Control__init (void);


#endif
