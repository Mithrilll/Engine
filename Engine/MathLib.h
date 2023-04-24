#pragma once

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

	void operator+=(const vec3& other);
	void operator-=(const vec3& other);
	void operator*=(float coeff);
};

struct mat3x3
{
	float** mat = nullptr;

	mat3x3();
	~mat3x3();
	mat3x3(const mat3x3& other);
	void operator=(const mat3x3& other);
	mat3x3(mat3x3&& other) noexcept;
	void operator=(mat3x3&& other) noexcept;

	void transpose();

	mat3x3 operator+(const mat3x3& other);
	mat3x3 operator-(const mat3x3& other);
	mat3x3 operator*(const mat3x3& other);
	vec3 operator*(const vec3& other);
	mat3x3 operator*(float coeff);

	void operator+=(const mat3x3& other);
	void operator-=(const mat3x3& other);
	void operator*=(const mat3x3& other);
	void operator*=(float coeff);
};