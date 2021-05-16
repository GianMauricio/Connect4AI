#include "AIPlayer.h"

#include <chrono>
#include <random>



#include "Board.h"

AIPlayer::AIPlayer(int maxDepth)
{
	this->maxDepth = maxDepth;
}

AIPlayer::~AIPlayer()
{
	
}

pair<int, int> AIPlayer::requestMove(vector<tileRow*> TileGrid, int depth)
{
	//Scan board for all possible moves
	vector<pair<int, int>> legalMoves; //Legal moves

	//Determine all legal moves
	for (int i = 0; i < TileGrid.size(); i++)
	{
		for (int j = 0; j < TileGrid[i]->size(); j++)
		{
			if (TileGrid.at(i)->at(j)->getOwner() == UNOWNED)
			{
				if (TileGrid.at(i)->at(j)->getID().second == 3)
				{
					legalMoves.push_back(TileGrid.at(i)->at(j)->getID());
				}

				else 
				{
					
					int checkCol = TileGrid.at(i)->at(j)->getID().second + 1;
					int checkRow = TileGrid.at(i)->at(j)->getID().first;

					
					if (TileGrid.at(checkRow)->at(checkCol)->getOwner() == RED ||
						TileGrid.at(checkRow)->at(checkCol)->getOwner() == YELLOW)
					{
						legalMoves.push_back(TileGrid.at(i)->at(j)->getID());
					}
				}
			}
		}
	}
	
	//Shuffle all possible moves to (try and) avoid side bias
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(legalMoves.begin(), legalMoves.end(), default_random_engine(seed));
	
	//Initialize values
	pair<int, int> BestTile = legalMoves.at(0); //best move is equal to the first move of randomized vector
	int bestScore = -99999999; /*Start at worst possible score*/
	
	//Set alpha/beta
	int Alpha = -99999999; //Assume min because we go up O wO
	int Beta = 99999999; //Assume max because we go down Uw U
	
	//For every legal move...
	for (auto& key_Val : legalMoves)
	{
		//Make fake board based on move
		vector<tileRow*> fakeBoard;

		//Initialize fake board
		for (int i = 0; i < 5; i++)
		{
			tileRow* newtileRow = new tileRow;

			for (int j = 0; j < 4; j++)
			{
				Tile* newpointer = new Tile(TileGrid.at(i)->at(j));
				newtileRow->push_back(newpointer);
			}

			fakeBoard.push_back(newtileRow);
		}

		//Make move on the FAKE board;
		fakeBoard.at(key_Val.first)->at(key_Val.second)->Claim(YELLOW);

		//USE FAKE BOARD OR WE ALL DIE
		int boardScore = bestMoveMin(fakeBoard, depth - 1, Alpha, Beta, YELLOW, RED); //Call starts with player turn because it's always player turn next U wU

		cout << "Move ID {" << key_Val.first << ", " << key_Val.second << "} has score " << boardScore << endl;
		
		if(boardScore > bestScore)
		{
			bestScore = boardScore;
			BestTile = key_Val;
		}
	}

	return BestTile;
}

//Maximize Alpha
int AIPlayer::bestMoveMax(vector<tileRow*> TileGrid, int depth, int alpha, int beta, Teams player, Teams opponent)
{
	//RECURSIVE THING
	vector<BoardState> legalBoards;
	
	//Get all legal moves for the current board state --> save as a vector of board states
	for (int i = 0; i < TileGrid.size(); i++)
	{
		for (int j = 0; j < TileGrid[i]->size(); j++)
		{
			if (TileGrid.at(i)->at(j)->getOwner() == UNOWNED)
			{
				if (TileGrid.at(i)->at(j)->getID().second == 3)
				{
					//Make fake board
					vector<tileRow*> fakeBoard; /*RIP Space complexity even more*/

					//Initialize fake board
					for (int i = 0; i < 5; i++)
					{
						tileRow* newtileRow = new tileRow;

						for (int j = 0; j < 4; j++)
						{
							Tile* newpointer = new Tile(TileGrid.at(i)->at(j));
							newtileRow->push_back(newpointer);
						}

						fakeBoard.push_back(newtileRow);
					}

					//Make move on the FAKE board;
					fakeBoard.at(i)->at(j)->Claim(YELLOW);
					legalBoards.push_back(fakeBoard);
				}

				else 
				{
					int checkCol = TileGrid.at(i)->at(j)->getID().second + 1;
					int checkRow = TileGrid.at(i)->at(j)->getID().first;

					if (TileGrid.at(checkRow)->at(checkCol)->getOwner() == RED ||
						TileGrid.at(checkRow)->at(checkCol)->getOwner() == YELLOW)
					{
						//Make fake board
						vector<tileRow*> fakeBoard; /*RIP Space complexity even more*/

						//Initialize fake board
						for (int i = 0; i < 5; i++)
						{
							tileRow* newtileRow = new tileRow;

							for (int j = 0; j < 4; j++)
							{
								Tile* newpointer = new Tile(TileGrid.at(i)->at(j));
								newtileRow->push_back(newpointer);
							}

							fakeBoard.push_back(newtileRow);
						}

						//Make move on the FAKE board;
						fakeBoard.at(i)->at(j)->Claim(YELLOW);
						legalBoards.push_back(fakeBoard);
					}
				}
			}
		}
	}

	
	//This is how the loop breaks, this must work OR WE DIE
	//Check to see if this board state (the one being fed TO this function), is a game over state;
	if(depth == 0 || boardDone(TileGrid) || legalBoards.empty())
	{
		return boardEval(TileGrid, player);
	}

	//If the board is not in a gameOver state
	//Set new alpha val;
	int newAlpha = alpha;

	//For Every legal board state after this one
	for(BoardState state : legalBoards)
	{
		int boardScore = -99999999;

		//If the current alpha is still smaller than the beta, then recur
		if(newAlpha < beta)
		{
			//Make fake board
			vector<tileRow*> fakeBoard; /*RIP Space complexity even more AGAIN*/

			//Initialize fake board
			for (int i = 0; i < 5; i++)
			{
				tileRow* newtileRow = new tileRow;

				for (int j = 0; j < 4; j++)
				{
					Tile* newpointer = new Tile(state.at(i)->at(j)); /*Tile clones tile from THIS particular board state*/
					newtileRow->push_back(newpointer);
				}

				fakeBoard.push_back(newtileRow);
			}

			//Recur to find out socre of future boards from THIS move, reduce depth and give the FAKE BOARD FROM THIS LOOP
			boardScore = bestMoveMin(fakeBoard, depth - 1, newAlpha, beta, player, opponent);
		}

		//Update alpha values
		if(boardScore > newAlpha)
		{
			newAlpha = boardScore;
		}
	}
	
	//Return new alpha
	return newAlpha;
}

//Minimize Beta
int AIPlayer::bestMoveMin(vector<tileRow*> TileGrid, int depth, int alpha, int beta, Teams player, Teams opponent)
{
	//RECURSIVE THING
	vector<BoardState> legalBoards;
	
	//Get all legal moves for the current board state --> save as a vector of board states
	for (int i = 0; i < TileGrid.size(); i++)
	{
		for (int j = 0; j < TileGrid[i]->size(); j++)
		{
			if (TileGrid.at(i)->at(j)->getOwner() == UNOWNED)
			{
				if (TileGrid.at(i)->at(j)->getID().second == 3)
				{
					//Make fake board
					vector<tileRow*> fakeBoard; /*RIP Space complexity even more*/

					//Initialize fake board
					for (int i = 0; i < 5; i++)
					{
						tileRow* newtileRow = new tileRow;

						for (int j = 0; j < 4; j++)
						{
							Tile* newpointer = new Tile(TileGrid.at(i)->at(j));
							newtileRow->push_back(newpointer);
						}

						fakeBoard.push_back(newtileRow);
					}

					//Make move on the FAKE board;
					fakeBoard.at(i)->at(j)->Claim(RED);
					legalBoards.push_back(fakeBoard);
				}

				else
				{
					int checkCol = TileGrid.at(i)->at(j)->getID().second + 1;
					int checkRow = TileGrid.at(i)->at(j)->getID().first;

					if (TileGrid.at(checkRow)->at(checkCol)->getOwner() == RED ||
						TileGrid.at(checkRow)->at(checkCol)->getOwner() == YELLOW)
					{
						//Make fake board
						vector<tileRow*> fakeBoard; /*RIP Space complexity even more*/

						//Initialize fake board
						for (int i = 0; i < 5; i++)
						{
							tileRow* newtileRow = new tileRow;

							for (int j = 0; j < 4; j++)
							{
								Tile* newpointer = new Tile(TileGrid.at(i)->at(j));
								newtileRow->push_back(newpointer);
							}

							fakeBoard.push_back(newtileRow);
						}

						//Make move on the FAKE board;
						fakeBoard.at(i)->at(j)->Claim(RED);
						legalBoards.push_back(fakeBoard);
					}
				}
			}
		}
	}

	//This is how the loop breaks, this must work OR WE DIE
	//Check to see if this board state (the one being fed TO this function), is a game over state;
	if (depth == 0 || boardDone(TileGrid) || legalBoards.empty())
	{
		return boardEval(TileGrid, player);
	}

	//If the board is not in a gameOver state
	//Set new alpha val;
	int newBeta = beta;

	//For Every legal board state after this one
	for (BoardState state : legalBoards)
	{
		int boardScore = -99999999;

		//If the current alpha is still smaller than the beta, then recur
		if (alpha < newBeta)
		{
			//Make fake board
			vector<tileRow*> fakeBoard; /*RIP Space complexity even more AGAIN*/

			//Initialize fake board
			for (int i = 0; i < 5; i++)
			{
				tileRow* newtileRow = new tileRow;

				for (int j = 0; j < 4; j++)
				{
					Tile* newpointer = new Tile(state.at(i)->at(j)); /*Tile clones tile from THIS particular board state*/
					newtileRow->push_back(newpointer);
				}

				fakeBoard.push_back(newtileRow);
			}

			//Recur to find out score of future boards from THIS move, reduce depth and give the FAKE BOARD FROM THIS LOOP
			boardScore = bestMoveMax(fakeBoard, depth - 1, alpha, newBeta, player, opponent);
		}

		//Update beta values
		if (boardScore < newBeta)
		{
			newBeta = boardScore;
		}
	}
	
	//Return new beta
	return newBeta;
}

//RISK PRONE --> Dangerous return sequence, check order if possible
bool AIPlayer::boardDone(BoardState state)
{
	bool iswon;
	bool istie;
	bool iswonistie;
	iswon = isWon(state) == UNOWNED;
	istie = isTied(state);
	iswonistie = iswon && istie;

	return iswonistie;
}

Teams AIPlayer::isWon(BoardState state)
{
	//Search alg
	Teams colInitial = UNOWNED;
	int count = 0;

	//Search through all Columns
	for (int i = 0; i < 5; i++) /*Column number*/
	{
		colInitial = state.at(i)->at(0)->getOwner();
		for (int j = 0; j < 4; j++) /*Circle in column*/
		{
			//If circle belongs to the this player
			if (state.at(i)->at(j)->getOwner() == colInitial)
			{
				//Increase count of current sequence search
				count++;
			}

			//Otherwise, reset count
			else
			{
				count = 0;
			}

			//If current sequence search has found enough circles in sequence
			if (count == 4)
			{
				return colInitial;
			}
		}

		//Once done checking this column, reset circles in sequence
		count = 0;
	} /*This isn't best implementation for this... too bad!*/

	//Search through all Rows
	count = 0;
	Teams rowInitial;
	//Row-search
	for (int i = 0; i < 4; i++) //Row number
	{
		rowInitial = state.at(0)->at(i)->getOwner();
		for (int j = 0; j < 5; j++) //Circle in row
		{
			//If circle in row is the same as player team
			if (state.at(j)->at(i)->getOwner() == rowInitial)
			{
				//Increase count
				count++;
			}

			//Otherwise
			else
			{
				//Reset count
				count = 0;
			}

			//If current sequence search has found enough circles in sequence
			if (count == 4)
			{
				return rowInitial;
			}
		}

		//Reset count before moving to next row
		count = 0;
	}

	//Search Diags
	//Diag ID:i
	//Reset count prior to doing anything, just in case
	count = 0;
	Teams initialDiag = state.at(0)->at(0)->getOwner();
	for (int i = 0; i < 4; i++)/*For every circle in diagonal*/
	{

		//If circle is of player team increment count
		if (state.at(i)->at(i)->getOwner() == initialDiag)
		{
			count++;
		}

		//Otherwise reset count
		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == 4)
		{
			return initialDiag;
		}
	}

	//Diag ID:i+1
	//Reset count prior to doing anything, just in case
	count = 0;
	initialDiag = state.at(1)->at(0)->getOwner();
	for (int i = 0; i < 4; i++) /*For every circle in diag*/
	{
		//If current player owns it, increment count
		if (state.at(i + 1)->at(i)->getOwner() == initialDiag)
		{
			count++;
		}

		//otherwise reset count
		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == 4)
		{
			return initialDiag;
		}
	}

	//Diag ID:3
	//Reset count prior to doing anything, just in case
	count = 0;
	initialDiag = state.at(3)->at(0)->getOwner();
	for (int i = 0; i < 4; i++)
	{
		if (state.at(3 - i)->at(i)->getOwner() == initialDiag)
		{
			count++;
		}

		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == 4)
		{
			return initialDiag;
		}
	}

	//DONE
	//Diag ID:4
	count = 0;
	initialDiag = state.at(4)->at(0)->getOwner();
	for (int i = 0; i < 4; i++)
	{
		if (state.at(4 - i)->at(i)->getOwner() == initialDiag)
		{
			count++;
		}

		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == 4)
		{
			return initialDiag;
		}
	}

	return UNOWNED;
}

bool AIPlayer::isTied(BoardState state)
{
	bool tied = true;
	for (int i = 0; i < state.size(); i++)
	{
		for (int j = 0; j < state[i]->size(); j++)
		{
			if (state.at(i)->at(j)->getOwner() == UNOWNED)
			{
				tied = false;
				break;
			}
		}
	}

	return tied;
}

int AIPlayer::boardEval(BoardState state, Teams currPlayer)
{
	Teams opponent;
	int finalEval;
	
	//Set up opponent perspective
	if(currPlayer == YELLOW)
	{
		opponent = RED;
	}

	else
	{
		opponent = YELLOW;
	}

	//Player score set
	int pFours, pThrees, pTwos;
	
	//Opponent score set
	int oThrees, oTwos; 
	
	//Evaluate board for 4 matches --> for us
	pFours = countSequence(state, currPlayer, 4);
	
	//Evaluate board for 3 matches
	pThrees = countSequence(state, currPlayer, 3);
	oThrees = countSequence(state, opponent, 3);
	
	//Evaluate board for 2 matches
	pTwos = countSequence(state, currPlayer, 2);
	oTwos = countSequence(state, opponent, 2);

	//Check if opponent has 4s --> if yes then give -MAXVal
	if(countSequence(state, opponent, 4) > 0)
	{
		finalEval = -9999999;
		return finalEval;
	}

	//Otherwise, determine score;
	int pTotals = pFours * 99999 + pThrees * 999 + pTwos * 9;
	int oTotals = oThrees * 999 + oTwos * 9;

	finalEval = pTotals - oTotals;

	return finalEval;
}

int AIPlayer::countSequence(BoardState currBoard, Teams currPlayer, int reqSequence)
{
	//Column sequence search
	//Search alg
	int count = 0;
	int totalCount = 0;

	//Search through all Columns
	for (int i = 0; i < 5; i++) /*Column number*/
	{
		for (int j = 0; j < 4; j++) /*Circle in column*/
		{
			//If circle belongs to the this player
			if (currBoard.at(i)->at(j)->getOwner() == currPlayer)
			{
				//Increase count of current sequence search
				count++;
			}

			//Otherwise, reset count
			else
			{
				count = 0;
			}

			//If current sequence search has found enough circles in sequence
			if (count == reqSequence)
			{
				//Increase total sequences found
				totalCount++;

				//Then reset circles in sequence
				count = 0;
			}
		}

		//Once done checking this column, reset circles in sequence
		count = 0;
	} /*This isn't best implementation for this... too bad!*/

	//Search through all Rows
	count = 0;

	//Row-search
	for (int i = 0; i < 4; i++) //Row number
	{
		for (int j = 0; j < 5; j++) //Circle in row
		{
			//If circle in row is the same as player team
			if (currBoard.at(j)->at(i)->getOwner() == currPlayer)
			{
				//Increase count
				count++;
			}

			//Otherwise
			else
			{
				//Reset count
				count = 0;
			}

			//If current sequence search has found enough circles in sequence
			if (count == reqSequence)
			{
				//Increase total sequences found
				totalCount++;

				//Then reset circles in sequence
				count = 0;
			}
		}

		//Reset count before moving to next row
		count = 0;
	}

	//Search Diags
	//Diag ID:i
	//Reset count prior to doing anything, just in case
	count = 0;
	for (int i = 0; i < 4; i++)/*For every circle in diagonal*/
	{

		//If circle is of player team increment count
		if (currBoard.at(i)->at(i)->getOwner() == currPlayer)
		{
			count++;
		}

		//Otherwise reset count
		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == reqSequence)
		{
			//Increase total sequences found
			totalCount++;

			//Then reset circles in sequence
			count = 0;
		}
	}

	//Diag ID:i+1
	//Reset count prior to doing anything, just in case
	count = 0;
	for (int i = 0; i < 4; i++) /*For every circle in diag*/
	{
		//If current player owns it, increment count
		if (currBoard.at(i + 1)->at(i)->getOwner() == currPlayer)
		{
			count++;
		}

		//otherwise reset count
		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == reqSequence)
		{
			//Increase total sequences found
			totalCount++;

			//Then reset circles in sequence
			count = 0;
		}
	}

	//Diag ID:3
	//Reset count prior to doing anything, just in case
	count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (currBoard.at(3 - i)->at(i)->getOwner() == currPlayer)
		{
			count++;
		}

		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == reqSequence)
		{
			//Increase total sequences found
			totalCount++;

			//Then reset circles in sequence
			count = 0;
		}
	}


	//DONE
	//Diag ID:4
	count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (currBoard.at(4 - i)->at(i)->getOwner() == currPlayer)
		{
			count++;
		}

		else
		{
			count = 0;
		}

		//If current sequence search has found enough circles in sequence
		if (count == reqSequence)
		{
			//Increase total sequences found
			totalCount++;

			//Then reset circles in sequence
			count = 0;
		}
	}
	
	return totalCount;
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
