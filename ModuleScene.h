#pragma once
#include "Module.h"
#include "Quadtree.h"
#include <vector>
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "assimp/scene.h"



class ModuleScene : public Module
{
public:
    const std::string PATH_TEXTURES = "./resources/textures/";
    const std::string PATH_MODELS = "./resources/models/";

    bool Init() override;
    bool CleanUp() override;

    void Load(const char* file_name);
    void LoadSingleTexture(const std::string& file_name);
    void LoadModel(std::string path);
    Quadtree* GetQuadtree();
    ComponentCamera* GetCamera();
    GameObject* GetRootNode() const;

    std::vector<GameObject*> GetObjectsInScene();

private:
    const std::string MODEL_EXTENSION_FBX = ".fbx";

    GameObject* root = nullptr;
    std::vector<GameObject*> objectsInScene;

    Quadtree* quadtree = nullptr;




    // TODO EXTERNALIZE INTO A MODEL LOADER
    //std::vector<ComponentMaterial*> LoadMaterials(const char* file_name, aiMaterial** const mMaterials, unsigned int mNumMaterials);
    ComponentMaterial* LoadMaterials(const char* file_name, aiMaterial* const mMaterial);
    std::vector<ComponentMesh*> LoadMeshes(const aiScene* scene);
    GameObject* LoadRecursively(const char* file_name, const aiScene* scene, const aiNode* node, GameObject* parent);


    std::string GetProcessedPath(const char* modelPath, const std::string& textureName);
    bool ExistsTexture(const std::string& path);
};

