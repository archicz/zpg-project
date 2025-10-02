cmake_minimum_required(VERSION 3.11)
project(zpg_project)

# Minimum is C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent
include(FetchContent)

# Copy to bin/
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Download libraries
FetchContent_Declare(
  sdl2
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-2.30.4
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/icaven/glm"
)

FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json"
)

FetchContent_Declare(
  plog
  GIT_REPOSITORY "https://github.com/SergiusTheBest/plog"
)

FetchContent_MakeAvailable(sdl2 glm json plog)

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
	SDL2::SDL2main
	SDL2::SDL2
	glm::glm
	nlohmann_json::nlohmann_json
	plog::plog
)