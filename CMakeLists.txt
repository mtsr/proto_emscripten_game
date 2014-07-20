cmake_minimum_required(VERSION 2.6)

if( EMSCRIPTEN )
	set(APP_NAME CrossPlatform.html)
else()
	set(APP_NAME CrossPlatform)
endif()
project (${APP_NAME}.bc)

# option(DEBUG_MODE "" ON)

# if(DEBUG_MODE)
#   set(CMAKE_BUILD_TYPE DEBUG)
# else(DEBUG_MODE)
#   set(CMAKE_BUILD_TYPE RELEASE)
# endif(DEBUG_MODE)

# if (MSVC)
# #	set(CMAKE_C_FLAGS_DEBUG "-DCOCOS2D_DEBUG=1")
# #	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

# 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

# elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
# 	set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
# 	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

# 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# endif()

if( UNIX ) #assume linux
set(GAME_SRC
  proj.emscripten/main.cpp
  # Classes/AppDelegate.cpp
  # Classes/HelloWorldScene.cpp
)
elseif ( WIN32 )
set(GAME_SRC
  proj.win32/main.cpp
  proj.win32/main.h
  proj.win32/resource.h
  Classes/AppDelegate.cpp
  Classes/HelloWorldScene.cpp
)
endif()

# if (WIN32)
# include_directories(
# )
# link_directories(
# )
# elseif(UNIX) #assumed linux
# include_directories(
# )
# link_directories(
# )
# endif()

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

# target_link_libraries(${APP_NAME}
# )

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")
