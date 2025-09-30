#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H
#pragma once

#include <unordered_map>
#include <vector>
#include <optional>
#include "shaderprogram.h"
#include "shader.h"

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

class Material
{
public:
	Material(ShaderProgramPtr shaderProgram);
	~Material();
public:
	bool IsValid();
public:
	bool Bind();
private:
	ShaderProgramPtr shaderProgram;
};

using MaterialPtr = std::shared_ptr<Material>;

class MaterialSystem
{
private:
	MaterialSystem();
	~MaterialSystem();
public:
    MaterialSystem(const MaterialSystem& other) = delete;
    MaterialSystem& operator=(const MaterialSystem& other) = delete;
    MaterialSystem(MaterialSystem&& other) = delete;
    MaterialSystem& operator=(MaterialSystem&& other) = delete;
public:
	static MaterialSystem& GetInstance();
public:
	bool HasShader(const std::string& name);
	bool AddShader(const std::string& name, std::string& vtxSource, std::string& fragSource);
	std::optional<ShaderProgramPtr> GetShader(const std::string& name);
public:
	bool HasMaterial(const std::string& name);
	bool AddMaterial(const std::string& name, const std::string& shaderName);
	std::optional<MaterialPtr> GetMaterial(const std::string& name);
private:
	std::unordered_map<std::string, ShaderProgramPtr> shaderPrograms;
	std::unordered_map<std::string, MaterialPtr> materials;
};

#endif