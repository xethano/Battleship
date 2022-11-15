#include "Functions_Array.h"

#ifdef USE_ARRAY
Grid* CreateGrid()
{
	Grid* pGrid = (Grid*)malloc(sizeof(Grid));
	pGrid->m_pArray = (GridPoint*)malloc(gridx * gridy * sizeof(Grid));
	for (int i = 0; i < gridx * gridy; i++)
	{
		pGrid->m_pArray[i].boatIndex = NO_BOAT;
		pGrid->m_pArray[i].guessed = false;
	}
	return pGrid;
}

void DestroyGrid(Grid* pGrid)
{
	free(pGrid->m_pArray);
	free(pGrid);
}

GridPoint* GridAtXY(Grid * pGrid, int x, int y)
{
	return pGrid->m_pArray + y * gridx + x;
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

	GridPoint* pXY = GridAtXY(pGrid, x, y);
	*pBoatIndex = pXY->boatIndex;
	*pGuessed = pXY->guessed;
}

int GridBoat(Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return NO_BOAT;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy) return NO_BOAT;
	GridPoint* pXY = GridAtXY(pGrid, x, y);
	return pXY->boatIndex;
}

bool GridGuessed(Grid* pGrid, int x, int y)
{

	if (pGrid == NULL) return false;
	if (x < 0 || x >= gridx || y < 0 || y >= gridy) return true;
	GridPoint* pXY = GridAtXY(pGrid, x, y);
	return pXY->guessed;
}

void SetOrAddGrid(Grid** ppGrid, int x, int y, int boat, bool guessed)
{
	GridPoint* pGrid = GridAtXY(*ppGrid, x, y);
	pGrid->boatIndex = boat;
	pGrid->guessed = guessed;
}

void FindAnyNonSunkHitInGrid(Grid* pGrid, Boat* pBoats, int* pX, int* pY, int Direc[])
{
	*pX = -1;
	*pY = -1;

	for (int i = 0; i < 4; i++)
	{
		Direc[i] = 0;
	}

	for (int sx = 0; sx < gridx; sx++)
	{
		for (int sy = 0; sy < gridy; sy++)
		{
			GridPoint* pXY = GridAtXY(pGrid, sx, sy);
			if (pXY->guessed && (pXY->boatIndex > NO_BOAT) && !pBoats[pXY->boatIndex].sunk)
			{
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
					*pX = sx;
					*pY = sy;
					return;
				}
			} // if guessed and a boat, and isn't sunk yet
		}
	}
}
#endif
