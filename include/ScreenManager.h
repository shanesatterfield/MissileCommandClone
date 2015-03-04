#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Screen.h>
#include <vector>

namespace mc
{
	enum ScreenName {
		TITLE_SCREEN     = 0,
		GAME_SCREEN      = 1,
		GAME_OVER_SCREEN = 2
	};

	class ScreenManager
	{
	public:
		ScreenManager();
		virtual ~ScreenManager();
		virtual bool init();
		virtual void update();
		virtual void render();

		virtual void changeScreen( ScreenName name );
		virtual void close();

		void handleEvent( SDL_Event &event );

	protected:
		std::vector<Screen*> screens;
		ScreenName currentScreen = TITLE_SCREEN;
	};
}

#endif
