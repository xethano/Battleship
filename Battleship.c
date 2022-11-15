#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "Functions_Array.h"


int main()
{
	srand((unsigned int)time(NULL));

	Grid* pMyGrid = CreateGrid();
	Grid* pCompGrid = CreateGrid();
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

		bool guessed;
		GridData(pCompGrid, x, y, &boatIndex, &guessed);
		if (guessed)
		{
			printf("You've already guessed that spot.\n");
			continue;
		}
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
				goto exitgame;
			}
		}

		printf("Computer guesses: %c, %c   (search len = %d)\n", 'A' + guessX, 'A' + guessY, searchLen);

		GridData(pMyGrid, guessX, guessY, &boatIndex, &guessed);

		if (guessed)
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
exitgame:
	DestroyGrid(pMyGrid);
	DestroyGrid(pCompGrid);
}


