#include "tinygl.h"
#include "disp.h"

#define DISPLAY_TIME 100 //time to display a player for
#define DISPLAY_LASER_TIME 50 // time to display a laser for
#define OUTPUT_HIGH 1
#define MAX_LASERS 10
#define SCREEN_WIDTH TINYGL_WIDTH
#define SCREEN_HEIGHT TINYGL_HEIGHT

Laser laserPool[MAX_LASERS];
Player self = {0, 0, 0};
Player enemy = {0, 0, 0};

/** sets screen off */
void disp_init(void)
{
    tinygl_init (PACER_RATE);
    self.counter = DISPLAY_TIME;
    enemy.counter = DISPLAY_TIME;
}

/** display self for a time */
void disp_add_self(int[2] position)
{
    self.counter = DISPLAY_TIME;
    self.x = position[0];
    self.y = position[1];
}

/** display the enemy for a time */
void disp_add_enemy(int[2] position)
{
    enemy.counter = DISPLAY_TIME;
    enemy.x = position[0];
    enemy.y = position[1];
}

/** creates new laser */
void disp_add_laser(int[3] laser)
{
    for (int i=0; i < MAX_LASERS;i++) {
        //if laser is no longer displayed, add new laser details
        if (laserPool[i].counter == 0) {
            laserPool[i].counter = DISPLAY_LASER_TIME;
            laserPool[i].x = laser[0];
            laserPool[i].y = laser[1];
            laserPool[i].direction = laser[2];
            num_lasers++;
        }
    }
}

/** Displays a laser for a frame*/
void disp_laser(Laser laser)
{
    tinygl_point start = tinygl_point (laser.x, laser.y);
    tinygl_point end = tinygl_point (laser.x, laser.y);
    switch (direction) {
        case 'N':
            end.y = 0;
            break;
        case 'W':
            end.x = SCREEN_WIDTH;
            break;
        case 'S':
            end.y = SCREEN_HEIGHT;
            break;
        case 'E':
            end.x = 0;
            break;
    }
    tinygl_draw_line (start, end);
}

/** deals with displaying current instances */
void disp_update(void)
{
    //draw players
    if (self.counter > 0) {
        tinygl_pixel_set(tinygl_point (self.x, self.y), OUTPUT_HIGH);
        self.counter--;
    }
    if (enemy.counter > 0) {
        tinygl_pixel_set(tinygl_point (enemy.x, enemy.y), OUTPUT_HIGH);
        enemy.counter--;
    }

    //draw lasers
    for (int i=0; i < MAX_LASERS;i++) {
        if (laserPool[i].counter > 0) {
            disp_laser(laserPool[i]);
            laserPool[i].counter--;
        }
    }

    //refresh frame
    tinygl_update ();
}
