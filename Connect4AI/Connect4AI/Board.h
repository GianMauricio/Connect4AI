#pragma once
#include "Tile.h"
#include <vector>
class Board
{
public:
	Board(int Twidth, int Theight);
	~Board();

	//Try to place a tile at click location
	void tryPlace(Vector2f mousePos);

	//Check if the board has a winning combo
	bool isWon();

	//Check if the board is a tie or has no possible moves
	bool isTie();

	//Draw tiles
	void draw(RenderWindow* window, RenderStates state);
private:
	int tileWidth = 204;
	int tileLength = 192;
	int TilesAcross;
	int TilesAlong;
	vector<Tile*> TileList;
	Teams currTeam = RED;

	//Change tile color to currently active team
	void PlaceTile(int ID);

	//Changes the current team turn
	void turnChange();

	//Check for combos of 4 horizontally, vertically, diagonally
	Teams Check4();
};

