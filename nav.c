/*
 * File:   nav.c
 * Author: Corbyn Greenwood
 * Date:   12 Oct 2018
 * Descr:  nav switch control module. deals with inputs from the nav
 *         switch.
*/

#include "system.h"
#include "stdbool.h"
#include "navswitch.h"
#include "nav.h"
#include <stdint.h>
#include <string.h>

/** Initialise and check what button was pressed. */
bool nav_getminput(char* prev_dir) {
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        *prev_dir = 'N';
        return true;
    } else if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        *prev_dir = 'E';
        return true;
    } else if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        *prev_dir = 'S';
        return true;
    } else if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        *prev_dir = 'W';
        return true;
    }
    return false;
}

/** Attempt to move the player in the direction they chose */
bool nav_move(int position[], char* prev_dir) {
    if (*prev_dir == 'N') {
        position[1] -= 1;
    } else if (*prev_dir == 'E') {
        position[0] += 1;
    } else if (*prev_dir == 'S') {
        position[1] += 1;
    } else if (*prev_dir == 'W') {
        position[0] -= 1;
    }
    return true;
}

/** Gets a horizontal nav switch event. Used for main menu
 *  level selection **/
char nav_getmhorizontal(void) {
    if (navswitch_push_event_p (NAVSWITCH_EAST))
        return 'E';
    else if (navswitch_push_event_p (NAVSWITCH_WEST))
        return 'W';
    return 'Q';
}

/** Return true if the player tried to shoot. */
bool nav_shoot(void) {
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
        return true;
    return false;
}

/** Check if the player hits one of the inside walls **/
bool nav_hitwallinner(int position[], char move_dir, const uint8_t bitmap[])
{
    // Check if hit inner walls
    int toMove[2] = {0,0};
    toMove[0] = position[0];
    toMove[1] = position[1];
    nav_move(toMove, &move_dir);

    return bitmap[toMove[0]] & (1 << toMove[1]);
}

/** Check if the player is trying to move into a wall. */
bool nav_hitwall(int position[], char move_dir, const uint8_t bitmap[])
{
    // Check if hit outer wall
    if (move_dir == 'N') {
        if (position[1] <= 0)
            return true;
    } else if (move_dir == 'S') {
        if (position[1] >= 6)
            return true;
    } else if (move_dir == 'E') {
        if (position[0] >= 4)
            return true;
    } else if (move_dir == 'W') {
        if (position[0] <= 0)
            return true;
    }

    // Check inner walls hit
    return(nav_hitwallinner(position, move_dir, bitmap));
}
