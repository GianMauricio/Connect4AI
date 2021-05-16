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

	pair<int, int> tileID = { 0, 0 };
	//Spawn tiles
	for (int i = 0; i < TilesAcross; i++) /*Set row*/
	{
		for (int j = 0; j < TilesAlong; j++) /*Set Col*/
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
	for (int i = 0; i < TileList.size(); i++)
	{
		for (int j = 0; j < TileList[i]->size(); j++)
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

						cout << "Checking tile with ID: " << checkRow << ", " << checkCol << endl;
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

	if (tied)
	{
		isQuit = true;
		cout << "Drawn" << endl;
	}

	return tied;
}

void Board::draw(RenderWindow* window, RenderStates state)
{
	/*if (isQuit)
	{
		window->close();
	}
	*/
	//else
	//{
		//Bug found
		for (int i = 0; i < TileList.size(); i++)
		{
			for (int j = 0; j < TileList[i]->size(); j++)
			{
				TileList.at(i)->at(j)->Draw(window, state);
			}
		}
	//}
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
		actionlessPlaceTile(this->opponent->requestMove(this->TileList, 6));
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
	cout << "Found ID: " << ID.first << ", " << ID.second << endl;
	TileList.at(ID.first)->at(ID.second)->Claim(currTeam);
	if (Check4() != UNOWNED) { isQuit = true; }
	turnChange();
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
	

*/