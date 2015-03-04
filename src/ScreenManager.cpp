#include <ScreenManager.h>

mc::ScreenManager::ScreenManager()
{
	screens.resize( 3 );
}

mc::ScreenManager::~ScreenManager()
{
	close();
}

bool mc::ScreenManager::init()
{
	screens[ TITLE_SCREEN ]     = new mc::TitleScreen;
	screens[ GAME_SCREEN ]      = new mc::GameScreen;
	screens[ GAME_OVER_SCREEN ] = new mc::GameOverScreen;

	bool result = true;
	for( int i = 0; i < screens.size(); ++i )
	{
		if( screens[i]->init() == false )
		{
			result = false;
			break;
		}
		screens[i]->setSM( this );
	}

	return result;
}

void mc::ScreenManager::update()
{
	if( screens[ currentScreen ] != nullptr )
		screens[ currentScreen ]->update();
}

void mc::ScreenManager::render()
{
	if( screens[ currentScreen ] != nullptr )
		screens[ currentScreen ]->render();
}

void mc::ScreenManager::changeScreen( ScreenName name )
{
	currentScreen = name;
	screens[ currentScreen ]->reset();
}

void mc::ScreenManager::close()
{
	for( int i = 0; i < screens.size(); ++i )
	{
		if( screens[i] != nullptr )
		{
			delete screens[i];
			screens[i] = nullptr;
		}
	}

	currentScreen = TITLE_SCREEN;
}

void mc::ScreenManager::handleEvent( SDL_Event &event )
{
	screens[ currentScreen ]->handleEvent( event );
}
