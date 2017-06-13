MODULE Msg;

CONST
  VEDAsoft* = "VEDAsoft Oberon Club '2017";
(*
#define NMENU 4
#define MENU_1COL 12
#define MENU_1STR 6

#define PLAY_STR MENU_1STR
#define PLAY_COL MENU_1COL

#define KEYBOARD_STR (MENU_1STR + 1)
#define KEYBOARD_COL MENU_1COL

#define HELP_STR (MENU_1STR + 2)
#define HELP_COL MENU_1COL

#define EXIT_STR (MENU_1STR + 3)
#define EXIT_COL MENU_1COL
*)
  MenuX* = 11; MenuY* = 6;
  MenuPlay     * = "PLAY";
  MenuKeyboard * = "KEYBOARD";
  MenuKempston * = "KEMPSTON";
  MenuSinclair * = "SINCLAIR";
  MenuCursor   * = "CURSOR  ";
  MenuHelp     * = "HELP";
  MenuExit     * = "EXIT TO BASIC";

  IfSureY     * = "Press Y if You sure";
  PressAnyKey * = "Press any key ...";
(*
enum {INVALID, NUM_OF, LINES, MLINE, MLENGTH,  /*Internal for init_mess*/
UNABLE_TO, AOPEN,
AREAD, BYTES_FROM_FILE_N,
AWRITE, BYTES_TO_FILE_N,
ACLOSE, ASEEK, POS_IN_FILE_N,
INVAL_LEN_OF, FILE_N,  /*Concerning to AFILE*/
EXITING_TO_DOS, /*In B_ABORT*/
PRESS_ANY_KEY, /*In PRESS_ANY_KEY*/
S_PLAY, S_KEYBOARD, S_HELP, S_EXIT, /*In MENU_SELECT*/
ILLEGAL, ROOMS, FILE_SIZE, /*In begin of MAIN*/
NONE_ROOMS_IN, RFILE,
ENTER_YOUR_NAME,
Y_PLACE_IN_TABLE, ACC_LEVEL, Y_HAVE_ACC_TO, ALL_CAVES, REC_SCORE,
CONGRAT, Y_NEW_REC_SCORE, GREAT_DEAL, Y_NEW_ACC_LEVEL, /*In mainloop*/
Y_NEW_PLACE_IN_TABLE,
IF_SURE_Y, U_LEVEL_N, /*In EXIT TO DOS, UNPACK_LAB*/
LIVES_N, SCORE_N, /*In DISPLAY_INF*/
Y_TRAVEL_IN_CAVES, IS_FINISHED, Y_GET_POINTS, /*In CONGRATE*/
D_UP, D_DOWN, D_LEFT, D_RIGHT, PRESS_4_KEYS, DIR_CTRL, PRESS_Y_IF_CORRECT,
 /*In SET_KEYBOARD_CONTROL*/
SEL_START_LEVEL, G_GAME, G_OVER, /*In GAME*/
S_SPRITE, NO_MEM_FOR, LOADING, S_LETTER,
NO_HWARE_FOR, CGA, GRAF, /*In INIT_SPRITE*/
MAX_MESSAGE};

extern char * _M[MAX_MESSAGE];

void init_message (void);
void cdecl end_message (void);
*)


(*
Invalid 
number of 
lines
line 
length
Unable to 
open 
read 
 bytes from file #
write 
 bytes to file #
close
seek
 in file #
Invalid length of
 file #
Exiting to DOS

PLAY
KEYBOARD
HELP
EXIT TO DOS
Illegal 
ROOMS 
file size
None rooms in 
file
Enter Your Name
Your place in table is
Access level is
You have access to
ALL caves of Bolder Dash
Record score is
Congratulations !
Your new record score is
Great Deal !
Your new access level is
Your new place in table is

Level 
Lives 
  Score 
Your travel in caverns of
Bolder Dash is finished
You get some more points
Up
Down
Left
Right
Press four keys to define
direction control
Press Y if those are correct
Select start level
GAME
OVER
sprite
No memory for 
s loading
letter
No hardware for 
CGA 
graphics

*)

  Instruction1* = "#  Bolder (boulder) Dash  is  a^chain of  puzzle-caverns.  The^cavern   consists   of   16x16^cells.  Each cell may be  of 8^cases:^  1.  Empty cell.  The man and^other  moving objects  can  go^thru this cells.";
  Instruction2* = "^  2.  Land  cell   (usually  a^grass or a dirt). Only man can^dig thru this cells  and makes^its empty.^  3.     Falling    invaluable^objects  (usually  a  stones).^Can fall down and roll to side@of  falling.  Can  kill living^objects.";
  Instruction3* = "  The man can push any^chains only of this ones.^  4.  Falling valuable objects^  (usually   a   diamonds   or^emeralds).  Can fall, roll and^kill (see 3).  The man can NOT^push them,  but can  and  MUST@eat   ALL   of  this  ones  to^perform   his";
  Instruction4* = "   task   in  the^cavern.^  5.  The MAN  (in form of ant^or  real  man).   He  is  user^ (player) - controlled  living^object.   Must  collect  (eat)^valuable     objects,     push@invaluable  objects  (to  kill^other  living  objects),   and^escape";
  Instruction5* = "  away  from all objects^falling   over   him  and  all^living  objects  chasing   for^him.^  6.  The  wall  (of  cemented^stones, bricks, wood  or other@hard stuff).  All  living  and^falling objects (including the^man) can't go thru it.";
  Instruction6* = "^  7.   Living    and   chasing^objects  (two types, usually a^batterflies     and    pulsing^squares, or other pairs).  Can^kill the man. Can be killed by@falling object.  Can move only^in empty cells.^-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_";
  Instruction7* = "^  Standard Bolder Dash set has^76 (0..75) caverns ...@";
END Msg.
