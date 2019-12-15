#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include <cmath>


class Vector2D
{
public:
	Vector2D(float x = 0.0, float y = 0.0) : m_x(x), m_y(y) {}
	~Vector2D();

	float getX() const { return m_x; }
	float getY() const { return m_y; }

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	float length() { return std::sqrt(m_x * m_x + m_y * m_y); }

	void normalize() {
		float l = length();
		if (l > 0) {
			*this *= 1 / l;
		}
	}

	Vector2D operator+(const Vector2D& v2) const {
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
	}

	Vector2D operator-(const Vector2D& v2) const {
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
	}

	Vector2D& operator+=(const Vector2D& v2) {
		m_x += v2.m_x;
		m_y += v2.m_y;

		return *this;
	}

	Vector2D& operator-=(const Vector2D& v2) {
		m_x -= v2.m_x;
		m_y -= v2.m_y;

		return *this;
	}

	Vector2D operator*(float scalar) {
		return Vector2D(m_x * scalar, m_y * scalar);
	}

	Vector2D& operator*=(float scalar) {
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2D operator/(float scalar) {
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/=(float scalar) {
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

private:
	float m_x;
	float m_y;
};

#endif
