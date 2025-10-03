cmake_minimum_required(VERSION 3.11)
project(zpg_project)

# Minimum is C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent
include(FetchContent)

# Copy to bin/
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# SDL2
set(SDL_TESTS OFF)
set(SDL_STATIC ON)

FetchContent_Declare(
	sdl2
	GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
	GIT_TAG release-2.30.4
)

# GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY "https://github.com/icaven/glm.git"
)

# Nlohmann JSON
FetchContent_Declare(
	json
	GIT_REPOSITORY "https://github.com/nlohmann/json.git"
)

# Assimp
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_GLTF_IMPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY "https://github.com/assimp/assimp.git"
)

# PLOG
FetchContent_Declare(
	plog
	GIT_REPOSITORY "https://github.com/SergiusTheBest/plog.git"
)

FetchContent_MakeAvailable(sdl2 glm json assimp plog)

# GLAD
add_subdirectory(glad)

# ImGui
add_subdirectory(imgui)

add_executable(zpg_project
	src/main.cpp
	src/application.cpp
	src/shader.cpp
	src/shaderprogram.cpp
	src/mesh.cpp
	src/asset.cpp
	src/assetprovider.cpp
	src/assetmanager.cpp
	src/renderer.cpp
	src/drawableobject.cpp
	src/camera.cpp
	src/transform.cpp
	src/scene.cpp
)

target_link_libraries(zpg_project PRIVATE
	glad
	imgui
	assimp
	SDL2::SDL2main
	SDL2::SDL2
	glm::glm
	nlohmann_json::nlohmann_json
	plog::plog
)