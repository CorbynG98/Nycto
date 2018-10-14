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
#include "disp.h"

#define MESSAGE_RATE 10
#define NUM_LEVELS 3

void build_level(char, uint8_t[]);
void set_player_pos(int[], char*, bool, char);

/** Loop untill a level has been selected by the player **/
void main_menu(char* level, bool* isPlayer1, int position[], char* direction, uint8_t bitmap[])
{
    // Variables
    int levelChosen = 0;
    int player1Chosen = 0;
    int youPlayer1 = 0;
    char currentMap = 'S';

    // Main Loop
    // Not using else if as that limits the amount of changes per
    // loop iteration
    while(!levelChosen) {

        pacer_wait();
        navswitch_update();
        tinygl_update ();

        // Display the current map (character)
        if (currentMap == 'S' || currentMap == 'W') {
            disp_character(currentMap);
        } else {
            disp_clear_character();
            // Build the currently selected level for the player to see.
            display_clear();
            build_level(currentMap, bitmap);
        }

        if (rec_got_data()) {
            // Data can  be received, get the data and deal with it
            unsigned char received = rec_get_data();
            if (rec_player(received) && !player1Chosen) {
                youPlayer1 = 0;
                *isPlayer1 = false;
                player1Chosen = 1;
                currentMap = 'W';
            }
            if (received >= 'A' && received <= ('A' + NUM_LEVELS - 1)) {
                levelChosen = 1;
                set_player_pos(position, direction, *isPlayer1, *level);
                currentMap = received;
                *level = received;
                build_level(currentMap, bitmap);
            }
        }

        if (nav_shoot() && player1Chosen && youPlayer1) {
            // Player has selected the current map, current map needs
            // to be selected and loop needs to break.
            transmit_map(currentMap);
            *level = currentMap;
            levelChosen = 1;
            *isPlayer1 = true;
            set_player_pos(position, direction, *isPlayer1, *level);
        }
        if (nav_shoot() && !player1Chosen) {
            // Someone has pressed the button, make them player 1.
            transmit_player1();
            player1Chosen = 1;
            youPlayer1 = 1;
            currentMap = 'A';
        }
        if (nav_getmhorizontal() == 'E' && youPlayer1) {
            // Player moved nav switch east.
            if (currentMap == 'A' + NUM_LEVELS - 1)
                currentMap = 'A';
            else
                currentMap += 1;
        }
        if (nav_getmhorizontal() == 'W' && youPlayer1) {
            // Player moved nav switch west.
            if (currentMap == 'A')
                currentMap = 'A' + NUM_LEVELS - 1;
            else
                currentMap -= 1;
        }
    }
    disp_clear_character();
}

void build_level(char map, uint8_t bitmap[]) {
    // Where the map gets made
    if (map == 'A') {
        // Build map A
        bitmap[0] = 0x00;
        bitmap[1] = 0x00;
        bitmap[2] = 0x77;
        bitmap[3] = 0x00;
        bitmap[4] = 0x00;
    } else if (map == 'B') {
        // Build map B
        bitmap[0] = 0x00;
        bitmap[1] = 0x37;
        bitmap[2] = 0x00;
        bitmap[3] = 0x76;
        bitmap[4] = 0x00;
        int laser[3] = {0, 3, 'E'};
        disp_add_npc_laser(laser);
    } else if (map == 'C') {
        // Build map C
        bitmap[0] = 0x00;
        bitmap[1] = 0x22;
        bitmap[2] = 0x2A;
        bitmap[3] = 0x22;
        bitmap[4] = 0x00;
    }
    disp_bitmap(bitmap);
}

void set_player_pos(int position[], char* direction, bool isPlayer1, char level) {
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
    } else if (level == 'B') {
        if (!isPlayer1) {
            position[0] = 0;
            position[1] = 0;
            *direction = 'S';
        } else {
            position[0] = 4;
            position[1] = 6;
            *direction = 'N';
        }
    } else if (level == 'C') {
        if (!isPlayer1) {
            position[0] = 1;
            position[1] = 0;
            *direction = 'E';
        } else {
            position[0] = 3;
            position[1] = 6;
            *direction = 'W';
        }
    }
}
