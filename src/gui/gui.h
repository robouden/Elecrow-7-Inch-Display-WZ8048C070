#ifndef GUI_H
#define GUI_H


// 16 bit colors in R5G6B5 notation
#define TFT_GREY 0x5AEB                       // New colour
#define TFT_DARKBLUE 0x439                    /* Upper dark blue area */
#define TFT_LIGHTBLUE 0x1E9F                  /* Main light blue area */
#define TFT_LOWBLUE  0x439                    /* Lower dark blue area */

#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
//#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_COALGREY    0x3186      /*50, 50, 50 */   // lighter: 0x630c /*96, 96, 96 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
// #define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

//byte LCD_BRIGHTNESS = 100;                                    /* LCD brightness value (0-250) */
//byte LCD_TEXTSIZE = 2;
//int LCD_BACKLIGHT_MODE = 2; // 0=manual, 1= linear, 2 = logarithmic

//uint16_t bgndColor = TFT_DARKGREEN;
//uint16_t fgndColor = TFT_WHITE;

//----- forward declarations
void gui_start();
void drawExamples();
void drawInLoop();
bool touchpadRead( uint16_t *x, uint16_t *y);
void touchpadTest();

#endif