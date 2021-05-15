#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHandler.h"

using namespace std;
using namespace sf;
enum Teams { UNOWNED = 0, RED = 1, YELLOW = 2 };
class Tile
{
public:
	Tile(int X, int Y, int width, int height);
	Tile(Tile* oldTile);
	~Tile();
	
	void Draw(RenderWindow* targetWindow, RenderStates state);
	void Claim(Teams newOwner);
	void setPosition(float x, float y);

	pair<int, int> getID();
	bool inBounds(Vector2f clickLoc);
	Teams getOwner();

	int getWidth();
	int getHeight();
	
private:
	pair<int, int> ID;
	Sprite* sBody;
	int width;
	int height;
	Teams currOwner = UNOWNED;
};

