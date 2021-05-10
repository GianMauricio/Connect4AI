#include "TextureHandler.h"
#include <iostream>

TextureHandler* TextureHandler::sharedInstance = NULL;
TextureHandler* TextureHandler::getInstance() {
	if (sharedInstance == NULL) {
		sharedInstance = new TextureHandler();
	}

	return sharedInstance;
}

void TextureHandler::loadAll() {
	vector <Texture*> Tiles;
	Textures.emplace("Tiles", Tiles);

	/*
	 * 0 - Unclaimed
	 * 1 - Team 1
	 * 2 - Team 2
	 */

	//Load Game Objects
	Texture* Unclaimed = new Texture();
	if (!Unclaimed->loadFromFile("Textures\\Unassigned.png")) cout << "Failed to Load blank tile";
	Textures["Tiles"].push_back(Unclaimed);

	Texture* Team1 = new Texture();
	if (!Team1->loadFromFile("Textures\\T1.png")) cout << "Failed to Load yellow tile";
	Textures["Tiles"].push_back(Team1);

	Texture* Team2 = new Texture();
	if (!Team2->loadFromFile("Textures\\T2.png")) cout << "Failed to Load red tile";
	Textures["Tiles"].push_back(Team2);

	//Load fonts
	Font* font = new Font();
	if (!font->loadFromFile("Fonts\\Sansation.ttf")) cout << "Failed to load font" << endl;
	Fonts.emplace("Default", font);
}

void TextureHandler::addTexture(Texture* newTexture, string objName) {
	if (Textures.count(objName) > 0) {
		Textures.at(objName).push_back(newTexture);
	}

	else {
		vector<Texture*> temp;
		temp.push_back(newTexture);
		Textures.emplace(objName, temp);
	}
}

Texture* TextureHandler::getTexture(string objName, int textureNum) {
	if (Textures.count(objName) > 0) {
		//cout << "Texture requested " << objName << endl;
		return Textures[objName].at(textureNum);
	}

	else {
		cout << "Object not found" << endl;
		return nullptr;
	}
}

Font* TextureHandler::getFont(string fontName) {
	cout << fontName << " Requested" << endl;
	return Fonts[fontName];
}
