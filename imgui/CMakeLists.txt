cmake_minimum_required(VERSION 3.8)
project(imgui)

add_library(imgui
	src/imgui.cpp
	src/imgui_demo.cpp
	src/imgui_draw.cpp
	src/imgui_tables.cpp
	src/imgui_widgets.cpp
	src/imgui_impl_sdl2.cpp
	src/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC include)