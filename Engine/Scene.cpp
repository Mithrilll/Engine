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

	while (window->pollEvent(event))
	{
		for (int i = 0; i < sf::Keyboard::KeyCount; i++)
			input.pressed_keys[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);

		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::MouseMoved:
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

void Scene::renderGizmo(const Camera& camera)
{
	vec3 zero_point		= { 0.0f, 0.0f, 0.0f };
	vec3 right_point	= { 1.0f, 0.0f, 0.0f };
	vec3 forward_point	= { 0.0f, 1.0f, 0.0f };
	vec3 up_point		= { 0.0f, 0.0f, 1.0f };

	// points from world to view space


	zero_point = camera.getView() * zero_point;
	right_point = camera.getView() * right_point;
	forward_point = camera.getView() * forward_point;
	up_point = camera.getView() * up_point;

	// points from view to clip space
	zero_point = zero_point * camera.getProjection();
	right_point = right_point * camera.getProjection();
	forward_point = forward_point * camera.getProjection();
	up_point = up_point * camera.getProjection();

	// points from clip to screen space
	zero_point.x = (-zero_point.x + 1) * 400;
	zero_point.y = (zero_point.y + 1) * 400;

	right_point.x = (-right_point.x + 1) * 400;
	right_point.y = (right_point.y + 1) * 400;

	forward_point.x = (-forward_point.x + 1) * 400;
	forward_point.y = (forward_point.y + 1) * 400;

	up_point.x = (-up_point.x + 1) * 400;
	up_point.y = (up_point.y + 1) * 400;

	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(zero_point.x, zero_point.y), sf::Color::Red),
		sf::Vertex(sf::Vector2f(right_point.x, right_point.y), sf::Color::Red)
	};

	sf::Vertex line2[] =
	{
		sf::Vertex(sf::Vector2f(zero_point.x, zero_point.y), sf::Color::Green),
		sf::Vertex(sf::Vector2f(forward_point.x, forward_point.y), sf::Color::Green)
	};

	sf::Vertex line3[] =
	{
		sf::Vertex(sf::Vector2f(zero_point.x, zero_point.y), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(up_point.x, up_point.y), sf::Color::Blue)
	};

	window->draw(line1, 2, sf::Lines);
	window->draw(line2, 2, sf::Lines);
	window->draw(line3, 2, sf::Lines);
	
	window->display();
}

void Scene::addMesh(ObjectMesh*obj)
{
	objects.push_back(obj);
}
