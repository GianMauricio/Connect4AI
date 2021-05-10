#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHandler.h"

using namespace sf;
enum Teams { UNOWNED = 0, RED = 1, YELLOW = 2 };
class Tile
{
public:
	Tile(int ID, int width, int height);
	~Tile();
	void Draw(RenderWindow* targetWindow, RenderStates state);
	void Claim(Teams newOwner);
	void setPosition(float x, float y);

	bool inBounds(Vector2i clickLoc);
	
private:
	int ID;
	Sprite* sBody;
	Teams currOwner = UNOWNED;
};

