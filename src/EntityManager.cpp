#include <EntityManager.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <MC_Config.h>
#include <TextureFactory.h>
#include <algorithm>

mc::EntityManager::~EntityManager(){}

bool mc::EntityManager::init()
{
	close();
	return true;
}

void mc::EntityManager::update()
{
	for( auto iter : deletedEntities )
		removeEntityNow( iter );

	deletedEntities.clear();

	for( auto i: entities )
	{
		for( auto j: entities )
		{
			if( i.second != j.second && i.second->isCollision( j.second ) )
			{
				i.second->hit( j.second->getType() );
				//j->second->hit( i->second->getType() );
				//printf("Hit\n");
			}
		}
	}

	/*
	for( auto i = entities.begin(); i != entities.end(); ++i )
	{
		for( auto j = i; j != entities.end(); ++j )
		{
		}
	}
	*/

	for( auto iter: entities )
		iter.second->update();
}

void mc::EntityManager::render()
{
	for( auto iter: entities )
		iter.second->render();
}

void mc::EntityManager::addEntity( mc::Entity* newEntity )
{
	// Will only add entity if it doesn't have an ID already assigned. Simplifies things.
	if( newEntity->getID() == 0 )
	{
		newEntity->setID( getNewID() );
		newEntity->setEntityManager( this );

		entities[ newEntity->getID() ] = newEntity;
	}
}

mc::Entity* mc::EntityManager::getEntity( unsigned int entityID )
{
	if( hasEntity( entityID ) )
		return entities[ entityID ];
	return nullptr;
}

bool mc::EntityManager::hasEntity( unsigned int entityID )
{
	return entities.find( entityID ) != entities.end();
}

void mc::EntityManager::removeEntity( unsigned int entityID )
{
	deletedEntities.push_back( entityID );
}

void mc::EntityManager::removeEntityNow( unsigned int entityID )
{
	if( hasEntity( entityID ) )
	{
		delete entities[ entityID ];
		entities.erase( entityID );

		freeIDs.push_back( entityID );
	}

}

unsigned int mc::EntityManager::getNewID()
{
	unsigned int id = 0;
	if( freeIDs.size() == 0 )
	{
		id = ++currIDs;
	}
	else
	{
		id = freeIDs.back();
		freeIDs.pop_back();
	}
	return id;
}

void mc::EntityManager::close()
{
	/*
	if( rq != nullptr )
		delete rq;
	*/

	for( auto iter: entities )
	{
		if( iter.second != nullptr )
			delete iter.second;
	}
	entities.clear();
}


mc::MCEM::~MCEM()
{
	close();
}

bool mc::MCEM::init()
{
	groundFloor = mc::TextureFactory::loadTexture("groundFloor", std::string(MC_RES_PATH) + "images/ground floor.png");
	if( groundFloor == nullptr )
	{
		printf("MCEM Error: Failed to load groundFloor texture.\n");
		return false;
	}

	for( int i = 1; i <= 6; ++i )
	{
		mc::City* city = new mc::City;
		if( city->init() == false )
			return false;

		city->setCity( i );

		addEntity( city );
		cities.push_back( city );
	}

	for( int i = 0; i < 3; ++i )
	{
		mc::CommandStation* cs = new mc::CommandStation;
		if( cs->init() == false )
			return false;

		cs->setCity( i );

		addEntity( cs );
		commandStations.push_back( cs );
	}

	return true;
}

void mc::MCEM::render()
{
	mc::EntityManager::render();

	for( int i = 0; i < commandStations.size(); ++i )
		commandStations[i]->render();

	SDL_RenderCopy( mc::renderer, groundFloor, nullptr, nullptr );
}

void mc::MCEM::close()
{
	for( int i = 0; i < entities.size(); ++i )
	{
		if( entities[i] != nullptr )
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	entities.clear();
	/*
	commandStations.clear();
	*/
	cities.clear();
}

void mc::MCEM::spawnEnemyMissile()
{
	mc::Missile* missile = new mc::EnemyMissile;

	// Setup the position before initialization because of order of things.
	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );
	missile->setPos( rand() % w, 0 );

	// Have to add entity to EntityManager before initalization because it needs the EntityManager pointer.
	addEntity( missile );

	if( missile->init() == false )
	{
		printf("Error: Failed to initialize EnemyMissile entity.\n");
		return;
	}
}

void mc::MCEM::fireMissile( int x, int y )
{

	mc::Missile* missile = new mc::PlayerMissile;

	// Setup the position before initialization because of order of things.
	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	// Set this up to fire from the pos of the command stations.
	int commandStation = x / (w / 3.0);

	missile->setPos( w/2.0 * commandStation - commandStations[commandStation]->getBBox()->w/2 * commandStation + commandStations[commandStation]->getBBox()->w/2, h - 50 );

	// Have to add entity to EntityManager before initalization because it needs the EntityManager pointer.
	addEntity( missile );

	if( missile->init( x, y ) == false || missile->init() == false )
	{
		printf("Error: Failed to initialize EnemyMissile entity.\n");
		return;
	}
}

void mc::MCEM::handleMouseMotion( int x, int y )
{
	// Setup the position before initialization because of order of things.
	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	// Set this up to fire from the pos of the command stations.
	int commandStation = x / (w / 3.0);

	if( mc::CommandStation* cs = dynamic_cast<mc::CommandStation*>( commandStations[ commandStation ] ) )
	{

		int dx = x - cs->getBBox()->x;
		int dy = y - cs->getBBox()->y;

		double angle = atan2( dy, dx ) * 180 / M_PI;

		cs->setAngle( angle );
	}
}

mc::Entity* mc::MCEM::getRandomStructure()
{
	// Get the target structure index;
	int structureAmount = cities.size();

	if( structureAmount < 1 )
		return nullptr;

	int target = rand() % ( structureAmount /* + commandStations.size()*/);

	// Return the structure. Depending on the value, it will either be a
	// command station or a city.
	if( target < cities.size() )
		return cities[ target ];
	return commandStations[ target - cities.size() ];
}

void mc::MCEM::destroyStructure( mc::Entity* ent )
{
	auto iter = std::find( cities.begin(), cities.end(), ent );
	if( iter != cities.end() )
	{
		cities.erase( iter );
		// Game Over if there are no cities left.
		if( cities.size() <= 0 )
		{
			mc::sm.changeScreen( GAME_OVER_SCREEN );
		}
	}
	else
	{
		iter = std::find( commandStations.begin(), commandStations.end(), ent );
		if( iter != commandStations.end() )
		{
			commandStations.erase( iter );
		}
	}
}
