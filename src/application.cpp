#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include "application.h"
#include "renderer.h"
#include "assetmanager.h"
#include "shader.h"
#include "shaderprogram.h"

Application::Application(const uint32_t _width, const uint32_t _height, const std::string& _title):
	width(_width), height(_height), title(_title)
{
}

Application::~Application()
{
	Destroy();
}

bool Application::Initialize()
{
	static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("log.txt", 8000, 2);
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &fileAppender).addAppender(&consoleAppender);

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

	AssetManager& am = AssetManager::GetInstance();
	am.RegisterLoader<VertexShaderAssetLoader>();
	am.RegisterLoader<FragmentShaderAssetLoader>();
	am.RegisterLoader<ShaderProgramAssetLoader>();

	am.Require<ShaderProgramAsset>(AssetURI("base://shaders/generic.json"));
	
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();

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
			ProcessEvent(&ev);
		}

		PreDraw();
		Draw();
		PostDraw();

		SDL_GL_SwapWindow(window);
	}
}

void Application::Destroy()
{
	running = false;

	Renderer::GetInstance().Destroy();
	
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::ProcessEvent(SDL_Event* ev)
{
	if (ev->type == SDL_QUIT)
	{
		running = false;
		return;
	}

	if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_ESCAPE)
	{
		running = false;
		return;
	}

	ImGui_ImplSDL2_ProcessEvent(ev);
}

void Application::PreDraw()
{
	Renderer::GetInstance().PreDraw();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Application::Draw()
{
}

void Application::PostDraw()
{
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	Renderer::GetInstance().PostDraw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}