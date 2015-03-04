#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <Entity.h>
#include <EntityManager.h>

namespace mc
{
	class ScreenManager;
	class Screen
	{
	public:
		virtual ~Screen();
		virtual bool init() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void handleEvent( SDL_Event &event ) = 0;
		virtual void reset();
		void setSM( ScreenManager* sm );

	protected:
		ScreenManager* sm = nullptr;
	};

	class TitleScreen: public Screen
	{
	public:
		virtual ~TitleScreen();
		virtual bool init();
		virtual void update();
		virtual void render();
		virtual void close();
		virtual void handleEvent( SDL_Event &event );

	private:
		SDL_Texture* title = nullptr;
	};

	class GameScreen: public Screen
	{
	public:
		virtual ~GameScreen();
		virtual bool init();
		virtual void update();
		virtual void render();
		virtual void close();
		virtual void reset();
		virtual void handleEvent( SDL_Event &event );

	private:
		MCEM em;
		int spawnDelayFrames 		= 0;
		int spawnDelayTotal  		= 200;
		int spawnDelayTotalInitial  = 200;
		int spawnPerWave 	 		= 4;
	};

	class GameOverScreen: public Screen
	{
	public:
		virtual ~GameOverScreen();
		virtual bool init();
		virtual void update();
		virtual void render();
		virtual void reset();
		virtual void handleEvent( SDL_Event &event );

	private:
		SDL_Texture* gameOverText   = nullptr;
		SDL_Texture* finalExplosion = nullptr;
		int explosionFrame = -1;
		int frameCounter   = 0;
		int finalFrame 	   = 11;
		SDL_Rect framePosition;
		SDL_Rect screenPosition;
	};
}

#endif
