#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "pacer.h"
#include <stdbool.h>

#include "disp.h"
#include "nav.h"

#define PACER_RATE 500

void game_init(int position[2], char* direction) {
    //setup position
    position[0] = 1;
    position[1] = 1;
    *direction = 'N';
}

//temporary transceive placeholders
void transmit_pos(int position[2]) {
    ir_uart_putc(position[0]);
}

void transmit_laser(int position[2], int direction) {
    ir_uart_putc(position[0]+direction);
}

bool rec_got_data(void) {
    return false;
}

char rec_get_data(void) {
    return 'E';
}

bool rec_is_enemy(char data) {
    return data == 'E';
}

void rec_get_enemy(int enemy[2], char data) {
    if (data == 'E') {
        enemy[0] = 3;
        enemy[1] = 3;
    }
}

void rec_get_laser(int laser[3], char data) {
    if (data == 'L') {
        laser[0] = 3;//x
        laser[1] = 3;//y
        laser[2] = 3;//direction 0,1,2,3 => n,e,s,w
    }
}


int main (void)
{
    int position[2] = {1,1};
    bool got_minput = false;
    char direction;

    //int health = 3;

    //all the usual inits
    system_init ();
    navswitch_init ();

    //wait for first button press
    game_init(position, &direction);//game is asymmetric so players start in different places
    pacer_init (PACER_RATE);
    disp_init();



    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();

        //take input and transmit
        got_minput = nav_getminput(&direction);//got movement input, if so changed direction

        if (got_minput) {
            if (nav_hitwall(position, direction)) {//if hit wall
                transmit_pos(position);
                disp_add_self(position);
            } else {
                nav_move(position, &direction);
            }
        }
        if (nav_shoot()) {
            transmit_laser(position, direction);
        }

        //receive data and display
        if (rec_got_data()) {
            // if any data received

            //store that data
            char data = rec_get_data();

            //identify data (check if data < 35)
            if (rec_is_enemy(data)) {
                int enemy[2];
                rec_get_enemy(enemy, data);//convert player data to two integers
                disp_add_enemy(enemy);
            } else {
                int laser[3];
                rec_get_laser(laser, data);//convert laser data to three integers
                disp_add_laser(laser);
            }
        }
        //do this at required frequency
        disp_update();
    }
}
