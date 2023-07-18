#pragma once
#include <SFML/Window.hpp>

class Input
{
	friend class Scene;

public:
	Input();
	~Input();

	bool isKeyPressed(sf::Keyboard::Key key);

	int getMouseDeltaX();
	int getMouseDeltaY();

private:
	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_delta_x = 0;
	int mouse_delta_y = 0;

	bool pressed_keys[sf::Keyboard::KeyCount];
};