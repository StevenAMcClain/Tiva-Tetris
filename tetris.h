///////////////////////////////////////////////
//              TETRIS
//
//  CREATED on: Oct 12, 2017
//          BY: ALEKSANDER OLESEVITCH
//
//  Updated for Tiva on Apr 11, 2026
//
///////////////////////////////////////////////

#ifndef TETRIS_H_
#define TETRIS_H_

#define HIGHSCORE_ADDRESS 0
#define MUSICSAVE_ADDRESS 8

// Colors

#define SILVER  0xC618
#define GRAY    0x8410
#define RED     0xF800
#define YELLOW  0xFFE0
#define LIME    0x07E0
#define AQUA    0x07FF
#define BLUE    0x001F
#define NAVY    0x0014
#define FUCHSIA 0xFC1F
#define TEAL    0x0410
#define WHITE   0xFFFF
#define BLACK   0x0000
//

#define YELLOW_BRICK        0x01
#define GREEN_BRICK         0x02
#define AQUA_BRICK          0x03
#define BLUE_BRICK          0x13
#define NAVY_BRICK          0x23
#define FUCSHIA_BRICK       0x33
#define WHITE_BRICK         0x50
#define RED_BRICK           0x100


#define BORDER_TOPLEFT      0x10
#define BORDER_TOP          0x11
#define BORDER_TOPRIGHT     0x12
#define BORDER_LEFT         0x20
#define BLACK_SPACE         0x21
#define BORDER_RIGHT        0x22
#define BORDER_BOTTOM       0x31
#define BORDER_BOTTOMLEFT   0x30
#define BORDER_BOTTOMRIGHT  0x32

#define BRICK_LEFTBORDER    0x38
#define BRICK               0x39
#define BRICK_RIGHTBORDER   0x3A

//
#define LETTER_A            0x04
#define LETTER_B            0x05
#define LETTER_C            0x06
#define LETTER_D            0x07
#define LETTER_E            0x08
#define LETTER_F            0x09
#define LETTER_G            0x0A
#define LETTER_H            0x14
#define LETTER_I            0x15
//#define LETTER_J            0x16
#define LETTER_K            0x17
#define LETTER_L            0x18
#define LETTER_M            0x19
#define LETTER_N            0x1A
#define LETTER_O            0x24
//#define LETTER_P            0x25
//#define LETTER_Q            0x26
#define LETTER_R            0x27
#define LETTER_S            0x28
#define LETTER_T            0x29
#define LETTER_U            0x2A
#define LETTER_V            0x34
#define LETTER_W            0x35
#define LETTER_X            0x36
//#define LETTER_Y            0x37
//#define LETTER_Z            0x4A

//#define NUMBER_0            0x40
//#define NUMBER_1            0x41
//#define NUMBER_2            0x42
//#define NUMBER_3            0x43
//#define NUMBER_4            0x44
//#define NUMBER_5            0x45
//#define NUMBER_6            0x46
//#define NUMBER_7            0x47
//#define NUMBER_8            0x48
//#define NUMBER_9            0x49

#define SYMBOL_ARROW        0x51
#define SYMBOL_COLON        0x52
#define SYMBOL_EXCLAMATION  0x53
//
//
//
#define YCOORD              0xF0
#define XCOORD              0x0F
#define END_OF_SCREEN       120
#define TILE                6
#define BOARD_SIZE          19
#define TILE_SIZE           5
#define START               0

#define MAX_PIECES          19
#define PIECEOFFSET         2
#define MOVELEFT            -1
#define MOVERIGHT           1
#define SHIFT               1

#define PIECE_SIZE           5
#define SHAPERANGE          4

#define BOTTOM              21
#define TOP                 0
#define BOARDWIDTH          9
#define TETRIS              10
#define INITIALX            5
#define INITIALY            2

#define ENABLE              1
#define DISABLED            0
#define FIRST_DIGIT_TEST    10
#define SECOND_DIGIT_TEST   100
#define THIRD_DIGIT_TEST    1000
#define FOURTH_DIGIT_TEST   10000
#define FIFTH_DIGIT_TEST    100000
#define NUMBER_START_POSITION 0x40

#define LINES_PER_LEVEL     10

#define DONT_RENDER         0xFFFF
#define DROP                1
#define BLOCK               1
#define GENERAL             0
#define OVERRIDE            2

#define PIECE_PREVIEW_TILE_OFFSET_X 13
#define PIECE_PREVIEW_TILE_OFFSET_Y 14
//
#define START_GAME          1
#define MUSIC_SELECT        2
#define ALL                 1
#define SFX                 2
#define OFF                 0

#define VOID_SCORE          1738

//#define TILE_1              1
#define TILE_2              2
#define TILE_3              3
#define TILE_4              4
#define TILE_5              5
#define TILE_6              6
#define TILE_7              7
#define TILE_8              8
#define TILE_9              9
#define TILE_10             10
#define TILE_11             11
#define TILE_12             12
#define TILE_13             13
#define TILE_14             14
#define TILE_15             15
#define TILE_16             16
//#define TILE_17             17
#define TILE_18             18
//#define TILE_19             19
//#define TILE_20             20

#define SOFT_DROP_THRESHOLD 1800
#define HIT_BOTTOM          1
#define FRAME_SOFT_DROP_ENABLE 3
#define NO_COLLISION        0
#define LOCK_POSITION       6
#define COLLISION_DECODE    8
//#define NO_COLLISION        0
#define COLLISION_FLAG_LEFT 1
#define COLLISION_FLAG_RIGHT 2

//#define BLOCK_SIZE          4096
#define ERROR               10000
#define SPRITE_DECODE       4

#define EASY                8
#define MEDIUM              9
#define HARD                12
#define VETERAN             15
#define INSANE              18
#define SADIST              28

#define START_SPEED         48
#define SPEED_SCALER        5
#define SECOND_SPEED        6
#define THIRD_SPEED         5
#define FOURTH_SPEED        4
#define FIFTH_SPEED         3
#define SIXTH_SPEED         2

#define RENDER_X_OFFSET     1
#define RENDER_Y_OFFSET     4

#define NUMBER_INDEX_START  0
#define NUMBER_INDEX_END    4

#define PRINT_NUM_ARRAY_SIZE 5

#define T_BLOCK_FIRST       0
#define T_BLOCK_LAST        3
#define J_BLOCK_FIRST       4
#define J_BLOCK_LAST        7
#define Z_BLOCK_FIRST       8
#define Z_BLOCK_LAST        9
#define O_BLOCK             10
#define S_BLOCK_FIRST       11
#define S_BLOCK_LAST        12
#define L_BLOCK_FIRST       13
#define L_BLOCK_LAST        16
#define I_BLOCK_FIRST       17
#define I_BLOCK_LAST        18

#define LEFT_COL_ONE        1
#define LEFT_COL_TWO        0

#define COL_TEST_OFFSET     1

#define RIGHT_COL_ONE       8
#define RIGHT_COL_TWO       9

#define LEFT_COLLISION_ADJUST  0x100
#define RIGHT_COLLISION_ADJUST 0x200

#define OVERLAP             2
#define MAXIMUM_LINES_CLEARED 4
#define FLOOR_COLLISION     22
#define LOW_BITS            0xFF


#define JOYSTICK_LEFT       1800
#define JOYSTICK_RIGHT      2200

#define SCREEN_ROWS         20
#define SCREEN_COLS         20

#define SPRITE_SHEET_ROWS   36
#define SPRITE_SHEET_COLS   66

#define BOARD_ROWS          22
#define BOARD_COLS          10

#define GAME_PIECE_ROWS     5
#define GAME_PIECE_COLS     95

#define BUFFER_ROWS         6
#define BUFFER_COLS         10
#define BUFFER_OFFSET       2

#define PIECE_PROXIMITY     1
#define PIECE_RENDER_X_MAX  9
#define PIECE_RENDER_Y_MAX  5

#define VOID    0x0000

int Do_Shift(unsigned int xpos, unsigned int ypos, unsigned int cshape, unsigned int ticks);
bool Check_Collision(int xref, unsigned int yref, unsigned int shape);
void Set_Piece(unsigned int xcur, unsigned int ycur, unsigned int pieceshape);
int Clear_Completed_Lines();
void Render_Piece(unsigned int renderlocx, unsigned int renderlocy, unsigned int clearlocx, unsigned int clearlocy, unsigned int pieceshape, unsigned int oldpieceshape);
int Select_Shape();
void Render_Board();
void Render_Background();
int Rotate_Piece(unsigned int x_col_test, unsigned int y_col_test, unsigned int current_orientation);
void Reset_Board();
void Render_Number(unsigned int number_to_render, unsigned int y_start_pos, unsigned int x_start_pos);
void Render_Piece_Preview(unsigned int previewpiece, unsigned int previouspiece);
int Calculate_Score (unsigned int number_of_lines);
void Render_Ghost(unsigned int ghostx , unsigned int ghosty , unsigned int oldghostx , unsigned int oldghosty , unsigned int ghostshape , unsigned int oldghostshape);
int Fall_Speed(unsigned int current_frame, unsigned int current_level);
int Start_Menu(unsigned int music_value);
void Game_Over(unsigned int final_score, unsigned int final_lines, unsigned int final_level, unsigned int high_score);

struct location {
        unsigned int y;
        unsigned int x;
        unsigned int piece;
        unsigned int score;
        unsigned int level;
        unsigned int linescleared;
        unsigned int framecount;
    };

#endif /* TETRIS_H_ */



//#define CONVERSION_FACTOR   16
//
//#define RESET               0
//#define PB1                 BIT1
//#define PB2                 BIT5
//#define DEBOUNCE            640 //320= 10ms * 32 cycles / ms
//#define OUTOFRANGE          10
//#define FIXEDRANGE          9
////#define NOCOLLISION         0
////#define COLLISION           1
////#define INGAME              1
////#define OVER                0

//#define RENDEROFFSET        6
//#define TESTFORBOTTOM       2
//#define SIXTYFPS    533
//
////#define ONE_LINE            1
////#define TWO_LINES           2
////#define THREE_LINES         3
////#define FOUR_LINES          4
////
////#define POINTSX1            100
////#define POINTSX2            200
////#define POINTSX3            400
////#define POINTSX4            800

//#define CLEAR_WALL_COLIDE_FLAG 0xFF
//#define INMENU              0
//#define END_OF_SOUND        -1
//#define SCORE_LOCATION 0x23000
