#include "system.h"
#include "stdbool.h"
#include "navswitch.h"
#include "nav.h"

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

char nav_getmhorizontal(void) {
    if (navswitch_push_event_p (NAVSWITCH_EAST))
        return 'E';
    else if (navswitch_push_event_p (NAVSWITCH_WEST))
        return 'W';
    return 'Q';
}

/* char nav_delta(position, last_pos) {
    // Calculate the direction the player has moved in.
    if (last_pos[0] - position[0] > 0) {
        return 'E';
    } else if (last_pos[0] - position[0] < 0) {
        return 'W';
    } else if (last_pos[1] - position[1] > 0) {
        return 'N';
    } else if (last_pos[1] - position[1] < 0) {
        return 'S';
    }
    return NULL;

} */

/** Return true if the player tried to shoot. */
bool nav_shoot(void) {
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
        return true;
    return false;
}

/** Convert the uint8_t bitmap value to a binary string **/
void ul32toBinary(uint8_t a, char binary[])
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1)
        (a&i)?strcat(binary, "0"):strcat(binary, "1");
}

/** Check if the player hits one of the inside walls **/
bool nav_hitwallinner(int position[], char move_dir, const uint8_t bitmap[])
{
    // Check if hit inner walls
    int toMove[2] = position;
    if (move_dir == 'N') {
        toMove[0] += 1;
    } else if (move_dir == 'S') {
        toMove[0] -= 1;
    } else if (move_dir == 'E') {
        toMove[1] += 1;
    } else if (move_dir == 'W') {
        toMove[1] -= 1;
    }

    char binary[8] = "";
    ul32toBinary(bitmap[toMove[0]], binary);
    if(binary[toMove[1]] == '1') {
        return true;
    }
    return false;
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
    return(nav_hitwallinner(position[] move_dir, bitmap[]));
}
