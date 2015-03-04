cmake_minimum_required( VERSION 3.0 )
project( MissileCommand )

add_compile_options( --std=c++11 )
add_compile_options( -g )

set( MC_RES_PATH "${PROJECT_SOURCE_DIR}/res/" CACHE STRING "Path to the Missile Command res directory." )
configure_file( "${PROJECT_SOURCE_DIR}/include/MC_Config.h.in" "${PROJECT_SOURCE_DIR}/include/MC_Config.h" )


# Find libraries
include( FindPkgConfig )
pkg_search_module( SDL2 REQUIRED sdl2 )
pkg_search_module( SDL2IMAGE REQUIRED SDL2_image )
pkg_search_module( SDL2TTF REQUIRED SDL2_ttf )

# Include directories
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIRS}
)

# Set libraries to link
set( MC_LIBRARIES
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	${SDL2TTF_LIBRARIES}
)

# Set sources
set( SOURCES
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/MC_Config.cpp
	${PROJECT_SOURCE_DIR}/src/ScreenManager.cpp
	${PROJECT_SOURCE_DIR}/src/Screen.cpp
	${PROJECT_SOURCE_DIR}/src/Entity.cpp
	${PROJECT_SOURCE_DIR}/src/TextureFactory.cpp
	${PROJECT_SOURCE_DIR}/src/EntityManager.cpp
)


# Compile
add_executable( MissileCommand ${SOURCES} )

# Link libraries
target_link_libraries( MissileCommand ${MC_LIBRARIES} )