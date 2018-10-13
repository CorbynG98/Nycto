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

void build_level(char);

/** Loop untill a level has been selected by the player **/
void main_menu(int num_levels, char* level, bool* isPlayer1)
{
    // Variables
    int levelChosen = 0;
    int player1Chosen = 0;
    int youPlayer1 = 0;
    char currentMap = 'A';

    // Main Loop
    // Not using else if as that limits the amount of changes per
    // loop iteration
    while(!levelChosen) {

        pacer_wait();
        navswitch_update();
        tinygl_update ();

        // Display the current map (character)
        disp_character(currentMap);

        if (rec_got_data()) {
            // Data can  be received, get the data and deal with it
            unsigned char received = rec_get_data();
            if (received == 171 && !player1Chosen) {
                youPlayer1 = 0;
                *isPlayer1 = false;
                player1Chosen = 1;
            }
            if (received >= 'A' && received <= ('A' + num_levels)) {
                levelChosen = 1;
                currentMap = received;
                *level = currentMap;
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
            *level = currentMap;
            levelChosen = 1;
            *isPlayer1 = true;
            build_level(currentMap);
        }
    }
    disp_clear_character();
}

void build_level(char map) {
    // Where the map gets made
}
