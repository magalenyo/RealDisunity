#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "DebugLogger.h"

#include "Math/float4x4.h"
#include "ModuleDebugDraw.h"
#include "debugdraw.h" 
#include "Math/float3.h"

#include "Assimp/cimport.h" // aiLogStream

ModuleRender::ModuleRender()
{

}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits
	SDL_GL_CreateContext(App->window->window);


	//glewInit();
	GLenum err = glewInit();
	// � check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	// Should be 2.0

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise


#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&DebugLogger::GLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif

	// This is to output the log from Assimp
	struct aiLogStream stream;
	stream.callback = DebugLogger::AssimpErrorFunction;
	aiAttachLogStream(&stream);
	// *************************

	unsigned vertexShaderTexture = App->program->CompileShader(GL_VERTEX_SHADER, "./shaders/VertexShaderTexture.glsl");
	unsigned fragmentShaderTexture = App->program->CompileShader(GL_FRAGMENT_SHADER, "./shaders/FragmentShaderTexture.glsl");
	defaultProgram = App->program->CreateProgram(vertexShaderTexture, fragmentShaderTexture);

	loadedModel = new Model("./resources/models/BakerHouse.fbx");

	gridColor = { 1.000000f, 1.000000f, 1.000000f };

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int w;
	int h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	LoadRenderConfiguration();
	RenderAxis();
	RenderGrid();
	int w, h;
	App->window->GetWindowSize(w, h);
	App->debugDraw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), w, h);

	RenderModel();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	if (loadedModel != nullptr) {
		delete loadedModel;
	}
	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void* ModuleRender::GetContext()
{
	return context;
}

unsigned int ModuleRender::GetDefaultProgram()
{
	return defaultProgram;
}

void ModuleRender::LoadModel(const char* path)
{
	delete loadedModel;
	loadedModel = nullptr;
	loadedModel = new Model(path);
}

void ModuleRender::SetGridColor(float3 newColor)
{
	gridColor = newColor;
}

float3 ModuleRender::GetGridColor() const
{
	return gridColor;
}

bool& ModuleRender::GetAxisState()
{
	return activeAxis;
}

bool& ModuleRender::GetGridState()
{
	return activeGrid;
}

bool& ModuleRender::GetModelState()
{
	return activeModel;
}

bool& ModuleRender::GetGLDepthTestState()
{
	return activeGLDepthTest;
}

bool& ModuleRender::GetGLCullFaceState()
{
	return activeGLCullFace;
}

void ModuleRender::TurnAxis(bool state)
{
	activeAxis = state;
}

void ModuleRender::TurnGrid(bool state)
{
	activeGrid = state;
}

void ModuleRender::RenderAxis()
{
	if (activeAxis) {
		dd::axisTriad(float4x4::identity, 0.1f, 2.0f);
	}
}

void ModuleRender::RenderGrid()
{
	if (activeGrid) {
		dd::xzSquareGrid(-25, 25, 0.0f, 1.0f, gridColor);
	}
}

void ModuleRender::RenderModel()
{
	if (activeModel) {
		loadedModel->Draw();
	}
}

void ModuleRender::LoadRenderConfiguration()
{
	if (activeGLCullFace) {
		glEnable(GL_CULL_FACE); // Enable cull backward faces
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (activeGLDepthTest) {
		glEnable(GL_DEPTH_TEST); // Enable depth test
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

