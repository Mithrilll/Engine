#include <algorithm>

#include "Objects.h"

Object::Object()
{
	translation = mat4();
	rotation = mat4();
	scale = mat4();
}

Object::~Object()
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

ObjectMesh::ObjectMesh(std::vector<vec3> pts, std::vector<std::vector<int>> inds)
{
	translation = mat4();
	rotation = mat4();
	scale = mat4();
	points = pts;
	indices = inds;
}

ObjectMesh::ObjectMesh(const ObjectMesh& other)
{
	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	points = other.points;
	indices = other.indices;
}

void ObjectMesh::operator=(const ObjectMesh& other)
{
	if (this == &other)
		return;

	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	points = other.points;
	indices = other.indices;
}

ObjectMesh::ObjectMesh(ObjectMesh&& other) noexcept
{
	translation = std::move(other.translation);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	points = std::move(other.points);
	indices = std::move(other.indices);
}

void ObjectMesh::operator=(ObjectMesh&& other) noexcept
{
	if (this == &other)
		return;

	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	points = other.points;
	indices = other.indices;
}

void ObjectMesh::rotateX(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[1][1] = cosf(radians);
	addition.mat[1][2] = sinf(radians);
	addition.mat[2][1] = -sinf(radians);
	addition.mat[2][2] = cosf(radians);

	rotation *= addition;
}

void ObjectMesh::rotateY(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][1] = cosf(radians);
	addition.mat[0][3] = sinf(radians);
	addition.mat[2][0] = -sinf(radians);
	addition.mat[2][3] = cosf(radians);

	rotation *= addition;
}

void ObjectMesh::rotateZ(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][0] = cosf(radians);
	addition.mat[0][1] = sinf(radians);
	addition.mat[1][0] = -sinf(radians);
	addition.mat[1][1] = cosf(radians);

	rotation *= addition;
}

void ObjectMesh::translate(vec3 translation)
{
	mat4 addition;
	addition.mat[0][3] = translation.x;
	addition.mat[1][3] = translation.y;
	addition.mat[2][3] = translation.z;

	this->translation *= addition;
}

void ObjectMesh::scaling(float coeff)
{
	mat4 addition;
	addition.mat[0][0] = coeff;
	addition.mat[1][1] = coeff;
	addition.mat[2][2] = coeff;

	scale *= addition;
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
			new_point = scale * new_point;
			new_point = translation * new_point;
			new_point = rotation * new_point;

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
	std::sort(triangles.begin(), triangles.end(), [](std::pair<std::vector<vec3>, float> a, std::pair<std::vector<vec3>, float> b) { return a.second > b.second; });

	for (int i = 0; i < triangles.size(); i++)
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
}

Camera::Camera() : Object()
{
	view = mat4();
	projection = mat4::getPerspective(1.0f, 60.0f, 0.1f, 100.0f);
	//projection = mat4::getOrtographic(1.0f, 8.0f, 8.0f, 0.1f, 1000.0f);
}

Camera::~Camera()
{
}

Camera::Camera(const Camera& other)
{
	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	view = other.view;
	projection = other.projection;
}

void Camera::operator=(const Camera& other)
{
	if (this == &other)
		return;

	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	view = other.view;
	projection = other.projection;
}

Camera::Camera(Camera&& other) noexcept
{
	translation = std::move(other.translation);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	view = std::move(other.view);
	projection = std::move(other.projection);
}

void Camera::operator=(Camera&& other) noexcept
{
	if (this == &other)
		return;

	translation = std::move(other.translation);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	view = std::move(other.view);
	projection = std::move(other.projection);
}

void Camera::rotateX(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[1][1] = cosf(radians);
	addition.mat[1][2] = sinf(radians);
	addition.mat[2][1] = -sinf(radians);
	addition.mat[2][2] = cosf(radians);

	rotation *= addition;

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::rotateY(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][1] = cosf(radians);
	addition.mat[0][3] = sinf(radians);
	addition.mat[2][0] = -sinf(radians);
	addition.mat[2][3] = cosf(radians);

	rotation *= addition;

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::rotateZ(float angle)
{
	mat4 addition;
	float radians = angle * 3.14f / 180.0f;
	addition.mat[0][0] = cosf(radians);
	addition.mat[0][1] = sinf(radians);
	addition.mat[1][0] = -sinf(radians);
	addition.mat[1][1] = cosf(radians);

	rotation *= addition;

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::translate(vec3 translating)
{
	mat4 addition;
	addition.mat[0][3] = translating.x;
	addition.mat[1][3] = translating.y;
	addition.mat[2][3] = translating.z;

	translation *= addition;

	view = mat4::getInverse(scale * translation * rotation);
}

void Camera::scaling(float coeff)
{
	mat4 addition;
	addition.mat[0][0] = coeff;
	addition.mat[1][1] = coeff;
	addition.mat[2][2] = coeff;

	scale *= addition;

	view = mat4::getInverse(scale * translation * rotation);
}

mat4 Camera::getView() const
{
	return view;
}

mat4 Camera::getProjection() const
{
	return projection;
}
