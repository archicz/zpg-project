#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "mesh.h"

class Renderer
{
public:
	friend class Application;
private:
	Renderer();
	~Renderer();
public:
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;
public:
	static Renderer& GetInstance();
public:
	bool IsValid();
protected:
	void Destroy();
	void PreDraw();
	void PostDraw();
};

#endif