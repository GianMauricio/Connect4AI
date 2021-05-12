#pragma once
#include "Tile.h"
#include <unordered_map>

class AIPlayer
{
public:
	AIPlayer();
	~AIPlayer();

	int requestMove(vector<Tile*> tileList);
private:
	int getValue(int tileTarget);
};

