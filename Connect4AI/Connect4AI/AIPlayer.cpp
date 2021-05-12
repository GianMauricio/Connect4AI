#include "AIPlayer.h"
#include "Board.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::~AIPlayer()
{
	
}

int AIPlayer::requestMove(vector<Tile*> tileList, Teams currTeam, int depth, int alpha, int beta)
{
	//Scan board for all possible moves and assign values to them
	vector<int> legalMoves;
	int BestTile = 0;
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
	
	//Using list of all tiles... find all legal moves
	for (int i = 0; i < tileList.size(); i++)
	{
		//If the tile is a legal move
		if (tileList[i]->getOwner() == UNOWNED)
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

	
	if (depth == 0) {
		return legalMoves[BestTile];
	}
	
	//For all legal moves, do this function again with depth - 1;

	//Set best Move according to last search

	//Compare score of ^^^ to currently established best move (across all searches)
	
	return requestMove(tileList, currTeam, depth - 1, alpha, beta);
}

/*
int AIPlayer::bestMove(vector<Tile*> tileList, vector<int> legalMoves, int depth, int alpha, int beta)
{
	//Determine if the move is good or not
	int value = 0;
	int iDBest = 0;

	
	return value;
}
*/