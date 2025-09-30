cmake_minimum_required(VERSION 3.8)
project(zpg_project)

# Minimum is C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Copy to bin/
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# SDL2, GLAD, GLM
find_package(SDL2 REQUIRED)
add_subdirectory(glad)
find_package(glm REQUIRED)

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

# PLOG
add_subdirectory(plog)

add_executable(zpg_project
	src/main.cpp
	src/application.cpp
	src/shader.cpp
	src/shaderprogram.cpp
	src/model.cpp
	src/materialsystem.cpp
	src/drawableobject.cpp
	src/transform.cpp
	src/scene.cpp
)

target_link_libraries(zpg_project PRIVATE
	glad
	SDL2main
	SDL2
	glm::glm
	nlohmann_json::nlohmann_json
	plog::plog
)