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

void Scene::pollEvents(Input& input)
{
	sf::Event event;

	input.mouse_delta_x	= 0;
	input.mouse_delta_y = 0;

	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			input.pressed_keys[event.key.code] = true;
			break;
		case sf::Event::KeyReleased:
			input.pressed_keys[event.key.code] = false;
		case sf::Event::MouseMoved:
			input.mouse_delta_x = sf::Mouse::getPosition().x - input.mouse_x;
			input.mouse_delta_y = sf::Mouse::getPosition().y - input.mouse_y;
			input.mouse_x = sf::Mouse::getPosition().x;
			input.mouse_y = sf::Mouse::getPosition().y;
			break;
		}
	}
}

void Scene::render(const Camera& camera)
{
	window->clear(sf::Color::Black);

	for (int i = 0; i < objects.size(); i++)
		objects[i]->render(*window, camera);

	window->display();
}

void Scene::addObject(Object* obj)
{
	objects.push_back(obj);
}
