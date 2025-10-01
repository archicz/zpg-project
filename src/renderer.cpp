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
