#include "GrApp.h"

void Control_ChangePalette (void);
unsigned char Control_Get (void);
unsigned char Control_GetCustom (void);
unsigned char Control_PressedAnyKey (void);
unsigned char Control_ReadKey (void);
void Control_Select (unsigned char n);
/*================================== Header ==================================*/

void Control_ChangePalette (void) {

} /*Control_ChangePalette*/

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_PressedAnyKey (void) {
  Control_ChangePalette();

} /*Control_PressedAnyKey*/

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_ReadKey (void) { /* With auto-repeat */

} /*Input_Read_Repeat*/

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_Get (void) {

} /*Control_Get*/

unsigned char Control_GetCustom (void) {
           Control_ChangePalette();

} /*Control_GetCustom*/

void Control_Select (unsigned char n) {

} /*Control_Select*/
