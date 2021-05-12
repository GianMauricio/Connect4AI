#include "Board.h"

#include <iostream>

Board::Board(int Twidth, int Theight)
{
	this->TilesAcross = Twidth;
	this->TilesAlong = Theight;

	int tileID = 0;
	//Spawn tiles
	for(int i = 0; i < TilesAcross; i++) /*Set row*/
	{
		for(int j = 0; j < TilesAlong; j++) /*Set Col*/
		{
			Tile* newTile = new Tile(tileID, tileWidth, tileLength);
			newTile->setPosition(i * tileWidth, j * tileLength);
			tileID++;


			newTile->setBoardPos(i, j);
			TileList.push_back(newTile);
		}
	}
}

Board::~Board()
{
}

void Board::tryPlace(Vector2f mousePos)
{
	for(int i = 0; i < TileList.size(); i++)
	{
		if(TileList[i]->inBounds(mousePos))
		{
			if(TileList[i]->getOwner() == UNOWNED)
			{
				if (TileList[i]->getBoardPos().y % 4 == 3)
				{
					//cout << "Checking for bottom tile" << endl;
					PlaceTile(TileList[i]->getID());
				}

				else {
					//cout << "Checking for non-bottom tile" << endl;
					int checkRow = TileList[i]->getBoardPos().x;
					int checkCol = TileList[i]->getBoardPos().y;

					for (Tile* tile : TileList)
					{
						if (tile->getBoardPos().x == checkRow && tile->getBoardPos().y == checkCol + 1)
						{
							//cout << "Checking tile with ID: " << tile->getID() << endl;
							if (tile->getOwner() == RED || tile->getOwner() == YELLOW)
							{
								PlaceTile(TileList[i]->getID());
							}
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
	for(Tile* tile : TileList)
	{
		if(tile->getOwner() == UNOWNED)
		{
			tied = false;
		}
	}
	
	if(tied)
	{
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
		for (Tile* tile : TileList)
		{
			tile->Draw(window, state);
		}
	}
}

void Board::PlaceTile(int ID)
{
	for (Tile* tile : TileList)
	{
		if(tile->getID() == ID)
		{
			cout << "Found ID: " << tile->getID() << endl;
			tile->Claim(currTeam);
			turnChange();
			break;
		}
	}
	if (Check4() != UNOWNED) { isQuit = true; }
}

void Board::turnChange()
{
	if (this->currTeam == RED)
	{
		currTeam = YELLOW;
	}

	else if (currTeam == YELLOW)
	{
		currTeam = RED;
	}
}

Teams Board::Check4()
{
	//Search alg
	Teams colInitial = UNOWNED;
	int count = 0;

	//Search through all Columns
	for (int i = 0; i < 5; i++) /*Column number*/
	{
		//Set initial team
		int initialTile = i * 4;
		colInitial = TileList[initialTile]->getOwner();

		//If circle is unowned, continue to next column
		if (colInitial == UNOWNED)
		{
			count = 0;
			continue;
		}

		//cout << "checking column: " << initialTile << endl;
		for (int j = 0; j < 4; j++) /*Circle in column*/
		{
			//If circle belongs to the opposite team, stop checking column
			if (TileList[initialTile + j]->getOwner() != colInitial)
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
			cout << "Col search found winner!" << endl;
			return colInitial;
		}
	}

	//Search through all rows
	count = 0;
	Teams rowInitial;
	
	//Forward facing row-search
	for (int i = 0; i < 4; i++) /*Row number*/
	{
		rowInitial = TileList[i]->getOwner();

		if (rowInitial == UNOWNED)
		{
			count = 0;
			continue;
		}

		for (int j = 0; j < 4; j++) /*Circle in row*/
		{
			int checkTile = i + (j * 4);
			//cout << "Tile being checked: " << checkTile << endl;
			if (TileList[checkTile]->getOwner() != rowInitial)
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
			cout << "F.Row search found winner!" << endl;
			return rowInitial;
		}
	}

	//Backwards facing row search
	count = 0;
	for(int i = 0; i < 4; i++)
	{
		int initialTile = 16 + i;
		rowInitial = TileList[initialTile]->getOwner();

		if (rowInitial == UNOWNED)
		{
			count = 0;
			continue;
		}

		for (int j = 0; j < 4; j++)
		{
			int checkTile = 16 + i - (j * 4);
			//cout << "Tile being checked: " << checkTile << endl;

			if (TileList[checkTile]->getOwner() != rowInitial)
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
			cout << "B.Row search found winner!" << endl;
			return rowInitial;
		}
	}

	//Diagonal check

	//Diag ID5
	count = 0;
	Teams initialDiag = TileList[0]->getOwner();
	for(int i = 0; i < 4; i++)
	{
		int tileCheck = i * 5;
		if(TileList[tileCheck]->getOwner() != initialDiag)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//If Diag ID:5 finds winner
	if (count == 4 && initialDiag != UNOWNED)
	{
		cout << "Diag 5 search found winner!" << endl;
		return initialDiag;
	}

	//Diag ID:The diagonal beside ID5
	count = 0;
	initialDiag = TileList[4]->getOwner();
	for (int i = 0; i < 4; i++)
	{
		int tileCheck = i * 5 + 4;
		if (TileList[tileCheck]->getOwner() != initialDiag)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//If Diag ID:The diagonal beside ID5 finds winner
	if (count == 4 && initialDiag != UNOWNED)
	{
		cout << "The diagonal beside ID5 search found a winner!" << endl;
		return initialDiag;
	}

	//Diag ID:3
	count = 0;
	initialDiag = TileList[12]->getOwner();
	for (int i = 1; i < 5; i++)
	{
		int tileCheck = i * 3;
		if (TileList[tileCheck]->getOwner() != initialDiag)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//If Diag ID:The diagonal beside ID5 finds winner
	if (count == 4 && initialDiag != UNOWNED)
	{
		cout << "Diag ID:3 found a winner" << endl;
		return initialDiag;
	}

	//Diag ID:7+
	count = 0;
	initialDiag = TileList[16]->getOwner();
	for (int i = 0; i < 4; i++)
	{
		int tileCheck = 7 + i * 3;
		if (TileList[tileCheck]->getOwner() != initialDiag)
		{
			count = 0;
			break;
		}

		else
		{
			count++;
		}
	}

	//If Diag ID:7+ finds winner
	if (count == 4 && initialDiag != UNOWNED)
	{
		cout << "Diag ID:7+ found a winner" << endl;
		return initialDiag;
	}

	isTie();
	
	return UNOWNED;
}