#include <cmath>
#include <utility>

#include "MathLib.h"

vec3::vec3(float _x, float _y, float _z) :
	x(_x), y(_y), z(_z)
{
}

vec3::vec3(const vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void vec3::operator=(const vec3& other)
{
	if (this == &other)
		return;

	x = other.x;
	y = other.y;
	z = other.z;
}

vec3::vec3(vec3&& other) noexcept
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void vec3::operator=(vec3&& other) noexcept
{
	if (this == &other)
		return;

	x = other.x;
	y = other.y;
	z = other.z;
}

void vec3::normalize()
{
	float inv_len = 1.0f / this->length();
	(*this) *= inv_len;
}

float vec3::length() const
{
	return sqrtf(x*x + y*y + z*z);
}

vec3 vec3::operator+(const vec3& other)
{
	vec3 vec(*this);
	vec += other;

	return vec;
}

vec3 vec3::operator-(const vec3& other)
{
	vec3 vec(*this);
	vec -= other;

	return vec;
}

vec3 vec3::operator*(float coeff)
{
	vec3 vec(*this);
	vec *= coeff;

	return vec;
}

void vec3::operator+=(const vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void vec3::operator-=(const vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void vec3::operator*=(float coeff)
{
	x *= coeff;
	y *= coeff;
	z *= coeff;
}

mat3x3::mat3x3()
{
	mat = new float* [3];
	mat[0] = new float[3];
	mat[1] = new float[3];
	mat[2] = new float[3];

	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
}

mat3x3::~mat3x3()
{
	delete[] mat[0];
	delete[] mat[1];
	delete[] mat[2];
	delete[] mat;
}

mat3x3::mat3x3(const mat3x3& other)
{
	mat = new float* [3];
	mat[0] = new float[3];
	mat[1] = new float[3];
	mat[2] = new float[3];

	mat[0][0] = other.mat[0][0]; mat[0][1] = other.mat[0][1]; mat[0][2] = other.mat[0][2];
	mat[1][0] = other.mat[1][0]; mat[1][1] = other.mat[1][1]; mat[1][2] = other.mat[1][2];
	mat[2][0] = other.mat[2][0]; mat[2][1] = other.mat[2][1]; mat[2][2] = other.mat[2][2];
}

void mat3x3::operator=(const mat3x3& other)
{
	if (this == &other)
		return;

	mat[0][0] = other.mat[0][0]; mat[0][1] = other.mat[0][1]; mat[0][2] = other.mat[0][2];
	mat[1][0] = other.mat[1][0]; mat[1][1] = other.mat[1][1]; mat[1][2] = other.mat[1][2];
	mat[2][0] = other.mat[2][0]; mat[2][1] = other.mat[2][1]; mat[2][2] = other.mat[2][2];
}

mat3x3::mat3x3(mat3x3&& other) noexcept
{
	mat = other.mat;
	other.mat = nullptr;
}

void mat3x3::operator=(mat3x3&& other) noexcept
{
	float** temp = mat;
	mat = other.mat;
	other.mat = temp;
}

void mat3x3::transpose()
{
	mat3x3 temp(*this);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mat[i][j] = temp.mat[j][i];
}

mat3x3 mat3x3::operator+(const mat3x3& other)
{
	mat3x3 new_mat(*this);
	new_mat += other;

	return new_mat;
}

mat3x3 mat3x3::operator-(const mat3x3& other)
{
	mat3x3 new_mat(*this);
	new_mat -= other;

	return new_mat;
}

mat3x3 mat3x3::operator*(const mat3x3& other)
{
	mat3x3 new_mat(*this);
	new_mat += other;

	return new_mat;
}

vec3 mat3x3::operator*(const vec3& other)
{
	vec3 ans;

	ans.x = mat[0][0] * other.x + mat[0][1] * other.y + mat[0][2] * other.z;
	ans.y = mat[1][0] * other.x + mat[1][1] * other.y + mat[1][2] * other.z;
	ans.z = mat[2][0] * other.x + mat[2][1] * other.y + mat[2][2] * other.z;

	return ans;
}

mat3x3 mat3x3::operator*(float coeff)
{
	mat3x3 new_mat(*this);
	new_mat *= coeff;

	return new_mat;
}

void mat3x3::operator+=(const mat3x3& other)
{
	mat[0][0] += other.mat[0][0]; mat[0][1] += other.mat[0][1]; mat[0][2] += other.mat[0][2];
	mat[1][0] += other.mat[1][0]; mat[1][1] += other.mat[1][1]; mat[1][2] += other.mat[1][2];
	mat[2][0] += other.mat[2][0]; mat[2][1] += other.mat[2][1]; mat[2][2] += other.mat[2][2];
}

void mat3x3::operator-=(const mat3x3& other)
{
	mat[0][0] -= other.mat[0][0]; mat[0][1] -= other.mat[0][1]; mat[0][2] -= other.mat[0][2];
	mat[1][0] -= other.mat[1][0]; mat[1][1] -= other.mat[1][1]; mat[1][2] -= other.mat[1][2];
	mat[2][0] -= other.mat[2][0]; mat[2][1] -= other.mat[2][1]; mat[2][2] -= other.mat[2][2];
}

void mat3x3::operator*=(const mat3x3& other)
{
	mat3x3 new_mat(*this);

	for(int i=0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			new_mat.mat[i][j] = 0;
			for (int k = 0; k < 3; k++)
				new_mat.mat[i][j] += mat[i][k] * other.mat[k][j];
		}

	(*this) = std::move(new_mat);
}

void mat3x3::operator*=(float coeff)
{
	mat[0][0] *= coeff; mat[0][1] *= coeff; mat[0][2] *= coeff;
	mat[1][0] *= coeff; mat[1][1] *= coeff; mat[1][2] *= coeff;
	mat[2][0] *= coeff; mat[2][1] *= coeff; mat[2][2] *= coeff;
}