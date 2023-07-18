#pragma once
#include <SFML/Window.hpp>

class Input
{
	friend class Scene;

public:
	Input();
	~Input();

	bool isKeyPressed(sf::Keyboard::Key key);

private:
	int mouse_x = 0;
	int mouse_y = 0;

	bool pressed_keys[sf::Keyboard::KeyCount];
};