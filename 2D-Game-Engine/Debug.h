#ifdef DEBUG_BUILD


#ifndef DEBUG_H
#define DEBUG_H

#include "Core.h"

class Game;

class Debug
{
private:
	Game* m_game;

	bool m_debugMode;

	bool m_zoomIn;
	bool m_zoomOut;

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
public:
	Debug(Game* game);
	virtual ~Debug();

	void tick();
	void render();

	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};

#endif
#endif