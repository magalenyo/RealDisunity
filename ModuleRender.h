#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"
#include "Math/float3.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:


	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void* GetContext();
	unsigned int GetDefaultProgram();
	void LoadModel(std::string path);

	void SetGridColor(float3 newColor);
	float3 GetGridColor() const;

	Model* GetModel() const;

	bool& GetAxisState();
	bool& GetGridState();
	bool& GetModelState();
	bool& GetGLDepthTestState();
	bool& GetGLCullFaceState();

	void TurnAxis(bool state);
	void TurnGrid(bool state);

private:
	
	void* context;
	unsigned int defaultProgram;
	Model* loadedModel = nullptr;

	/* CONFIGURATION VARIABLES */
	float3 gridColor;
	bool activeAxis = true;
	bool activeGrid = true;
	bool activeModel = true;
	bool activeGLDepthTest = true;
	bool activeGLCullFace = true;

	void LoadRenderConfiguration();
	void RenderAxis();
	void RenderGrid();
	void RenderModel();
};
