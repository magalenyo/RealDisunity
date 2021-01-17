#pragma once
#include "Module.h"
#include "Quadtree.h"
#include <string>
#include <vector>
#include "assimp/scene.h"

class GameObject;
class ComponentCamera;
class ComponentMaterial;
class ComponentMesh;

class ModuleScene : public Module
{
public:
    const std::string PATH_TEXTURES         = "./resources/textures/";
    const std::string PATH_MODELS           = "./resources/models/";

    bool Init() override;
    update_status Update() override;
    bool CleanUp() override;

    void Load(const char* file_name);
    void LoadSingleTexture(const std::string& file_name);
    void LoadModel(std::string path);
    Quadtree* GetQuadtree();
    GameObject* GetCamera();
    GameObject* GetRootNode() const;
    void RemoveObjectFromScene(GameObject* gameObject);
    void ResetQuadtree(); //Rebuilds the quadtree.
    GameObject* SendRay(LineSegment& picking, float& distance); //Sends the ray passed by parameter to test if it collides with any object on the scene, and if so, returns it.

    std::vector<GameObject*> GetObjectsInScene();

private:
    const std::string MODEL_EXTENSION_FBX   = ".fbx";

    GameObject* root                        = nullptr;
    GameObject* camera                      = nullptr;
    std::vector<GameObject*> objectsInScene;

    Quadtree* quadtree                      = nullptr;

    void TestRay(LineSegment& picking, float& distance, GameObject** picked); //Searchs if there is and object in the scene that collides with the ray passed by parameter.
    
    ComponentMaterial* LoadMaterial(const char* file_name, const aiMaterial* mMaterial);
    std::vector<ComponentMesh*> LoadMeshes(const aiScene* scene);
    GameObject* LoadRecursively(const char* file_name, const aiScene* scene, const aiNode* node, GameObject* parent);



    std::string GetProcessedPath(const char* modelPath, const std::string& textureName);            // Returns the path to the directory where the texture is. ModelPath is the directory from which the fbx has been loaded and textureName is the name from the file.data
                                                                                                    // Returns empty if the file has not been found

    std::string SanitizeTextureName(const std::string& textureName, bool withExtension = true);     // Clears directories of the file_name, for eg: 
                                                                                                    // input: \\textures\\street_environment.tcg
                                                                                                    // returns street_environment.tcg

    bool ExistsTexture(const std::string& path);                                                    // Returns true if the path corresponds to a file or directory


    
};

