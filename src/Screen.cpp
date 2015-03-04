#include <Screen.h>
#include <ScreenManager.h>
#include <MC_Config.h>

#include <iostream>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <TextureFactory.h>

mc::Screen::~Screen(){}
void mc::Screen::setSM( mc::ScreenManager* sm )
{
	this->sm = sm;
}

mc::TitleScreen::~TitleScreen()
{
	close();
}

bool mc::TitleScreen::init()
{
	/*
	SDL_Color color = {255, 255, 255};
	std::string fontPath = std::string(MC_RES_PATH) + "fonts/ZOMBIFIED.ttf";
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 25);
	if( font == nullptr )
	{
		printf("Error: Could not load font at %s\n", fontPath.c_str());
		return false;
	}

	SDL_Surface* surf = TTF_RenderText_Solid(font, "Missile Command", color);
	if( surf == nullptr )
	{
		TTF_CloseFont( font );
		return false;
	}

	title = SDL_CreateTextureFromSurface( mc::renderer, surf );
	if( title == nullptr )
	{
		TTF_CloseFont( font );
		SDL_FreeSurface( surf );
		return false;
	}

	TTF_CloseFont( font );
	SDL_FreeSurface( surf );
	*/
	title = mc::TextureFactory::loadTexture("titleImage", std::string(MC_RES_PATH) + "images/title_bg.png");
	if( title == nullptr )
	{
		printf("Error: Failed to load title screen background image.\n");
		std::cout << std::string(MC_RES_PATH) + "images/title_bg.png" << std::endl;
		return false;
	}
	return true;
}

void mc::TitleScreen::update()
{

}

void mc::TitleScreen::render()
{
	//SDL_Rect dest = {100, 100, 500, 150};
	//SDL_RenderCopy( mc::renderer, title, nullptr, &dest );
	SDL_RenderCopy( mc::renderer, title, nullptr, nullptr );
}

void mc::TitleScreen::close()
{
	/*
	if( title != nullptr )
	{
		SDL_DestroyTexture( title );
		title = nullptr;
	}
	*/
}

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
	/*
	SDL_Surface* surf = IMG_Load((std::string(MC_RES_PATH) + "images/missile.png").c_str());
	enemyMissile = SDL_CreateTextureFromSurface( mc::renderer, surf );
	if( enemyMissile == nullptr )
	{
		printf("Error: Could not load enemy missile texture.\n");
		SDL_FreeSurface( surf );
		return false;
	}

	SDL_FreeSurface( surf );
	*/

	/*
	enemyMissile = new mc::EnemyMissile;
	if( enemyMissile->init() == false )
		return false;
	*/

	if( em.init() == false )
		return false;

	for( int i = 0; i < 20; ++i )
		em.spawnEnemyMissile();
	/*
	*/
	return true;
}

void mc::GameScreen::update()
{
	//enemyMissile->update();
	//em.spawnEnemyMissile();

	if( spawnDelayFrames-- <= 0 )
	{
		// spawnDelayFrames = rand() % 100 + 100;

		if( spawnDelayTotal > 0 )
			spawnDelayTotal -= 1;

		spawnDelayFrames = spawnDelayTotal;

		for( int i = 0; i < 3; ++i )
			em.spawnEnemyMissile();
	}
	em.update();
}

void mc::GameScreen::render()
{
	//SDL_RenderCopy( mc::renderer, enemyMissile, 0, 0 );
	//enemyMissile->render();
	em.render();
}

void mc::GameScreen::close()
{
	/*
	if( enemyMissile != nullptr )
	{
		//SDL_DestroyTexture( enemyMissile );
		delete enemyMissile;
		enemyMissile = nullptr;
	}
	*/
	em.close();
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

	// finalExplosion = mc::TextureFactory::loadTexture("finalExplosion", std::string(MC_RES_PATH) + "images/realigned_explosion_01.png");
	finalExplosion = mc::TextureFactory::loadTexture("finalExplosion", std::string(MC_RES_PATH) + "images/end-explosion-fixed.png");
	if( finalExplosion == nullptr )
	{
		printf("GameOverScreen Error: Failed to load finalExplosion.\n");
		return false;
	}

	// 192
	framePosition.x = 0;
	framePosition.y = 0;
	framePosition.w = 192;
	framePosition.h = 268;

	screenPosition.x = mc::SCREEN_WIDTH /2.0 - framePosition.w/2.0;
	screenPosition.y = mc::SCREEN_HEIGHT/2.0 - framePosition.h/2.0;
	screenPosition.w = framePosition.w;
	screenPosition.h = framePosition.h;
	/*
	*/

	return true;
}

void mc::GameOverScreen::update()
{
	frameCounter += 1;
	if( explosionFrame < finalFrame && frameCounter > 1 )
	{
		explosionFrame += 1;
		framePosition.x = framePosition.w * explosionFrame;
		printf("Frame Position X: %d\n", framePosition.x);
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

}
