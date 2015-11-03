void Sound_TitleNext (void);
void Sound_Death (unsigned char y);
void Sound_DropStoneOrAlmas (unsigned char y);
void Sound_GetAlmas (unsigned int xn_plus_yn);
void Sound_KeyPressed (void);
/*================================== Header ==================================*/

static void BEEPER (unsigned int de, unsigned int hl) {

} /*BEEPER*/

/*--------------------------------- Cut here ---------------------------------*/
void Sound_Death (unsigned char y) {
/*
  unsigned ctr = y << 2, lim = ctr + 16;
  for (;ctr < lim; ctr ++)
    snd (((ctr >> 3) | (ctr << 5)) & 0xFF, 8); //Terrible Sound Of Death
*/
} /*Sound_Death*/

/*--------------------------------- Cut here ---------------------------------*/
void Sound_DropStoneOrAlmas (unsigned char y) {
/*
  snd (200 + (y << 1), 10);
*/
} /*Sound_DropStoneOrAlmas*/

/*--------------------------------- Cut here ---------------------------------*/
void Sound_GetAlmas (unsigned int xn_plus_yn) {
/*
  snd (60 + ((xn + yn) << 1), 8);
*/
} /*Sound_GetAlmas*/

/*--------------------------------- Cut here ---------------------------------*/
void Sound_KeyPressed (void) {
/*
  snd (64, 64);

  PC
  ==
  SpkrPort = fr * 6 = 64 * 6 = 384
  Hz = 1193181 / 384 = 3107.2421875
  
  ZX
  ==
  HL = 437500 / Hz - 30.125 ~= 110.675
*/
  BEEPER(50, 111);
} /*Sound_KeyPressed*/

/*--------------------------------- Cut here ---------------------------------*/
static unsigned int p;

void Sound_TitleNext (void) {
/*
  unsigned p = 0;
  while (1) {
    unsigned m = p, k;
    m = ++ p;
    for (k = 0; m; m >>= 1)
      if (m & 1)
        k ++;
    snd (30 + (k << 4), 30);
  }
*/
  unsigned int m, k;
  m = ++ p;
  for (k = 36; m; m >>= 1) if (m & 1) k += 35;
  BEEPER(25, k);
} /*Sound_TitleNext*/

/*--------------------------------- Cut here ---------------------------------*/
void Sound__init (void) { p = 0; } /*Sound__init*/
