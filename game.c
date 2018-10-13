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

int main (void)
{
    int position[2] = {1,1};
    bool got_minput = false;
    char direction;

    static char debugCharacter = 'A';
    //int health = 3;

    //all the usual inits
    system_init ();
    navswitch_init ();

    // initialize the ir transmitter and receiver
    ir_uart_init();

    //wait for first button press
    disp_init();
    pacer_init (PACER_RATE);

    //
    main_menu(NUM_LEVELS);

    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();
        disp_update();

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
            } else {
                int laser[3];
                rec_get_laser(laser, data);//convert laser data to three integers
                disp_add_enemy_laser(laser);
            }
        }
        //do this at required frequency
        if (debugCharacter == 'Z') {
            disp_character(debugCharacter);
        }
    }
}
