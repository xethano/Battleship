#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "Game_Defaults.h"
#include "Functions_Array.h"

bool AnyUntriedPathsAroundUs(struct Grid* pGrid, int x, int y);
bool HasAnyInProximityBeenGuessed(struct Grid* pGrid, int x, int y, int len);
void FindUnguessedRegion(struct Grid* pGrid, int* px, int* py, int len);
bool IsSpaceForBoat(struct Grid* pGrid, struct BoatInfo* b);
int IsEntireBoatSunk(struct Grid* pGrid, struct BoatInfo* pBoats, int x, int y);
void PlaceBoatInGrid(struct Grid* pGrid, struct BoatInfo* b);
void PrintGrid(struct Grid* pMyGrid, struct Grid* pOpponentGrid);
