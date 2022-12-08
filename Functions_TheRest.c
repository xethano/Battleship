#include "Functions_TheRest.h"

bool AnyUntriedPathsAroundUs(struct Grid* pGrid, int x, int y)
{
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		b |= !IsGridGuessedAtXY(pGrid, x + StepOffsetX[i], y + StepOffsetY[i]);
	}
	return b;
}

bool HasAnyInProximityBeenGuessed(struct Grid* pGrid, int x, int y, int len)
{
	x -= len / 2;
	y -= len / 2;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			int sx = x + i;
			int sy = y + j;
			if (sx < 0 || sy < 0 || sx >= GridWidth || sy >= GridHeight)
			{
				continue;
			}
			bool bGuessed = IsGridGuessedAtXY(pGrid, sx, sy);
			if (bGuessed)
			{
				return true;
			}
		}
	}
	return false;
}

void FindUnguessedRegion(struct Grid* pGrid, int* px, int* py, int len)
{
	for (int i = 0; i < 10000; i++)
	{
		int rx = rand() % GridWidth;
		int ry = rand() % GridHeight;
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

bool IsSpaceForBoat(struct Grid* pGrid, struct BoatInfo* b)
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
		if (x >= GridWidth || y >= GridHeight)
		{
			return false;
		}
		if (GetGridBoatAtXY(pGrid, x, y) > NO_BOAT)
		{
			return false;
		}
	}
	return true;
}

int IsEntireBoatSunk(struct Grid* pGrid, struct BoatInfo* pBoats, int x, int y)
{
	int boat = GetGridBoatAtXY(pGrid, x, y);
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
		bool bGuessed = IsGridGuessedAtXY(pGrid, bx, by);
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

void PlaceBoatInGrid(struct Grid* pGrid, struct BoatInfo* b)
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
		SetGridDataAtXY(pGrid, x, y, b->index, false);
	}
}


void PrintGrid(struct Grid* pMyGrid, struct Grid* pOpponentGrid)
{
	const char* szSpacesBetweenBoards = "        ";

	printf("    ");
	for (int x = 0; x < GridWidth; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  ");

	printf(szSpacesBetweenBoards);

	printf("    ");
	for (int x = 0; x < GridWidth; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("  ");

	printf("\n");

	printf("   +");
	for (int x = 0; x < GridWidth; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf(szSpacesBetweenBoards);

	printf("   +");
	for (int x = 0; x < GridWidth; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf("\n");

	for (int y = 0; y < GridHeight; y++)
	{
		printf("%c ||", 'A' + y);

		bool bOpponentGrid = false;

		for (int x = 0; x < GridWidth; x++)
		{
			const char* szDef = ". ";

			int boat;
			bool bGuessed;
			GetGridDataAtXY(pMyGrid, x, y, &boat, &bGuessed);
			bool bSunk = IsEntireBoatSunk(pMyGrid, MyBoats, x, y) != NO_BOAT;

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

		for (int x = 0; x < GridWidth; x++)
		{
			const char* szDef = ". ";
			const char* szHit = "! ";

			int boat;
			bool bGuessed;
			GetGridDataAtXY(pOpponentGrid, x, y, &boat, &bGuessed);
			bool bSunk = IsEntireBoatSunk(pOpponentGrid, ComputersBoats, x, y) != NO_BOAT;

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
	for (int x = 0; x < GridWidth; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf(szSpacesBetweenBoards);

	printf("   +");
	for (int x = 0; x < GridWidth; x++)
	{
		printf("==");
	}
	printf("+ ");

	printf("\n");

	printf("    ");
	for (int x = 0; x < GridWidth / 2 - 5; x++)
	{
		printf("  ");
	}
	printf("YOUR BOARD\n");
}
