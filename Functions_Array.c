#include "Functions_Array.h"

struct Grid* CreateGrid()
{
	struct Grid* pGrid = (struct Grid*) malloc(sizeof(struct Grid));
	if (pGrid == NULL) return NULL;
	int sizeOfGridData = sizeof(struct GridData);
	pGrid->m_pArray = (struct GridData*) malloc(GridWidth * GridHeight * sizeOfGridData);
	for (int i = 0; i < GridWidth * GridHeight; i++)
	{
		pGrid->m_pArray[i].boatIndex = NO_BOAT;
		pGrid->m_pArray[i].guessed = false;
	}
	return pGrid;
}

void DestroyGrid(struct Grid* pGrid)
{
	free(pGrid->m_pArray);
	free(pGrid);
}

struct GridData* GridDataAtXY(struct Grid * pGrid, int x, int y)
{
	// we return the address of the griddata, not the struct itself.
	return &pGrid[y * GridWidth + x];
}

void GetGridDataAtXY(struct Grid* pGrid, int x, int y, int* pBoatIndex, bool* pGuessed)
{
	*pBoatIndex = NO_BOAT;
	*pGuessed = false;

	if (pGrid == NULL) return;
	if (x < 0 || x >= GridWidth || y < 0 || y >= GridHeight)
	{
		*pGuessed = true;
		return;
	}

	struct GridData* pXY = GridDataAtXY(pGrid, x, y);
	*pBoatIndex = pXY->boatIndex;
	*pGuessed = pXY->guessed;
}

int GetGridBoatAtXY(struct Grid* pGrid, int x, int y)
{
	if (pGrid == NULL) return NO_BOAT;
	if (x < 0 || x >= GridWidth || y < 0 || y >= GridHeight) return NO_BOAT;
	struct GridData* pXY = GridDataAtXY(pGrid, x, y);
	return pXY->boatIndex;
}

bool IsGridGuessedAtXY(struct Grid* pGrid, int x, int y)
{

	if (pGrid == NULL) return false;
	if (x < 0 || x >= GridWidth || y < 0 || y >= GridHeight) return true;
	struct GridData* pXY = GridDataAtXY(pGrid, x, y);
	return pXY->guessed;
}

void SetGridDataAtXY(struct Grid* pGrid, int x, int y, int boat, bool guessed)
{
	struct GridData* pGridData = GridDataAtXY(pGrid, x, y);
	pGridData->boatIndex = boat;
	pGridData->guessed = guessed;
}

void FindAnyNonSunkHitInGrid(struct Grid* pGrid, struct BoatInfo* pBoats, int* pX, int* pY, int Direc[])
{
	*pX = -1;
	*pY = -1;

	for (int i = 0; i < 4; i++)
	{
		Direc[i] = 0;
	}

	for (int sx = 0; sx < GridWidth; sx++)
	{
		for (int sy = 0; sy < GridHeight; sy++)
		{
			struct GridData* pXY = GridDataAtXY(pGrid, sx, sy);
			if (pXY->guessed && (pXY->boatIndex > NO_BOAT) && !MyBoats[pXY->boatIndex].sunk)
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
						int x = sx + StepOffsetX[i] * j;
						int y = sy + StepOffsetY[i] * j;
						if (x < 0 || y < 0 || x >= GridWidth || y >= GridHeight)
						{
							// don't go this direction
							j = 0;
							break;
						}

						int boatIndex;
						bool guessed;
						GetGridDataAtXY(pGrid, x, y, &boatIndex, &guessed);

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
