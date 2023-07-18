#pragma once

struct mat4;

struct vec3
{
	float x;
	float y;
	float z;

	vec3(float _x = 0, float _y = 0, float _z = 0);
	vec3(const vec3& other);
	void operator=(const vec3& other);
	vec3(vec3&& other) noexcept;
	void operator=(vec3&& other) noexcept;

	void normalize();

	float length() const;

	vec3 operator+(const vec3& other);
	vec3 operator-(const vec3& other);
	vec3 operator*(float coeff);
	vec3 operator*(const mat4& other);

	void operator+=(const vec3& other);
	void operator-=(const vec3& other);
	void operator*=(float coeff);
};

struct mat4
{
	float** mat = nullptr;

	mat4();
	~mat4();
	mat4(const mat4& other);
	void operator=(const mat4& other);
	mat4(mat4&& other) noexcept;
	void operator=(mat4&& other) noexcept;

	void transpose();

	mat4 operator+(const mat4& other);
	mat4 operator-(const mat4& other);
	mat4 operator*(const mat4& other);
	vec3 operator*(const vec3& other);
	mat4 operator*(float coeff);

	void operator+=(const mat4& other);
	void operator-=(const mat4& other);
	void operator*=(const mat4& other);
	void operator*=(float coeff);

	float getDeterminant() const;

	static mat4 getInverse(const mat4& matrix);

	static mat4 getPerspective(float aspect, float FOV, float near, float far);
	static mat4 getPerspective(float l, float r, float b, float t, float n, float f);
	static mat4 getOrtographic(float aspect, float width, float height, float near, float far);

private:

	static float getDeterminant(float** mat, int size);
};