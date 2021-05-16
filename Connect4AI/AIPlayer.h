#pragma once
#include "Tile.h"
#include <limits>
#include <unordered_map>
#include <map>
#include <iostream>

typedef vector<Tile*> tileRow;
typedef vector<tileRow*> BoardState;

class AIPlayer
{
public:
	AIPlayer(int maxDepth);
	~AIPlayer();

	pair< int, int>  requestMove(vector<tileRow*> TileGrid, int depth);
private:
	int maxDepth;
	int bestMoveMin(vector<tileRow*> TileGrid, int depth, int alpha, int beta, Teams player, Teams opponent);
	int bestMoveMax(vector<tileRow*> TileGrid, int depth, int alpha, int beta, Teams player, Teams opponent);

	bool boardDone(BoardState state);
	Teams isWon(BoardState state);
	bool isTied(BoardState state);

	int boardEval(BoardState state, Teams currPlayer); /*We don't do '/' here*/
	int countSequence(BoardState currBoard, Teams currPlayer, int reqSequence);
};
