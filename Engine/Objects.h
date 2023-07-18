#pragma once
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "MathLib.h"

class Object
{
public:
	Object();
	virtual ~Object() = 0;

	virtual void rotateX(float angle) = 0;
	virtual void rotateY(float angle) = 0;
	virtual void rotateZ(float angle) = 0;
	virtual void translate(vec3 translation) = 0;
	virtual void scaling(float coeff) = 0;

protected:
	mat4 translation;
	mat4 rotation;
	mat4 scale;
};

class Camera;

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

	void rotateX(float angle) override;
	void rotateY(float angle) override;
	void rotateZ(float angle) override;
	void translate(vec3 translation) override;
	void scaling(float coeff) override;

	void render(sf::RenderWindow& window, const Camera& camera);

private:
	std::vector<vec3> points;
	std::vector<std::vector<int>> indices;
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

	RenderMode getRenderMode() const;
	void setRenderMode(RenderMode mode);

	mat4 getView() const;
	mat4 getProjection() const;

private:
	mat4 view;
	mat4 projection;

	RenderMode render_mode;
};