///////////////////////////////////////////////
// File: tetris.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include <input.h>
#include "common.h"


#include "eeprom.h"
#include "lcd.h"
#include "music.h"
#include "portbits.h"
#include "initialize.h"

#define PB2_TIMEOUT_TICKS 5

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
        unsigned int tile_y_offset;
        for (tile_y_offset = START; tile_y_offset <= SHAPERANGE; tile_y_offset++) // render the shape
        {
            unsigned int tile_x_offset;
            for (tile_x_offset = START; tile_x_offset <= SHAPERANGE; tile_x_offset++)
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

                    unsigned int sprite_pixel_y_offset;
                    for (sprite_pixel_y_offset = START; sprite_pixel_y_offset <= TILE_SIZE; sprite_pixel_y_offset++) // set the desired pixels necessary to print the sprite
                    {
                        unsigned int sprite_pixel_x_offset;
                        for (sprite_pixel_x_offset = START; sprite_pixel_x_offset <= TILE_SIZE; sprite_pixel_x_offset++)
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

        unsigned int sprite_pixel_y_offset;
        for (sprite_pixel_y_offset = START; sprite_pixel_y_offset <= TILE_SIZE; sprite_pixel_y_offset++) // print using the picel colour info from the sprite sheet
        {
            unsigned int sprite_pixel_x_offset;
            for (sprite_pixel_x_offset = START; sprite_pixel_x_offset <= TILE_SIZE; sprite_pixel_x_offset++)
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
        seed = 5457 * (input.thumbv & LOW_BITS) * (input.thumbv & LOW_BITS) - 1;
    }

    seed = (RAND_MULTIPLIER * seed + RAND_INCREMENT) % RAND_MODULUS;

    return seed % MAX_PIECES;
}


#define DEFAULT_SHIFT_TIMEOUT 5
uint32_t shift_max_timeout = DEFAULT_SHIFT_TIMEOUT;
uint32_t shift_timeout = 0;

PUBLIC int Do_Shift(unsigned int xpos, unsigned int ypos, unsigned int cshape, unsigned int ticks)
//
// shift - reads the x-value of the joy-stick to determine if the piece should shift
// Arguments: xpos - the x position of the piece
//            ypos - the y position of the piece
//            cshape - the shape of the piece
//            ticks  - # of ticks since last call to Do_Shift
// Return Value: xpos - the new x position
{
    int adjustment = 0;

    unsigned int joypos = input.thumbh;   // read the x-value from the joystick

    if (joypos >= JOYSTICK_RIGHT) {
       adjustment = MOVERIGHT;
    }
    else if (joypos <= JOYSTICK_LEFT)  {
        adjustment = MOVELEFT;
    }
    else {
        shift_max_timeout = DEFAULT_SHIFT_TIMEOUT;
    }

    if(shift_timeout > 0) shift_timeout -= ticks;
    else if (adjustment != 0) {
        if(shift_max_timeout > 0) shift_max_timeout -= 1;
        shift_timeout = shift_max_timeout;
        if (Check_Collision(xpos + adjustment, ypos, cshape)) // check if this movement causes a collision
        {
            return xpos;
        } else return xpos + adjustment;
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
    int checky;
    for (checky = START; checky <= SHAPERANGE; checky++)
    {
        int checkx;
        for (checkx = START; checkx <= SHAPERANGE; checkx++)
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
    unsigned int final_y;
    for (final_y = START; final_y <= SHAPERANGE; final_y++)
    {
        unsigned int final_x;
        for (final_x = START; final_x <= SHAPERANGE; final_x++)
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

    int yvalue;
    for (yvalue = BOTTOM; yvalue >= TOP; yvalue--)
    {
        unsigned int sum = VOID;

        unsigned int xvalue;
        for (xvalue = START; xvalue <= BOARDWIDTH; xvalue++)
        {
            if (gameboard[yvalue][xvalue]) // check if a board location contains a piece
            {
                sum++;
            }
        }

        if (sum == TETRIS) // check if a line has been completed
        {
            int yval;
            for (yval = START; yval <= BOTTOM; yval++)
            {
                int xval;
                for (xval = START; xval <= BOARDWIDTH; xval++)
                {
                    bufferboard[yval][xval] = gameboard[yval][xval]; // copy the current gameboard to the bufferboard
                }
            }

            lines_cleared++;

            SoundFX_Play(lines_cleared);  // play the line cleared sound effect

            unsigned int Tx, Ty;
            for (Ty = START; (yvalue - Ty) >= OVERLAP; Ty++) // play an animation to signify the line is being cleared
            {
                if (Ty == START)
                {
                    for (Tx = START; Tx <= BOARDWIDTH; Tx++)
                    {
                        gameboard[yvalue][Tx] = WHITE_BRICK;
                    }

                    Render_Board();

                    for (Tx = START; Tx <= BOARDWIDTH; Tx++)
                    {
                        bufferboard[yvalue][Tx] = WHITE_BRICK;
                    }

                    SoundFX_Wait();     // wait for the sfx to finish
                }

                for (Tx = START; Tx <= BOARDWIDTH; Tx++)
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
    unsigned int x, y;
    for (y = START; y <= BOTTOM; y++)
    {
        for (x = START; x <= BOARDWIDTH; x++)
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

    unsigned int x, y;
    for (y = START; y <= BOARD_SIZE; y++) // renders every sprite of the background array onto the screen
    {
        for (x = START; x <= BOARD_SIZE; x++)
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
    unsigned int j, k;
    for (j = START; j <= BOTTOM; j++) // sets every value in the gameboard array to 0
    {
        for (k = START; k <= BOARDWIDTH; k++)
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
    static unsigned int digit[PRINT_NUM_ARRAY_SIZE] = {NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION, NUMBER_START_POSITION}; // initialize the array that will contain the sprite locations of the numbers

    digit[4] +=  number_to_render % FIRST_DIGIT_TEST; //decode all the digits
    digit[3] += (number_to_render % SECOND_DIGIT_TEST) / FIRST_DIGIT_TEST;
    digit[2] += (number_to_render % THIRD_DIGIT_TEST) / SECOND_DIGIT_TEST;
    digit[1] += (number_to_render % FOURTH_DIGIT_TEST) / THIRD_DIGIT_TEST;
    digit[0] += (number_to_render % FIFTH_DIGIT_TEST) / FOURTH_DIGIT_TEST;

    unsigned int number_index;
    for (number_index = NUMBER_INDEX_START; number_index <= NUMBER_INDEX_END; number_index++) // print the numbers to the screen
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


#define MENU_SHIFT_DOWN     1800        // Threshold for DOWN.
#define MENU_SHIFT_UP       2200        // Threshold for UP.
PRIVATE int get_joystick_event(void)
{
    int joy_y = input.thumbv;

    return (joy_y >= MENU_SHIFT_UP) ? MENU_SHIFT_UP
         : (joy_y <= MENU_SHIFT_DOWN) ? MENU_SHIFT_DOWN : 0;
}




PRIVATE void Render_Music_Menu_Value(unsigned int music_value)
{
    int top, mid, bot;

    if (music_value == OFF)   // Render OFF
        { top = LETTER_O;  mid = LETTER_F;  bot = LETTER_F; }
    else if (music_value == ALL)   // Render ON
        { top = BLACK_SPACE;  mid = LETTER_O;  bot = LETTER_N; }
    else if (music_value == SFX)   // Render SFX
        { top = LETTER_S;  mid = LETTER_F;  bot = LETTER_X; }
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
    unsigned int index_x, index_y;
    for (index_y = START; index_y <= BOARD_SIZE; index_y++) // render the main menu graphics
    {
        for (index_x = START; index_x <= BOARD_SIZE; index_x++)
        {
            if (mainmenu[index_y][index_x])
            {
                render_sprite(mainmenu[index_y][index_x], index_x, index_y, GENERAL, VOID);
            }
        }
    }
}

#define PB1_TIMEOUT_TICKS 5

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

    int32_t pb2_timeout = 0;
    while (in_menu)        // ENTER THE MAIN MENU STATE
    {
        uint32_t ticks = SysTick_check();
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


        if(pb2_timeout > 0) pb2_timeout -= ticks;
        else if (input.pb2)
        {
            pb2_timeout = PB1_TIMEOUT_TICKS;

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
    while (!input.pb2); // wait until PB is pressed

    lcdClear(BLACK); // clear the screen
}


void main(void)
{
    configure_all();

    // Initialize all the variables

    struct location current = {0};
    struct location ghost = {0};

    struct location old = {0};
    struct location old_ghost = {0};

    bool playing_game = true;

    unsigned int finalposition = VOID;

    current.y = INITIALY;
    current.x = INITIALX;

    // Select the first two pieces.
    unsigned int next_piece = current.piece = Select_Shape();
    next_piece = Select_Shape();
    while (next_piece == current.piece) // make sure the first 2 pieces are not the same for more variety
    {
        next_piece = Select_Shape();
    }

    unsigned int lines_cleared_this_round = 0;
    bool soft_drop = false;

    // SETUP FOR THE HIGH SCORE
    unsigned int high_score = EEPROM_Read(HIGHSCORE_ADDRESS);
    unsigned int music = EEPROM_Read(MUSICSAVE_ADDRESS);

  //  printf("high_score %d, music %d\n", high_score, music);

    if (high_score == 0xFFFFFFFF) { high_score = 0; }
    if (music == 0xFFFFFFFF) { music = OFF; }

    // ---------------------------------------------------------------------------------------------- TEST
    //Music_On(SFX);
    //SoundFX_Play(SOUND_EFFECT_7); // play the game over sound effect
//    SoundFX_Play(SOUND_EFFECT_8); // play the level sfx
//    SoundFX_Play(SOUND_EFFECT_9); // play the lines cleared sfx
//    SoundFX_Play(SOUND_EFFECT_10); // play the score sfx
//    SoundFX_Play(SOUND_EFFECT_12); // play old high score sound effect
//    SoundFX_Wait(); // wait for the sound effect to finish
//    Music_Off();
//    while (1);

    // ---------------------------------------------------------------------------------------------- END TEST

    int32_t pb1_timeout = 0, pb2_timeout = 0;

    while (FOREVER)
    {
        int new_music = Start_Menu(music); // go to the menu state, check if music is enabled

        Music_On(new_music);

        if (new_music != music)
        {
            music = new_music;
            EEPROM_Write(MUSICSAVE_ADDRESS, new_music);
        }

        Render_Background();

        Render_Piece_Preview(next_piece, current.piece);

        Render_Number(current.score, TILE_8, TILE_13); // update score on screen
        Render_Number(current.level, TILE_3, TILE_13); // update level number on screen

        while (playing_game)            // Play the game!
        {
            uint32_t ticks = SysTick_check();

            old.x = current.x;
            old.y = current.y;
            old.piece = current.piece;

            // Check if the PB has been pressed and timed out
            if(pb1_timeout > 0) pb1_timeout -= ticks;
            else if (input.pb1)
            {
                pb1_timeout = PB1_TIMEOUT_TICKS;
                current.piece = Rotate_Piece(current.x, current.y, current.piece); // rotate the piece

                unsigned int collision = current.piece >> COLLISION_DECODE;

                if (collision)                    // checks if the wall collision flag is set
                {
                    if (collision == COLLISION_FLAG_LEFT)           // if the flag is set, a collision with the left wall has occurred
                    {
                        current.piece &= 0xFF;                      // strip the collision flag, keep the orientation
                        current.x += SHIFT;                         // move the piece to the right to avoid collision
                    }

                    if (collision == COLLISION_FLAG_RIGHT)          // check if the collision was with the right wall
                    {
                        current.piece &= 0xFF;                      // strip the collision flag, keep the orientation
                        current.x -= SHIFT;                         // move the piece to the left to avoid collision
                    }
                }
            }

            current.x = Do_Shift(current.x, current.y, current.piece, ticks); // Shift the current piece corresponding to the joy stick.

            if ( (input.thumbv <= SOFT_DROP_THRESHOLD)
                  || (finalposition >= HIT_BOTTOM))        // Check if the user has initiated a soft drop, or speed things up if we hit the bottom.
            {
                soft_drop = true;
            }

            if ((Fall_Speed(current.framecount, current.level)) || (soft_drop && (current.framecount >= FRAME_SOFT_DROP_ENABLE))) // check if its time to shift the piece down
            {
                current.framecount = 0;         // reset the frame count.
                current.y += SHIFT;             // shift the current piece down

                if (Check_Collision(current.x, current.y, current.piece)) // check for collisions
                {
                    current.y -= SHIFT;         // revert the shift down
                    finalposition++;            // flag that the piece may have reached its final position
                }
                else
                {
                    finalposition = 0;          // reset the final position count so the piece doesn't freeze.
                }
            }

            ghost.x = current.x; // update the ghost pieces location
            ghost.y = current.y;
            ghost.piece = current.piece;

            while (!Check_Collision(ghost.x, ghost.y, ghost.piece)) // Shift the ghost piece down as far as possible.
            {
                ghost.y += SHIFT;
            }

            ghost.y -= SHIFT;                       // Pull the ghost piece up so its not colliding.

            // Check if the PB has been pressed, signifying a "HARD DROP".
            if(pb2_timeout > 0) pb2_timeout -= ticks;
            else if (input.pb2)
            {
               pb2_timeout = PB2_TIMEOUT_TICKS;
               current.y = ghost.y;                // Set the current piece location to the ghost piece at the bottom.
               finalposition = LOCK_POSITION;      // Set the final position to lock the piece.
            }

            if (!input.pb2 && (  (ghost.x != old_ghost.x)
                                || (ghost.y != old_ghost.y)
                                || (ghost.piece != old_ghost.piece)) ) // check if the ghost needs to be re-rendered
            {
                Render_Ghost(ghost.x, ghost.y, old_ghost.x, old_ghost.y, ghost.piece, old_ghost.piece);
            }

            old_ghost.x = ghost.x;              // store all the ghost location values so we can use them later to reduce rendering
            old_ghost.y = ghost.y;
            old_ghost.piece = ghost.piece;

            if (  (current.x != old.x)
               || (current.y != old.y)
               || (current.piece != old.piece)
               || ((current.framecount == 0) && (current.y == INITIALY))) // check if the current piece needs to be re-rendered
            {
                Render_Piece(current.x,current.y,old.x,old.y,current.piece, old.piece);
            }

            current.framecount += ticks; // increment the frame count

            if(ticks == 0) continue;

            if (finalposition >= LOCK_POSITION)
            {
                finalposition = 0;      // reset the final position flag.

                SoundFX_Start(SOUND_EFFECT_6); // Play the hit lock piece sound effect
                Set_Piece(current.x, current.y, current.piece); // the the current piece onto the gameboard

                if (lines_cleared_this_round = Clear_Completed_Lines()) // check if any lines were cleared
                {
                    old.level = current.level; // update all the score and level variables
                    old.score = current.score;

                    current.linescleared += lines_cleared_this_round;
                    current.level = current.linescleared / LINES_PER_LEVEL;
                    current.score += Calculate_Score(lines_cleared_this_round);
                }

                Render_Board(); // render any changes to the board

                if (current.score != old.score)
                {
                    Render_Number(current.score, TILE_8, TILE_13); // update score on screen
                }

                if (current.level != old.level)
                {
                    Render_Number(current.level, TILE_3, TILE_13); // update level number on screen
                }

                current.piece = next_piece; // get the next piece to be played
                next_piece = Select_Shape(); // generate the new next piece

                while (next_piece == current.piece) // make sure we dont get two of the same piece
                {
                    next_piece = Select_Shape();
                }

                Render_Piece_Preview(next_piece, current.piece); // render the piece preview

                current.y = INITIALY; // preparing all our location variables for the next round
                current.x = INITIALX;

                old.x = current.x;
                old.y = current.y;

                old.piece = current.piece;
                old_ghost.x = DONT_RENDER;
                old_ghost.y = DONT_RENDER;
                old_ghost.piece = DONT_RENDER;
                soft_drop = false;

                if (Check_Collision(current.x, current.y, current.piece))       // check if there is a collision spawning the new piece (indicating game over)
                {
                    playing_game = false;
                }
                else
                {
                    Render_Piece(current.x, current.y, old.x, old.y, current.piece, old.piece);     // if there is no collision, spawn the new piece
                }
            }
        }

        if ((current.score > high_score) || (high_score >= ERROR)) // check if the highscore has been beaten, or an error has occurred with the saved score
        {
            EEPROM_Write(HIGHSCORE_ADDRESS, current.score);
            high_score = current.score;
        }

        if (music == ALL)     // turn off the music for the GAME OVER screen.
        {
            Music_On(SFX);
        }

        Game_Over(current.score, current.linescleared, current.level, high_score); // goto the game over screen state

        current.level = 0; // reset all our score variables
        current.linescleared = 0;
        current.score = 0;

        Reset_Board(); // reset the board

        playing_game = true; // restart the fun
    }
}

// end file: tetris.c
