#include "AIPlayer.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
	
}

int AIPlayer::requestMove(vector<Tile*> tileList)
{
	//Scan board for all possible moves and assign values to them
	unordered_map<int, int> legalMoves;

	//Using list of all tiles...
	for (int i = 0; i < tileList.size(); i++)
	{
		//If the tile is a legal move
		if (tileList[i]->getOwner() == UNOWNED)
		{
			if (tileList[i]->getBoardPos().y % 4 == 3)
			{
				//Add to map of legal moves with value
				legalMoves.emplace(i, getValue(i));
			}

			else {
				int checkRow = tileList[i]->getBoardPos().x;
				int checkCol = tileList[i]->getBoardPos().y;

				for (Tile* tile : tileList)
				{
					if (tile->getBoardPos().x == checkRow && tile->getBoardPos().y == checkCol + 1)
					{
						if (tile->getOwner() == RED || tile->getOwner() == YELLOW)
						{
							legalMoves.emplace(i, getValue(i));
						}
					}
				}
			}
		}
	}


	//Once all legal moves have been mapped out; determine best one to play.
	int highestValue = -1;
	int bestID = -1;
	
	for(pair<int, int> tile : legalMoves)
	{
		//If checked tile is better than current best...
		if(tile.second > highestValue)
		{
			//Set tile to be current best
			highestValue = tile.second;
			bestID = tile.first;
		}
	}

	return bestID;
}

int AIPlayer::getValue(int tileTarget)
{
	//Determine if the move is good or not
	return rand() % 100; /*AI be dumb AF*/
}
