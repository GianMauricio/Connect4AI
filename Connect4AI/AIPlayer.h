#pragma once
#include "Tile.h"
#include <unordered_map>

typedef vector<Tile*> tileRow;
class AIPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	pair< int, int>  requestMove(vector<tileRow*> TileList, Teams currTeam, int depth, int alpha, int beta);
private:
	//int bestMove(vector<Tile*> tileList, vector<int> legalMoves, int depth, int alpha, int beta);
};

