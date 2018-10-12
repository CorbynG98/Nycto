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

#define PACER_RATE 500
#define MESSAGE_RATE 10

void game_init(int position[2], char* direction) {
    //setup position
    position[0] = 1;
    position[1] = 1;
    *direction = 'N';
}

/*
//temporary transceive placeholders
void transmit_pos(int position[2]) {
    if (position[0] == 8) {
        position[1] = 9;
    }
}

void transmit_laser(int position[2], int direction) {
    if (position[0] == 8) {
        position[1] = 9 + direction;
    }
}

bool rec_got_data(void) {
    return true;
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
// END TEMP
*/

void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

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
    game_init(position, &direction);//game is asymmetric so players start in different places
    pacer_init (PACER_RATE);
    disp_init();

    //debug
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);



    while (1)
    {
        //system upkeep
        pacer_wait();
        navswitch_update();
        disp_update();

        //debugCharacter = getDebugChar();

        //take input and transmit
        got_minput = nav_getminput(&direction);//got movement input, if so changed direction

        if (got_minput) {
            debugCharacter = 'M';
            if (nav_hitwall(position, direction)) {//if player will hit a wall
                debugCharacter = 'H';
                transmit_pos(position);
                disp_add_self(position);
            } else {
                debugCharacter = 'N';
                nav_move(position, &direction);
            }
        }
        if (nav_shoot()) {
            int laser[3] = {position[0], position[1], direction};
            disp_add_laser(laser);
            transmit_laser(position, direction);
        }

        //receive data and display
        if (rec_got_data()) {
            // if any data received

            //store that data
            unsigned char data = rec_get_data();
            debugCharacter = 'G';

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
        if (debugCharacter == 'Z') {
            display_character (debugCharacter);
        }
    }
}
