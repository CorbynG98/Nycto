/** @file   lasers.c
    @author Benjamin Scott
    @date   16 October 2018
    @brief  Laser construction & collision detection
*/

#include "lasers.h"

/** given a starting position, find laser coordinates */
void set_laser_coords(int* startx, int* starty, int* endx, int* endy, char direction, const uint8_t level[], bool isNPC)
{
    //set default position if hit no walls, moving one position past the player
    int increment = 1;
    if (isNPC)
        increment = 0;
    switch (direction) {
        case 'N':
            (*starty)-=increment;
            *endy = 0;
            break;
        case 'E':
            (*startx)+=increment;
            *endx = TINYGL_WIDTH;
            break;
        case 'S':
            (*starty)+=increment;
            *endy = TINYGL_HEIGHT;
            break;
        case 'W':
            (*startx)-=increment;
            *endx = 0;
            break;
    }

    //check if laser hits a wall
    bool hitWall = false;
    if (*startx < *endx || *starty < *endy) {
        //if pointing down or right
        int i=*startx;
        while (i <= *endx && !hitWall) {
            int j=*starty;
            while (j <= *endy && !hitWall) {
                if (level[i] & (1 << j)) {
                    //if level has a wall at i,j
                    *endx = i;
                    *endy = j;
                    hitWall = true;
                }
                j++;
            }
            i++;
        }
    } else {
        int i=*startx;
        while (i >= *endx && !hitWall) {
            int j=*starty;
            while (j >= *endy && !hitWall) {
                if (level[i] & (1 << j)) {
                    //if level has a wall at i,j
                    *endx = i;
                    *endy = j;
                    hitWall = true;
                }
                j--;
            }
            i--;
        }
    }



}

/** swaps two integers */
void swap_int(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

/** check if the laser intersects with a position, taking into account
 * the level */
bool laser_hit_self(int laser[3], int position[2], const uint8_t level[], bool isNPC)
{
    int endx = laser[0];
    int endy = laser[1];
    int startx = laser[0];
    int starty = laser[1];

    set_laser_coords(&startx ,&starty, &endx, &endy, laser[2], level, isNPC);

    //make sure start is before end
    if (startx > endx) {
        swap_int(&startx, &endx);
    }
    if (starty > endy) {
        swap_int(&starty, &endy);
    }

    //if hit by enemy laser
    for (int i=startx;i <= endx; i++) {
        for (int j=starty;j <= endy; j++) {
            if (position[0] == i && position[1] == j) {
                return true;
            }
        }
    }
    return false;
}
