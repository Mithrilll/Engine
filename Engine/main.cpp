#include <iostream>

#include "Scene.h"

int main()
{
	// system init
	Scene my_scene;
	Camera my_camera;
	Input my_input;

	// client init
	vec3 point0( 5.0f,  5.0,  5.0f);
	vec3 point1( 5.0f,  5.0, -5.0f);
	vec3 point2(-5.0f,  5.0, -5.0f);
	vec3 point3(-5.0f,  5.0,  5.0f);
	vec3 point4( 5.0f, -5.0,  5.0f);
	vec3 point5( 5.0f, -5.0, -5.0f);
	vec3 point6(-5.0f, -5.0, -5.0f);
	vec3 point7(-5.0f, -5.0,  5.0f);

	ObjectMesh pyramid({ point0, point1, point2, point3, point4, point5, point6, point7 }, { {0, 1, 2}, {0, 2, 3}, {0, 5, 1}, 
																							{0, 4, 1}, {2, 5, 1}, {2, 6, 5}, 
																							{6, 3, 2}, {6, 3, 7}, {4, 6, 5}, 
																							{4, 7, 6}, {0, 3, 4}, {4, 3, 7} });
	my_scene.addMesh(&pyramid);

	my_scene.pollEvents(my_input);

	my_camera.translate(vec3(0.0f, 0.0f, -30.0f));

	float phi = 0.0f;
	float theta = 0.0f;

	// start of main loop
	while (my_scene.isOpen())
	{
		// system update
		my_scene.pollEvents(my_input);

		// client update
		if (my_input.isKeyPressed(sf::Keyboard::W))
			my_camera.translate(vec3(0.0f, 0.0f, 0.1f));

		if (my_input.isKeyPressed(sf::Keyboard::S))
			my_camera.translate(vec3(0.0f, 0.0f, -0.1f));

		if (my_input.isKeyPressed(sf::Keyboard::A))
			my_camera.translate(vec3(0.1f, 0.0f, 0.0f));

		if (my_input.isKeyPressed(sf::Keyboard::D))
			my_camera.translate(vec3(-0.1f, 0.0f, 0.0f));

		if (my_input.isKeyPressed(sf::Keyboard::Q))
			my_camera.translate(vec3(0.0f, 0.1f, 0.0f));

		if (my_input.isKeyPressed(sf::Keyboard::E))
			my_camera.translate(vec3(0.0f, -0.1f, 0.0f));

		phi += -my_input.getMouseDeltaX() * 0.1f;
		theta += my_input.getMouseDeltaY() * 0.1f;

		my_camera.setRotation(vec3(theta, phi, 0.0f));

		// system render
		my_scene.render(my_camera);
	}

	return 0;
}