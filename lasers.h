/** @file   lasers.h
    @author Benjamin Scott
    @date   16 October 2018
    @brief  Laser construction & collision detection
*/

#ifndef LASERS_H
#define LASERS_H

#include <stdbool.h>
#include <stdint.h>
#include "tinygl.h"

/** given a starting position, find laser coordinates */
void set_laser_coords(int* startx, int* starty, int* endx, int* endy, char direction, const uint8_t level[], bool isNPC);

/** swaps two integers */
void swap_int(int* x, int* y);

/** check if we the laser intersects with position, taking into account
 * the level */
bool laser_hit_self(int laser[3], int position[2], const uint8_t level[], bool isNPC);

#endif
