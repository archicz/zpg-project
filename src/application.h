#ifndef APPLICATION_H
#define APPLICATION_H
#pragma once

#include <string>
#include <SDL2/SDL.h>

class Application
{
public:
	Application(const uint32_t width, const uint32_t height, const std::string& title);
	~Application();
public:
	bool Initialize();
	void Run();
	void Destroy();
private:
	void ProcessEvent(SDL_Event* ev);
public:
	void PreDraw();
	void Draw();
	void PostDraw();
private:
	const uint32_t width = 0;
	const uint32_t height = 0;
	std::string title;
	SDL_Window* window = nullptr;
	bool running = false;
};

#endif