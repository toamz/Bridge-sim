#pragma once
#include <SFML/System/Vector2.hpp>

template <typename T>
struct vec2 {
	T x, y;

	vec2()
		: x(0), y(0) { }

	vec2(T val)
		: x(val), y(val) { }

	vec2(T x, T y) 
		: x(x), y(y) { }

	vec2(sf::Vector2<T> v)
		: x(v.x), y(v.y) { }

	vec2<T> operator+ (const vec2<T>& other) const {
		vec2<T> ret;
		ret.x = this->x + other.x;
		ret.y = this->y + other.y;
		return ret;
	}

	vec2<T>& operator+= (const vec2<T>& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	vec2<T> operator* (const vec2<T>& other) const {
		vec2<T> ret;
		ret.x = this->x * other.x;
		ret.y = this->y * other.y;
		return ret;
	}

	vec2<T> operator- (const vec2<T>& other) const {
		vec2<T> ret;
		ret.x = this->x - other.x;
		ret.y = this->y - other.y;
		return ret;
	}

	vec2<T> operator- () const {
		vec2<T> ret;
		ret.x = -this->x;
		ret.y = -this->y;
		return ret;
	}

	vec2<T> operator/ (const vec2<T>& other) const {
		vec2<T> ret;
		ret.x = this->x / other.x;
		ret.y = this->y / other.y;
		return ret;
	}

	vec2<T> operator* (const float& other) const {
		vec2<T> ret;
		ret.x = this->x * other;
		ret.y = this->y * other;
		return ret;
	}

	operator sf::Vector2<T>* () {
		return (sf::Vector2<T>*)this;
	}
	operator sf::Vector2<T> () {
		return *(sf::Vector2<T>*)this;
	}

	T length() const;
};

typedef vec2<float> vec2f;

#pragma warning( disable : 4244 26451 )

template<typename T>
inline T vec2<T>::length() const
{
	if (typeid(T) == typeid(float))
		return sqrtf(x * x + y * y);
	if (typeid(T) == typeid(double))
		return sqrt(x * x + y * y);

	return sqrtl(x * x + y * y);
}
