///////////////////////////////////////////////
// File: tetris.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"

#include <interrupt.h>

#include "eeprom.h"
#include "joystick.h"
#include "lcd.h"
#include "music.h"
#include "portbits.h"
#include "pushbuttons.h"

// GAMEPLAY DATA ARRAYS //

// ARRAY OF ALL THE PIECE SHAPES
PRIVATE const unsigned int pieces[GAME_PIECE_ROWS][GAME_PIECE_COLS] =
{
{   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   FUCSHIA_BRICK,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   },
{   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   VOID,   VOID,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   GREEN_BRICK,    VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BLUE_BRICK, VOID,   VOID,   VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   NAVY_BRICK, NAVY_BRICK, VOID,   VOID,   VOID,   VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   FUCSHIA_BRICK,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   },
{   VOID,   RED_BRICK,  RED_BRICK,  RED_BRICK,  VOID,   VOID,   VOID,   RED_BRICK,  RED_BRICK,  VOID,   VOID,   RED_BRICK,  RED_BRICK,  RED_BRICK,  VOID,   VOID,   RED_BRICK,  RED_BRICK,  VOID,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   YELLOW_BRICK,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   YELLOW_BRICK,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   VOID,   GREEN_BRICK,    GREEN_BRICK,    VOID,   VOID,   VOID,   VOID,   GREEN_BRICK,    GREEN_BRICK,    VOID,   VOID,   AQUA_BRICK, AQUA_BRICK, VOID,   VOID,   VOID,   VOID,   BLUE_BRICK, BLUE_BRICK, VOID,   VOID,   VOID,   BLUE_BRICK, BLUE_BRICK, VOID,   VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   NAVY_BRICK, NAVY_BRICK, NAVY_BRICK, VOID,   VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   NAVY_BRICK, NAVY_BRICK, NAVY_BRICK, VOID,   VOID,   VOID,   FUCSHIA_BRICK,  VOID,   VOID,   FUCSHIA_BRICK,  FUCSHIA_BRICK,  FUCSHIA_BRICK,  FUCSHIA_BRICK,  VOID,   },
{   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   VOID,   RED_BRICK,  VOID,   VOID,   VOID,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   VOID,   VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   GREEN_BRICK,    GREEN_BRICK,    VOID,   VOID,   VOID,   GREEN_BRICK,    VOID,   VOID,   VOID,   AQUA_BRICK, AQUA_BRICK, VOID,   VOID,   VOID,   BLUE_BRICK, BLUE_BRICK, VOID,   VOID,   VOID,   VOID,   VOID,   BLUE_BRICK, VOID,   VOID,   VOID,   NAVY_BRICK, NAVY_BRICK, VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   VOID,   VOID,   NAVY_BRICK, VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   FUCSHIA_BRICK,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   },
{   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   }
};

// A BUFFER BOARD USED TO RENDER AS LITTLE AS POSSIBLE
PRIVATE unsigned int bufferboard[BOARD_ROWS][BOARD_COLS] = {0};

// THE GAMEBOARD HOLDS THE LOCATIONS OF EVERYTHING IN THE PLAYFIELD WE NEED TO RENDER
PRIVATE unsigned int gameboard[BOARD_ROWS][BOARD_COLS] = {0};


PRIVATE void render_sprite(unsigned int sprite_shape, unsigned int screen_x_position, unsigned int screen_y_position, unsigned int rendertype, unsigned int override_tile)
//
// render_sprite - renders a sprite or shape to the screen
// Arguments: sprite_shape - the shape of the sprite to be rendered
//            screen_x_position - the x-pos that the sprite should be rendered to
//            screen_y_position - the y-pos that the sprite should be rendered to
//            rendertype - indicates what kind of render is to be performed (BLOCK for the game piece, OVERRIDE for custom colored game pieces, VOID for a regular sprite)
//            override_tile - the tile to be used if an override is enabled
// Return Value: none
{
    static const unsigned int spritesheet[SPRITE_SHEET_ROWS][SPRITE_SHEET_COLS] =           // 29 and 65
    { /* 0       1       2       3       4       5           6       7       8       9       10      11          12      13      14      15      16      17          18      19      20      21      22      23          24      25      26      27      28      29          30      31      32      33      34      35          36      37      38      39      40      41          42      43      44      45      46      47          48      49      50      51      52      53          54      55      56      57      58      59          60      61      62      63      64      65  */
        {RED,    RED,    RED,    RED,    RED,    BLACK,      YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,      LIME,   LIME,   LIME,   LIME,   LIME,   BLACK,      AQUA,   AQUA,   AQUA,   AQUA,   AQUA,   BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK},
        {RED,    RED,    RED,    RED,    RED,    BLACK,      YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,      LIME,   LIME,   LIME,   LIME,   LIME,   BLACK,      AQUA,   AQUA,   AQUA,   AQUA,   AQUA,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},
        {RED,    RED,    RED,    RED,    RED,    BLACK,      YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,      LIME,   LIME,   LIME,   LIME,   LIME,   BLACK,      AQUA,   AQUA,   AQUA,   AQUA,   AQUA,   BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  WHITE,  WHITE,  WHITE,  BLACK},
        {RED,    RED,    RED,    RED,    RED,    BLACK,      YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,      LIME,   LIME,   LIME,   LIME,   LIME,   BLACK,      AQUA,   AQUA,   AQUA,   AQUA,   AQUA,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {RED,    RED,    RED,    RED,    RED,    BLACK,      YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,      LIME,   LIME,   LIME,   LIME,   LIME,   BLACK,      AQUA,   AQUA,   AQUA,   AQUA,   AQUA,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK},
        {BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},

        {BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   BLACK,      BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  BLACK,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   BLACK,      BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  WHITE,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  WHITE,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   TEAL,   TEAL,   BLACK,      BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  WHITE,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},

        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      NAVY,   NAVY,   NAVY,   NAVY,   NAVY,   BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      NAVY,   NAVY,   NAVY,   NAVY,   NAVY,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      NAVY,   NAVY,   NAVY,   NAVY,   NAVY,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      NAVY,   NAVY,   NAVY,   NAVY,   NAVY,   BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      NAVY,   NAVY,   NAVY,   NAVY,   NAVY,   BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},

        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  SILVER, SILVER,     SILVER, SILVER, GRAY,   SILVER, SILVER, SILVER,     SILVER, SILVER, BLACK,  GRAY,   GRAY,   BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  GRAY,   TEAL,   TEAL,   BLACK,      FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  WHITE,  BLACK,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   BLACK,  GRAY,   GRAY,   BLACK},
        {BLACK,  TEAL,   TEAL,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   TEAL,   TEAL,   BLACK,      FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  WHITE,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  WHITE,  BLACK,  WHITE,  BLACK,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  GRAY,   SILVER,     SILVER, SILVER, SILVER, SILVER, GRAY,   SILVER,     SILVER, SILVER, BLACK,  GRAY,   GRAY,   BLACK},
        {BLACK,  TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   BLACK,      FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    BLACK,      BLACK,  WHITE,  BLACK,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  WHITE,  BLACK,  WHITE,  BLACK,      BLACK,  WHITE,  BLACK,  WHITE,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  GRAY,   SILVER,     SILVER, SILVER, SILVER, SILVER, GRAY,   SILVER,     SILVER, SILVER, BLACK,  GRAY,   GRAY,   BLACK},
        {BLACK,  TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   BLACK,      FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    FUCHSIA,    BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  WHITE,  BLACK,  WHITE,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  GRAY,   GRAY,       GRAY,   GRAY,   GRAY,   GRAY,   GRAY,   GRAY,       GRAY,   GRAY,   BLACK,  GRAY,   GRAY,   BLACK},
        {BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  GRAY,   GRAY,   BLACK,  SILVER, SILVER,     SILVER, SILVER, GRAY,   SILVER, SILVER, SILVER,     SILVER, SILVER, BLACK,  GRAY,   GRAY,   BLACK},

        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK},
        {WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  WHITE,  BLACK,  BLACK},
        {WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK},
        {WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      BLACK,  WHITE,  BLACK,  BLACK,  BLACK,  BLACK},
        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK},
        {BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},

        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK},
        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  WHITE,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK},
        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK},
        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  BLACK,  WHITE,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},
        {WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  WHITE,  BLACK,  BLACK,  BLACK},
        {BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,      BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK},
    };

    unsigned int sprite_sheet_location_x;
    unsigned int sprite_sheet_location_y;

    if ((rendertype == BLOCK) || (rendertype == OVERRIDE)) // check if a BLOCK or OVERRIDE render is desired
    {
        for (unsigned int tile_y_offset = START; tile_y_offset <= SHAPERANGE; tile_y_offset++) // render the shape
        {
            for (unsigned int tile_x_offset = START; tile_x_offset <= SHAPERANGE; tile_x_offset++)
            {
                if (pieces[tile_y_offset][(sprite_shape * PIECE_SIZE) + tile_x_offset]) // render the sprite
                {
                    if (rendertype != OVERRIDE) // if its a normal block render
                    {
                        sprite_sheet_location_x =  (XCOORD & pieces[tile_y_offset][(sprite_shape * PIECE_SIZE) + tile_x_offset]);
                        sprite_sheet_location_y = ((YCOORD & pieces[tile_y_offset][(sprite_shape * PIECE_SIZE) + tile_x_offset]) >> SPRITE_DECODE); // decode the sprite
                    }
                    else    // if its an override
                    {
                        sprite_sheet_location_x =  (XCOORD & override_tile);
                        sprite_sheet_location_y = ((YCOORD & override_tile) >> SPRITE_DECODE); // decode the override sprite
                    }

                    for (unsigned int sprite_pixel_y_offset = START; sprite_pixel_y_offset <= TILE_SIZE; sprite_pixel_y_offset++) // set the desired pixels necessary to print the sprite
                    {
                        for (unsigned int sprite_pixel_x_offset = START; sprite_pixel_x_offset <= TILE_SIZE; sprite_pixel_x_offset++)
                        {
                            lcdSetPixel((END_OF_SCREEN - (screen_x_position + tile_x_offset) * TILE - sprite_pixel_x_offset),
                                        (END_OF_SCREEN - (screen_y_position + tile_y_offset) * TILE - sprite_pixel_y_offset),
                                        spritesheet[(sprite_sheet_location_y * TILE) + sprite_pixel_y_offset]
                                                   [(sprite_sheet_location_x * TILE) + sprite_pixel_x_offset]);
                        }
                    }
                }
            }
        }
    }
    else // if its just a single sprite render
    {
        sprite_sheet_location_y = ((YCOORD & sprite_shape) >> 4); // decode the sprites location on the sheet
        sprite_sheet_location_x = (XCOORD & sprite_shape);

        if (sprite_shape == VOID)
        {
            sprite_sheet_location_y = ((YCOORD & BLACK_SPACE) >> 4);
            sprite_sheet_location_x =  (XCOORD & BLACK_SPACE);
        }

        for (unsigned int sprite_pixel_y_offset = START; sprite_pixel_y_offset <= TILE_SIZE; sprite_pixel_y_offset++) // print using the picel colour info from the sprite sheet
        {
            for (unsigned int sprite_pixel_x_offset = START; sprite_pixel_x_offset <= TILE_SIZE; sprite_pixel_x_offset++)
            {
                lcdSetPixel((END_OF_SCREEN - (screen_x_position) * TILE - sprite_pixel_x_offset),
                            (END_OF_SCREEN - (screen_y_position) * TILE - sprite_pixel_y_offset),
                            spritesheet[(sprite_sheet_location_y * TILE) + sprite_pixel_y_offset]
                                       [(sprite_sheet_location_x * TILE) + sprite_pixel_x_offset]);
            }
        }

    }
}


#define SEED_INIT           0xDEAD
#define RAND_MULTIPLIER     0x41C64E6D
#define RAND_INCREMENT      0x3039
#define RAND_MODULUS        0x80000000

PRIVATE int random_piece_number()
//
// random_piece_number - generates a random piece number
//                           (designed based off of https://en.wikipedia.org/wiki/Linear_congruential_generator)
// Arguments: none
// Return Value: seed - the randomly generated piece number
{
    static unsigned int seed = SEED_INIT;

    if (seed == SEED_INIT) // initialize the seed to a unpredictable number
    {
        seed = 5457 * (JoyStick_Read_X() & LOW_BITS) * (JoyStick_Read_Y() & LOW_BITS) - 1;
    }

    seed = (RAND_MULTIPLIER * seed + RAND_INCREMENT) % RAND_MODULUS;

    return seed % MAX_PIECES;
}


PUBLIC int Do_Shift(unsigned int xpos, unsigned int ypos, unsigned int cshape)
//
// shift - reads the x-value of the joy-stick to determine if the piece should shift
// Arguments: xpos - the x position of the piece
//            ypos - the y position of the piece
//            cshape - the shape of the piece
// Return Value: xpos - the new x position
{
    static int holdthisposition = 0;
    int adjustment = 0;

    // a staggered shift is used to allow for precise movement of the piece
    //
    if (   (holdthisposition <= SHIFT_THRESHOLD_1)
        || (holdthisposition == SHIFT_THRESHOLD_2)
        || (holdthisposition == SHIFT_THRESHOLD_3)
        || (holdthisposition == SHIFT_THRESHOLD_4)
        || (holdthisposition >= SHIFT_THRESHOLD_5))
    {
        unsigned int joypos = JoyStick_Read_X();   // read the x-value from the joystick

             if (joypos >= JOYSTICK_RIGHT) { adjustment = MOVERIGHT; }      // move to the right
        else if (joypos <= JOYSTICK_LEFT)  { adjustment = MOVELEFT; }       // move to the left
        else                               { holdthisposition = 0; }        // no movement, reset.

        xpos += adjustment;
    }

    if (holdthisposition <= SHIFT_THRESHOLD_5)
    {
        holdthisposition++;
    }

    if (Check_Collision(xpos, ypos, cshape)) // check if this movement causes a collision
    {
        xpos -= adjustment;
    }

    return xpos;
}


PUBLIC bool Check_Collision(int xref, unsigned int yref, unsigned int shape)
//
// collision - checks for a collision
// Arguments: xref - the x position of the piece to be tested for collisions
//            yref - the y position of the piece to be tested for collisions
//            shape - the shape of the piece to be tested for collisions
// Return Value: returnvalue - returns true if collision has occured
{
    for (int checky = START; checky <= SHAPERANGE; checky++)
    {
        for (int checkx = START; checkx <= SHAPERANGE; checkx++)
        {
            if ( pieces[checky][shape * PIECE_SIZE + checkx]
               && (  ((int)(checkx + xref - PIECEOFFSET) < 0)
                  || ((checkx + xref - PIECEOFFSET) > BOARDWIDTH)
                  || ((checky + yref - PIECEOFFSET) >= FLOOR_COLLISION)
                  || (gameboard[checky + yref - PIECEOFFSET][checkx + xref - PIECEOFFSET]) ))
            {
                return true;
            }
        }
    }

    return false;
}


PUBLIC void Set_Piece(unsigned int xcur, unsigned int ycur, unsigned int shape)
//
// Set_Piece - sets the current game piece onto the game board
// Arguments: xcur - the x position of the game piece
//            ycur - the y position of the game piece
//            pieceshape - the shape of the game piece
// Return Value: none
{
    for (unsigned int final_y = START; final_y <= SHAPERANGE; final_y++)
    {
        for (unsigned int final_x = START; final_x <= SHAPERANGE; final_x++)
        {
            unsigned int piece = pieces[final_y][shape * PIECE_SIZE + final_x];

            if (piece)
            {
                gameboard[final_y + ycur - PIECEOFFSET][final_x + xcur - PIECEOFFSET] = piece;
            }
        }
    }
}


PUBLIC int Clear_Completed_Lines()
//
// Clear_Completed_Lines - clears the completed lines and shifts all above down
// Arguments: none
// Return Value: lines_cleared - the number of lines cleared
{
    unsigned int lines_cleared = VOID;

    for (int yvalue = BOTTOM; yvalue >= TOP; yvalue--)
    {
        unsigned int sum = VOID;

        for (unsigned int xvalue = START; xvalue <= BOARDWIDTH; xvalue++)
        {
            if (gameboard[yvalue][xvalue]) // check if a board location contains a piece
            {
                sum++;
            }
        }

        if (sum == TETRIS) // check if a line has been completed
        {
            for (int yval = START; yval <= BOTTOM; yval++)
            {
                for (int xval = START; xval <= BOARDWIDTH; xval++)
                {
                    bufferboard[yval][xval] = gameboard[yval][xval]; // copy the current gameboard to the bufferboard
                }
            }

            lines_cleared++;

            SoundFX_Play(lines_cleared);  // play the line cleared sound effect

            for (unsigned int Ty = START; (yvalue - Ty) >= OVERLAP; Ty++) // play an animation to signify the line is being cleared
            {
                if (Ty == START)
                {
                    for (unsigned int Tx = START; Tx <= BOARDWIDTH; Tx++)
                    {
                        gameboard[yvalue][Tx] = WHITE_BRICK;
                    }

                    Render_Board();

                    for (unsigned int Tx = START; Tx <= BOARDWIDTH; Tx++)
                    {
                        bufferboard[yvalue][Tx] = WHITE_BRICK;
                    }

                    SoundFX_Wait();     // wait for the sfx to finish
                }

                for (unsigned int Tx = START; Tx <= BOARDWIDTH; Tx++)
                {
                    gameboard[yvalue - Ty][Tx] = gameboard[yvalue - Ty - SHIFT][Tx];
                }
            }

            if (lines_cleared != MAXIMUM_LINES_CLEARED)
            {
                Render_Board();
            }
            yvalue++;
        }
    }
    return lines_cleared;
}


PUBLIC void Render_Piece(unsigned int renderlocx, unsigned int renderlocy, unsigned int clearlocx, unsigned int clearlocy, unsigned int pieceshape, unsigned int oldpieceshape)
//
// Render_Piece - renders a game piece onto the screen
// Arguments: renderlocx - the x-pos of the new piece to be rendered
//            renderlocy - the y-pos of the new piece to be rendered
//            clearlocx - the x-pos of the old piece to be cleared
//            clearlocy - the y-pos of the old piece to be cleared
//            pieceshape - the shape of the new piece to render
//            oldpieceshape - the shape of the old piece to be cleared
// Return Value: none
{
    unsigned int render_x = VOID;
    unsigned int render_y = VOID;
    unsigned int bufferpiece[BUFFER_ROWS][BUFFER_COLS] = {VOID};

    if ((renderlocy - clearlocy) <= PIECE_PROXIMITY)                                            // IF THE TWO ARE CLOSE TOGETHER, AVOID RERENDERING THE OVERLAP
    {
        for (render_y = START; render_y <= SHAPERANGE; render_y++)                                 // PUT BLACK SPACES OVER THE LOCATIONS OF THE OLD SHAPE IN THE BUFFER
        {
            for (render_x = START; render_x <= SHAPERANGE; render_x++)
            {
                if (pieces[render_y][oldpieceshape*PIECE_SIZE + render_x])
                {
                    bufferpiece[render_y][clearlocx - BUFFER_OFFSET + render_x] = BLACK_SPACE;
                }
            }
        }

        for (render_y = START; render_y <= SHAPERANGE; render_y++)                                 // PLACE THE NEW SHAPE ON THE BUFFER, OVERRIGHTING ANY OVERLAP WITH THE OLD SHAPE
        {
            for (render_x = START; render_x <= SHAPERANGE; render_x++)
            {
                if (pieces[render_y][pieceshape * PIECE_SIZE + render_x])
                {
                    bufferpiece[renderlocy - clearlocy + render_y][renderlocx - BUFFER_OFFSET + render_x] = pieces[render_y][pieceshape  * PIECE_SIZE + render_x];
                }
            }
        }

        for (render_y = START; render_y <= PIECE_RENDER_Y_MAX; render_y++)                         // RENDER THE BUFFER PIECE TO THE SCREEN
        {
            for (render_x = START; render_x <= PIECE_RENDER_X_MAX; render_x++)
            {
                if (bufferpiece[render_y][render_x])
                {
                    render_sprite(bufferpiece[render_y][render_x], (render_x + RENDER_X_OFFSET), (clearlocy + render_y - RENDER_Y_OFFSET), GENERAL, VOID);
                }
            }
        }
    }
    else                                                                                        // IF THERE FAR APART, JUST RENDER THEM INDIVIDUALY
    {
        render_sprite(oldpieceshape, (clearlocx - RENDER_X_OFFSET), (clearlocy - RENDER_Y_OFFSET), OVERRIDE, BLACK_SPACE);
        render_sprite(pieceshape, (renderlocx - RENDER_X_OFFSET), (renderlocy - RENDER_Y_OFFSET), BLOCK, VOID);
    }
}


PUBLIC int Select_Shape(void)
//
// Select_Shape - Selects the next piece to drop.
// Arguments: none.
// Return Value: outputshape - the converted shape
{
    unsigned int inputshape = random_piece_number();

    unsigned int outputshape;

    if (inputshape <= T_BLOCK_LAST)
    {
        outputshape = T_BLOCK_FIRST;
    }
    else if (inputshape >= J_BLOCK_FIRST && inputshape <= J_BLOCK_LAST)
    {
        outputshape = J_BLOCK_FIRST;
    }
    else if (inputshape >= Z_BLOCK_FIRST && inputshape <= Z_BLOCK_LAST)
    {
        outputshape = Z_BLOCK_FIRST;
    }
    else if ( inputshape == O_BLOCK)
    {
        outputshape = O_BLOCK;
    }
    else if (inputshape >= S_BLOCK_FIRST && inputshape <= S_BLOCK_LAST)
    {
        outputshape = S_BLOCK_FIRST;
    }
    else if (inputshape >= L_BLOCK_FIRST && inputshape <= L_BLOCK_LAST)
    {
        outputshape = L_BLOCK_FIRST;
    }
    else if (inputshape >= I_BLOCK_FIRST)
    {
        outputshape = I_BLOCK_FIRST;
    }
    else
    {
        outputshape = T_BLOCK_FIRST;  // safe default
    }
    return outputshape;
}


PUBLIC void Render_Board()
//
// Render_Board - renders any differences between the game and buffer board
// Arguments: none
// Return Value: none
{
    for (unsigned int y = START; y <= BOTTOM; y++)
    {
        for (unsigned int x = START; x <= BOARDWIDTH; x++)
        {
            if (gameboard[y][x] != bufferboard[y][x])
            {
                render_sprite(gameboard[y][x], (x + 1), (y - 2), GENERAL, VOID);
                bufferboard[y][x] = gameboard[y][x];
            }
        }
    }
}


PUBLIC void Render_Background()
//
// Render_Background - renders the game background
// Arguments: none
// Return Value: none
{
    static const unsigned int background[SCREEN_ROWS][SCREEN_COLS] =
    {
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_TOPLEFT, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOPRIGHT,    BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    LETTER_L,   LETTER_E,   LETTER_V,   LETTER_E,   LETTER_L,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_BOTTOMLEFT,   BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOMRIGHT, BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_TOPLEFT, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOPRIGHT,    BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    LETTER_S,   LETTER_C,   LETTER_O,   LETTER_R,   LETTER_E,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_BOTTOMLEFT,   BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOMRIGHT, BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_TOPLEFT, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOP, BORDER_TOPRIGHT,    BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    LETTER_N,   LETTER_E,   LETTER_X,   LETTER_T,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_LEFT,    VOID,   VOID,   VOID,   VOID,   VOID,   BORDER_RIGHT,   BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BORDER_BOTTOMLEFT,   BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOM,  BORDER_BOTTOMRIGHT, BRICK   },
         {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER,   BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK,  BRICK   },
    };

    for (unsigned int y = START; y <= BOARD_SIZE; y++) // renders every sprite of the background array onto the screen
    {
        for (unsigned int x = START; x <= BOARD_SIZE; x++)
        {
            if (background[y][x])
            {
                render_sprite(background[y][x], x, y, GENERAL, VOID);
            }
        }
    }
}


PUBLIC void Reset_Board()
//
// Reset_Board - resets the entire game board
// Arguments: none
// Return Value: none
{
    for (unsigned int j = START; j <= BOTTOM; j++) // sets every value in the gameboard array to 0
    {
        for (unsigned int k = START; k <= BOARDWIDTH; k++)
        {
            gameboard[j][k] = 0;
            bufferboard[j][k] = ~0;
        }
    }
}


PUBLIC int Rotate_Piece(unsigned int x_col_test, unsigned int y_col_test, unsigned int current_orientation)
//
// Rotate_Piece - rotates the gape piece shape
// Arguments: x_col_test - the x-pos of the shapes anchor, for collision testing
//            y_col_test - the y-pos of the shapes anchor, for collision testing
//            current_orientation - the current orientation of the shape of the gamepiece
// Return Value: current_orientation - returns the new shape, with a collision flag if a collision occurs
{
    unsigned int reset_orientation = current_orientation;

    // changes the shape of the gamepiece, if the piece is in its last rotation position its set back to its first

    current_orientation = (current_orientation == T_BLOCK_LAST) ? T_BLOCK_FIRST
                        : (current_orientation == J_BLOCK_LAST) ? J_BLOCK_FIRST
                        : (current_orientation == Z_BLOCK_LAST) ? Z_BLOCK_FIRST
                        : (current_orientation == O_BLOCK)      ? O_BLOCK
                        : (current_orientation == S_BLOCK_LAST) ? S_BLOCK_FIRST
                        : (current_orientation == L_BLOCK_LAST) ? L_BLOCK_FIRST
                        : (current_orientation == I_BLOCK_LAST) ? I_BLOCK_FIRST
                        : (current_orientation + 1);

    if (Check_Collision(x_col_test, y_col_test, current_orientation))
    {
        if (   (x_col_test == LEFT_COL_ONE)
            || (x_col_test == LEFT_COL_TWO))    // THIS FIXES A BUG IF YOUR HITTING THE LEFT WALL
        {
            if (Check_Collision(x_col_test + COL_TEST_OFFSET, y_col_test, current_orientation))
            {
                current_orientation = reset_orientation;
            }
            else
            {
                current_orientation += LEFT_COLLISION_ADJUST;
            }
        }
        else if (   (x_col_test == RIGHT_COL_ONE)
                 || (x_col_test == RIGHT_COL_TWO)) // THIS FIXES A BUG IF YOUR HITTING THE RIGHT WALL
        {
             if (Check_Collision(x_col_test - COL_TEST_OFFSET, y_col_test, current_orientation))
             {
                current_orientation = reset_orientation;
             }
             else
             {
                current_orientation += RIGHT_COLLISION_ADJUST;
             }
        }
        else
        {
            current_orientation = reset_orientation;
        }
    }

    if (current_orientation != reset_orientation)
    {
        SoundFX_Start(SOUND_EFFECT_5); // play the rotate sound effect
    }

    return current_orientation;
}


PUBLIC void Render_Number(unsigned int number_to_render, unsigned int y_start_pos, unsigned int x_start_pos)
//
// Render_Number - renders a 5 digit number onto the screen
// Arguments: number_to_render - the number we want to render onto the screen
//            y_start_pos - the ypos of the first number sprite location
//            x_start_pos - the xpos of the first number sprite location
// Return Value: none
{
    unsigned int digit[PRINT_NUM_ARRAY_SIZE] = {NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION}; // initialize the array that will contain the sprite locations of the numbers

    digit[4] +=  number_to_render % FIRST_DIGIT_TEST; //decode all the digits
    digit[3] += (number_to_render % SECOND_DIGIT_TEST) / FIRST_DIGIT_TEST;
    digit[2] += (number_to_render % THIRD_DIGIT_TEST) / SECOND_DIGIT_TEST;
    digit[1] += (number_to_render % FOURTH_DIGIT_TEST) / THIRD_DIGIT_TEST;
    digit[0] += (number_to_render % FIFTH_DIGIT_TEST) / FOURTH_DIGIT_TEST;

    for (unsigned int number_index = NUMBER_INDEX_START; number_index <= NUMBER_INDEX_END; number_index++) // print the numbers to the screen
    {
        render_sprite(digit[number_index], (x_start_pos + number_index), y_start_pos, GENERAL, VOID);
    }
}



PUBLIC void Render_Piece_Preview(unsigned int previewpiece, unsigned int previouspiece)
//
// Render_Piece_Preview - renders the piece preview in its window
// Arguments: previewpiece - the piece we want to render in the window
//            previouspiece - the piece currently in the window
// Return Value: none
{
    render_sprite(previouspiece, PIECE_PREVIEW_TILE_OFFSET_X, PIECE_PREVIEW_TILE_OFFSET_Y, OVERRIDE, BLACK_SPACE);      // CLEAR THE OLD PIECE PREVIEW
    render_sprite(previewpiece,  PIECE_PREVIEW_TILE_OFFSET_X, PIECE_PREVIEW_TILE_OFFSET_Y, BLOCK, VOID);                // RENDER THE NEW PIECE PREVIEW
}


PUBLIC int Calculate_Score (unsigned int number_of_lines)
//
// Calculate_Score - calculates the score generated from the current round
// Arguments: number_of_lines - the number of lines cleared by the player this round
// Return Value: score_out - the calculated score
{
    const int scores[] = {0, 100, 200, 400, 800};

    return scores[number_of_lines < ARRAY_SIZE(scores) ? number_of_lines : 0];
}


PUBLIC void Render_Ghost(unsigned int ghostx , unsigned int ghosty , unsigned int oldghostx , unsigned int oldghosty , unsigned int ghostshape , unsigned int oldghostshape)
//
// Render_Ghost - renders the ghost, as long as the DONT_RENDER flag isnt set
// Arguments: ghostx - the current x-pos of the ghost
//            ghosty - the current y-pos of the ghost
//            oldghostx - the old x-pos of the ghost
//            oldghosty - the old y-pos of the ghost
//            ghostshape - the current shape of the ghost piece
//            oldghostshape - the old shape of the ghost piece
// Return Value: none
{
    if (oldghostshape != DONT_RENDER) // check if the ghost should be rendered
    {
        render_sprite(oldghostshape, (oldghostx - RENDER_X_OFFSET), (oldghosty - RENDER_Y_OFFSET), OVERRIDE, BLACK_SPACE);
    }
    render_sprite(ghostshape, (ghostx - RENDER_X_OFFSET), (ghosty - RENDER_Y_OFFSET), OVERRIDE, WHITE_BRICK);
}


PUBLIC int Fall_Speed(unsigned int current_frame, unsigned int current_level)
//
// Fall_Speed - controls when the piece should be shifted down, with respect to the current difficulty level
// Arguments: current_frame - the current frame the game is on
//            current_level - the current level the player is on
// Return Value: result - the flag indicating whether the piece should be shifted
{
    uint32_t drop_threshold = (current_level <= EASY)    ? (START_SPEED - SPEED_SCALER * current_level)
                            : (current_level == MEDIUM)  ? SECOND_SPEED
                            : (current_level <= HARD)    ? THIRD_SPEED
                            : (current_level <= VETERAN) ? FOURTH_SPEED
                            : (current_level <= INSANE)  ? FIFTH_SPEED
                            : (current_level <= SADIST)  ? SIXTH_SPEED : 0;

    return (current_frame >= drop_threshold) ? DROP : 0;    // return whether or not a drop should occur
}


PRIVATE int get_joystick_event(void)
{
    int joy_y = JoyStick_Read_Y();

    return (joy_y >= MENU_SHIFT_UP) ? MENU_SHIFT_UP
         : (joy_y <= MENU_SHIFT_DOWN) ? MENU_SHIFT_DOWN : 0;
}




PRIVATE void Render_Music_Menu_Value(unsigned int music_value)
{
    int top, mid, bot;

    if (music_value == OFF)   // if the music was off
    {
        music_value = ALL;    // turn the music on

        // RENDER THE "ON" indicator to the screen
        top = BLACK_SPACE;  mid = LETTER_O;  bot = LETTER_N;
    }
    else if (music_value == ALL)   // if the music was on
    {
        music_value = SFX;    // turn the music off

        // RENDER THE "OFF" indicator to the screen
        top = LETTER_S;  mid = LETTER_F;  bot = LETTER_X;
    }
    else if (music_value == SFX)   // if the music was on
    {
        music_value = OFF;    // turn the music off

        // RENDER THE "OFF" indicator to the screen
        top = LETTER_O;  mid = LETTER_F;  bot = LETTER_F;
    }
    render_sprite(top, TILE_12, TILE_12, GENERAL, VOID);
    render_sprite(mid, TILE_13, TILE_12, GENERAL, VOID);
    render_sprite(bot, TILE_14, TILE_12, GENERAL, VOID);
}


PRIVATE void render_main_menu(void)
{
    static const unsigned int mainmenu[SCREEN_ROWS][SCREEN_COLS] =
    {
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  RED_BRICK,  RED_BRICK,  RED_BRICK,  YELLOW_BRICK,   YELLOW_BRICK,   YELLOW_BRICK,   GREEN_BRICK,    GREEN_BRICK,    GREEN_BRICK,    AQUA_BRICK, AQUA_BRICK, AQUA_BRICK, BLUE_BRICK, BLUE_BRICK, BLUE_BRICK, FUCSHIA_BRICK,  FUCSHIA_BRICK,  FUCSHIA_BRICK,  BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   RED_BRICK,  VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   GREEN_BRICK,    VOID,   AQUA_BRICK, VOID,   AQUA_BRICK, VOID,   BLUE_BRICK, VOID,   FUCSHIA_BRICK,  VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   RED_BRICK,  VOID,   YELLOW_BRICK,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   GREEN_BRICK,    VOID,   AQUA_BRICK, AQUA_BRICK, VOID,   VOID,   BLUE_BRICK, VOID,   FUCSHIA_BRICK,  FUCSHIA_BRICK,  FUCSHIA_BRICK,  BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   RED_BRICK,  VOID,   YELLOW_BRICK,   VOID,   VOID,   VOID,   GREEN_BRICK,    VOID,   AQUA_BRICK, VOID,   AQUA_BRICK, VOID,   BLUE_BRICK, VOID,   VOID,   VOID,   FUCSHIA_BRICK,  BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   RED_BRICK,  VOID,   YELLOW_BRICK,   YELLOW_BRICK,   YELLOW_BRICK,   VOID,   GREEN_BRICK,    VOID,   AQUA_BRICK, VOID,   AQUA_BRICK, BLUE_BRICK, BLUE_BRICK, BLUE_BRICK, FUCSHIA_BRICK,  FUCSHIA_BRICK,  FUCSHIA_BRICK,  BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   SYMBOL_ARROW,   LETTER_S,   LETTER_T,   LETTER_A,   LETTER_R,   LETTER_T,   VOID,   LETTER_G,   LETTER_A,   LETTER_M,   LETTER_E,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   LETTER_S,   LETTER_O,   LETTER_U,   LETTER_N,   LETTER_D,   SYMBOL_COLON,   VOID,   LETTER_O,   LETTER_F,   LETTER_F,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   LETTER_A,   LETTER_L,   LETTER_E,   LETTER_K,   LETTER_S,   LETTER_A,   LETTER_N,   LETTER_D,   LETTER_E,   LETTER_R,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   LETTER_O,   LETTER_L,   LETTER_E,   LETTER_S,   LETTER_E,   LETTER_V,   LETTER_I,   LETTER_T,   LETTER_C,   LETTER_H,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    },
        {BRICK_RIGHTBORDER,  VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   VOID,   BRICK_LEFTBORDER    }
    };

    // RENDER THE MAIN MENU
    for (unsigned int index_y = START; index_y <= BOARD_SIZE; index_y++) // render the main menu graphics
    {
        for (unsigned int index_x = START; index_x <= BOARD_SIZE; index_x++)
        {
            if (mainmenu[index_y][index_x])
            {
                render_sprite(mainmenu[index_y][index_x], index_x, index_y, GENERAL, VOID);
            }
        }
    }
}


PUBLIC int Start_Menu(unsigned int music_value)
//
// Start_Menu - puts the user into the start menu screen until the user presses PB2 while start game is selected
// Arguments: none
// Return Value: music_value - the flag indicating whether music should be played
{
    bool in_menu = true;

    unsigned int menu_item = START_GAME;

    int last_joy_event = 0;     // holds last joy stick event.

    render_main_menu();
    Render_Music_Menu_Value(music_value);

    while (in_menu)        // ENTER THE MAIN MENU STATE
    {
        int event = get_joystick_event();

        if (event == last_joy_event) { event = 0; }
        else                         { last_joy_event = event; }

        if ( (event == MENU_SHIFT_UP)    // check if the joy-stick indicated the user wants to change what menu item is selected
           || (event == MENU_SHIFT_DOWN) )
        {
            int top, bot;

            if (menu_item == START_GAME)
            {
                menu_item = MUSIC_SELECT; // change the cursor position indicator flag to MUSIC
                top = BLACK_SPACE;    bot = SYMBOL_ARROW;
            }
            else
            {
                menu_item = START_GAME; // change the cursor position indicator flag to START GAME
                top = SYMBOL_ARROW;    bot = BLACK_SPACE;
            }
            render_sprite(top, TILE_4, TILE_10, GENERAL, VOID);  // de-render the old cursor indicator
            render_sprite(bot, TILE_4, TILE_12, GENERAL, VOID); // render the new cursor indicator
        }


        if (PB2_pressed) // check if the PB was pressed
        {
            PB2_pressed = false;     // reset the PB flag

            if (menu_item == MUSIC_SELECT)
            {
                     if (music_value == OFF) { music_value = ALL; }   // if the music was off turn the music on
                else if (music_value == ALL) { music_value = SFX; }   // if the music was on turn the music off
                else if (music_value == SFX) { music_value = OFF; }   // if the music was on turn the music off

                Render_Music_Menu_Value(music_value);
            }
            else if (menu_item == START_GAME)
            {
                in_menu = false;         // start the game
            }
        }
    }
    lcdClear(BLACK); // clear the screen

    return music_value;
}


PUBLIC void Game_Over(unsigned int final_score, unsigned int final_lines, unsigned int final_level, unsigned int high_score)
//
// Game_Over - puts the user into the game over screen until PB2 is pressed
// Arguments: final_score - the final score achieved by the player
//            final_lines - the number of lines cleared by the player
//            final_level - the final level reached by the player
//            high_score  - the current high-score saved in memory
// Return Value: none
{
    SoundFX_Start(SOUND_EFFECT_7); // play the game over sound effect

    lcdClear(BLACK); // clear the screen

    render_sprite(LETTER_G, TILE_5, TILE_4, GENERAL, VOID); // render the GAME OVER text
    render_sprite(LETTER_A, TILE_6, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_M, TILE_7, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_8, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_O, TILE_10, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_V, TILE_11, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_12, TILE_4, GENERAL, VOID);
    render_sprite(LETTER_R, TILE_13, TILE_4, GENERAL, VOID);

    SoundFX_Wait(); // wait for the sound effect to finish

    render_sprite(LETTER_L, TILE_4, TILE_8, GENERAL, VOID); // display the LEVEL text
    render_sprite(LETTER_E, TILE_5, TILE_8, GENERAL, VOID);
    render_sprite(LETTER_V, TILE_6, TILE_8, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_7, TILE_8, GENERAL, VOID);
    render_sprite(LETTER_L, TILE_8, TILE_8, GENERAL, VOID);

    SoundFX_Start(SOUND_EFFECT_8); // play the level sfx

    Render_Number(final_level, TILE_8, TILE_10); // render the number of the final level reached

    SoundFX_Wait(); // wait for the sound effect to finish

    render_sprite(LETTER_L, TILE_4, TILE_10, GENERAL, VOID); // display the LINES text
    render_sprite(LETTER_I, TILE_5, TILE_10, GENERAL, VOID);
    render_sprite(LETTER_N, TILE_6, TILE_10, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_7, TILE_10, GENERAL, VOID);
    render_sprite(LETTER_S, TILE_8, TILE_10, GENERAL, VOID);

    SoundFX_Start(SOUND_EFFECT_9); // play the lines cleared sfx

    Render_Number(final_lines, TILE_10, TILE_10); // render the number of the final number or lines cleared

    SoundFX_Wait(); // wait for the sound effect to finish

    render_sprite(LETTER_S, TILE_4, TILE_12, GENERAL, VOID); // display the SCORE text
    render_sprite(LETTER_C, TILE_5, TILE_12, GENERAL, VOID);
    render_sprite(LETTER_O, TILE_6, TILE_12, GENERAL, VOID);
    render_sprite(LETTER_R, TILE_7, TILE_12, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_8, TILE_12, GENERAL, VOID);

    SoundFX_Start(SOUND_EFFECT_10); // play the score sfx

    Render_Number(final_score, TILE_12, TILE_10);  // render the number of the final score

    SoundFX_Wait(); // wait for the sound effect to finish

    render_sprite(LETTER_B, TILE_4, TILE_14, GENERAL, VOID);
    render_sprite(LETTER_E, TILE_5, TILE_14, GENERAL, VOID);
    render_sprite(LETTER_S, TILE_6, TILE_14, GENERAL, VOID);
    render_sprite(LETTER_T, TILE_7, TILE_14, GENERAL, VOID);

    if (final_score >= high_score) // check if the high-score was beat
    {
        SoundFX_Start(SOUND_EFFECT_11); // play the new high-score sound

        Render_Number(final_score, TILE_14, TILE_10); // display the number of the new high score

        render_sprite(LETTER_N, TILE_2, TILE_18, GENERAL, VOID); // display the NEW HIGH SCORE text
        render_sprite(LETTER_E, TILE_3, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_W, TILE_4, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_H, TILE_6, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_I, TILE_7, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_G, TILE_8, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_H, TILE_9, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_S, TILE_11, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_C, TILE_12, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_O, TILE_13, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_R, TILE_14, TILE_18, GENERAL, VOID);
        render_sprite(LETTER_E, TILE_15, TILE_18, GENERAL, VOID);
        render_sprite(SYMBOL_EXCLAMATION, TILE_16, TILE_18, GENERAL, VOID);

        SoundFX_Wait(); // wait for the sound effect to finish
    }
    else
    {
        SoundFX_Start(SOUND_EFFECT_12); // play the highscore display sfx

        Render_Number(high_score, TILE_14, TILE_10); // render the number of the highscore

        SoundFX_Wait(); // wait for the sound effect to finish
    }

    Music_Off();
    while (!PB2_pressed); // wait until PB is pressed
    PB2_pressed = false; // reset the PB flag

    lcdClear(BLACK); // clear the screen
}


// end file: tetris.c
