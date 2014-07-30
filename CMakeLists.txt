set(APP_NAME CrossPlatform)
project (${APP_NAME})

cmake_minimum_required(VERSION 2.6)

if(EMSCRIPTEN)
else(EMSCRIPTEN)
# include(FindGLEW)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
endif()

OPTION(EMSCRIPTEN "build for emscripten" OFF)

option(DEBUG_MODE "" ON)

if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE DEBUG)
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

# if (MSVC)
# #	set(CMAKE_C_FLAGS_DEBUG "-DCOCOS2D_DEBUG=1")
# #	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

# 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

# elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
# 	set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
# 	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

# 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# endif()

# fix entityx CROSS_COMPILING
if(EMSCRIPTEN)
	SET(CROSS_COMPILING "" ON)
endif()
add_subdirectory(entityx)

if(EMSCRIPTEN)
include_directories(
	SDL/include/
	SDL/build.emscripten/include
	include/
)
else(EMSCRIPTEN)
include_directories(
	include/
	${OPENGLES2_INCLUDE_DIR}
)
endif()

if(EMSCRIPTEN)
set(GAME_SRC
)
elseif( UNIX ) #assume linux
set(GAME_SRC
)
endif()

set(components
  src/components/Body.h
  src/components/Camera.h
  src/components/Sprite.h
  src/components/Transform.h
)
source_group(src\\components FILES ${components})

set(events
  src/events/ResizeEvent.h
  src/events/QuitEvent.h
)
source_group(src\\events FILES ${events})

set(systems
  src/systems/WindowSystem.cpp
  src/systems/WindowSystem.h
  src/systems/RenderSystem.cpp
  src/systems/RenderSystem.h
  src/systems/EventSystem.cpp
  src/systems/EventSystem.h
)
source_group(src\\systems FILES ${systems})

set(src
  src/main.cpp
  src/App.cpp
  src/App.h
)
source_group(src FILES ${src})

set(GAME_SRC
	${GAME_SRC}
	${src}
	${components}
	${events}
	${systems}
)

if ( WIN32 )
	# add the executable
	add_executable(${APP_NAME}
	  WIN32
	  ${GAME_SRC}
	)
else()
	# add the executable
	add_executable(${APP_NAME}
	  ${GAME_SRC}
	)
endif()

IF(EMSCRIPTEN)
set(CMAKE_EXECUTABLE_SUFFIX ".html")
ENDIF()

IF(EMSCRIPTEN)
target_link_libraries(${APP_NAME}
	/Users/jonasmatser/Development/SDL-emscripten/build.emscripten/build/.libs/libSDL2.a
	entityx
)
ELSE(EMSCRIPTEN)
target_link_libraries(${APP_NAME}
	/Users/jonasmatser/Development/SDL-emscripten/build/.libs/libSDL2.dylib
	# ${GLEW_LIBRARY}
	${OPENGL_LIBRARIES}
	entityx
)
ENDIF()

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")
