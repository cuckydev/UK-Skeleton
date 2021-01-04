cmake_minimum_required(VERSION 3.8)

if((${CMAKE_VERSION} VERSION_EQUAL 3.9) OR (${CMAKE_VERSION} VERSION_GREATER 3.9))
	cmake_policy(SET CMP0069 NEW)
endif()

###########
# Options #
###########
option(BACKEND_SDL2 "Compile the SDL2 backend" ON)

option(SANITIZE "Enable sanitization" OFF)
option(LTO "Enable link-time optimization" OFF)
option(PKG_CONFIG_STATIC_LIBS "On platforms with pkg-config, static-link the dependencies (good for Windows builds, so you don't need to bundle DLL files)" OFF)
option(MSVC_LINK_STATIC_RUNTIME "Link the static MSVC runtime library (Visual Studio only)" OFF)

#########
# Setup #
#########

# Define project, common source, and includes
project(UKSkeleton LANGUAGES CXX)

add_executable(UKSkeleton WIN32
	# Game
	"src/Main.cpp"
	"src/Main.h"
	"src/DisplayError.h"
	"src/Render.cpp"
	"src/Render.h"
	"src/Game.cpp"
	"src/Game.h"
	"src/GameMode_Title.cpp"
	"src/GameMode_Title.h"
	"src/GameMode_Editor.cpp"
	"src/GameMode_Editor.h"
	"src/Map.cpp"
	"src/Map.h"
	"src/Player/Player.cpp"
	"src/Player/Player.h"
	
	# Common
	"src/Util/Error.cpp"
	"src/Util/Error.h"
	"src/Util/TextureData.cpp"
	"src/Util/TextureData.h"
	
	# Backend
	"src/Backend/Backend.h"
	"src/Backend/Core.h"
	"src/Backend/Render.h"
	"src/Backend/Input.h"
	
	# Library
	"lib/lodepng/lodepng.cpp"
	"lib/lodepng/lodepng.h"
)

target_include_directories(UKSkeleton PRIVATE "src")
target_include_directories(UKSkeleton PRIVATE "lib")

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
)

##################
# Compiler flags #
##################

# Sanitization
if(SANITIZE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ggdb3 -fsanitize=address")
endif()

# Strip release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# Statically link other libraries
if(PKG_CONFIG_STATIC_LIBS)
	target_link_options(UKSkeleton PRIVATE "-static")
endif()

# Use LTO if requested
if(LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result)
	if(result)
		set_target_properties(UKSkeleton PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

##############################
# Other compile-time defines #
##############################

# Determine endianness
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(ENDIAN)
if(ENDIAN)
	message("Target is big-endian")
	target_compile_definitions(UKSkeleton PRIVATE _BIG_ENDIAN)
else()
	message("Target is little-endian")
	target_compile_definitions(UKSkeleton PRIVATE _LIL_ENDIAN)
endif()

########
# MSVC #
########

# This is messy as hell, and has been replaced by CMAKE_MSVC_RUNTIME_LIBRARY,
# but that's a very recent CMake addition, so we're still doing it this way for now
if(MSVC AND MSVC_LINK_STATIC_RUNTIME)
	# Statically-link the CRT (vcpkg static libs do this)
	foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif()
	endforeach()
endif()

# Do some other MSVC fixes
if(MSVC)
	# Disable warnings that normally fire up on MSVC when using "unsafe" functions instead of using MSVC's "safe" _s functions
	target_compile_definitions(UKSkeleton PRIVATE _CRT_SECURE_NO_WARNINGS)
	
	# Make it so source files are recognized as UTF-8 by MSVC
	target_compile_options(UKSkeleton PRIVATE "/utf-8")
	
	# Use `main` instead of `WinMain`
	set_target_properties(UKSkeleton PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

##########################
# Further compiler setup #
##########################

# Force strict C++11
set_target_properties(UKSkeleton PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

# Set debug executable name
set_target_properties(UKSkeleton PROPERTIES DEBUG_OUTPUT_NAME "UKSkeleton_debug")

# Set output directory
set(BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set_target_properties(UKSkeleton PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BUILD_DIRECTORY}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIRECTORY}
)

###########
# Backend #
###########

# Find PkgConfig for dependency linking
find_package(PkgConfig QUIET)

# Compile SDL2 backend
if(BACKEND_SDL2)
	target_compile_definitions(UKSkeleton PRIVATE COMPILE_SDL2)
	target_sources(UKSkeleton PRIVATE
		"src/DisplayError_SDL2.cpp"
		"src/Backend/SDL2/Core.cpp"
		"src/Backend/SDL2/Render.cpp"
		"src/Backend/SDL2/Input.cpp"
	)
	
	# Find SDL2
	find_package(SDL2)
	if(PKG_CONFIG_FOUND)
		pkg_check_modules(sdl2 QUIET IMPORTED_TARGET sdl2)
	endif()
	
	# Link SDL2
	if(TARGET PkgConfig::sdl2)
		if(PKG_CONFIG_STATIC_LIBS)
			# Pkg-Config static
			message(STATUS "Using system SDL2 (pkg-config, static)")
			list(REMOVE_ITEM sdl2_STATIC_CFLAGS "-Dmain=SDL_main")
			list(REMOVE_ITEM sdl2_STATIC_LDFLAGS "-lSDL2main")
			target_compile_options(UKSkeleton PRIVATE ${sdl2_STATIC_CFLAGS})
			target_link_libraries(UKSkeleton PRIVATE ${sdl2_STATIC_LDFLAGS})
		else()
			# Pkg-Config dynamic
			message(STATUS "Using system SDL2 (pkg-config, dynamic)")
			list(REMOVE_ITEM sdl2_CFLAGS "-Dmain=SDL_main")
			list(REMOVE_ITEM sdl2_LDFLAGS "-lSDL2main")
			target_compile_options(UKSkeleton PRIVATE ${sdl2_CFLAGS})
			target_link_libraries(UKSkeleton PRIVATE ${sdl2_LDFLAGS})
		endif()
	elseif(TARGET SDL2::SDL2)
		# CMake-generated config (Arch, vcpkg, Raspbian)
		message(STATUS "Using system SDL2 (CMake, dynamic)")
		target_link_libraries(UKSkeleton PRIVATE SDL2::SDL2)
	elseif(TARGET SDL2::SDL2-static)
		# CMake-generated config (Arch, vcpkg, Raspbian)
		message(STATUS "Using system SDL2 (CMake, static)")
		target_link_libraries(UKSkeleton PRIVATE SDL2::SDL2-static)
	elseif(SDL2_FOUND)
		# Autotools-generated config (MSYS2)
		message(STATUS "Using system SDL2 (Autotools)")
		target_include_directories(UKSkeleton PRIVATE ${SDL2_INCLUDE_DIRS})
		target_link_libraries(UKSkeleton PRIVATE ${SDL2_LIBRARIES})
	endif()
endif()
