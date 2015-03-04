#include <Screen.h>
#include <ScreenManager.h>
#include <MC_Config.h>

#include <iostream>

#include <SDL2/SDL_image.h>

#include <TextureFactory.h>

mc::Screen::~Screen(){}
void mc::Screen::setSM( mc::ScreenManager* sm )
{
	this->sm = sm;
}

void mc::Screen::reset(){}

mc::TitleScreen::~TitleScreen()
{
	close();
}

bool mc::TitleScreen::init()
{
	title = mc::TextureFactory::loadTexture("titleImage", std::string(MC_RES_PATH) + "images/title_bg.png");
	if( title == nullptr )
	{
		printf("Error: Failed to load title screen background image.\n");
		std::cout << std::string(MC_RES_PATH) + "images/title_bg.png" << std::endl;
		return false;
	}
	return true;
}

void mc::TitleScreen::update(){}

void mc::TitleScreen::render()
{
	SDL_RenderCopy( mc::renderer, title, nullptr, nullptr );
}

void mc::TitleScreen::close(){}

void mc::TitleScreen::handleEvent( SDL_Event &event )
{
	if( event.type == SDL_KEYDOWN &&
		event.key.keysym.sym == SDLK_RETURN ||
		event.type == SDL_MOUSEBUTTONUP &&
		event.button.button == SDL_BUTTON_LEFT
	 )
	{
		this->sm->changeScreen( GAME_SCREEN );
	}
}

/////////////////
// Game Screen //
/////////////////

mc::GameScreen::~GameScreen()
{
	close();
}

bool mc::GameScreen::init()
{
	if( em.init() == false )
		return false;

	spawnDelayFrames = 0;
	spawnDelayTotal = spawnDelayTotalInitial;

	return true;
}

void mc::GameScreen::update()
{
	if( spawnDelayFrames-- <= 0 )
	{
		// spawnDelayFrames = rand() % 100 + 100;

		if( spawnDelayTotal > 15 )
			spawnDelayTotal -= 5;

		spawnDelayFrames = spawnDelayTotal;

		for( int i = 0; i < spawnPerWave; ++i )
			em.spawnEnemyMissile();
	}
	em.update();
}

void mc::GameScreen::render()
{
	em.render();
}

void mc::GameScreen::close()
{
	em.close();
}

void mc::GameScreen::reset()
{
	this->close();
	this->init();
}

void mc::GameScreen::handleEvent( SDL_Event &event )
{
	if( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT )
	{
		em.fireMissile( event.button.x, event.button.y );
	}
	else if( event.type == SDL_MOUSEMOTION )
	{
		em.handleMouseMotion( event.motion.x, event.motion.y );
	}
}

////////////////////
// GameOverScreen //
////////////////////

mc::GameOverScreen::~GameOverScreen(){}

bool mc::GameOverScreen::init()
{
	gameOverText = mc::TextureFactory::loadTexture("gameOverText", std::string(MC_RES_PATH) + "images/GAME OVER.png");
	if( gameOverText == nullptr )
	{
		printf("GameOverScreen Error: Failed to load gameOverText texture.\n");
		return false;
	}

	finalExplosion = mc::TextureFactory::loadTexture("finalExplosion", std::string(MC_RES_PATH) + "images/end-explosion-fixed.png");
	if( finalExplosion == nullptr )
	{
		printf("GameOverScreen Error: Failed to load finalExplosion.\n");
		return false;
	}

	frameCounter = 0;

	framePosition.x = 0;
	framePosition.y = 0;
	framePosition.w = 192;
	framePosition.h = 268;

	screenPosition.x = mc::SCREEN_WIDTH /2.0 - framePosition.w/2.0;
	screenPosition.y = mc::SCREEN_HEIGHT/2.0 - framePosition.h/2.0;
	screenPosition.w = framePosition.w;
	screenPosition.h = framePosition.h;

	return true;
}

void mc::GameOverScreen::update()
{
	frameCounter += 1;
	if( explosionFrame < finalFrame && frameCounter > 1 )
	{
		explosionFrame += 1;
		framePosition.x = framePosition.w * explosionFrame;
		frameCounter = 0;
	}
}

void mc::GameOverScreen::render()
{
	SDL_RenderCopy( mc::renderer, this->gameOverText, nullptr, nullptr );
	SDL_RenderCopy( mc::renderer, this->finalExplosion, &framePosition, &screenPosition );
}

void mc::GameOverScreen::handleEvent( SDL_Event &event )
{
	if( event.type == SDL_KEYDOWN &&
		event.key.keysym.sym == SDLK_RETURN
	)
	{
		this->sm->changeScreen( TITLE_SCREEN );
	}
}

void mc::GameOverScreen::reset()
{
	frameCounter = 0;
	explosionFrame = -1;
}
