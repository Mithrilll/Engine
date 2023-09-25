#include <iostream>

#include "Scene.h"

int main()
{
	// system init
	Scene my_scene;
	Camera my_camera;
	Input my_input;

	// client init
	vec3 point0(0.0f, -10.0, 0.0f);
	vec3 point1(-5.0f, 0.0, 5.0f);
	vec3 point2(5.0f, 0.0, 5.0f);
	vec3 point3(5.0f, 0.0, -5.0f);
	vec3 point4(-5.0f, 0.0, -5.0f);

	ObjectMesh pyramid({ point0, point1, point2, point3, point4 }, { {0, 1, 2}, {0, 2, 3}, {0, 3, 4},
																	{0, 4, 1}, {3, 2, 4}, {2, 1, 4} });
	//my_scene.addObject(&pyramid);

	ObjectSurface surface(std::function<float(float, float)>([](float x, float z) { return x * x + z * z; }), 2.0f, 2.0f);

	surface.generateMesh(0.20f);

	surface.rotateX(180.0f);

	my_scene.addObject(&surface);

	my_scene.pollEvents(my_input);

	my_camera.translate(vec3(0.0f, -10.0f, -10.0f));

	float phi = 0.0f;
	float theta = 0.0f;
	float speed = 0.5f;


	// start of main loop
	while (my_scene.isOpen())
	{
		// system update
		my_scene.pollEvents(my_input);

		// client update
		if (my_input.isKeyPressed(sf::Keyboard::W))
			my_camera.translate(my_camera.getViewDirection() * speed);

		if (my_input.isKeyPressed(sf::Keyboard::S))
			my_camera.translate(my_camera.getViewDirection() * speed * -1);

		if (my_input.isKeyPressed(sf::Keyboard::A))
			my_camera.translate(my_camera.getRightDirection() * speed);

		if (my_input.isKeyPressed(sf::Keyboard::D))
			my_camera.translate(my_camera.getRightDirection() * speed * -1);

		if (my_input.isKeyPressed(sf::Keyboard::Q))
			my_camera.translate(vec3(0.0f, speed, 0.0f));

		if (my_input.isKeyPressed(sf::Keyboard::E))
			my_camera.translate(vec3(0.0f, -speed, 0.0f));

		if (my_input.isKeyPressed(sf::Keyboard::P))
			surface.rotateY(1.0f);

		if (my_input.isKeyPressed(sf::Keyboard::O))
			surface.rotateY(-1.0f);

		if (my_input.isKeyPressed(sf::Keyboard::LShift))
		{
			phi += -my_input.getMouseDeltaX() * 0.1f;
			theta += my_input.getMouseDeltaY() * 0.1f;
		}

		my_camera.setRotation(vec3(theta, phi, 0.0f));

		// system render
		my_scene.render(my_camera);
	}

	return 0;
}