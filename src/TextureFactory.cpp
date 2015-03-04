#include <TextureFactory.h>

std::map< std::string, SDL_Texture*> mc::TextureFactory::textureMap;

SDL_Texture* mc::TextureFactory::loadTexture( std::string name, std::string path )
{
	if( textureMap.find( name ) != textureMap.end() )
		return textureMap[ name ];

	SDL_Surface* surf = IMG_Load( path.c_str() );
	if( surf == nullptr )
	{
		printf("TextureFactory Error: Failed to load image at path %s\n", path.c_str());
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface( mc::renderer, surf );
	if( tex == nullptr )
	{
		printf("TextureFactory Error: Failed to create texture from SDL_Surface* for image %s\n", path.c_str());
		SDL_FreeSurface( surf );
		return nullptr;
	}

	SDL_FreeSurface( surf );
	textureMap[ name ] = tex;
	return textureMap[ name ];
}

bool mc::TextureFactory::isLoadedTexture( std::string name )
{
	return textureMap.find( name ) != textureMap.end();
}

SDL_Texture* mc::TextureFactory::getTexture( std::string name )
{
	if( textureMap.find( name ) == textureMap.end() )
		return nullptr;
	return textureMap[ name ];
}

void mc::TextureFactory::close()
{
	for( auto &iter: textureMap )
	{
		if( iter.second != nullptr )
		{
			// printf("Deallocating Texture: %s\n", iter.first.c_str());
			SDL_DestroyTexture( iter.second );
			iter.second = nullptr;
			textureMap[ iter.first ] = nullptr;
		}
	}
}
