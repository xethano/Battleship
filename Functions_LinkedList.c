#include "Functions_LinkedList.h"

#ifndef USE_ARRAY
Grid* CreateGrid()
{
	return NULL;
}

void DestroyGrid(Grid* pGrid)
{
	while (pGrid)
	{
		Grid* pNext = pGrid->pNext;
		free(pGrid);
		pGrid = pNext;
	}
}

void GridData(Grid* pGrid, int x, int y, int* pBoatIndex, bool* pGuessed)
{
	*pBoatIndex = NO_BOAT;
	*pGuessed = false;

	if (pGrid == NULL) return;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy)
	{
		*pGuessed = true;
		return;
	}

	while (pGrid)
	{
		if (pGrid->x == x && pGrid->y == y)
		{
			*pBoatIndex = pGrid->boatIndex;
			*pGuessed = pGrid->guessed;
			return;
		}
		pGrid = pGrid->pNext;
	}
	return;
}

int GridBoat(Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return NO_BOAT;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy) return NO_BOAT;
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

	Grid* pNewGrid = (Grid*)malloc(sizeof(Grid));
	pNewGrid->x = x;
	pNewGrid->y = y;
	pNewGrid->boatIndex = boat;
	pNewGrid->guessed = guessed;
	pNewGrid->pNext = *ppGrid;
	*ppGrid = pNewGrid;
}

void FindAnyNonSunkHitInGrid(Grid* pGrid, Boat* pBoats, int* pX, int* pY, int Direc[])
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
		if (p->guessed && (p->boatIndex > NO_BOAT) && !pBoats[p->boatIndex].sunk)
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

					int boatIndex;
					bool guessed;
					GridData(pGrid, x, y, &boatIndex, &guessed);

					if (!guessed)
					{
						// if j == 1, then we are at a non-guess spot. 
						// if j > 1 then we stepped over some 
						// guessed-and-hit spots and got here. 
						// still need guess this last spot
						break;
					}

					// we already guessed this spot before.
					// if it's boat, keep going

					if (boatIndex == NO_BOAT)
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
#endif
