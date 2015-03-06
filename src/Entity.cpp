#include <Entity.h>
#include <MC_Config.h>
#include <TextureFactory.h>
#include <EntityManager.h>

SDL_Rect windowBBox = { 0, 0, mc::SCREEN_WIDTH, mc::SCREEN_HEIGHT };

mc::Entity::~Entity(){}

bool mc::Entity::init()
{
	return true;
}

void mc::Entity::close(){}

void mc::Entity::setPos( int x, int y )
{
	this->pos.x = x;
	this->pos.y = y;
}

SDL_Rect* mc::Entity::getBBox()
{
	this->bbox.x = this->pos.x;
	this->bbox.y = this->pos.y;

	return &this->bbox;
}

SDL_Rect* mc::Entity::getPos()
{
	return &this->pos;
}

void mc::Entity::setBBox( SDL_Rect &rect )
{
	this->bbox.x = rect.x;
	this->bbox.y = rect.y;
	this->bbox.w = rect.w;
	this->bbox.h = rect.h;
}

void mc::Entity::setBBox( int x, int y, int w, int h )
{
	this->bbox.x = x;
	this->bbox.y = y;
	this->bbox.w = w;
	this->bbox.h = h;
}

mc::EntityType mc::Entity::getType()
{
	return this->type;
}

bool mc::Entity::isCollision( mc::Entity* b )
{
	SDL_Rect *otherBox = b->getBBox();
	return b->isCollidable() && isCollision( otherBox );
}

bool mc::Entity::isCollision( SDL_Rect* otherBox )
{
	return !(
		this->bbox.x + this->bbox.w < otherBox->x ||
		this->bbox.y + this->bbox.h < otherBox->y ||
		this->bbox.x > otherBox->x + otherBox->w  ||
		this->bbox.y > otherBox->y + otherBox->h
	);
}

bool mc::Entity::isCollidable()
{
	return this->collidable;
}

void mc::Entity::setCollidable( bool value )
{
	this->collidable = value;
}

void mc::Entity::hit( EntityType type ){}


// Added
void mc::Entity::kill()
{
	if( this->em != nullptr )
		this->em->removeEntity( getID() );
}

unsigned int mc::Entity::getID()
{
	return id;
}

void mc::Entity::setID( unsigned int newID )
{
	id = newID;
}


void mc::Entity::setEntityManager( mc::EntityManager *entityManager )
{
	em = entityManager;
}

mc::EntityManager *mc::Entity::getEntityManager()
{
	return em;
}

/////////////
// Missile //
/////////////

mc::Missile::Missile()
{
	pos.w = 50;
	pos.h = 20;

	bbox.w = 50;
	bbox.h = 50;
}

mc::Missile::~Missile()
{
	close();
}

bool mc::Missile::init()
{
	return mc::Entity::init();
}

bool mc::Missile::init( mc::Point &target )
{
	return init( target.x, target.y );
}

bool mc::Missile::init( float x, float y )
{
	missileTexture = mc::TextureFactory::loadTexture("enemyMissile", std::string(MC_RES_PATH) + "images/missile_horizontal.png");
	if( missileTexture == nullptr )
		return false;

	pos.w = 15;
	pos.h = 5;

	bbox.w = 15;
	bbox.h = 15;

	vel.x = x - (pos.x + pos.w / 2.0);
	vel.y = y - (pos.y + pos.h / 2.0);


	double magnitude = sqrt( pow(vel.x, 2) + pow(vel.y, 2) );

	vel.x /= magnitude;
	vel.y /= magnitude;

	this->angle = atan2( vel.y, vel.x ) * 180 / M_PI;

	currPos.x = pos.x;
	currPos.y = pos.y;
	return true;
}

void mc::Missile::update()
{
	// currPos has floats and pos is an SDL_Rect which has ints. It'd lose precision otherwise.
	currPos.x = currPos.x + vel.x * velMagnitude;
	currPos.y = currPos.y + vel.y * velMagnitude;

	pos.x = currPos.x;
	pos.y = currPos.y;

	if( isCollision( &windowBBox ) == false )
	{
		this->kill();
	}
}

void mc::Missile::render()
{
	SDL_RenderCopyEx( mc::renderer, missileTexture, nullptr, &pos, this->angle, nullptr, SDL_FLIP_NONE );
}

void mc::Missile::hit( mc::EntityType type )
{
	if( type != ENTITY_NONE )
	{
		this->kill();
	}
}


//////////////////
// EnemyMissile //
//////////////////

mc::EnemyMissile::~EnemyMissile()
{
	close();
}

bool mc::EnemyMissile::init()
{
	this->type = ENEMY_MISSILE;

	missileTexture = mc::TextureFactory::loadTexture("enemyMissile", std::string(MC_RES_PATH) + "images/missile_horizontal.png");
	if( missileTexture == nullptr )
		return false;

	mc::Entity* structure = nullptr;
	if( mc::MCEM* tempEM = dynamic_cast<mc::MCEM*>(this->getEntityManager()) )
	{
		structure = tempEM->getRandomStructure();
	}

	if( structure == nullptr )
	{
		printf("Missile Error: Failed to get random structure structure.\n");
		return false;
	}

	SDL_Rect* structurePos = structure->getPos();

	vel.x = (structurePos->x + (rand() % structurePos->w) );
	vel.y = (structurePos->y + (rand() % structurePos->h) );

	if( mc::Missile::init( vel.x, vel.y ) == false )
		return false;

	return true;
}

void mc::EnemyMissile::hit( mc::EntityType type )
{
	if( type != ENTITY_NONE && type != ENEMY_MISSILE )
	{
		this->kill();
	}
}


///////////////////
// PlayerMissile //
///////////////////

mc::PlayerMissile::~PlayerMissile()
{
	close();
}

bool mc::PlayerMissile::init()
{
	this->type = PLAYER_MISSILE;
	velMagnitude = 7;

	missileTexture = mc::TextureFactory::loadTexture("playerMissile", std::string(MC_RES_PATH) + "images/missile_horizontal.png");
	if( missileTexture == nullptr )
		return false;

	SDL_SetTextureColorMod( missileTexture, 255, 0, 0 );

	return mc::Missile::init();
}

void mc::PlayerMissile::hit( mc::EntityType type )
{
	if( type == ENEMY_MISSILE || type == PLAYER_MISSILE )
	{
		kill();
	}
}


//////////
// City //
//////////

mc::City::~City()
{
	close();
}

bool mc::City::init()
{
	this->type = ENTITY_STRUCTURE;

	cityTexture = mc::TextureFactory::loadTexture("city", std::string(MC_RES_PATH) + "images/city 1.png");
	if( cityTexture == nullptr )
		return false;

	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	pos.x = 300;
	pos.y = h - 200;
	pos.w = 100;
	pos.h = 100;

	bbox.w = w/9 * 0.9f;
	bbox.h = 200;
	bbox.x = 0;
	bbox.y = h - bbox.h;
	return true;
}

void mc::City::update(){}

void mc::City::render()
{
	SDL_RenderCopy( mc::renderer, cityTexture, nullptr, nullptr );
}

void mc::City::setCity( int cityNumber )
{
	this->cityNumber = cityNumber;
	cityTexture = mc::TextureFactory::loadTexture("city" + std::to_string(cityNumber), std::string(MC_RES_PATH) + "images/city " + std::to_string(cityNumber) + ".png" );
	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	int change = cityNumber + cityNumber / 4;
	change = w/9 * change + w/9 * 0.05f * change;

	bbox.x = change;
	pos.x = change;
	// printf("City %d: %d %d %d %d\n", this->cityNumber, bbox.x, bbox.y, bbox.w, bbox.h);
}

int mc::City::getCityNumber()
{
	return this->cityNumber;
}

void mc::City::hit( mc::EntityType type )
{
	if( type == ENEMY_MISSILE )
	{
		// printf("City %d Destroyed\n", this->getCityNumber());
		// kill();
		this->setCollidable( false );

		if( mc::MCEM* tempEM = dynamic_cast<mc::MCEM*>(this->getEntityManager()) )
		{
			tempEM->destroyStructure( this );
		}

		cityTexture = mc::TextureFactory::loadTexture("ruble_0" + std::to_string(cityNumber), std::string(MC_RES_PATH) + "images/ruble_0" + std::to_string(cityNumber) + ".png" );
	}
}



////////////////////
// CommandStation //
////////////////////

mc::CommandStation::~CommandStation()
{
	close();
}

bool mc::CommandStation::init()
{
	this->type = ENTITY_STRUCTURE;

	cityTexture = mc::TextureFactory::loadTexture("commandStation", std::string(MC_RES_PATH) + "images/ground base artillery.png");
	if( cityTexture == nullptr )
		return false;

	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	pos.w = 134;
	pos.h = 100;
	pos.x = 300;
	pos.y = h - pos.h;

	bbox.w = pos.h;
	bbox.h = bbox.w;
	bbox.x = 0;
	bbox.y = h - bbox.h;


	return true;
}

void mc::CommandStation::setCity( int cityNumber )
{
	this->cityNumber = cityNumber;

	int w, h;
	SDL_GetWindowSize( mc::window, &w, &h );

	int change = w/2;

	bbox.x = change * cityNumber - bbox.w/2 * cityNumber;

	pos.x = bbox.x;

	centerPoint.x = bbox.h/2;
	centerPoint.y = pos.h - bbox.h/2;

	/*
	printf("Command Station BBox %d: %d %d %d %d\n", this->cityNumber, bbox.x, bbox.y, bbox.w, bbox.h);
	printf("Command Station  Pos %d: %d %d %d %d\n", this->cityNumber,  pos.x,  pos.y,  pos.w,  pos.h);
	printf("Command Station   CP %d: %d %d\n", this->cityNumber,  centerPoint.x,  centerPoint.y);
	*/
}

void mc::CommandStation::setAngle( double newAngle )
{
	this->angle = newAngle;
}

void mc::CommandStation::update(){}

void mc::CommandStation::render()
{
	SDL_RenderCopyEx( mc::renderer, cityTexture, nullptr, &pos, angle, &centerPoint, SDL_FLIP_NONE );
}

void mc::CommandStation::hit( mc::EntityType type )
{
	if( type == ENEMY_MISSILE )
	{
		// printf("Structure %d Destroyed\n", this->getCityNumber());
		// kill();
		this->setCollidable( false );

		/*
		if( mc::MCEM* tempEM = dynamic_cast<mc::MCEM*>(this->getEntityManager()) )
		{
			tempEM->destroyStructure( this );
		}
		*/
	}
}
