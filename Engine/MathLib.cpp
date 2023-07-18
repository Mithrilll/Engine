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
	return sqrtf(x * x + y * y + z * z);
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

vec3 vec3::operator*(const mat4& other)
{
	vec3 ans;

	ans.x = other.mat[0][0] * x + other.mat[1][0] * y + other.mat[2][0] * z + other.mat[0][3];
	ans.y = other.mat[0][1] * x + other.mat[1][1] * y + other.mat[2][1] * z + other.mat[1][3];
	ans.z = other.mat[0][2] * x + other.mat[1][2] * y + other.mat[2][2] * z + other.mat[2][3];

	return ans;
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

mat4::mat4()
{
	mat = new float* [4];
	mat[0] = new float[4];
	mat[1] = new float[4];
	mat[2] = new float[4];
	mat[3] = new float[4];

	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
}

mat4::~mat4()
{
	if (mat == nullptr)
		return;

	delete[] mat[0];
	delete[] mat[1];
	delete[] mat[2];
	delete[] mat[3];
	delete[] mat;
}

mat4::mat4(const mat4& other)
{
	mat = new float* [4];
	mat[0] = new float[4];
	mat[1] = new float[4];
	mat[2] = new float[4];
	mat[3] = new float[4];

	mat[0][0] = other.mat[0][0]; mat[0][1] = other.mat[0][1]; mat[0][2] = other.mat[0][2]; mat[0][3] = other.mat[0][3];
	mat[1][0] = other.mat[1][0]; mat[1][1] = other.mat[1][1]; mat[1][2] = other.mat[1][2]; mat[1][3] = other.mat[1][3];
	mat[2][0] = other.mat[2][0]; mat[2][1] = other.mat[2][1]; mat[2][2] = other.mat[2][2]; mat[2][3] = other.mat[2][3];
	mat[3][0] = other.mat[3][0]; mat[3][1] = other.mat[3][1]; mat[3][2] = other.mat[3][2]; mat[3][3] = other.mat[3][3];
}

void mat4::operator=(const mat4& other)
{
	if (this == &other)
		return;

	mat[0][0] = other.mat[0][0]; mat[0][1] = other.mat[0][1]; mat[0][2] = other.mat[0][2];	mat[0][3] = other.mat[0][3];
	mat[1][0] = other.mat[1][0]; mat[1][1] = other.mat[1][1]; mat[1][2] = other.mat[1][2];	mat[1][3] = other.mat[1][3];
	mat[2][0] = other.mat[2][0]; mat[2][1] = other.mat[2][1]; mat[2][2] = other.mat[2][2];	mat[2][3] = other.mat[2][3];
	mat[3][0] = other.mat[3][0]; mat[3][1] = other.mat[3][1]; mat[3][2] = other.mat[3][2];	mat[3][3] = other.mat[3][3];
}

mat4::mat4(mat4&& other) noexcept
{
	mat = other.mat;
	other.mat = nullptr;
}

void mat4::operator=(mat4&& other) noexcept
{
	float** temp = mat;
	mat = other.mat;
	other.mat = temp;
}

void mat4::transpose()
{
	mat4 temp(*this);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			mat[i][j] = temp.mat[j][i];
}

mat4 mat4::operator+(const mat4& other)
{
	mat4 new_mat(*this);
	new_mat += other;

	return new_mat;
}

mat4 mat4::operator-(const mat4& other)
{
	mat4 new_mat(*this);
	new_mat -= other;

	return new_mat;
}

mat4 mat4::operator*(const mat4& other)
{
	mat4 new_mat(*this);
	new_mat *= other;

	return new_mat;
}

vec3 mat4::operator*(const vec3& other)
{
	vec3 ans;

	ans.x = mat[0][0] * other.x + mat[0][1] * other.y + mat[0][2] * other.z + mat[0][3];
	ans.y = mat[1][0] * other.x + mat[1][1] * other.y + mat[1][2] * other.z + mat[1][3];
	ans.z = mat[2][0] * other.x + mat[2][1] * other.y + mat[2][2] * other.z + mat[2][3];

	return ans;
}

mat4 mat4::operator*(float coeff)
{
	mat4 new_mat(*this);
	new_mat *= coeff;

	return new_mat;
}

void mat4::operator+=(const mat4& other)
{
	mat[0][0] += other.mat[0][0]; mat[0][1] += other.mat[0][1]; mat[0][2] += other.mat[0][2]; mat[0][3] += other.mat[0][3];
	mat[1][0] += other.mat[1][0]; mat[1][1] += other.mat[1][1]; mat[1][2] += other.mat[1][2]; mat[1][3] += other.mat[1][3];
	mat[2][0] += other.mat[2][0]; mat[2][1] += other.mat[2][1]; mat[2][2] += other.mat[2][2]; mat[2][3] += other.mat[2][3];
	mat[3][0] += other.mat[3][0]; mat[3][1] += other.mat[3][1]; mat[3][2] += other.mat[3][2]; mat[3][3] += other.mat[3][3];
}

void mat4::operator-=(const mat4& other)
{
	mat[0][0] -= other.mat[0][0]; mat[0][1] -= other.mat[0][1]; mat[0][2] -= other.mat[0][2]; mat[0][3] -= other.mat[0][3];
	mat[1][0] -= other.mat[1][0]; mat[1][1] -= other.mat[1][1]; mat[1][2] -= other.mat[1][2]; mat[1][3] -= other.mat[1][3];
	mat[2][0] -= other.mat[2][0]; mat[2][1] -= other.mat[2][1]; mat[2][2] -= other.mat[2][2]; mat[2][3] -= other.mat[2][3];
	mat[3][0] -= other.mat[3][0]; mat[3][1] -= other.mat[3][1]; mat[3][2] -= other.mat[3][2]; mat[3][3] -= other.mat[3][3];
}

void mat4::operator*=(const mat4& other)
{
	mat4 new_mat(*this);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			new_mat.mat[i][j] = 0;
			for (int k = 0; k < 4; k++)
				new_mat.mat[i][j] += mat[i][k] * other.mat[k][j];
		}
	}

	(*this) = std::move(new_mat);
}

void mat4::operator*=(float coeff)
{
	mat[0][0] *= coeff; mat[0][1] *= coeff; mat[0][2] *= coeff;	 mat[0][3] *= coeff;
	mat[1][0] *= coeff; mat[1][1] *= coeff; mat[1][2] *= coeff;	 mat[1][3] *= coeff;
	mat[2][0] *= coeff; mat[2][1] *= coeff; mat[2][2] *= coeff;	 mat[2][3] *= coeff;
	mat[3][0] *= coeff; mat[3][1] *= coeff; mat[3][2] *= coeff;	 mat[3][3] *= coeff;
}

mat4 mat4::getInverse(const mat4& matrix)
{
	mat4 ans;
	
	float det = matrix.getDeterminant();

	if (det == 0.0f)
	{
		ans.mat[0][0] = 0; ans.mat[0][1] = 0; ans.mat[0][2] = 0; ans.mat[0][3] = 0;
		ans.mat[1][0] = 0; ans.mat[1][1] = 0; ans.mat[1][2] = 0; ans.mat[1][3] = 0;
		ans.mat[2][0] = 0; ans.mat[2][1] = 0; ans.mat[2][2] = 0; ans.mat[2][3] = 0;
		ans.mat[3][0] = 0; ans.mat[3][1] = 0; ans.mat[3][2] = 0; ans.mat[3][3] = 0;
	}
	else
	{
		float** m = new float* [3];
		m[0] = new float[3];
		m[1] = new float[3];
		m[2] = new float[3];

		//---------------

		m[0][0] = matrix.mat[1][1]; m[0][1] = matrix.mat[1][2]; m[0][2] = matrix.mat[1][3];
		m[1][0] = matrix.mat[2][1]; m[1][1] = matrix.mat[2][2]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][1]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A00 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[1][0]; m[0][1] = matrix.mat[1][2]; m[0][2] = matrix.mat[1][3];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][2]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A01 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[1][0]; m[0][1] = matrix.mat[1][1]; m[0][2] = matrix.mat[1][3];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][1]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][3];

		float A02 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[1][0]; m[0][1] = matrix.mat[1][1]; m[0][2] = matrix.mat[1][2];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][1]; m[1][2] = matrix.mat[2][2];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][2];

		float A03 = -getDeterminant(m, 3) / det;

		//---------------

		m[0][0] = matrix.mat[0][1]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[2][1]; m[1][1] = matrix.mat[2][2]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][1]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A10 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][2]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A11 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][1]; m[1][2] = matrix.mat[2][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][3];

		float A12 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][2];
		m[1][0] = matrix.mat[2][0]; m[1][1] = matrix.mat[2][1]; m[1][2] = matrix.mat[2][2];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][2];

		float A13 = getDeterminant(m, 3) / det;

		//---------------

		m[0][0] = matrix.mat[0][1]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][1]; m[1][1] = matrix.mat[1][2]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[3][1]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A20 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][2]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][2];	m[2][2] = matrix.mat[3][3];

		float A21 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][1]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][3];

		float A22 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][2];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][1]; m[1][2] = matrix.mat[1][2];
		m[2][0] = matrix.mat[3][0]; m[2][1] = matrix.mat[3][1];	m[2][2] = matrix.mat[3][2];

		float A23 = -getDeterminant(m, 3) / det;

		//---------------

		m[0][0] = matrix.mat[0][1]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][1]; m[1][1] = matrix.mat[1][2]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[2][1];	m[2][1] = matrix.mat[2][2]; m[2][2] = matrix.mat[2][3];

		float A30 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][2]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][2]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[2][0];	m[2][1] = matrix.mat[2][2]; m[2][2] = matrix.mat[2][3];

		float A31 = getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][3];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][1]; m[1][2] = matrix.mat[1][3];
		m[2][0] = matrix.mat[2][0];	m[2][1] = matrix.mat[2][1]; m[2][2] = matrix.mat[2][3];

		float A32 = -getDeterminant(m, 3) / det;

		m[0][0] = matrix.mat[0][0]; m[0][1] = matrix.mat[0][1]; m[0][2] = matrix.mat[0][2];
		m[1][0] = matrix.mat[1][0]; m[1][1] = matrix.mat[1][1]; m[1][2] = matrix.mat[1][2];
		m[2][0] = matrix.mat[2][0];	m[2][1] = matrix.mat[2][1]; m[2][2] = matrix.mat[2][2];

		float A33 = getDeterminant(m, 3) / det;

		delete[] m[0];
		delete[] m[1];
		delete[] m[2];
		delete[] m;

		ans.mat[0][0] = A00; ans.mat[0][1] = A10; ans.mat[0][2] = A20; ans.mat[0][3] = A30;
		ans.mat[1][0] = A01; ans.mat[1][1] = A11; ans.mat[1][2] = A21; ans.mat[1][3] = A31;
		ans.mat[2][0] = A02; ans.mat[2][1] = A12; ans.mat[2][2] = A22; ans.mat[2][3] = A32;
		ans.mat[3][0] = A03; ans.mat[3][1] = A13; ans.mat[3][2] = A23; ans.mat[3][3] = A33;
	}

	return ans;
}

mat4 mat4::getPerspective(float aspect, float FOV, float near, float far)
{
	mat4 ans;

	float radians = FOV * 3.14f / 180.0f;

	ans.mat[0][0] = 1.0f / (aspect * tanf(radians / 2.0f));
	ans.mat[1][1] = 1.0f / tanf(radians / 2.0f);
	ans.mat[2][2] = -(far + near) / (far - near);
	ans.mat[2][3] = -2.0f * far * near / (far - near);
	ans.mat[3][2] = -1.0f;
	ans.mat[3][3] = 0.0f;

	return ans;
}

mat4 mat4::getPerspective(float l, float r, float b, float t, float n, float f)
{
	mat4 ans;

	ans.mat[0][0] =	2.0f * n / (r - l);
	ans.mat[1][1] = 2.0f * n / (t - b);
	ans.mat[0][2] = (r + l) / (r - l);
	ans.mat[1][2] = (t + b) / (t - b);
	ans.mat[2][2] = -(f + n) / (f - n);
	ans.mat[3][2] = -1;
	ans.mat[2][3] = -2 * f * n / (f - n);
	ans.mat[3][3] = 0.0f;

	return mat4();
}

mat4 mat4::getOrtographic(float aspect, float width, float height, float near, float far)
{
	mat4 ans;

	ans.mat[0][0] = 1.0f / (aspect * width);
	ans.mat[1][1] = 1.0f / height;
	ans.mat[2][2] = -2.0f / (far - near);
	ans.mat[2][3] = -(far + near) / (far - near);
	ans.mat[3][3] = 1.0f;

	return ans;

	return mat4();
}

float mat4::getDeterminant() const
{
	return getDeterminant(mat, 4);
}

float mat4::getDeterminant(float** mat, int size)
{
	if (size < 1)
		return 0.0f;

	if (size == 1)
		return mat[0][0];

	if (size == 2)
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	float sum = 0;

	for (int i = 0; i < size; i++)
	{
		float** new_mat = new float* [size - 1];
		for (int j = 0; j < size - 1; j++)
			new_mat[j] = new float[size - 1];

		for (int j = 1; j < size; j++)
		{
			int ind_k = 0;
			for (int k = 0; k < size; k++)
			{
				if (k == i)
					continue;

				new_mat[j-1][ind_k] = mat[j][k];
				ind_k++;
			}

		}

		int coeff = 1;
		if (i % 2 == 1)
			coeff = -1;

		sum += coeff * mat[0][i] * getDeterminant(new_mat, size - 1);

		for (int j = 0; j < size - 1; j++)
			delete[] new_mat[j];
		delete[] new_mat;
	}

	return sum;
}
