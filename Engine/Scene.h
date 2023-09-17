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

	void addObject(Object *obj);

private:
	std::vector<Object*> objects;

	sf::RenderWindow *window;
};
