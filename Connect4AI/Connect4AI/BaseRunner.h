#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "TextureHandler.h"
#include "Tile.h"
#include "Board.h"

using namespace std;
using namespace sf;

class BaseRunner : private NonCopyable
{
public:
	static const Time PER_FRAME;
	static const int HEIGHT = 768,
		WIDTH = 1024;

	BaseRunner();
	void run();
	
private:
	Board* tileBoard;
	Sprite* test;
	Time ticks;
	RenderWindow window;
	RenderStates* baseState;

	void render();
	void processEvents();
};

