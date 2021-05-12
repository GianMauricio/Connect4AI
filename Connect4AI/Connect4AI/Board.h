#pragma once
#include "Tile.h"
#include "AIPlayer.h"
#include <vector>

class Board
{
public:
	static const int LOSS = -1000;
	static const int WIN = 1000;
	
	Board(int Twidth, int Theight);
	~Board();

	//Set AI opponent
	void setAI(AIPlayer* newOpponent);
	
	//Try to place a tile at click location
	void tryPlace(Vector2f mousePos);

	//Try to place a tile at with a given ID
	void tryPlace(int targetTile); /* Overload is here for AI */

	//Check if the board is a tie or has no possible moves
	bool isTie();

	//Draw tiles
	void draw(RenderWindow* window, RenderStates state);

	//Check for combos of 4 horizontally, vertically, diagonally
	//Returns team that has Connect4
	Teams Check4();
	
private:
	static const int MAX_DEPTH = 19;
	bool isQuit = false;
	int tileWidth = 204;
	int tileLength = 192;
	int TilesAcross;
	int TilesAlong;
	vector<Tile*> TileList;
	Teams currTeam = RED;
	AIPlayer* opponent;

	//Change tile color to currently active team
	void PlaceTile(int ID);

	//Place tile but for AI
	void actionlessPlaceTile(int ID);

	//Changes the current team turn
	void turnChange();
};

