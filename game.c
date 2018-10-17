/** @file   game.c
    @author Benjamin Scott
    @date   16 October 2018
    @brief  Game manager, utilises the other modules heavily
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"
#include <stdbool.h>
#include <stdint.h>

#include "transceive.h"
#include "disp.h"
#include "lasers.h"
#include "nav.h"
#include "menu.h"

#define NPC_WAIT_TIME 200 //time between npc laser bursts
#define END_WAIT_TIME 1000 //time to display players before end

void move (int position[2], char* direction, uint8_t bitmap[5], bool* gameLost, int* npc_counter);
void interpret_data (int position[2], uint8_t bitmap[5], bool* gameWon, bool* gameLost);

int main (void)
{
    //--------GAME VARIABLES-------
    //unchanging variables (set once by menu)
    char level;
    bool isPlayer1;

    //player state
    int position[2] = {1,1};
    char direction;

    //level state
    int npc_counter = 0;
    uint8_t bitmap[5] = {0x00, 0x00, 0x00, 0x00, 0x00};//basically an array of chars (8-bit ints)

    //game state
    int end_counter = 0;
    bool gameWon = false;
    bool gameLost = false;

    //-----MODULE INITIALISATION----
    //all the usual inits
    system_init ();
    navswitch_init ();
    ir_uart_init();// initialize the ir transmitter and receiver
    disp_init();

    //----------GAME START----------
    main_menu(&level, &isPlayer1, position, &direction, bitmap);

    while (1)
    {
        //system upkeep
        disp_update(bitmap);
        navswitch_update();

        if (gameLost) {
            if (end_counter >= END_WAIT_TIME) {
                disp_game_lose();
            } else if (end_counter % DISP_TIME == 0){
                transmit_pos(position);
                disp_add_self(position);
                end_counter++;
            } else {
                end_counter++;
            }
        } else if (gameWon) {
            if (end_counter >= END_WAIT_TIME) {
                disp_game_win();
            } else if (end_counter % DISP_TIME == DISP_TIME/2){
                transmit_pos(position);
                disp_add_self(position);
                end_counter++;
            } else {
                end_counter++;
            }
        } else {
            //take input and transmit
            move(position, &direction, bitmap, &gameLost, &npc_counter);
        }

        //receive data and display
        if (rec_got_data()) {
            // if any data received
            interpret_data (position, bitmap, &gameWon, &gameLost);
        }
    }
}

/** use nav to work out how to move, display changes using disp */
void move (int position[2], char* direction, uint8_t bitmap[5], bool* gameLost, int* npc_counter)
{
    bool got_minput = nav_getminput(direction);//got movement input, if so changed direction

    if (got_minput) {
        if (nav_hitwall(position, *direction, bitmap)) {//if player will hit a wall
            transmit_pos(position);
            disp_add_self(position);
        } else {
            nav_move(position, direction);
        }
    }
    if (nav_shoot()) {
        int laser[3] = {position[0], position[1], *direction};
        disp_add_self_laser(laser);
        transmit_laser(position, *direction);
    }
    if (have_npc_laser()) {
        //if npc laser exists, turn it on occasionally
        (*npc_counter)++;
        if ((*npc_counter) >= NPC_WAIT_TIME) {
            disp_refresh_npc_laser();
            (*npc_counter) = 0;
        }
        if (hit_npc_laser(position, bitmap)) {
            //if hit by npc laser, lose
            transmit_loss();
            *gameLost = true;
        }
    }
}

/** use transceive to receive actions, display them using disp*/
void interpret_data (int position[2], uint8_t bitmap[5], bool* gameWon, bool* gameLost)
{
    //store data
    unsigned char data = rec_get_data();

    //identify data (check if data < 35)
    if (rec_is_enemy(data)) {
        int enemy[2];
        rec_get_enemy(enemy, data);//convert player data to two integers
        disp_add_enemy(enemy);
    } else if (rec_win(data)) {
        *gameWon = true;
        transmit_pos(position);
    } else {
        int laser[3];
        rec_get_laser(laser, data);//convert laser data to three integers
        disp_add_enemy_laser(laser);
        if (laser_hit_self(laser, position, bitmap, false) && !*gameWon) {
            //if hit by enemy laser, we lose
            transmit_loss();
            *gameLost = true;
        }
    }
}
