#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "Game_Defaults.h"

#ifdef USE_ARRAY
#include "Functions_Array.h"
#else
#include "Functions_LinkedList.h"
#endif

bool AnyUntriedPathsAroundUs(Grid* pGrid, int x, int y);
bool HasAnyInProximityBeenGuessed(Grid* pGrid, int x, int y, int len);
void FindUnguessedRegion(Grid* pGrid, int* px, int* py, int len);
bool IsSpaceForBoat(Grid* pGrid, Boat* b);
int IsEntireBoatSunk(Grid* pGrid, Boat* pBoats, int x, int y);
void PlaceBoatInGrid(Grid** ppGrid, Boat* b);
void PrintGrid(Grid* pMyGrid, Grid* pOpponentGrid);
