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
