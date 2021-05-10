#include "BaseRunner.h"
using namespace sf;
const Time BaseRunner::PER_FRAME = seconds(1.f / 60.f);

Tile* test;
Sprite* testSprite;
BaseRunner::BaseRunner() : window(VideoMode(WIDTH, HEIGHT), "Connect4", Style::Close)
{
	//Summon textures
	TextureHandler::getInstance()->loadAll();
	baseState = new RenderStates;

	//Add TestTile
	test = new Tile(0, 204, 192);
	test->setPosition(WIDTH / 2, HEIGHT / 2);

	testSprite = new Sprite();
	testSprite->setTexture(*TextureHandler::getInstance()->getTexture("Tiles", 0));
}

void BaseRunner::run()
{
	Clock clock;
	Time FixedUpdate = Time::Zero;

	while (window.isOpen()) {
		Time elapsedTime = clock.restart();
		FixedUpdate += elapsedTime;

		while (FixedUpdate > PER_FRAME) {
			FixedUpdate -= PER_FRAME;

			processEvents();
		}
		render();
	}
}

void BaseRunner::render()
{
	test->Draw(&window, *baseState);
}

void BaseRunner::processEvents()
{
	Event event;
	if (window.pollEvent(event)) {
		switch (event.type) {
		default:
			//Process click event
			if(event.type == Event::MouseButtonPressed)
			{
				if (test->inBounds(Mouse::getPosition()))
				{
					test->Claim(RED);
				}
			}
			break;

		case Event::Closed:
			window.close();
			break;
		}
	}
}