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

#define MESSAGE_RATE 10

/** Loop untill a level has been selected by the player **/
void main_menu(int num_levels)
{
    //DEBUG
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    // Variables
    int levelChoosen = 1;
    int player1Choosen = 0;
    int youPlayer1 = 0;
    char currentMap = 1;

    // Main Loop
    // Not using else if as that limits the amount of changes per
    // loop iteration
    while(levelChoosen) {

        if (rec_got_data()) {
            // Data can  be received, get the data and deal with it
            if (rec_get_data() == 171) {
                youPlayer1 = 0;
                player1Choosen = 1;
            }
        }

        if (nav_shoot() && !player1Choosen) {
            // Someone has pressed the button, make them player 1.
            transmit_player1();
            player1Choosen = 1;
            youPlayer1 = 1;
        }
        if (nav_getmhorizontal() == 'E') {
            // Player moved nav switch east.
            if (currentMap == num_levels)
                currentMap = 0;
            else
                currentMap += 1;
        }
        if (nav_getmhorizontal() == 'W') {
            // Player moved nav switch west.
            if (currentMap == 0)
                currentMap = num_levels;
            else
                currentMap -= 1;
        }
        if (nav_shoot() && player1Choosen && youPlayer1) {
            // Player has selected the current map, current map needs
            // to be selected and loop needs to break.
            transmit_map(currenMap);
            levelChoosen = 0;
        }
        // Display the current map (character)
        display_character(currentMap);
    }
}
