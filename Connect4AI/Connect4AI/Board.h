#pragma once
#include "Tile.h"
#include <vector>

typedef vector<Tile*> tileRow;
class Board
{
public:
	Board(int Twidth, int Theight);
	~Board();

	//Try to place a tile at click location
	void tryPlace(Vector2f mousePos);

	//Check if the board is a tie or has no possible moves
	bool isTie();

	//Draw tiles
	void draw(RenderWindow* window, RenderStates state);

	//Check for combos of 4 horizontally, vertically, diagonally
	//Returns team that has Connect4
	Teams Check4();
	
private:
	int tileWidth = 204;
	int tileLength = 192;
	int TilesAcross;
	int TilesAlong;
	bool isQuit = false;
	vector<tileRow*> TileList;
	Teams currTeam = RED;

	//Change tile color to currently active team
	void PlaceTile(pair< int, int> ID);

	//Changes the current team turn
	void turnChange();
};

