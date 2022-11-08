// Battleship.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include <stdlib.h>

const int gridx = 10;
const int gridy = 10;
const int boats = 5;
int boatLengths[] = { 2, 2, 3, 4, 5 };

typedef struct _Point
{
	int x;
	int y;
} Point;

typedef struct _Grid
{
	Point p;
	int type; // 1-5 = boat number, if 10's is set, it's been guessed, if 100's is set, its been sunk
	_Grid* pNext;
} Grid;

typedef struct _Boat
{
	Point pos;
	int len;
	bool isHorz;
} Boat;

typedef struct _BoatList
{
	Boat boat;
	_BoatList* pNext;
} BoatList;

BoatList* pMyBoats = NULL;
BoatList* pComputerBoats = NULL;

int GridVal(Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return 0;
	while (pGrid)
	{
		if (pGrid->p.x >= gridx)
		{
			return 1000; // out of bounds
		}
		if (pGrid->p.y >= gridy)
		{
			return 1000;
		}
		if (pGrid->p.x == x && pGrid->p.y == y)
		{
			return pGrid->type;
		}
		pGrid = pGrid->pNext;
	}
	return 0;
}

bool IsSpaceForBoat(Grid* pGrid, Boat* b)
{
	int sx = b->pos.x;
	int sy = b->pos.y;

	for (int i = 0; i < b->len; i++)
	{
		int x, y;
		if (b->isHorz)
		{
			x = sx + i;
			y = sy;
		}
		else
		{
			x = sx;
			y = sy + i;
		}
		if (GridVal(pGrid, x, y))
		{
			return false;
		}
	}
	return true;
}

void SetGrid(Grid** ppGrid, int x, int y, int val)
{
	Grid* pNewGrid = (Grid*)malloc(sizeof(Grid));
	pNewGrid->p.x = x;
	pNewGrid->p.y = y;
	pNewGrid->type = val;
	pNewGrid->pNext = *ppGrid;
	*ppGrid = pNewGrid;
}

void PlaceBoatInGrid(Grid** ppGrid, Boat* b)
{
	int sx = b->pos.x;
	int sy = b->pos.y;

	for (int i = 0; i < b->len; i++)
	{
		int x, y;
		if (b->isHorz)
		{
			x = sx + i;
			y = sy;
		}
		else
		{
			x = sx;
			y = sy + i;
		}
		SetGrid(ppGrid, x, y, b->len);
	}
}

void PrintGrid(Grid* pMyGrid, Grid * pOpponentGrid)
{
	const char* szSpacesBetweenBoards = "        ";

	printf("    ");
	for (int x = 0; x < gridx; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  ");
	
	printf(szSpacesBetweenBoards);

	printf("    ");
	for (int x = 0; x < gridx; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  ");

	printf("\n");

	printf("   +");
	for (int x = 0; x < gridx; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf(szSpacesBetweenBoards);

	printf("   +");
	for (int x = 0; x < gridx; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf("\n");

	for (int y = 0; y < gridy; y++)
	{
		printf("%c ||", 'A' + y);

		bool bOpponentGrid = false;

		for (int x = 0; x < gridx; x++)
		{
			const char* szDef = ". ";

			int t = GridVal(pMyGrid, x, y);
			int ship = t % 10; // ones column
			if (!t)
			{
				printf(szDef);
			}
			else
			{
				switch (t)
				{
				case 10:
					// a try, but a miss. No ship there.
					printf("! ");
					break;
				case 1:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("- ");
					break;
				case 11:
				case 111:
					printf("+ ");
					break;
				case 2:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("s ");
					break;
				case 12:
				case 112:
					printf("$ ");
					break;
				case 3:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("a ");
					break;
				case 13:
				case 113:
					printf("@ ");
					break;
				case 4:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("x ");
					break;
				case 14:
				case 114:
					printf("X ");
					break;
				case 5:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("o ");
					break;
				case 15:
				case 115:
					printf("d ");
					break;
				}
			}
		}
		printf("||");

		printf(szSpacesBetweenBoards);
		bOpponentGrid = true;

		printf("%c ||", 'A' + y);

		for (int x = 0; x < gridx; x++)
		{
			const char* szDef = ". ";

			int t = GridVal(pOpponentGrid, x, y);
			int ship = t % 10; // ones column
			if (!t)
			{
				printf(szDef);
			}
			else
			{
				switch (t)
				{
				case 10:
					// a try, but a miss. No ship there.
					printf("! ");
					break;
				case 1:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("- ");
					break;
				case 11:
				case 111:
					printf("+ ");
					break;
				case 2:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("s ");
					break;
				case 12:
				case 112:
					printf("$ ");
					break;
				case 3:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("a ");
					break;
				case 13:
				case 113:
					printf("@ ");
					break;
				case 4:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("x ");
					break;
				case 14:
				case 114:
					printf("X ");
					break;
				case 5:
					if (bOpponentGrid) { printf(szDef); break; }
					printf("o ");
					break;
				case 15:
				case 115:
					printf("d ");
					break;
				}
			}
		}
		printf("||");
		printf("\n");
	}

	printf("   +");
	for (int x = 0; x < gridx; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf(szSpacesBetweenBoards);

	printf("   +");
	for (int x = 0; x < gridx; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf("\n");

	printf("    ");
	for (int x = 0; x < gridx / 2 - 5; x++)
	{
		printf("  ");
	}
	printf("YOUR BOARD\n");
}

int main()
{
	srand(time(NULL));

	Grid* pMyGrid = NULL;
	Grid* pCompGrid = NULL;
	int totalHits = 0;
	int myHits = 0;

	for (int i = 0; i < boats; i++)
	{
		int len = boatLengths[i];
		totalHits += len;

	repeat:
		Boat b;
		b.len = len;
		b.isHorz = rand() % 2 ? true : false;
		int x, y;
		if (b.isHorz)
		{
			x = rand() % (gridx - (len-1));
			y = rand() % gridy;
		}
		else
		{
			x = rand() % gridx;
			y = rand() % (gridy - (len-1));
		}
		b.pos.x = x;
		b.pos.y = y;
		if (!IsSpaceForBoat(pMyGrid, &b))
		{
			goto repeat;
		}
		PlaceBoatInGrid(&pMyGrid, &b);
	}

	for (int i = 0; i < boats; i++)
	{
		int len = boatLengths[i];
	repeat2:
				
		Boat b;
		b.len = len;
		b.isHorz = rand() % 2 ? true : false;
		int x, y;
		if (b.isHorz)
		{
			x = rand() % (gridx - (len - 1));
			y = rand() % gridy;
		}
		else
		{
			x = rand() % gridx;
			y = rand() % (gridy - (len - 1));
		}
		b.pos.x = x;
		b.pos.y = y;
		if (!IsSpaceForBoat(pCompGrid, &b))
		{
			goto repeat2;
		}
		PlaceBoatInGrid(&pCompGrid, &b);
	}

	while (true)
	{
		PrintGrid(pMyGrid, pCompGrid);
		printf("\n");
		printf("move:");
		char movex;
		char movey;
		movex = getchar();
		movey = getchar();
		if (movex == 'q' || movey == 'q')
		{
			break;
		}
		int x = movex - 'a';
		int y = movey - 'a';
		printf("\n");
		if (x < 0 || x >= gridx || y < 0 || y >= gridy)
		{
			printf("Invalid move.\n");
			continue;
		}
		int v = GridVal(pCompGrid, x, y);
		if (v >= 10)
		{
			printf("You've already guessed that spot.\n");
			continue;
		}
		int boatNum = v % 10;
		if (boatNum == 0)
		{
			printf("Miss!\n");
			SetGrid(&pCompGrid, x, y, 10);
			continue;
		}

		printf("Hit!\n");
		SetGrid(&pCompGrid, x, y, v + 10);
		myHits++;

		// are all this battleship sunk?
		if (myHits == totalHits)
		{
			// you won.
			printf("You sunk all of his battleships! You won!\n");
			break;
		}

		continue;

	} // while

}

