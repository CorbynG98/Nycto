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
#include "nav.h"
#include "menu.h"

#define PACER_RATE 250
#define DISP_TIME 60
#define NPC_WAIT_TIME 200
#define END_WAIT_TIME 200

int main (void)
{
    int position[2] = {1,1};
    bool got_minput = false;
    char direction;
    char level;
    bool isPlayer1;

    int npc_counter = 0;
    int end_counter = 0;

    //basically an array of chars (8-bit ints)
    uint8_t bitmap[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

    static char debugCharacter = 'A';

    bool gameWon = false;
    bool gameLost = false;

    //all the usual inits
    system_init ();
    navswitch_init ();

    // initialize the ir transmitter and receiver
    ir_uart_init();

    //wait for first button press
    disp_init();
    pacer_init (PACER_RATE);

    //
    main_menu(&level, &isPlayer1, position, &direction, bitmap);

    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();
        disp_update(bitmap);

        if (!gameWon && !gameLost) {
            //take input and transmit
            got_minput = nav_getminput(&direction);//got movement input, if so changed direction

            if (got_minput) {
                if (nav_hitwall(position, direction, bitmap)) {//if player will hit a wall
                    transmit_pos(position);
                    disp_add_self(position);
                } else {
                    nav_move(position, &direction);
                }
            }
            if (nav_shoot()) {
                int laser[3] = {position[0], position[1], direction};
                disp_add_self_laser(laser);
                transmit_laser(position, direction);
            }
            if (have_npc_laser()) {
                //if npc laser exists, turn it on occasionally
                npc_counter++;
                if (npc_counter >= NPC_WAIT_TIME) {
                    disp_refresh_npc_laser();
                    npc_counter = 0;
                }
                if (hit_npc_laser(position, bitmap)) {
                    //if hit by npc laser, lose
                    transmit_loss();
                    gameLost = true;
                }
            }
        } else if (gameLost) {
            if (end_counter >= END_WAIT_TIME) {
                disp_game_lose();
            } else if (end_counter % DISP_TIME == 0){
                transmit_pos(position);
                end_counter++;
            } else {
                end_counter++;
            }
        } else if (gameWon) {
            if (end_counter >= END_WAIT_TIME) {
                disp_game_win();
            } else if (end_counter % DISP_TIME == 15){
                transmit_pos(position);
                end_counter++;
            } else {
                end_counter++;
            };
        }

        //receive data and display
        if (rec_got_data()) {
            // if any data received

            //store that data
            unsigned char data = rec_get_data();

            //identify data (check if data < 35)
            if (rec_is_enemy(data)) {
                int enemy[2];
                rec_get_enemy(enemy, data);//convert player data to two integers
                disp_add_enemy(enemy);
            } else if (rec_win(data)) {
                gameWon = true;
                transmit_pos(position);
            } else {
                int laser[3];
                rec_get_laser(laser, data);//convert laser data to three integers
                disp_add_enemy_laser(laser);
                if (laser_hit_self(laser, position, bitmap, false)) {
                    //if hit by enemy laser, we lose
                    transmit_loss();
                    gameLost = true;
                }
            }
        }
        //do this at required frequency
        if (debugCharacter == 'Z') {
            disp_character(debugCharacter);
        }
    }
}
