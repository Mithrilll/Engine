#pragma once
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "MathLib.h"

class Object
{
public:
	Object();
	~Object();

	Object(std::vector<std::vector<vec3>> p);
	Object(const Object& other);
	void operator=(const Object& other);
	Object(Object&& other) noexcept;
	void operator=(Object&& other) noexcept;

	void rotate(float angle);
	void translate(vec3 translation);
	void scaling(float coeff);

	void render(sf::RenderWindow& window);
private:
	mat3x3 translation;
	mat3x3 rotation;
	mat3x3 scale;
	std::vector<std::vector<vec3>> shapes;
};