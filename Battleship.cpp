// Battleship.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <conio.h>

const int gridx = 10;
const int gridy = 10;
const int boats = 5;
int boatLengths[] = { 2, 2, 3, 4, 5 };
const char* szDirections[] = { "N", "E", "S", "W" };
const int offsetX[] = { 0, 1, 0, -1 };
const int offsetY[] = { -1, 0, 1, 0 };

static const int NO_BOAT = -1;

typedef struct _Grid
{
	int x;
	int y;
	// a guess at an xy location that has no boat there, will have boatIndex = NO_BOAT and guessed = true
	int boatIndex; // 0-based boat index
	bool guessed;
	_Grid* pNext;
	static _Grid* Create( int _x, int _y, int _boatIndex)
	{
		struct _Grid* pGrid = (Grid*)malloc(sizeof(_Grid));
		pGrid->x = _x;
		pGrid->y = _y;
		pGrid->boatIndex = _boatIndex;
		pGrid->pNext = NULL;
		return pGrid;
	}
} Grid;

typedef struct _Boat
{
	int index; // which index in the array is it
	int x;
	int y;
	int len;
	bool isHorz;
	bool sunk;
	_Boat(int _index, int _x, int _y, int _len, bool _isHorz)
	{
		index = _index;
		x = _x;
		y = _y;
		len = _len;
		isHorz = _isHorz;
		sunk = false;
	}
	_Boat()
	{
		x = 0;
		y = 0;
		len = 0;
		isHorz = true;
		sunk = false;
	}
} Boat;

Boat pMyBoats[5];
Boat pComputerBoats[5];

int GridBoat(Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return NO_BOAT;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy) return 0;
	while (pGrid)
	{
		if (pGrid->x == x && pGrid->y == y)
		{
			return pGrid->boatIndex;
		}
		pGrid = pGrid->pNext;
	}
	return NO_BOAT;
}

bool GridGuessed(Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return false;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy) return true;
	while (pGrid)
	{
		if (pGrid->x == x && pGrid->y == y)
		{
			return pGrid->guessed;
		}
		pGrid = pGrid->pNext;
	}
	return false;
}

void SetOrAddGrid(Grid** ppGrid, int x, int y, int boat, bool guessed)
{
	Grid* pGrid = *ppGrid;
	while (pGrid)
	{
		if (pGrid->x == x && pGrid->y == y)
		{
			pGrid->boatIndex = boat;
			pGrid->guessed = guessed;
			return;
		}
		pGrid = pGrid->pNext;
	}

	Grid* pNewGrid = _Grid::Create(x, y, boat);
	pNewGrid->guessed = guessed;
	pNewGrid->pNext = *ppGrid;
	*ppGrid = pNewGrid;
}

bool AnyUntriedPathsAroundUs(Grid* pGrid, int x, int y)
{
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		b |= !GridGuessed(pGrid, x + offsetX[i], y + offsetY[i]);
	}
	return b;
}

bool HasAnyInProximityBeenGuessed(Grid* pGrid, int x, int y, int len)
{
	x -= len / 2;
	y -= len / 2;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			int sx = x + i;
			int sy = y + j;
			if (sx < 0 || sy < 0 || sx >= gridx || sy >= gridy)
			{
				continue;
			}
			bool bGuessed = GridGuessed(pGrid, sx, sy);
			if (bGuessed)
			{
				return true;
			}
		}
	}
	return false;
}

void FindUnguessedRegion(Grid* pGrid, int* px, int* py, int len)
{
	for (int i = 0; i < 10000; i++)
	{
		int rx = rand() % gridx;
		int ry = rand() % gridy;
		bool bIsPop = HasAnyInProximityBeenGuessed(pGrid, rx, ry, len);
		if (bIsPop)
		{
			continue;
		}
		*px = rx;
		*py = ry;
		return;
	}
	*px = -1;
	*py = -1;
}

bool IsSpaceForBoat(Grid* pGrid, Boat* b)
{
	int sx = b->x;
	int sy = b->y;

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
		if (x >= gridx || y >= gridy)
		{
			return false;
		}
		if (GridBoat(pGrid, x, y) > NO_BOAT)
		{
			return false;
		}
	}
	return true;
}

int IsEntireBoatSunk(Grid* pGrid, Boat* pBoats, int x, int y)
{
	int boat = GridBoat(pGrid, x, y);
	if (boat == NO_BOAT) return NO_BOAT;

	int bx = pBoats[boat].x;
	int by = pBoats[boat].y;
	bool bHorz = pBoats[boat].isHorz;
	int len = pBoats[boat].len;
	int ix = 0;
	int iy = 0;
	if (bHorz)
		ix = 1;
	else
		iy = 1;
	bool bSunk = true;
	for (int l = 0; l < len; l++)
	{
		bool bGuessed = GridGuessed(pGrid, bx, by);
		if (!bGuessed)
		{
			bSunk = false;
			break;
		}
		bx += ix;
		by += iy;
	}
	if (!bSunk) return NO_BOAT;
	return boat;
}

void PlaceBoatInGrid(Grid** ppGrid, Boat* b)
{
	int sx = b->x;
	int sy = b->y;

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
		SetOrAddGrid(ppGrid, x, y, b->index, false);
	}
}

void FindAnyNonSunkHitInGrid(Grid* pGrid, Boat* pBoats, int* pX, int* pY, int Direc[] )
{
	*pX = -1;
	*pY = -1;
	Grid* p = pGrid;

	for (int i = 0; i < 4; i++)
	{
		Direc[i] = 0;
	}

	while (p)
	{
		if (p->guessed && ( p->boatIndex > NO_BOAT ) && !pBoats[p->boatIndex].sunk )
		{
			int sx = p->x;
			int sy = p->y;

			for (int i = 0; i < 4; i++)
			{
				Direc[i] = 0;
			}

			bool bFound = false;

			for (int i = 0; i < 4; i++)
			{
				// go in this direction unless:
				// hit non-guessed
				// hit border
				// hit guessed but not ship

				Direc[i] = 0;
				int j = 1;
				for (; ; j++)
				{
					int x = sx + offsetX[i] * j;
					int y = sy + offsetY[i] * j;
					if (x < 0 || y < 0 || x >= gridx || y >= gridy)
					{
						// don't go this direction
						j = 0;
						break;
					}

					if (!GridGuessed(pGrid, x, y))
					{
						// if j == 1, then we are at a non-guess spot. 
						// if j > 1 then we stepped over some 
						// guessed-and-hit spots and got here. 
						// still need guess this last spot
						break;
					}

					// we already guessed this spot before.
					// if it's boat, keep going

					if (GridBoat(pGrid, x, y) == NO_BOAT)
					{
						j = 0;
						break;
					}
				} // for j

				if (j)
				{
					Direc[i] = j;
					bFound = true;
				}
			}

			if (bFound)
			{
				*pX = p->x;
				*pY = p->y;
				return;
			}
		} // if guessed and a boat, and isn't sunk yet
		p = p->pNext;
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

			int boat = GridBoat(pMyGrid, x, y);
			bool bGuessed = GridGuessed(pMyGrid, x, y);
			bool bSunk = IsEntireBoatSunk(pMyGrid, pMyBoats, x, y) != NO_BOAT;

			switch (boat)
			{
			case NO_BOAT:
				if (bGuessed)
				{
					printf("* ");
				}
				else
				{
					printf(szDef);
				}
				break;
			case 0:
				if (!bGuessed)
				{
					printf("- ");
				}
				else if (bSunk)
				{
					printf("++");
				}
				else
				{
					printf("+ ");
				}
				break;
			case 1:
				if (!bGuessed)
				{
					printf("s ");
				}
				else if (bSunk)
				{
					printf("$$");
				}
				else
				{
					printf("$ ");
				}
				break;
			case 2:
				if (!bGuessed)
				{
					printf("a ");
				}
				else if (bSunk)
				{
					printf("@@");
				}
				else
				{
					printf("@ ");
				}
				break;
			case 3:
				if (!bGuessed)
				{
					printf("x ");
				}
				else if (bSunk)
				{
					printf("XX");
				}
				else
				{
					printf("X ");
				}
				break;
			case 4:
				if (!bGuessed)
				{
					printf("= ");
				}
				else if (bSunk)
				{
					printf("##");
				}
				else
				{
					printf("# ");
				}
				break;
			}
		} // for x

		printf("||");

		printf(szSpacesBetweenBoards);
		// bOpponentGrid = true;

		printf("%c ||", 'A' + y);

		for (int x = 0; x < gridx; x++)
		{
			const char* szDef = ". ";
			const char* szHit = "! ";

			int boat = GridBoat(pOpponentGrid, x, y);
			bool bGuessed = GridGuessed(pOpponentGrid, x, y);
			bool bSunk = IsEntireBoatSunk(pOpponentGrid, pComputerBoats, x, y) != NO_BOAT;

			// if we don't want to reveal the computer's boats, then if we havent' guessed
			// the spot yet, then don't show what boat type it is, pretend it's "no boat".
			// but if we have guessed the spot, then show the real info.
			bool bShowComputersBoats = true;
			int possiblyBoat = boat;
			if (!bShowComputersBoats)
			{
				if (!bGuessed)
				{
					possiblyBoat = NO_BOAT;
				}
			}

			switch (possiblyBoat)
			{
			case NO_BOAT:
				if (bGuessed)
				{
					printf("* ");
				}
				else
				{
					printf(szDef);
				}
				break;
			case 0:
				if (!bGuessed)
				{
					printf("- ");
				}
				else if (bSunk)
				{
					printf("++");
				}
				else
				{
					printf("+ ");
				}
				break;
			case 1:
				if (!bGuessed)
				{
					printf("s ");
				}
				else if (bSunk)
				{
					printf("$$");
				}
				else
				{
					printf("$ ");
				}
				break;
			case 2:
				if (!bGuessed)
				{
					printf("a ");
				}
				else if (bSunk)
				{
					printf("@@");
				}
				else
				{
					printf("@ ");
				}
				break;
			case 3:
				if (!bGuessed)
				{
					printf("x ");
				}
				else if (bSunk)
				{
					printf("XX");
				}
				else
				{
					printf("X ");
				}
				break;
			case 4:
				if (!bGuessed)
				{
					printf("= ");
				}
				else if (bSunk)
				{
					printf("##");
				}
				else
				{
					printf("# ");
				}
				break;
			}
		} // for x

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
	int boatIndex;
	int guessX;
	int guessY;

	// computer's variables
	int searchLen = 5;

	// computer finding a run along a ship's length
	int KnownHitX = -1;
	int KnownHitY = -1;
	int guessDir = 0;
	int workingDir = -1;
	int guessLen = 0;
	bool guessDirFlipped = false;
	bool lastComputerMoveWasHit = false;

	//==================================================
	//
	//       SETUP YOUR STUFF
	//
	//==================================================

	for (int i = 0; i < boats; i++)
	{
		int len = boatLengths[i];

	repeat:
		Boat bt;
		bt.index = i;
		bt.sunk = false;
		bt.len = len;
		bt.isHorz = rand() % 2 ? true : false;
		int x, y;
		if (bt.isHorz)
		{
			x = rand() % (gridx - (len-1));
			y = rand() % gridy;
		}
		else
		{
			x = rand() % gridx;
			y = rand() % (gridy - (len-1));
		}
		bt.x = x;
		bt.y = y;
		if (!IsSpaceForBoat(pMyGrid, &bt))
		{
			goto repeat;
		}
		PlaceBoatInGrid(&pMyGrid, &bt);
		pMyBoats[i] = bt;
	}

	//==================================================
	//
	//       SETUP COMPUTER STUFF
	//
	//==================================================

	for (int i = 0; i < boats; i++)
	{
		int len = boatLengths[i];

	repeat2:
		Boat bt;
		bt.index = i;
		bt.sunk = false;
		bt.len = len;
		bt.isHorz = rand() % 2 ? true : false;
		int x, y;
		if (bt.isHorz)
		{
			x = rand() % (gridx - (len - 1));
			y = rand() % gridy;
		}
		else
		{
			x = rand() % gridx;
			y = rand() % (gridy - (len - 1));
		}
		bt.x = x;
		bt.y = y;
		if (!IsSpaceForBoat(pCompGrid, &bt))
		{
			goto repeat2;
		}
		PlaceBoatInGrid(&pCompGrid, &bt);
		pComputerBoats[i] = bt;
	}

	//==================================================
	//
	//       MAIN LOOP
	//
	//==================================================

	while (true)
	{
		printf("|-------------------------- the board --------------------\n");
		printf("\n");
		PrintGrid(pMyGrid, pCompGrid);

		//-------------------
		// your move
		//-------------------

		printf("\n");
		printf("|-------------------------- your move --------------------\n");
		printf("move:");
		char movex;
		char movey;
		movex = _getch();
		printf("%c", movex);
		movey = _getch();
		printf("%c", movey);
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
		if (GridGuessed(pCompGrid, x, y))
		{
			printf("You've already guessed that spot.\n");
			continue;
		}
		boatIndex = GridBoat(pCompGrid, x, y);
		if (boatIndex == NO_BOAT)
		{
			printf("Miss!\n");
			SetOrAddGrid(&pCompGrid, x, y, NO_BOAT, true);
		}
		else
		{
			printf("Hit!\n");
			SetOrAddGrid(&pCompGrid, x, y, boatIndex, true);

			// see if the entire boat is hit
			int nWhichBoatSunkIndex = IsEntireBoatSunk(pCompGrid, pComputerBoats, x, y);
			if (nWhichBoatSunkIndex > NO_BOAT)
			{
				printf("You sunk his battleship! You sunk boat # %d\n", nWhichBoatSunkIndex);
				pComputerBoats[nWhichBoatSunkIndex].sunk = true;
				bool bAllSunk = true;
				for (int i = 0; i < 5; i++)
				{
					if (!pComputerBoats[i].sunk)
					{
						bAllSunk = false;
						break;
					}
				}
				if (bAllSunk)
				{
					printf("You sunk ALL his battleships! You win!\n");
					break;
				}
			}
		}


		//-------------------
		// computer's move
		//-------------------

		printf("|-------------------------- comp move --------------------\n");

		int hx, hy;
		int Direc[4];
		FindAnyNonSunkHitInGrid(pMyGrid, pMyBoats, &hx, &hy, Direc);
		if (hx != -1)
		{
			// what's the max len of Direc?
			int maxlen = 0;
			int maxdir = -1;
			for (int i = 0; i < 4; i++)
			{
				if (Direc[i] > maxlen)
				{
					maxlen = Direc[i];
					maxdir = i;
				}
			}
		retry1:
			int pickeddir = rand() % 4;
			if (Direc[pickeddir] != maxlen)
				goto retry1;
			guessX = hx + offsetX[pickeddir] * maxlen;
			guessY = hy + offsetY[pickeddir] * maxlen;
		}
		else
		{
			// no idea where to look, try a random spot with given radius of free space
			while (searchLen > 0)
			{
				FindUnguessedRegion(pMyGrid, &guessX, &guessY, searchLen);
				if (guessX == -1)
				{
					searchLen--;
					printf("Debug: computer couldn't find an unguessed region, search len is now %d\n", searchLen);
					continue;
				}

				boatIndex = GridBoat(pMyGrid, guessX, guessY);
				if (boatIndex > NO_BOAT)
				{
					// found one of my boats
					KnownHitX = guessX;
					KnownHitY = guessY;
					workingDir = -1;
					guessDir = rand() % 4;
					printf("Debug: Computer got a hit, and is going to start guessing in the direction: %s\n", szDirections[guessDir]);
					guessLen = 1;
				}
				break;
			}
			if (searchLen == 0)
			{
				printf("Computer couldn't find a single spot to move. we programmed this wrong.\n");
				exit(0);
			}
		}

		printf("Computer guesses: %c, %c   (search len = %d)\n", 'A' + guessX, 'A' + guessY, searchLen);

		boatIndex = GridBoat(pMyGrid, guessX, guessY);

		bool bAlreadyGuessed = GridGuessed(pMyGrid, guessX, guessY);
		if (bAlreadyGuessed)
		{
			printf("why did the computer guess the same spot twice?\n");
			exit(0);
		}

		SetOrAddGrid(&pMyGrid, guessX, guessY, boatIndex, true);


		if (boatIndex == NO_BOAT)
		{
			printf("    .... it's a miss!\n");
			lastComputerMoveWasHit = false;
		}
		else
		{
			printf("    .... it's a hit!\n");
			lastComputerMoveWasHit = true;
		}

		int boatsunk = IsEntireBoatSunk(pMyGrid, pMyBoats, guessX, guessY);
		if (boatsunk > NO_BOAT)
		{
			KnownHitX = -1;
			KnownHitY = -1;
			workingDir = -1;
			guessDir = 0;
			guessLen = 0;
			guessDirFlipped = false;

			printf("That dick sunk your battleship! Ship = # %d\n", boatsunk);
			pMyBoats[boatsunk].sunk = true;
			bool allSunk = true;
			for (int i = 0; i < 5; i++)
			{
				if (!pMyBoats[i].sunk)
				{
					allSunk = false;
					break;
				}
			}
			if (allSunk)
			{
				printf("You, unfortunately, lose.\n");
				break;
			}
		}
	} // while

	// end of game
}


