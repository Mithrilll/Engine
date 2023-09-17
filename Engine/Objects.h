#pragma once
#include <vector>
#include <functional>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "MathLib.h"

class Camera;

class Object
{
public:
	Object();
	virtual ~Object();

	Object(const Object& other);
	void operator=(const Object& other);
	Object(Object&& other) noexcept;
	void operator=(Object&& other) noexcept;

	virtual void rotateX(float angle);
	virtual void rotateY(float angle);
	virtual void rotateZ(float angle);
	virtual void translate(vec3 translation);
	virtual void scaling(float coeff);
	virtual mat4 getTransform() const;

	virtual void render(sf::RenderWindow& window, const Camera& camera);

protected:
	mat4 translation;
	mat4 rotation;
	mat4 scale;
	mat4 transform;
};

class ObjectMesh :  public Object
{
public:
	ObjectMesh();
	~ObjectMesh() override;

	ObjectMesh(std::vector<vec3> pts, std::vector<std::vector<int>> inds);
	ObjectMesh(const ObjectMesh& other);
	void operator=(const ObjectMesh& other);
	ObjectMesh(ObjectMesh&& other) noexcept;
	void operator=(ObjectMesh&& other) noexcept;

	void render(sf::RenderWindow& window, const Camera& camera) override;

private:
	std::vector<vec3> points;
	std::vector<std::vector<int>> indices;
};

class ObjectSurface : public Object
{
public:
	ObjectSurface();
	~ObjectSurface() override;

	ObjectSurface(std::function<float(float, float)> f, float x, float z);
	ObjectSurface(const ObjectSurface& other);
	void operator=(const ObjectSurface& other);
	ObjectSurface(ObjectSurface&& other) noexcept;
	void operator=(ObjectSurface&& other) noexcept;

	void generatePoints(float step);

	void render(sf::RenderWindow& window, const Camera& camera) override;

private:
	std::function<float(float, float)> func = nullptr;

	float x_bound = 0.0f;
	float z_bound = 0.0f;

	std::vector<vec3> points;
	std::vector<vec3> clip_space_points;
};

class Camera : public Object
{
public:
	enum class RenderMode
	{
		Wireframe = 0,
		Solid
	};


	Camera();
	~Camera() override;

	//TODO: add constructor for projecttion matrices parameters

	Camera(const Camera& other);
	void operator=(const Camera& other);
	Camera(Camera&& other) noexcept;
	void operator=(Camera&& other) noexcept;

	void rotateX(float angle) override;
	void rotateY(float angle) override;
	void rotateZ(float angle) override;
	void setRotation(vec3 angles);
	void translate(vec3 translation) override;
	void scaling(float coeff) override;
	vec3 getViewDirection() const;
	vec3 getRightDirection() const;


	RenderMode getRenderMode() const;
	void setRenderMode(RenderMode mode);

	mat4 getView() const;
	mat4 getProjection() const;

private:
	mat4 view;
	mat4 projection;

	RenderMode render_mode;
};