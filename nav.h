#ifndef NAV_H
#define NAV_H

/** Initialise and check what button was pressed. */
bool nav_getminput(char* prev_dir);

/** Attempt to move the player in the direction they chose */
bool nav_move(int[] position, char* prev_dir);

/** Return true if the player tried to shoot. */
bool nav_shoot(position, direction);

/** Check if the player is trying to move into a wall. */
bool nav_hitwall(int[] position, char move_dir);

#endif
