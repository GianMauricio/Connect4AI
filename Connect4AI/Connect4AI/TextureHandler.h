#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

class TextureHandler {
public:
	void addTexture(Texture* newTexture, string objName);
	Texture* getTexture(string objName, int textureNum);
	Font* getFont(string fontName);
	static TextureHandler* getInstance();
	void testFunc();
	void loadAll();

private:
	TextureHandler() {};
	TextureHandler(TextureHandler const&) {};
	TextureHandler& operator = (TextureHandler const&) {};
	static TextureHandler* sharedInstance;

	unordered_map<string, vector<Texture*>> Textures;
	unordered_map<string, Font*> Fonts;
};

