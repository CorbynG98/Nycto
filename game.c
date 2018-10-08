#include "system.h"
#include "navswitch.h"
#include "pacer.h"
#include <stdbool.h>
#include "disp.h"

#define PACER_RATE 500

void game_init(int[2] position, char direction) {
    //setup position

}

int main (void)
{
    int position[2] = {0,0};
    int last_pos[2] = {0,0};
    bool got_minput = false;
    char direction = 'N';
    int health = 3;

    int enemy_position[2] = {0,0};

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
                display_self(position);
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
            char data = rec_get_data();
            if (rec_is_player(data)) {
                int[2] player;
                rec_get_player(player, data);
                disp_add_player(player);
            } else {
                int[3] laser;
                rec_get_laser(laser, data);
                disp_add_laser(laser);
            }
        }
        //do this at required frequency
        disp_update();

        //update vars
        last_pos[0] = position[0];
        last_pos[1] = position[1];
    }
}
