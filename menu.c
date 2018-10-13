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
    // Variables
    int levelChosen = 1;
    int player1Chosen = 0;
    int youPlayer1 = 0;
    char currentMap = 'A';

    // Main Loop
    // Not using else if as that limits the amount of changes per
    // loop iteration
    while(levelChosen) {

        pacer_wait();
        navswitch_update();
        tinygl_update ();
        if (rec_got_data()) {
            // Data can  be received, get the data and deal with it
            if (rec_get_data() == 171) {
                youPlayer1 = 0;
                player1Chosen = 1;
            }
        }

        if (nav_shoot() && !player1Chosen) {
            // Someone has pressed the button, make them player 1.
            transmit_player1();
            player1Chosen = 1;
            youPlayer1 = 1;
        }
        if (nav_getmhorizontal() == 'E') {
            // Player moved nav switch east.
            if (currentMap == 'A' + num_levels - 1)
                currentMap = 'A';
            else
                currentMap += 1;
        }
        if (nav_getmhorizontal() == 'W') {
            // Player moved nav switch west.
            if (currentMap == 'A')
                currentMap = 'A' + num_levels - 1;
            else
                currentMap -= 1;
        }
        if (nav_shoot() && player1Chosen && youPlayer1) {
            // Player has selected the current map, current map needs
            // to be selected and loop needs to break.
            transmit_map(currentMap);
            levelChosen = 0;
        }
        // Display the current map (character)
        disp_character(currentMap);
    }
}
