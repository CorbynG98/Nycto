#include "tinygl.h"
#include "disp.h"

#define DISPLAY_RATE 500 //should be the same as PACER_RATE
#define DISPLAY_TIME 1500 //time to display a player for
#define FADER_RESOLUTION 3
#define DISPLAY_LASER_TIME 50 // time to display a laser for
#define OUTPUT_HIGH 1
#define MAX_LASERS 10

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
            endx = TINYGL_WIDTH;
            break;
        case 'S':
            endy = TINYGL_HEIGHT;
            break;
        case 'W':
            endx = 0;
            break;
    }
    tinygl_draw_line (tinygl_point (laser.x, laser.y),
                      tinygl_point (endx, endy),
                      OUTPUT_HIGH);
}

/** Returns whether an LED should be turned on to establish a fading
 * effect as counter ramps down from max */
bool fader(int counter, int max) {
    //unless pacer rate is higher, dimming becomes perceptible flickering above res=3
    bool answer = true;
    for (int i=1; i<FADER_RESOLUTION; i++) {
        if (counter < (i*max)/FADER_RESOLUTION) {
            answer = !(counter % (FADER_RESOLUTION + 1 - i));
            break;
        }
    }
    return answer;
}

/** deals with displaying current instances */
void disp_update(void)
{
    display_clear ();

    //draw players
    if (self.counter > 0) {
        if (fader(self.counter, DISPLAY_TIME)) {
            tinygl_pixel_set(tinygl_point (self.x, self.y), OUTPUT_HIGH);
        }
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
