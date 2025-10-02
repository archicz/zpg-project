#include <imgui.h>
#include "renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

bool Renderer::IsValid()
{
	return true;
}

void Renderer::Destroy()
{
}

void Renderer::PreDraw()
{
}

void Renderer::PostDraw()
{
}
