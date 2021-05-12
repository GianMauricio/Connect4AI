#include "Tile.h"
#include <iostream>

Tile::Tile(int X, int Y, int width, int height)
{
	this->ID = {X, Y};
	this->sBody = new Sprite();
	
	//Set unowned to current texture
	this->sBody->setTexture(*TextureHandler::getInstance()->getTexture("Tiles", 0));
	this->sBody->setTextureRect(IntRect(0, 0, width, height));
}

Tile::~Tile()
{
	
}

void Tile::Draw(RenderWindow* targetWindow, RenderStates state)
{
	if (this->sBody != nullptr) {
		targetWindow->draw(*sBody, state); /*Draw parent*/
		state.transform = this->sBody->getTransform() * state.transform; /*Apply current transform values*/
	}
}

bool Tile::inBounds(Vector2f clickLoc)
{
	
	FloatRect bounds = sBody->getGlobalBounds();

	if(bounds.contains(clickLoc))
	{
		//cout << this->sBody->getPosition().x;
		//cout << this->sBody->getPosition().y;
		cout << endl;

		return true;
	}
	return false;
}

Teams Tile::getOwner()
{
	return currOwner;
}

void Tile::Claim(Teams newOwner)
{
	this->currOwner = newOwner;

	//cout << this->sBody->getPosition().x;
	//cout << this->sBody->getPosition().y;
	//cout << endl;
	
	if(currOwner == YELLOW)
	{
		//Set yellow to current texture
		Texture* newText = TextureHandler::getInstance()->getTexture("Tiles", 1);
		this->sBody->setTexture(*newText);
	}

	else if(currOwner == RED)
	{
		//Set red to current texture
		Texture* newText = TextureHandler::getInstance()->getTexture("Tiles", 2);
		this->sBody->setTexture(*newText);
	}

	else
	{
		//Set unowned to current texture
		Texture* newText = TextureHandler::getInstance()->getTexture("Tiles", 0);
		this->sBody->setTexture(*newText);
	}
}

void Tile::setPosition(float x, float y)
{
	sBody->setPosition(Vector2f(x, y));
}

pair<int, int> Tile::getID()
{
	return this->ID;
}
