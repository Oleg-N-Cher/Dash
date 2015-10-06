#ifndef Control__h
#define Control__h


extern unsigned char Control_ReadKey (void); // With auto-repeat
extern unsigned char Control_Pressed (unsigned char key);
extern unsigned char Control_PressedAnyKey (void);
#define Control_Close()

#define Control__init()


#endif
