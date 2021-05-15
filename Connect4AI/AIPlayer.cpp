#include "AIPlayer.h"
#include "Board.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
	
}


pair< int, int> AIPlayer::requestMove(vector<tileRow*> TileList, Teams currTeam, int depth, int alpha, int beta)
{
	//Scan board for all possible moves and assign values to them
	pair<int, int> legalMoves;
	pair<int, int> BestTile = { 0, 0 };
	int bestScore;

	//Determine starting value of best score based on current perspective.
	if(currTeam == YELLOW)
	{
		bestScore = Board::LOSS; /*Assume worst*/
	}
	else
	{
		bestScore = Board::WIN; /*Assume best*/
	}

	//??? Check board state?

	//Using list of all tiles... find all legal moves //ADDEDENDUM NOT YET Working
	for (int i = 0; i < TileList.size(); i++)
	{
		for (int j = 0; j < TileList[i]->size(); j++)
		{
			if (TileList.at(i)->at(j)->getOwner() == UNOWNED)
			{
				if (TileList.at(i)->at(j)->getID().second == 3)
				{
				//	legalMoves.push_back(i);
				}

				else {
					
					int checkCol = TileList.at(i)->at(j)->getID().second + 1;
					int checkRow = TileList.at(i)->at(j)->getID().first;

					
					if (TileList.at(checkRow)->at(checkCol)->getOwner() == RED ||
						TileList.at(checkRow)->at(checkCol)->getOwner() == YELLOW)
					{
				//		legalMoves.push_back(i);
					}
				}
			}
		}
	}
	
	
	
	if (depth == 0) {
		return legalMoves = { BestTile };
	}
	
	//For all legal moves, do this function again with depth - 1;

	//Set best Move according to last search

	//Compare score of ^^^ to currently established best move (across all searches)
	
	return requestMove(TileList, currTeam, depth - 1, alpha, beta);
}

/*----------------------------------------NOAH ARK---------------------------------------------------
int AIPlayer::bestMove(vector<Tile*> tileList, vector<int> legalMoves, int depth, int alpha, int beta)
{
	//Determine if the move is good or not
	int value = 0;
	int iDBest = 0;

	
	return value;
}

void Board::actionlessPlaceTile(int ID)
{
	for (Tile* tile : TileList)
	{
		if (tile->getID() == ID)
		{
			//cout << "Found ID: " << tile->getID() << endl;
			tile->Claim(currTeam);
			break;
		}
	}

	if (Check4() != UNOWNED) isQuit = true;
}

void Board::turnChange()
{
	currTeam = YELLOW; Give turn to AI
	//this->TileList, currTeam, MAX_DEPTH, LOSS, WIN

actionlessPlaceTile(opponent->requestMove(this->TileList, currTeam, MAX_DEPTH, LOSS, WIN));
currTeam = RED;
}


//Prior implementation of line 34 //ADDEDENDUM NOT YET FIXED
	for (int i = 0; i < TileList.size(); i++)
	{
		//If the tile is a legal move
		if (TileList[i]->getOwner() == UNOWNED)
		{
			if (tileList[i]->getBoardPos().y % 4 == 3)
			{
				//Add to map of legal moves
				legalMoves.push_back(i);
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
							legalMoves.push_back(i);
						}
					}
				}
			}
		}
	}

*/
