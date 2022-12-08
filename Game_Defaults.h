#ifndef __GAME_DEFAULTS_H__
#define __GAME_DEFAULTS_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

static const int GridWidth = 10;
static const int GridHeight = 10;
static const int BoatCount = 5;
static int BoatLengthArray[] = { 2, 2, 3, 4, 5 };
// this is just to tell the user what direction it's looking in, a description string!
static const char* DirectionString[] = { "N", "E", "S", "W" };
// computer can look in 1 of 4 directions. These offsets define how much it steps by on the grid,
// for any one of the directions, in x, and in y. for example, if it's direction 0, x steps by 0, y steps by -1.
// (when looking for another hit)
// by the way, axises are x = 1 to the right, and y = 1 to the down (south)
static const int StepOffsetX[] = { 0, 1, 0, -1 }; // direction order is N,E,S,W
static const int StepOffsetY[] = { -1, 0, 1, 0 };

#define NO_BOAT -1

struct BoatInfo
{
	int index; // which index in the array is it
	int x;
	int y;
	int len;
	int isHorz;
	int sunk;
};

struct BoatInfo MyBoats[5];
struct BoatInfo ComputersBoats[5];

#endif
