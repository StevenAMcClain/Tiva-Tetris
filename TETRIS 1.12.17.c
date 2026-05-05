///////////////////////////////////////////////
// File: tetris 1.12.17.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include <stdio.h>

#include "common.h"

#include "initialize.h"

#include "eeprom.h"
#include "joystick.h"
#include "music.h"
#include "pushbuttons.h"
#include "systick.h"

// Game play globals for interrupts
#include "portbits.h"

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

    printf("high_score %d, music %d\n", high_score, music);

    if (high_score == 0xFFFFFFFF) { high_score = 0; }
    if (music == 0xFFFFFFFF) { music = OFF; }

    // ---------------------------------------------------------------------------------------------- TEST
//    Music_On(SFX);
//    SoundFX_Play(SOUND_EFFECT_7); // play the game over sound effect
//    SoundFX_Play(SOUND_EFFECT_8); // play the level sfx
//    SoundFX_Play(SOUND_EFFECT_9); // play the lines cleared sfx
//    SoundFX_Play(SOUND_EFFECT_10); // play the score sfx
//    SoundFX_Play(SOUND_EFFECT_12); // play old high score sound effect
//    SoundFX_Wait(); // wait for the sound effect to finish
//    Music_Off();
//    while (1);

    // ---------------------------------------------------------------------------------------------- END TEST


	while (FOREVER)
	{
	    int new_music = Start_Menu(music); // go to the menu state, check if music is enabled

	    if (new_music == ALL) // if music is enabled
	    {
            Music_On(new_music);
	    }

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
            Frame_Tick = false;

            old.x = current.x;
            old.y = current.y;
            old.piece = current.piece;

            if (PB1_pressed)  // Check if the PB has been pressed
 	        {
                PB1_pressed = false;

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

            current.x = Do_Shift(current.x, current.y, current.piece); // Shift the current piece corresponding to the joy stick.

            if ( (JoyStick_Read_Y() <= SOFT_DROP_THRESHOLD)
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

            if (PB2_pressed)                        // Check if the PB has been pressed, signifying a "HARD DROP".
            {
                PB2_pressed = false;                // Reset the PB flag.
                current.y = ghost.y;                // Set the current piece location to the ghost piece at the bottom.
                finalposition = LOCK_POSITION;      // Set the final position to lock the piece.
            }

            if (!PB2_pressed && (  (ghost.x != old_ghost.x)
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

            current.framecount++; // increment the frame count

            while (!Frame_Tick && (finalposition != LOCK_POSITION))  { continue; }

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


// end file: Tetris 1.12.17.c
