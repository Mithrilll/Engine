#include <conio.h>

#include "Scene.h"

int main()
{
	Scene my_scene;

	std::vector<std::vector<vec3>> shapes;
	std::vector<vec3> points;
	points.push_back(vec3(-5, 0, 1));
	points.push_back(vec3(-5, 200, 1));
	points.push_back(vec3(5, 200, 1));
	points.push_back(vec3(5, 0, 1));
	shapes.push_back(points);
	points.clear();
	points.push_back(vec3(0, 175, 1));
	points.push_back(vec3(-25, 200, 1));
	points.push_back(vec3(0, 225, 1));
	points.push_back(vec3(25, 200, 1));
	shapes.push_back(points);

	Object *obj = new Object(shapes);
	my_scene.addObject(obj);

	obj->translate(vec3(400, 200, 1));

	float start_angle = 55.0f;
	float start_delta = 0.05;
	float damping = 5.0f;
	float max_angle = start_angle;
	float angle = -start_angle;
	float delta = start_delta;
	int dir = 1;
	obj->rotate(angle);

	while (my_scene.isOpen())
	{
		bool isHit = false;
		my_scene.pollEvents(isHit);

		if (isHit)
		{
			delta = start_delta;
			max_angle = start_angle;
			obj->rotate(-angle);
			angle = -start_angle;
			obj->rotate(angle);
			dir = 1;
		}

		if (max_angle >= 0)
		{
			obj->rotate(delta * dir);
			angle += delta * dir;
			if ((angle >= max_angle && dir > 0) || (angle <= -max_angle && dir < 0))
			{
				dir *= -1;
				max_angle -= damping;
				if(max_angle > 0)
					delta = start_delta * (max_angle / start_angle);
			}
		}

		my_scene.render();
	}


	return 0;
}