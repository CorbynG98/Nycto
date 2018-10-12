#include "tinygl.h"
#include "disp.h"

#define DISP_RATE 500 //should be the same as PACER_RATE
#define DISP_TIME 60 //time to display a player for
#define DISP_LASER_TIME 40 // time to display a laser for
#define OUTPUT_HIGH 1

Laser selfLaser = {0, 0, 0, 'N'};
Laser enemyLaser = {0, 0, 0, 'N'};
Player self = {0, 0, 0};
Player enemy = {0, 0, 0};

static char dispDebugChar = 'D';

char getDebugChar(void) {
    return dispDebugChar;
}

/** sets screen off */
void disp_init(void)
{
    tinygl_init (DISP_RATE);
    //self.counter = DISPLAY_TIME;
    //enemy.counter = DISPLAY_TIME;
}

/** display self for a time */
void disp_add_self(int position[2])
{
    self.counter = DISP_TIME;
    self.x = position[0];
    self.y = position[1];
}

/** display the enemy for a time */
void disp_add_enemy(int position[2])
{
    enemy.counter = DISP_TIME;
    enemy.x = position[0];
    enemy.y = position[1];
}

/** creates self laser */
void disp_add_self_laser(int laser[3])
{
    //generalize these
    selfLaser.counter = DISP_LASER_TIME;
    selfLaser.x = laser[0];
    selfLaser.y = laser[1];
    selfLaser.direction = laser[2];
    //also add self player
    int self[2] = {laser[0], laser[1]};
    disp_add_self(self);
}

/** creates enemy laser */
void disp_add_enemy_laser(int laser[3])
{
    enemyLaser.counter = DISP_LASER_TIME;
    enemyLaser.x = laser[0];
    enemyLaser.y = laser[1];
    enemyLaser.direction = laser[2];
    //also add enemy player
    int enemy[2] = {laser[0], laser[1]};
    disp_add_enemy(enemy);
}

/** Displays a laser for a frame*/
void disp_laser(Laser laser)
{
    int endx = laser.x;
    int endy = laser.y;
    int startx = laser.x;
    int starty = laser.y;
    switch (laser.direction) {
        case 'N':
            starty--;
            endy = 0;
            break;
        case 'E':
            startx++;
            endx = TINYGL_WIDTH;
            break;
        case 'S':
            starty++;
            endy = TINYGL_HEIGHT;
            break;
        case 'W':
            startx--;
            endx = 0;
            break;
    }
    tinygl_draw_line (tinygl_point (startx, starty),
                      tinygl_point (endx, endy),
                      OUTPUT_HIGH);
}

/** Returns whether an LED should be turned on to establish a fading
 * effect as counter ramps down from max. Causes lag and is basically
 * useless */
 /*
bool fader(int counter, int max) {
    if (counter < max/2) {
        return counter % 2;
    } else {
        return true;
    }
}
* */

/** deals with displaying current instances */
void disp_update(void)
{
    display_clear ();

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
    if (selfLaser.counter > 0) {
        disp_laser(selfLaser);
        selfLaser.counter--;
    }
    if (enemyLaser.counter > 0) {
        disp_laser(enemyLaser);
        enemyLaser.counter--;
    }

    //refresh frame
    tinygl_update ();
}
