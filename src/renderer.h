#ifndef RENDERER_H
#define RENDERER_H
#pragma once

class Renderer
{
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
};

#endif