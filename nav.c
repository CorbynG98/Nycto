#include "system.h"
#include "stdbool.h"
#include "navswitch.h"

bool nav_getminput(char* prev_dir) {
    // Initialise and check what button was pressed.
    system_init ();
    navswitch_init ();
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

bool nav_move(int[] position, char* prev_dir) {
    // Attempt to move the player in the direction they chose
    if (*prev_dir = 'N') {
        position[0] += 1;
    } else if (*prev_dir = 'E') {
        position[1] -= 1;
    } else if (*prev_dir = 'S') {
        position[0] -= 1;
    } else if (*prev_dir = 'W') {
        position[1] += 1;
    }
    return true;
}

/* char nav_delta(position, last_pos) {
    // Calculate the direction the player has moved in.
    if (last_pos[0] - position[0] > 0) {
        return 'E';
    } else if (last_pos[0] - positon[0] < 0) {
        return 'W';
    } else if (last_pos[1] - positon[1] > 0) {
        return 'N';
    } else if (last_pos[1] - positon[1] < 0) {
        return 'S';
    }
    return NULL;

} */

bool nav_shoot(position, direction) {
    // Return true if the player tried to shoot.
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
        return true;
    return false;
}

bool nav_hitwall(int[] position, char move_dir) {
    // Check if the player is trying to move into a wall.
    if (move_dir == 'N') {
        if (position[1] == 0)
            return true;
    } else if (move_dir == 'S') {
        if (position[1] == 4)
            return true;
    } else if (move_dir == 'E') {
        if (position[0] == 6)
            return true;
    } else if (move_dir == 'W') {
        if (position[0] == 0)
            return true;
    }
    return false;
}
