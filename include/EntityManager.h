#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <Entity.h>
#include <vector>
#include <map>

namespace mc
{
	class EntityManager
	{
	public:
		virtual ~EntityManager();
		virtual bool init();
		virtual void update();
		virtual void render();
		virtual void close();

		virtual void addEntity( Entity* );
		virtual Entity* getEntity( unsigned int entityID );
		virtual bool hasEntity( unsigned int entityID );
		virtual void removeEntity( unsigned int entityID );

	protected:
		//std::vector< Entity* > entities;
		std::map<unsigned int, Entity*> entities;

	private:
		std::vector<unsigned int> deletedEntities;
		void removeEntityNow( unsigned int entityID );

		// Number of IDs that are currently in use.
		unsigned int currIDs;

		// IDs that were used, but have since been freed.
		std::vector<unsigned int> freeIDs;

		// Starts at 1, so there will never be an id of 0.
		unsigned int getNewID();
	};

	// Missile Command Entity Manager which does some game logic.
	class MCEM: public EntityManager
	{
	public:
		virtual ~MCEM();
		virtual bool init();
		//virtual void update();
		virtual void render();
		virtual void close();

		// Spawns an enemy missile entity randomly.
		virtual void spawnEnemyMissile();

		// Player fires a missile. Logic is handled here.
		virtual void fireMissile( int x, int y );

		// Use this to get a random city or command station.
		// This is useful for the missiles targetting system.
		virtual Entity* getRandomStructure();

		virtual void destroyStructure( Entity* ent );

		virtual void handleMouseMotion( int x, int y );

	protected:
		std::vector< Entity* > cities;
		std::vector< Entity* > commandStations;

		SDL_Texture* groundFloor = nullptr;
	};
}

#endif
