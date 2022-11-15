#ifndef __GAME_DEFAULTS_H__
#define __GAME_DEFAULTS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

static const int gridx = 10;
static const int gridy = 10;
static const int boats = 5;
static int boatLengths[] = { 2, 2, 3, 4, 5 };
static const char* szDirections[] = { "N", "E", "S", "W" };
static const int offsetX[] = { 0, 1, 0, -1 };
static const int offsetY[] = { -1, 0, 1, 0 };

#define NO_BOAT -1
#define bool int
#define true 1
#define false 0
#define USE_ARRAY

typedef struct _Boat
{
	int index; // which index in the array is it
	int x;
	int y;
	int len;
	int isHorz;
	int sunk;
} Boat;

Boat pMyBoats[5];
Boat pComputerBoats[5];

#endif
