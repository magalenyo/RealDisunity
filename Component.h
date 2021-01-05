#pragma once

#include <string>

class Component
{
public:
	enum ComponentType {
		Material,
		Mesh,
		Transform
	};

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

private:
	ComponentType type;
	std::string uid;
};

