#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "Game_Defaults.h"

typedef struct _Grid
{
	int x;
	int y;
	// a guess at an xy location that has no boat there, will have boatIndex = NO_BOAT and guessed = true
	int boatIndex; // 0-based boat index
	bool guessed;
	struct _Grid* pNext;
} Grid;

Grid* CreateGrid();
void DestroyGrid(Grid* pGrid);
void GridData(Grid* pGrid, int x, int y, int* pBoatIndex, bool* pGuessed);
int GridBoat(Grid* pGrid, int x, int y);
bool GridGuessed(Grid* pGrid, int x, int y);
void SetOrAddGrid(Grid** ppGrid, int x, int y, int boat, bool guessed);
void FindAnyNonSunkHitInGrid(Grid* pGrid, Boat* pBoats, int* pX, int* pY, int Direc[]);

