#pragma once

template <typename T>
class Vector2
{
public:
	Vector2() = default;
	Vector2(T x, T y) : x(x), y(y) {}
	//Vector2(const Vector2& vect) : Vector2(vect.x, vect.y) {}

	template <typename T2>
	explicit operator Vector2<T2>()
	{
		return{ (T2)x,(T2)y };
	}
	T LenSq() const
	{
		return x * x + y * y;
	}
	T Len() const
	{
		return sqrt(LenSq());
	}
	Vector2& Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Vector2	GetNormalized() const
	{
		Vector2 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vector2	operator-() const
	{
		return Vector2(-x, -y);
	}
	/*Vector2& operator=(const Vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}*/
	Vector2& operator+=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2& operator-=(const Vector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vector2	operator+(const Vector2 &rhs) const
	{
		return Vector2(*this) += rhs;
	}
	Vector2	operator-(const Vector2 &rhs) const
	{
		return Vector2(*this) -= rhs;
	}
	Vector2& operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vector2	operator*(const T &rhs) const
	{
		return Vector2(*this) *= rhs;
	}
	T operator*(const Vector2& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	Vector2& operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vector2	operator/(const T &rhs) const
	{
		return Vector2(*this) /= rhs;
	}
	bool operator==(const Vector2 &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vector2 &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;