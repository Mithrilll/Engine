#include <algorithm>
#include <iostream>

#include "Objects.h"

Object::Object()
{
	translation = mat4();
	rotation = mat4();
	scale = mat4();
	transform = mat4();
}

Object::~Object()
{
}

Object::Object(const Object& other)
{
	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	transform = other.transform;
}

void Object::operator=(const Object& other)
{
	if (this == &other)
		return;

	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	transform = other.transform;
}

Object::Object(Object&& other) noexcept
{
	translation = std::move(other.translation);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	transform = std::move(other.transform);
}

void Object::operator=(Object&& other) noexcept
{
	if (this == &other)
		return;

	translation = std::move(other.translation);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	transform = std::move(other.transform);
}

void Object::rotateX(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[1][1] = cosf(radians);
	addition.mat[1][2] = sinf(radians);
	addition.mat[2][1] = -sinf(radians);
	addition.mat[2][2] = cosf(radians);

	rotation *= addition;

	transform = scale * translation * rotation;
}

void Object::rotateY(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][0] = cosf(radians);
	addition.mat[0][2] = sinf(radians);
	addition.mat[2][0] = -sinf(radians);
	addition.mat[2][2] = cosf(radians);

	rotation *= addition;

	transform = scale * translation * rotation;
}

void Object::rotateZ(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][0] = cosf(radians);
	addition.mat[0][1] = sinf(radians);
	addition.mat[1][0] = -sinf(radians);
	addition.mat[1][1] = cosf(radians);

	rotation *= addition;

	transform = scale * translation * rotation;
}

void Object::translate(vec3 translating)
{
	mat4 addition;
	addition.mat[0][3] = translating.x;
	addition.mat[1][3] = translating.y;
	addition.mat[2][3] = translating.z;

	translation *= addition;

	transform = scale * translation * rotation;
}

void Object::scaling(float coeff)
{
	mat4 addition;
	addition.mat[0][0] = coeff;
	addition.mat[1][1] = coeff;
	addition.mat[2][2] = coeff;

	scale *= addition;

	transform = scale * translation * rotation;
}

mat4 Object::getTransform() const
{
	return transform;
}

void Object::render(sf::RenderWindow& window, const Camera& camera)
{
}

ObjectMesh::ObjectMesh() : Object()
{
	points = std::vector<vec3>(0);
	indices = std::vector<std::vector<int>>(0, std::vector<int>(3));
}

ObjectMesh::~ObjectMesh()
{
}

ObjectMesh::ObjectMesh(std::vector<vec3> pts, std::vector<std::vector<int>> inds) : Object()
{
	points = pts;
	indices = inds;
}

ObjectMesh::ObjectMesh(const ObjectMesh& other) : Object(other)
{
	points = other.points;
	indices = other.indices;
}

void ObjectMesh::operator=(const ObjectMesh& other)
{
	if (this == &other)
		return;

	(Object)* this = (Object)other;

	points = other.points;
	indices = other.indices;
}

ObjectMesh::ObjectMesh(ObjectMesh&& other) noexcept : Object(std::move(other))
{
	points = std::move(other.points);
	indices = std::move(other.indices);
}

void ObjectMesh::operator=(ObjectMesh&& other) noexcept
{
	if (this == &other)
		return;

	(Object)*this = (Object)other;

	points = std::move(other.points);
	indices = std::move(other.indices);
}

void ObjectMesh::render(sf::RenderWindow& window, const Camera& camera)
{
	std::vector<std::pair<std::vector<vec3>, float>> triangles(indices.size());

	for (int i = 0; i < indices.size(); i++)
	{
		triangles[i].first.resize(3);

		// update triangle
		float z_max = -INFINITY;
		float z_min = INFINITY;

		for (int j = 0; j < 3; j++)
		{
			// point from local to world space
			vec3 new_point = points[indices[i][j]];
			//new_point = scale * new_point;
			//new_point = translation * new_point;
			//new_point = rotation * new_point;
			new_point = transform * new_point;

			// point from world to view space
			new_point = camera.getView() * new_point;

			mat4 proj = camera.getProjection();

			float w = new_point.x * proj.mat[3][0] + new_point.y * proj.mat[3][1] + new_point.z * proj.mat[3][2] + proj.mat[3][3];

			// point from view to clip space
			new_point = camera.getProjection() * new_point;

			// point from clip to screen space
			new_point.x = (new_point.x / w + 1) * 400;
			new_point.y = (-new_point.y / w + 1) * 400;

			if (z_max < new_point.z)
				z_max = new_point.z;

			if (z_min > new_point.z)
				z_min = new_point.z;

			// setup screen points
			triangles[i].first[j] = new_point;
		}

		triangles[i].second = (z_max + z_min) / 2.0f;
	}

	// shapes depth sort
	std::sort(triangles.begin(), triangles.end(), [](std::pair<std::vector<vec3>, float> a, std::pair<std::vector<vec3>, float> b) { return a.second < b.second; });

	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].second > 1)
			continue;

		float ax = triangles[i].first[1].x - triangles[i].first[0].x;
		float ay = triangles[i].first[1].y - triangles[i].first[0].y;

		float bx = triangles[i].first[2].x - triangles[i].first[0].x;
		float by = triangles[i].first[2].y - triangles[i].first[0].y;

		float dir = ax * by - ay * bx;
		if (dir > 0)
			continue;

		switch (camera.getRenderMode())
		{
		case Camera::RenderMode::Wireframe:
			{
				sf::Vertex line1[] =
				{
					sf::Vertex(sf::Vector2f(triangles[i].first[0].x, triangles[i].first[0].y), sf::Color::Green),
					sf::Vertex(sf::Vector2f(triangles[i].first[1].x, triangles[i].first[1].y), sf::Color::Blue)
				};

				sf::Vertex line2[] =
				{
					sf::Vertex(sf::Vector2f(triangles[i].first[1].x, triangles[i].first[1].y), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(triangles[i].first[2].x, triangles[i].first[2].y), sf::Color::Red)
				};

				sf::Vertex line3[] =
				{
					sf::Vertex(sf::Vector2f(triangles[i].first[2].x, triangles[i].first[2].y), sf::Color::Red),
					sf::Vertex(sf::Vector2f(triangles[i].first[0].x, triangles[i].first[0].y), sf::Color::Green)
				};

				window.draw(line1, 2, sf::Lines);
				window.draw(line2, 2, sf::Lines);
				window.draw(line3, 2, sf::Lines);
			}
			break;
		case Camera::RenderMode::Solid:
			{
				sf::ConvexShape triangle(3);
				triangle.setPoint(0, sf::Vector2f(triangles[i].first[0].x, triangles[i].first[0].y));
				triangle.setPoint(1, sf::Vector2f(triangles[i].first[1].x, triangles[i].first[1].y));
				triangle.setPoint(2, sf::Vector2f(triangles[i].first[2].x, triangles[i].first[2].y));

				triangle.setFillColor(sf::Color::Green);
				triangle.setOutlineColor(sf::Color::White);
				triangle.setOutlineThickness(0.3f);

				window.draw(triangle);
			}
			break;
		}
	}
}

ObjectSurface::ObjectSurface() : Object()
{
	func = nullptr;
}

ObjectSurface::~ObjectSurface()
{
}

ObjectSurface::ObjectSurface(std::function<float(float, float)> f, float x, float z) : Object()
{
	func = f;
	x_bound = abs(x);
	z_bound = abs(z);
}

ObjectSurface::ObjectSurface(const ObjectSurface& other) : Object(other)
{
	func = other.func;
}

void ObjectSurface::operator=(const ObjectSurface& other)
{
	if (this == &other)
		return;

	(Object)*this = (Object)other;

	func = other.func;
}

ObjectSurface::ObjectSurface(ObjectSurface&& other) noexcept : Object(std::move(other))
{
	func = std::move(other.func);
}

void ObjectSurface::operator=(ObjectSurface&& other) noexcept
{
	if (this == &other)
		return;

	(Object)*this = (Object)other;

	func = std::move(other.func);
}

void ObjectSurface::generateMesh(float step)
{
	points.clear();
	points.reserve(int(x_bound / step * 2.0f * z_bound / step * 2.0f) + 10);

	lines.resize(2 * x_bound / step);
	for (float x = -x_bound, i = 0; x <= x_bound; x += step, i++)
	{
		lines[i].first.resize(2 * z_bound / step);
		lines[i].second = sf::Color(i * 10, 255, 255, 255);
		for (float z = -z_bound, j = 0; z <= z_bound; z += step, j++)
		{
			float y = func(x, z);
			points.push_back(vec3(x, y, z));
			lines[i].first[j] = points[points.size() - 1];
		}
	}
}

void ObjectSurface::render(sf::RenderWindow& window, const Camera& camera)
{
	std::vector<std::vector<vec3>> polygons;

	for (int i = 1; i < lines.size(); i++)
	{
		for (int j = 1; j < lines[i].first.size(); j++)
		{
			// point from local to world space
			vec3 new_point_1_1= lines[i].first[j];
			vec3 new_point_1_2= lines[i].first[j - 1];
			vec3 new_point_2_1 = lines[i - 1].first[j];
			vec3 new_point_2_2 = lines[i - 1].first[j - 1];
			//new_point = scale * new_point;
			//new_point = translation * new_point;
			//new_point = rotation * new_point;
			new_point_1_1 = transform * new_point_1_1;
			new_point_1_2 = transform * new_point_1_2;

			new_point_2_1 = transform * new_point_2_1;
			new_point_2_2 = transform * new_point_2_2;

			// point from world to view space
			new_point_1_1 = camera.getView() * new_point_1_1;
			new_point_1_2 = camera.getView() * new_point_1_2;

			new_point_2_1 = camera.getView() * new_point_2_1;
			new_point_2_2 = camera.getView() * new_point_2_2;

			mat4 proj = camera.getProjection();

			float w_1_1 = new_point_1_1.x * proj.mat[3][0] + new_point_1_1.y * proj.mat[3][1] + new_point_1_1.z * proj.mat[3][2] + proj.mat[3][3];
			float w_1_2 = new_point_1_2.x * proj.mat[3][0] + new_point_1_2.y * proj.mat[3][1] + new_point_1_2.z * proj.mat[3][2] + proj.mat[3][3];

			float w_2_1 = new_point_2_1.x * proj.mat[3][0] + new_point_2_1.y * proj.mat[3][1] + new_point_2_1.z * proj.mat[3][2] + proj.mat[3][3];
			float w_2_2 = new_point_2_2.x * proj.mat[3][0] + new_point_2_2.y * proj.mat[3][1] + new_point_2_2.z * proj.mat[3][2] + proj.mat[3][3];

			// point from view to clip space
			new_point_1_1 = camera.getProjection() * new_point_1_1;
			new_point_1_2 = camera.getProjection() * new_point_1_2;

			new_point_2_1 = camera.getProjection() * new_point_2_1;
			new_point_2_2 = camera.getProjection() * new_point_2_2;

			// point from clip to screen space
			new_point_1_1.x = (new_point_1_1.x / w_1_1 + 1) * 400;
			new_point_1_1.y = (-new_point_1_1.y / w_1_1 + 1) * 400;

			new_point_1_2.x = (new_point_1_2.x / w_1_2 + 1) * 400;
			new_point_1_2.y = (-new_point_1_2.y / w_1_2 + 1) * 400;


			new_point_2_1.x = (new_point_2_1.x / w_2_1 + 1) * 400;
			new_point_2_1.y = (-new_point_2_1.y / w_2_1 + 1) * 400;

			new_point_2_2.x = (new_point_2_2.x / w_2_2 + 1) * 400;
			new_point_2_2.y = (-new_point_2_2.y / w_2_2 + 1) * 400;

			if((new_point_1_1.z + new_point_1_2.z + new_point_2_1.z + new_point_2_2.z) / 4 < 1)
				polygons.push_back({new_point_1_1, new_point_1_2, new_point_2_1, new_point_2_2});
		}
	}

	std::sort(polygons.begin(), polygons.end(), [](std::vector<vec3> a, std::vector<vec3> b) {
		float d1 = a[0].z + a[1].z + a[2].z + a[3].z;
		d1 = d1 / 4;
		float d2 = b[0].z + b[1].z + b[2].z + b[3].z;
		d2 = d2 / 4;

		return d1 < d2;
		});

	for (int i = 0; i < polygons.size(); i++)
	{
		sf::ConvexShape polygon{ 4 };
		polygon.setPoint(0, sf::Vector2f(polygons[i][0].x, polygons[i][0].y));
		polygon.setPoint(1, sf::Vector2f(polygons[i][1].x, polygons[i][1].y));
		polygon.setPoint(2, sf::Vector2f(polygons[i][3].x, polygons[i][3].y));
		polygon.setPoint(3, sf::Vector2f(polygons[i][2].x, polygons[i][2].y));

		sf::Vertex line1[] =
		{
			sf::Vertex(sf::Vector2f(polygons[i][0].x, polygons[i][0].y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(polygons[i][1].x, polygons[i][1].y), sf::Color::Red)
		};

		sf::Vertex line2[] =
		{
			sf::Vertex(sf::Vector2f(polygons[i][2].x, polygons[i][2].y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(polygons[i][3].x, polygons[i][3].y), sf::Color::Red)
		};

		window.draw(polygon);

		window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
	}
}

Camera::Camera() : Object()
{
	view = mat4();
	projection = mat4::getPerspective(1.0f, 60.0f, 0.1f, 100.0f);
	//projection = mat4::getOrtographic(1.0f, 8.0f, 8.0f, 0.1f, 1000.0f);

	render_mode = RenderMode::Solid;
}

Camera::~Camera()
{
}

Camera::Camera(const Camera& other) : Object(other)
{
	view = other.view;
	projection = other.projection;
	render_mode = other.render_mode;
}

void Camera::operator=(const Camera& other)
{
	if (this == &other)
		return;

	(Object)*this = (Object)other;

	view = other.view;
	projection = other.projection;
	render_mode = other.render_mode;
}

Camera::Camera(Camera&& other) noexcept : Object(std::move(other))
{
	view = std::move(other.view);
	projection = std::move(other.projection);
	render_mode = other.render_mode;
}

void Camera::operator=(Camera&& other) noexcept
{
	if (this == &other)
		return;

	(Object)*this = (Object)other;

	view = std::move(other.view);
	projection = std::move(other.projection);
	render_mode = other.render_mode;
}

void Camera::rotateX(float angle)
{
	Object::rotateX(angle);

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::rotateY(float angle)
{
	Object::rotateY(angle);

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::rotateZ(float angle)
{
	Object::rotateZ(angle);

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::setRotation(vec3 angles)
{
	mat4 rotX;
	float radians = angles.x * 3.14f / 180.0f;
	rotX.mat[1][1] = cosf(radians);
	rotX.mat[1][2] = sinf(radians);
	rotX.mat[2][1] = -sinf(radians);
	rotX.mat[2][2] = cosf(radians);

	mat4 rotY;
	radians = angles.y * 3.14f / 180.0f;
	rotY.mat[0][0] = cosf(radians);
	rotY.mat[0][2] = sinf(radians);
	rotY.mat[2][0] = -sinf(radians);
	rotY.mat[2][2] = cosf(radians);

	mat4 rotZ;
	radians = angles.z * 3.14f / 180.0f;
	rotZ.mat[0][0] = cosf(radians);
	rotZ.mat[0][1] = sinf(radians);
	rotZ.mat[1][0] = -sinf(radians);
	rotZ.mat[1][1] = cosf(radians);

	rotation = rotZ * rotY * rotX;

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::translate(vec3 translating)
{
	Object::translate(translating);

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::scaling(float coeff)
{
	Object::scaling(coeff);

	view = mat4::getInverse(scale * translation * rotation);
}

vec3 Camera::getViewDirection() const
{
	vec3 dir = { transform.mat[0][2], transform.mat[1][2], transform.mat[2][2] };
	dir.normalize();

	return dir;
}

vec3 Camera::getRightDirection() const
{
	vec3 dir = { transform.mat[0][0], transform.mat[1][0], transform.mat[2][0] };
	dir.normalize();

	return dir;
}

Camera::RenderMode Camera::getRenderMode() const
{
	return render_mode;
}

void Camera::setRenderMode(Camera::RenderMode mode)
{
	render_mode = mode;
}

mat4 Camera::getView() const
{
	return view;
}

mat4 Camera::getProjection() const
{
	return projection;
}