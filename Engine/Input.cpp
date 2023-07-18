#include "Input.h"

Input::Input()
{
	mouse_x = 0;
	mouse_y = 0;

	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
		pressed_keys[i] = false;
}

Input::~Input()
{
}

bool Input::isKeyPressed(sf::Keyboard::Key key)
{
	return pressed_keys[key];
}
