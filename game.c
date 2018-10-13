#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"
#include <stdbool.h>

#include "transceive.h"
#include "disp.h"
#include "nav.h"
#include "menu.h"

#define PACER_RATE 250
#define NUM_LEVELS 3

void game_init(int position[2], char* direction, char level, bool isPlayer1) {
    if (level == 'A') {
        if (isPlayer1) {
            position[0] = 0;
            position[1] = 0;
            *direction = 'S';
        } else {
            position[0] = 4;
            position[1] = 6;
            *direction = 'N';
        }
    } else {
        if (!isPlayer1) {
            position[0] = 0;
            position[1] = 0;
            *direction = 'S';
        } else {
            position[0] = 4;
            position[1] = 6;
            *direction = 'N';
        }
    }
}

int main (void)
{
    int position[2] = {1,1};
    bool got_minput = false;
    char direction;
    char level;
    bool isPlayer1;

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
    main_menu(NUM_LEVELS, &level, &isPlayer1);
    game_init(position, &direction, level, isPlayer1);

    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();
        disp_update();

        if (!gameWon && !gameLost) {
            //take input and transmit
            got_minput = nav_getminput(&direction);//got movement input, if so changed direction

            if (got_minput) {
                if (nav_hitwall(position, direction)) {//if player will hit a wall
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
                disp_game_win();
            } else {
                int laser[3];
                rec_get_laser(laser, data);//convert laser data to three integers
                disp_add_enemy_laser(laser);
                if (laser_hit_self(laser, position)) {
                    //if hit by enemy laser, we lose
                    transmit_loss();
                    gameLost = true;
                    disp_game_lose();
                }
            }
        }
        //do this at required frequency
        if (debugCharacter == 'Z') {
            disp_character(debugCharacter);
        }
    }
}
