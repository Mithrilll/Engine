#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Objects.h"
#include "Input.h"

class Scene
{
public:
	Scene();
	~Scene();

	bool isOpen();

	void pollEvents(Input& input);

	void render(const Camera& camera);

	void renderGizmo(const Camera& camera);

	void addMesh(ObjectMesh *obj);

private:
	std::vector<ObjectMesh*> objects;

	sf::RenderWindow *window;
};
