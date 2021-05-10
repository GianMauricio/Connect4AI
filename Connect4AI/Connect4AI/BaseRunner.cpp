#include "BaseRunner.h"
using namespace sf;
const Time BaseRunner::PER_FRAME = seconds(1.f / 60.f);

BaseRunner::BaseRunner() : window(VideoMode(WIDTH, HEIGHT), "Connect4", Style::Close)
{
	//Summon textures
	TextureHandler::getInstance()->loadAll();
	baseState = new RenderStates;

	tileBoard = new Board(5, 4);

	//MakeTestSprite
	test = new Sprite();
	test->setTexture(*TextureHandler::getInstance()->getTexture("Tiles", 0));
	test->setPosition(WIDTH / 2, HEIGHT / 2);
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
	/*This is what fixes the draw bug*/
	window.clear();

	tileBoard->draw(&window, *baseState);

	/*This too*/
	window.display();
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
				//To pixel coords
				Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				tileBoard->tryPlace(mousePos);
			}
			break;

		case Event::Closed:
			window.close();
			break;
		}
	}
}