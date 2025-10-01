#include <iostream>
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "application.h"

Application::Application(const uint32_t _width, const uint32_t _height, const std::string& _title):
	width(_width), height(_height), title(_title)
{
}

Application::~Application()
{
}

bool Application::Initialize()
{
	static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("log.txt", 8000, 2);
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		PLOGE << "SDL_Init() failed: " << SDL_GetError() << '\n';
		return false;
	}

	window = SDL_CreateWindow(
		title.c_str(), 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		width, height, 
		SDL_WINDOW_OPENGL
	);

	if (!window)
	{
		PLOGE << "SDL_CreateWindow() failed: " << SDL_GetError() << '\n';
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	PLOGI << "Renderer: " << glGetString(GL_RENDERER);
	PLOGI << "Vendor: " << glGetString(GL_VENDOR);
	PLOGI << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

	return true;
}

void Application::Run()
{
	SDL_Event ev;
	
	running = true;

	while (running)
	{
		while (SDL_PollEvent(&ev))
		{
			ProcessEvents(&ev);
		}

		PreDraw();
		Draw();
		PostDraw();

		SDL_GL_SwapWindow(window);
	}
}

void Application::Close()
{
	running = false;

	DestroyAssets();
	
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Application::CreateAssets()
{
	return true;
}

void Application::DestroyAssets()
{
}

void Application::ProcessEvents(SDL_Event *ev)
{
	if (ev->type == SDL_QUIT)
	{
		Close();
	}

	if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_ESCAPE)
	{
		Close();
	}
}

void Application::PreDraw()
{
}

void Application::Draw()
{
}

void Application::PostDraw()
{
}