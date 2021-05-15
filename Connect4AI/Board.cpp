#include "Board.h"
#include <iostream>
#include "BaseRunner.h"

Board::Board(int Twidth, int Theight)
{
	this->TilesAcross = Twidth;
	this->TilesAlong = Theight;

	//Initialize list
	for (int i = 0; i < TilesAcross; i++)
	{
		tileRow* newtileRow = new tileRow;
		
		for (int j = 0; j < TilesAlong; j++)
		{
			Tile* newpointer;
			newtileRow->push_back(newpointer);
		}

		TileList.push_back(newtileRow);
	}
	
	pair<int, int> tileID = {0, 0};
	//Spawn tiles
	for(int i = 0; i < TilesAcross; i++) /*Set row*/
	{
		for(int j = 0; j < TilesAlong; j++) /*Set Col*/
		{
			Tile* newTile = new Tile(i, j, tileWidth, tileLength);
			newTile->setPosition(i * tileWidth, j * tileLength);

			TileList.at(i)->at(j) = newTile;
		}
	}
}

Board::~Board()
{
}

void Board::setAI(AIPlayer* newOpponent)
{
	this->opponent = newOpponent;
}

void Board::tryPlace(Vector2f mousePos)
{
	for(int i = 0; i < TileList.size(); i++)
	{
		for(int j = 0; j < TileList[i]->size(); j++)
		{
			if (TileList.at(i)->at(j)->inBounds(mousePos))
			{
				cout << "Checking Tile ID: " << TileList.at(i)->at(j)->getID().first << ", " << TileList.at(i)->at(j)->getID().second << endl;
				if (TileList.at(i)->at(j)->getOwner() == UNOWNED)
				{
					if (TileList.at(i)->at(j)->getID().second == 3)
					{
						cout << "Checking for bottom tile" << endl;
						PlaceTile(TileList.at(i)->at(j)->getID());
						cout << std::numeric_limits<int>::max() * -1 << endl;
					}

					else {
						cout << "Checking for non-bottom tile" << endl;
						int checkCol = TileList.at(i)->at(j)->getID().second + 1;
						int checkRow = TileList.at(i)->at(j)->getID().first;

						cout << "Checking tile with ID: " << checkRow << ", "<< checkCol << endl;
						if (TileList.at(checkRow)->at(checkCol)->getOwner() == RED || 
							TileList.at(checkRow)->at(checkCol)->getOwner() == YELLOW)
						{
							PlaceTile(TileList.at(i)->at(j)->getID());
						}
					}
				}
			}
		}
	}
}

bool Board::isTie()
{
	bool tied = true;
	for (int i = 0; i < TileList.size(); i++)
	{
		for (int j = 0; j < TileList[i]->size(); j++)
		{
			if (TileList.at(i)->at(j)->getOwner() == UNOWNED)
			{
				tied = false;
			}
		}
	}
	
	if(tied)
	{
		isQuit = true;
		cout << "Drawn" << endl;
	}
	
	return tied;
}

void Board::draw(RenderWindow* window, RenderStates state)
{
	if(isQuit)
	{
		window->close();
	}
  
	else
	{
		//Bug found
		for (int i = 0; i < TileList.size(); i++)
		{
			for (int j = 0; j < TileList[i]->size(); j++)
			{
				TileList.at(i)->at(j)->Draw(window, state);
			}
		}
	}
}

void Board::PlaceTile(pair< int, int> ID)
{
	for (int i = 0; i < TileList.size(); i++)
	{
		for (int j = 0; j < TileList[i]->size(); j++)
		{
			if (TileList.at(i)->at(j)->getID() == ID)
			{
				cout << "Found ID: " << TileList.at(i)->at(j)->getID().first << ", " << TileList.at(i)->at(j)->getID().second << endl;
				TileList.at(i)->at(j)->Claim(currTeam);
				
				break;
			}
		}
	}

	if (Check4() != UNOWNED) { 
		isQuit = true;
	}

	turnChange();
}
// Give turn to AI;
void Board::turnChange()
{
	if (currTeam == RED) {
		currTeam = YELLOW;
		//ADD AI check here
	}
	else {
		currTeam = RED;
	}
	//actionlessPlaceTile(opponent->requestMove(this->TileList, currTeam, MAX_DEPTH, LOSS, WIN));
	//currTeam = RED;
}

void Board::actionlessPlaceTile(pair< int, int> ID)
{
	for (int i = 0; i < TileList.size(); i++)
	{
		for (int j = 0; j < TileList[i]->size(); j++)
		{
			if (TileList.at(i)->at(j)->getID() == ID)
			{
				cout << "Found ID: " << TileList.at(i)->at(j)->getID().first << ", " << TileList.at(i)->at(j)->getID().second << endl;
				TileList.at(i)->at(j)->Claim(currTeam);
				//turnChange();
				break;
			}
		}
	}

	if (Check4() != UNOWNED) { isQuit = true; }
}


Teams Board::Check4()
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
			if (TileList.at(i)->at(j)->getOwner() == currTeam)
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
				string winner = "";
				if (currTeam == RED)
				{
					winner = "Red Team";
				}
				else
				{
					winner = "Yellow Team";
				}
				cout << "Search found that " << winner << " is the winner!" << endl;
				return currTeam;
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
			if (TileList.at(j)->at(i)->getOwner() == currTeam)
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
				string winner = "";
				if (currTeam == RED)
				{
					winner = "Red Team";
				}
				else
				{
					winner = "Yellow Team";
				}
				cout << "Search found that " << winner << " is the winner!" << endl;
				return currTeam;
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
		if (TileList.at(i)->at(i)->getOwner() == currTeam)
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
			string winner = "";
			if (currTeam == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return currTeam;
		}
	}

	//Diag ID:i+1
	//Reset count prior to doing anything, just in case
	count = 0;
	for (int i = 0; i < 4; i++) /*For every circle in diag*/
	{
		//If current player owns it, increment count
		if (TileList.at(i + 1)->at(i)->getOwner() == currTeam)
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
			string winner = "";
			if (currTeam == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return currTeam;
		}
	}

	//Diag ID:3
	//Reset count prior to doing anything, just in case
	count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (TileList.at(3 - i)->at(i)->getOwner() == currTeam)
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
			string winner = "";
			if (currTeam == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return currTeam;
		}
	}


	//DONE
	//Diag ID:4
	count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (TileList.at(4 - i)->at(i)->getOwner() == currTeam)
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
			string winner = "";
			if (currTeam == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return currTeam;
		}
	}

	
	isTie();
	return UNOWNED;
}

/*
//Search alg
	Teams colInitial = UNOWNED;
	int count = 0;

	//Search through all Columns
	for (int i = 0; i < 5; i++) /*Column number
{
	//Set initial team
	colInitial = TileList.at(i)->at(0)->getOwner();

	//If circle is unowned, continue to next column
	if (colInitial == UNOWNED)
	{
		count = 0;
		continue;
	}

	//cout << "checking column: " << initialTile << endl;
	for (int j = 0; j < 4; j++) /*Circle in column
	{
		//If circle belongs to the opposite team, stop checking column
		if (TileList.at(i)->at(j)->getOwner() != colInitial)
		{
			count = 0;
			break;
		}

		//Otherwise, increase count
		else
		{
			count++;
		}
	}

	//cout << "Current col count: " << count << endl;
	if (count == 4)
	{
		string winner = "";
		if (colInitial == RED)
		{
			winner = "Red Team";
		}
		else
		{
			winner = "Yellow Team";
		}
		cout << "Col search found that " << winner << " is the winner!" << endl;
		return colInitial;
	}
}

//Search through all rows
count = 0;
Teams rowInitial;

//Forward facing row-search
for (int i = 0; i < 4; i++) //Row number
{
	rowInitial = TileList.at(0)->at(i)->getOwner();

	if (rowInitial == UNOWNED)
	{
		count = 0;
		continue;
	}

	for (int j = 0; j < 4; j++) //Circle in row
	{
		//cout << "Tile being checked: " << checkTile << endl;
		if (TileList.at(j)->at(i)->getOwner() != rowInitial)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//if forward facing finds stuff
	if (count == 4)
	{
		string winner = "";
		if (rowInitial == RED)
		{
			winner = "Red Team";
		}
		else
		{
			winner = "Yellow Team";
		}
		cout << "F. Row search found that " << winner << " is the winner!" << endl;
		return rowInitial;
	}
}


//Backwards facing row search
count = 0;
for (int i = 0; i < 4; i++)
{
	rowInitial = TileList.at(4)->at(i)->getOwner();

	if (rowInitial == UNOWNED)
	{
		count = 0;
		continue;
	}

	for (int j = 4; j > 0; j--)
	{
		if (TileList.at(j)->at(i)->getOwner() != rowInitial)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//if backwards facing finds stuff
	if (count == 4)
	{
		string winner = "";
		if (rowInitial == RED)
		{
			winner = "Red Team";
		}
		else
		{
			winner = "Yellow Team";
		}
		cout << "B. Row search found that " << winner << " is the winner!" << endl;
		return rowInitial;
	}
}


//Diagonal check
//Diag ID:i
count = 0;
Teams initialDiag = TileList.at(0)->at(0)->getOwner();
for (int i = 0; i < 4; i++)
{
	if (TileList.at(i)->at(i)->getOwner() != initialDiag)
	{
		count = 0;
		break;
	}

	else
	{
		count++;
	}
}

//If Diag ID:i finds winner
if (count == 4 && initialDiag != UNOWNED)
{
	string winner = "";
	if (initialDiag == RED)
	{
		winner = "Red Team";
	}
	else
	{
		winner = "Yellow Team";
	}
	cout << "Diag ID:i found that " << winner << " is the winner!" << endl;
	return initialDiag;
}

//DONE
//Diag ID:i+1
count = 0;
initialDiag = TileList.at(1)->at(0)->getOwner();
for (int i = 0; i < 4; i++)
{
	if (TileList.at(i + 1)->at(i)->getOwner() != initialDiag)
	{
		count = 0;
		break;
	}

	else
	{
		count++;
	}
}

//If Diag ID:i+1
if (count == 4 && initialDiag != UNOWNED)
{
	string winner = "";
	if (initialDiag == RED)
	{
		winner = "Red Team";
	}
	else
	{
		winner = "Yellow Team";
	}
	cout << "Diag ID:i+1 search found that " << winner << " is the winner!" << endl;
	return initialDiag;
}


//Diag ID:3
count = 0;
initialDiag = TileList.at(3)->at(0)->getOwner();
for (int i = 0; i < 4; i++)
{
	if (TileList.at(3 - i)->at(i)->getOwner() != initialDiag)
	{
		count = 0;
		break;
	}

	else
	{
		count++;
	}
}

//If Diag ID:3
if (count == 4 && initialDiag != UNOWNED)
{
	string winner = "";
	if (initialDiag == RED)
	{
		winner = "Red Team";
	}
	else
	{
		winner = "Yellow Team";
	}
	cout << "Diag ID3 search found that " << winner << " is the winner!" << endl;
	return initialDiag;
}


//DONE
//Diag ID:4
count = 0;
initialDiag = TileList.at(4)->at(0)->getOwner();
for (int i = 0; i < 4; i++)
{
	if (TileList.at(4 - i)->at(i)->getOwner() != initialDiag)
	{
		count = 0;
		break;
	}

	else
	{
		count++;
	}
}

//If Diag ID:4
if (count == 4 && initialDiag != UNOWNED)
{
	string winner = "";
	if (initialDiag == RED)
	{
		winner = "Red Team";
	}
	else
	{
		winner = "Yellow Team";
	}
	cout << "Diag ID 4 search found that " << winner << " is the winner!" << endl;
	return initialDiag;
}



Teams AIPlayer::isWon(BoardState state)
{
	//Search alg
	Teams colInitial = UNOWNED;
	int count = 0;

	//Search through all Columns
	for (int i = 0; i < 5; i++) /*Column number
	{
	colInitial = state.at(i)->at(0)->getOwner();
	for (int j = 0; j < 4; j++) /*Circle in column
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
			string winner = "";
			if (colInitial == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return colInitial;
		}
	}

	//Once done checking this column, reset circles in sequence
	count = 0;
	} /*This isn't best implementation for this... too bad!

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
				string winner = "";
				if (rowInitial == RED)
				{
					winner = "Red Team";
				}
				else
				{
					winner = "Yellow Team";
				}
				cout << "Search found that " << winner << " is the winner!" << endl;
				return rowInitial;
			}
		}

		//Reset count before moving to next row
		count = 0;
	}

	//Backwards facing row search
	count = 0;
	for (int i = 0; i < 4; i++)
	{
		rowInitial = state.at(4)->at(i)->getOwner();

		if (rowInitial == UNOWNED)
		{
			count = 0;
			continue;
		}

		for (int j = 4; j > 0; j--)
		{
			if (state.at(j)->at(i)->getOwner() != rowInitial)
			{
				count = 0;
				break;
			}

			else
			{
				count++;
			}
		}

		//if backwards facing finds stuff
		if (count == 4)
		{
			return rowInitial;
		}
	}

	//Search Diags
	//Diag ID:i
	//Reset count prior to doing anything, just in case
	count = 0;
	Teams initialDiag = state.at(0)->at(0)->getOwner();
	for (int i = 0; i < 4; i++)/*For every circle in diagonal
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
			string winner = "";
			if (initialDiag == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return initialDiag;
		}
	}

	//Diag ID:i+1
	//Reset count prior to doing anything, just in case
	count = 0;
	initialDiag = state.at(1)->at(0)->getOwner();
	for (int i = 0; i < 4; i++) /*For every circle in diag
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
			string winner = "";
			if (initialDiag == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
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
			string winner = "";
			if (initialDiag == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
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
			string winner = "";
			if (initialDiag == RED)
			{
				winner = "Red Team";
			}
			else
			{
				winner = "Yellow Team";
			}
			cout << "Search found that " << winner << " is the winner!" << endl;
			return initialDiag;
		}
	}
}


*/