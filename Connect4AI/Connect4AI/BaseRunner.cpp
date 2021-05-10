#include "BaseRunner.h"
using namespace sf;
const Time BaseRunner::PER_FRAME = seconds(1.f / 60.f);

BaseRunner::BaseRunner() : window(VideoMode(WIDTH, HEIGHT), "Connect4", Style::Close)
{
	//Summon textures
	TextureHandler::getInstance()->loadAll();
	baseState = new RenderStates;

	//Add Tiles
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

			//update(PER_FRAME); /*Might not be needed*/
		}
		render();
	}
}

void BaseRunner::render()
{
	//Use list of tiles
}

void BaseRunner::processEvents()
{
	Event event;
	if (window.pollEvent(event)) {
		switch (event.type) {
		default:
			//Process event
			break;

		case Event::Closed:
			window.close();
			break;
		}
	}
}