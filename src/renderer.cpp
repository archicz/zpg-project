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

void Renderer::Begin()
{
}

void Renderer::End()
{
	ImGui::Begin("test");
		ImGui::Button("test button");
	ImGui::End();
}
