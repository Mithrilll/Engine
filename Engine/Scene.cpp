#include "Scene.h"

Scene::Scene()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window = new sf::RenderWindow(sf::VideoMode(800, 800), "Engine", sf::Style::Default, settings);
}

Scene::~Scene()
{
	if (window)
		delete window;
}

bool Scene::isOpen()
{
	return window->isOpen();
}

void Scene::pollEvents(bool& restart)
{
	restart = false;

	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}

		if (event.type == sf::Event::KeyPressed)
			if (event.key.code == sf::Keyboard::R)
				restart = true;
	}
}

void Scene::render()
{
	window->clear(sf::Color(1, 50, 32, 255));

	for (int i = 0; i < objects.size(); i++)
		objects[i]->render(*window);

	window->display();
}

void Scene::addObject(Object *obj)
{
	objects.push_back(obj);
}
