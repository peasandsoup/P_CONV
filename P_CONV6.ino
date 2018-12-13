
#include <Wire.h>           
#include "fonts.h"          
#include "DEV_Config.h"     
#include "LCD_Driver.h"     
#include "LCD_GUI.h"        

/*                                           * means undecided color
 | blank    | red         | green        | green     | green   | green         |
 | blank    | blank       | blank        | *         | blank   | blank         |
 | blank    | blank       | blank        | blank     | *       | blank         |
 |----------+-------------+--------------+-----------+---------+---------------|
 | nothing  | PS 101      | PR 101       | PB 101    | PE 101  | RS 101        |
 |          |             |              |           |         |               |
 |----------+-------------+--------------+-----------+---------+---------------|
 | computer | Power on    | Power on     | Problem   | Problem | reset problem |
 | off      | sent to 101 | received 101 | begin 101 | end 101 |               |

 */

int A = 5;
int B = 10;
int relay[] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};
int hori[] = {5, 45, 85, 125, 165, 205, 245, 285, 325, 365, 405, 445, 5, 45, 85, 125};
int vert[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 70, 70, 70, 70};
String input;
int count = 20;
char *Input = (char*)malloc(50);
char word1[30], word3[30];
int word2 = 3;


void setup()
{
  for(int i=0; i<16; i++)
  {
    pinMode(relay[i], INPUT_PULLUP);
  }
//////////// here begins the screen setup //////////////////////////////
  int X, Y;

  Serial2.begin(9600);

  System_Init();

  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;  
  LCD_Init( Lcd_ScanDir, 100);
  LCD_Clear(LCD_BACKGROUND);

  GUI_Clear(WHITE);

  X = 5;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "01", &Font24, LCD_BACKGROUND, BLUE); 
  X = 45;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "02", &Font24, LCD_BACKGROUND, BLUE); 
  X = 85;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "03", &Font24, LCD_BACKGROUND, BLUE); 
  X = 125;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "04", &Font24, LCD_BACKGROUND, BLUE); 
  X = 165;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "05", &Font24, LCD_BACKGROUND, BLUE); 
  X = 205;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "06", &Font24, LCD_BACKGROUND, BLUE); 
  X = 245;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "07", &Font24, LCD_BACKGROUND, BLUE); 
  X = 285;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "08", &Font24, LCD_BACKGROUND, BLUE); 
  X = 325;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "09", &Font24, LCD_BACKGROUND, BLUE);
  X = 365;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "10", &Font24, LCD_BACKGROUND, BLUE); 
  X = 405;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "11", &Font24, LCD_BACKGROUND, BLUE);
  X = 445;
  Y = 10;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "12", &Font24, LCD_BACKGROUND, BLUE);
  X = 5;
  Y = 70;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "13", &Font24, LCD_BACKGROUND, BLUE);
  X = 45;
  Y = 70;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "14", &Font24, LCD_BACKGROUND, BLUE);
  X = 85;
  Y = 70;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "15", &Font24, LCD_BACKGROUND, BLUE);
  X = 125;
  Y = 70;
  LEDS (X, Y);
  GUI_DisString_EN(X, Y+40, "16", &Font24, LCD_BACKGROUND, BLUE);

  GUI_DisString_EN(10, 150, "   NSCC - COMPUTE CLUSTER", &Font24, LCD_BACKGROUND, BLUE);
  GUI_DrawRectangle(20, 140, 460, 180, BLUE,  DRAW_EMPTY, DOT_PIXEL_2X2);

}

int LEDS (int X, int Y)
{
//  STATUS LEDS
GUI_DrawRectangle(X,    Y, X+30, Y+10, WHITE,  DRAW_FULL,  DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+11, X+30, Y+21, WHITE,  DRAW_FULL,  DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+22, X+30, Y+33, WHITE,  DRAW_FULL,  DOT_PIXEL_DFT);
GUI_DrawRectangle(X,    Y, X+30, Y+10, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+11, X+30, Y+21, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+22, X+30, Y+33, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
}
void Redraw_Boxes(int X, int Y) //may be able to redefine draw functions so pixels don't overlap
{
GUI_DrawRectangle(X,    Y, X+30, Y+10, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+11, X+30, Y+21, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
GUI_DrawRectangle(X, Y+22, X+30, Y+33, BLUE,   DRAW_EMPTY, DOT_PIXEL_DFT);
}
void Parse_Input()
{
  strcpy(word1, "nothing");
  strcpy(word3, "nothing");
  int enable = 0;
  for(count = 0; count < 20; count ++)
  {
    if(Input[count] == ' ')
    {
      enable = 1;
      break;
    }
  }
  if(!enable)
    return;
  strcpy(word3, &Input[count+1]);
  for(int i=0; i<count; i++) // copy Input into word1
    word1[i] = Input[i];
  word1[count] = '\0'; // delimit string
  word2 = ((word3[1] - 48)*10 + word3[2] - 48) - 1; // do the string to int conversion
  if(word2 > 15 || word2 < 0) // if number command is out of range 1 <= word2 <= 16, make word1 nothing
  {
    strcpy(word1, "nothing");
  }
}

void loop()
{
  while(!Serial2.available()){} //wait for an input

  input = Serial2.readString();
  Input = input.c_str();
  Parse_Input();

  if(!strcmp(word1, "PS"))
  {
    GUI_DrawRectangle(hori[word2],    vert[word2], hori[word2]+30, vert[word2]+10, RED,   DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+11, hori[word2]+30, vert[word2]+21, WHITE, DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+22, hori[word2]+30, vert[word2]+33, WHITE,  DRAW_FULL, DOT_PIXEL_DFT);
    Redraw_Boxes(hori[word2], vert[word2]);
    pinMode(relay[word2], OUTPUT);
    digitalWrite(relay[word2], LOW);
    delay(1000);
    pinMode(relay[word2], INPUT_PULLUP);
  }
  else if(!strcmp(word1, "PR"))
  {
    GUI_DrawRectangle(hori[word2],    vert[word2], hori[word2]+30, vert[word2]+10, GREEN,   DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+11, hori[word2]+30, vert[word2]+21, WHITE, DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+22, hori[word2]+30, vert[word2]+33, WHITE,  DRAW_FULL, DOT_PIXEL_DFT);
    Redraw_Boxes(hori[word2], vert[word2]);
  }
  else if(!strcmp(word1, "PB"))
  {
    GUI_DrawRectangle(hori[word2],    vert[word2], hori[word2]+30, vert[word2]+10, GREEN,   DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+11, hori[word2]+30, vert[word2]+21, BLUE, DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+22, hori[word2]+30, vert[word2]+33, WHITE,  DRAW_FULL, DOT_PIXEL_DFT);
    Redraw_Boxes(hori[word2], vert[word2]);
  }
  else if(!strcmp(word1, "PE"))
  {
    GUI_DrawRectangle(hori[word2],    vert[word2], hori[word2]+30, vert[word2]+10, GREEN,   DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+11, hori[word2]+30, vert[word2]+21, WHITE, DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+22, hori[word2]+30, vert[word2]+33, BLUE,  DRAW_FULL, DOT_PIXEL_DFT);
    Redraw_Boxes(hori[word2], vert[word2]);
  }
  else if(!strcmp(word1, "RS"))
  {
    GUI_DrawRectangle(hori[word2],    vert[word2], hori[word2]+30, vert[word2]+10, GREEN,   DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+11, hori[word2]+30, vert[word2]+21, WHITE, DRAW_FULL, DOT_PIXEL_DFT);
    GUI_DrawRectangle(hori[word2], vert[word2]+22, hori[word2]+30, vert[word2]+33, WHITE,  DRAW_FULL, DOT_PIXEL_DFT);
    Redraw_Boxes(hori[word2], vert[word2]);
  }

  
}


