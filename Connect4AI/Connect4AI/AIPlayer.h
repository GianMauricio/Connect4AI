#pragma once
#include "Tile.h"
#include <unordered_map>

class AIPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	int requestMove(Tile* BoardState[4][5] , Teams currTeam, int depth, int alpha, int beta);
private:
	//int bestMove(vector<Tile*> tileList, vector<int> legalMoves, int depth, int alpha, int beta);
};

