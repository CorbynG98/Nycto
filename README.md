# Space Gladiator

This is an arena shooter game, with level selection. The goal of this game is to find and shoot the other player with your laser. The catch however, is that you can only see the other player when they shoot or hit a wall and the same goes for yourself, you only see your own position when you hit a wall or shoot your laser.

### Getting started

make sure to have cloned the ENCE260 funkit repository. This repo has all the required drivers, fonts, utils, etc. required to run this game on the Electrical Computer Engineering UC Fun Kit 4.

### Installing

Clone the repository for this game into a folder in the assignment folder. located in the previously cloned directory.
Once the game files have been cloned into the correct directory open a console and traverse to this directory.
run the command make program with the board plugged into the computer (reset the board if there are problems)

### How to play

- Navswitch left, right, forward and back to control player position
- Press navswitch in to shoot the laser

On the first screen seen, one of the two boards must press the nav switch button in, this will set them as player 1 and tell the other board that they are now player 2.
Player 1 can now select a level moving the navswitch left and/or right.
Once the player has selected a suitable level, player 1 must press the nav switch button in again, this sets the required level and tells the other board what level is selected so they both have the same setup.

When the game first starts it may look like nothing has happened, however this is not the case as both players are seen untill either a wall is hit or a laser is shot.
The goal is to try locate the other player and shoot them before they do the same to you.
When the game is over the game will freeze for about a second showing both player position on the display before, on the winning player showing a W and flashing a blue LED. And on the loosing player shows an L on the LED Matrix.

## Authors

* **Corbyn Greenwood** - *main menu, transmitting and receiving, nav switch*
* **Benjamin Scott** - *main game loop, display module*
* **Joint Effort** - *bug fixing, make file, headers, readme*

## Acknowledgments

* **Eletrical Engineering / Computer Engineering / Computer Science** - *ENCE260 Fun Kit repository, Fun Kit board*
* **Derrick Edward** - *Name of the game*
