#ifndef Control__h
#define Control__h


extern unsigned char Control_Get (void);
extern unsigned char Control_GetCustom (void);
extern unsigned char Control_ReadKey (void); // With auto-repeat
extern unsigned char Control_PressedAnyKey (void);
extern void Control_Select (unsigned char n);
#define Control_Close()
#ifdef __detect_kempston__
  void Control__init (void);
#else
# define Control__init()
#endif //__detect_kempston__


#endif
