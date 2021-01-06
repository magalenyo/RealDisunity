#include "GameObject.h"
#include "GUIDGenerator.h"

GameObject::GameObject() : uid(GenerateUID()) {}

GameObject::GameObject(const std::string& name, GameObject* parent) : name(name), parent(parent), uid(GenerateUID()) {}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::AddGameObject(GameObject* gameObject)
{
	children.push_back(gameObject);
}

void GameObject::SetParent(GameObject* gameObject)
{
	parent = gameObject;
}

bool GameObject::HasComponents() const
{
	return !components.empty();
}
