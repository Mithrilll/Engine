#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Object.h"

class Scene
{
public:
	Scene();
	~Scene();

	bool isOpen();

	void pollEvents(bool& restart);

	void render();

	void addObject(Object *obj);

private:
	std::vector<Object*> objects;

	sf::RenderWindow *window;
};
