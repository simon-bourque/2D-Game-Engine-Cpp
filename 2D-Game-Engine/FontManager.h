#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "Core.h"

#include "ResourceManager.h"
#include "Font.h"

class Game;

class FontManager : public ResourceManager<Font> {
public:
	FontManager();
	virtual ~FontManager();

	Font* createFont(const string& name, Game* game);
};

#endif