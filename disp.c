#include "tinygl.h"
#include "disp.h"

#define DISPLAY_RATE 500 //should be the same as PACER_RATE
#define DISPLAY_TIME 50 //time to display a player for
#define DISPLAY_LASER_TIME 50 // time to display a laser for
#define OUTPUT_HIGH 1
#define MAX_LASERS 10
#define SCREEN_WIDTH TINYGL_WIDTH
#define SCREEN_HEIGHT TINYGL_HEIGHT

Laser laserPool[MAX_LASERS];
Player self = {0, 0, 0};
Player enemy = {0, 0, 0};

static char dispDebugChar = 'D';

char getDebugChar(void) {
    return dispDebugChar;
}

/** sets screen off */
void disp_init(void)
{
    tinygl_init (DISPLAY_RATE);
    //self.counter = DISPLAY_TIME;
    //enemy.counter = DISPLAY_TIME;
}

/** display self for a time */
void disp_add_self(int position[2])
{
    self.counter = DISPLAY_TIME;
    self.x = position[0];
    self.y = position[1];
}

/** display the enemy for a time */
void disp_add_enemy(int position[2])
{
    enemy.counter = DISPLAY_TIME;
    enemy.x = position[0];
    enemy.y = position[1];
}

/** creates new laser */
void disp_add_laser(int laser[3])
{
    dispDebugChar = 'Z';
    for (int i=0; i < MAX_LASERS;i++) {
        //if laser is no longer displayed, add new laser details
        if (laserPool[i].counter == 0) {
            laserPool[i].counter = DISPLAY_LASER_TIME;
            laserPool[i].x = laser[0];
            laserPool[i].y = laser[1];
            laserPool[i].direction = laser[2];
        }
    }
}

/** Displays a laser for a frame*/
void disp_laser(Laser laser)
{
    int endx = laser.x;
    int endy = laser.y;
    switch (laser.direction) {
        case 'N':
            endy = 0;
            break;
        case 'E':
            endx = SCREEN_WIDTH;
            break;
        case 'S':
            endy = SCREEN_HEIGHT;
            break;
        case 'W':
            endx = 0;
            break;
    }
    tinygl_draw_line (tinygl_point (laser.x, laser.y),
                      tinygl_point (endx, endy),
                      OUTPUT_HIGH);
}

/** deals with displaying current instances */
void disp_update(void)
{
    display_clear ();

    //draw players
    if (self.counter > 0) {
        tinygl_pixel_set(tinygl_point (self.x, self.y), OUTPUT_HIGH);
        dispDebugChar = self.counter + 'C';
        self.counter--;
    }
    if (enemy.counter > 0) {
        tinygl_pixel_set(tinygl_point (enemy.x, enemy.y), OUTPUT_HIGH);
        enemy.counter--;
    }

    //draw lasers
    for (int i=0; i < MAX_LASERS;i++) {
        if (laserPool[i].counter > 0) {
            dispDebugChar = 'K';
            disp_laser(laserPool[i]);
            laserPool[i].counter--;
        }
    }

    //refresh frame

    tinygl_update ();
}
