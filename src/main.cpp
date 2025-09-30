/*
	ZPG Projekt 2025
	FER0151
*/

#include <iostream>
#include <plog/Log.h>
#include "application.h"

int main()
{
	Application* app = new Application(1024, 768, "ZPG");

	if (!app->Initialize())
	{
		PLOGE << "Application::Initialize() failed";
		return EXIT_FAILURE;
	}

	if (!app->CreateAssets())
	{
		PLOGE << "Application::CreateAssets() failed";
		return EXIT_FAILURE;
	}

	app->Run();

	return EXIT_SUCCESS;
}