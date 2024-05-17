cmake_minimum_required( VERSION 3.1 )

project( Randey )

set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" FORCE)

add_subdirectory( ext/glfw )

include_directories( 
	ext/glm/
	ext/glfw/include/
	ext/glad/include/
	ext/
	src/
	inc/
	)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( SRC
	 ext/glad/src/glad.c
	 src/main.cpp
	 src/core/renderer.cpp
	 src/core/shader.cpp
	 src/core/mesh.cpp
	 src/core/texture.cpp
	)

add_executable( Randey ${SRC})
target_link_libraries( Randey 
						glfw 
					)
target_compile_options( Randey PRIVATE )
set_property(TARGET Randey PROPERTY CXX_STANDARD 17)