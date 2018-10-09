#include "system.h"
#include "navswitch.h"
#include "pacer.h"
#include <stdbool.h>

#include "disp.h"
#include "nav.h"

#define PACER_RATE 500

void game_init(int[2] position, char direction) {
    //setup position
    position[0] = 0;
    position[1] = 0;
    direction = 'N';
}

int main (void)
{
    int position[2];
    bool got_minput = false;
    char direction;

    //int health = 3;

    //all the usual inits
    system_init ();
    navswitch_init ();

    //wait for first button press
    game_init(position, direction);//game is asymmetric so players start in different places
    disp_init();
    pacer_init (PACER_RATE);



    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();

        //take input and transmit
        got_minput = nav_getminput(&direction);//got movement input

        if (got_minput) {
            if (nav_hit_wall(position, &direction)) {//if hit wall
                transmit_pos(position);
                disp_add_self(position);
            } else {
                nav_move(position, &direction);
            }
        }
        if (nav_shoot(position)) {
            transmit_laser(position, direction);
        }

        //receive data and display
        if (rec_got_data()) {
            // if any data received

            //store that data
            char data = rec_get_data();

            //identify data (check if data < 35)
            if (rec_is_enemy(data)) {
                int[2] enemy;
                rec_get_player(enemy, data);//convert data to two integers
                disp_add_enemy(enemy);
            } else {
                int[3] laser;
                rec_get_laser(laser, data);//convert data to three integers
                disp_add_laser(laser);
            }
        }
        //do this at required frequency
        disp_update();
    }
}
