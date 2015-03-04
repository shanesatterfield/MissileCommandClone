#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

namespace mc
{
	enum EntityType
	{
		ENTITY_NONE,
		ENEMY_MISSILE,
		PLAYER_MISSILE,
		ENTITY_STRUCTURE
	};

	struct Point
	{
		float x;
		float y;
	};

	struct Rect
	{
		float x;
		float y;
		float w;
		float h;
	};

	class EntityManager;
	class Entity
	{
	public:
		virtual ~Entity();
		virtual bool init();
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void close();

		virtual SDL_Rect* getPos();
		virtual SDL_Rect* getBBox();

		virtual void setPos( int x, int y );
		virtual void setBBox( SDL_Rect &rect );
		virtual void setBBox( int x, int y, int w, int h );

		EntityType getType();

		/**
		 * @brief Checks if there is a collision between this Entity and the parameterized Entity.
		 *
		 * @param b Checks if this Entity collides with that Entity.
		 * @return True if there is a collision between the two objects and False otherwise.
		 */
		virtual bool isCollision( Entity* b );
		virtual bool isCollision( SDL_Rect* otherBox );

		bool isCollidable();
		void setCollidable( bool value );

		/**
		 * @brief Call this function to signal to the entity that it has been hit by something.
		 */
		virtual void hit( EntityType type );

		// Added
		virtual void kill();

		unsigned int getID();
		void setID( unsigned int newID );

		void setEntityManager( EntityManager * );
		EntityManager *getEntityManager();

	protected:
		SDL_Rect pos;
		SDL_Rect bbox;
		EntityType type = ENTITY_NONE;
		bool collidable = true;

		unsigned int id = 0;
		EntityManager *em = nullptr;
	};

	class Missile: public Entity
	{
	public:
		Missile();
		virtual ~Missile();
		virtual bool init();
		virtual bool init( float x, float y );
		virtual bool init( Point& target );
		virtual void update();
		virtual void render();
		virtual void hit( EntityType type );

	protected:
		SDL_Texture* missileTexture;
		Point vel;
		Point currPos;
		float velMagnitude = 1;
		double angle;
	};


	class EnemyMissile: public Missile
	{
	public:
		virtual ~EnemyMissile();
		virtual bool init();
		virtual void hit( EntityType type );
	};

	class PlayerMissile: public Missile
	{
	public:
		virtual ~PlayerMissile();
		virtual bool init();
		virtual void hit( EntityType type );
	};

	class City: public Entity
	{
	public:
		virtual ~City();
		virtual bool init();
		virtual void update();
		virtual void render();

		virtual void setCity( int cityNumber );
		virtual int getCityNumber();

		virtual void hit( EntityType type );

	protected:
		SDL_Texture* cityTexture = nullptr;
		int cityNumber           = 0;
	};

	class CommandStation: public City
	{
	public:
		virtual ~CommandStation();
		virtual bool init();
		virtual void update();
		virtual void render();

		virtual void setCity( int cityNumber );
		virtual void hit( EntityType type );

		virtual void setAngle( double newAngle );

	private:
		double angle = -90.f;
		SDL_Point centerPoint;
	};
}

#endif
