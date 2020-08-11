#pragma once

template<typename T>
class Vec<T, 2>;

template<typename T>
std::ostream &operator<<(std::ostream &outputStream, const Vec<T, 2> &vec);

template<typename T>
class Vec<T, 2> {
private:
	T elements[2];

public:
	Vec();
	Vec(T elements[2]);
	Vec(T x, T y);
	Vec(T element);

	Vec<T, 2> &operator=(const Vec<T, 2> &otherVec);
	Vec<T, 2> &operator+=(const Vec<T, 2> &otherVec);
	Vec<T, 2> &operator-=(const Vec<T, 2> &otherVec);

	Vec<T, 2> &operator+=(const T addend);
	Vec<T, 2> &operator-=(const T addend);
	Vec<T, 2> &operator*=(const T addend);
	Vec<T, 2> &operator/=(const T addend);

	Vec<T, 2> operator+(const Vec<T, 2> &otherVec) const;
	Vec<T, 2> operator-(const Vec<T, 2> &otherVec) const;

	Vec<T, 2> operator+(const T addend) const;
	Vec<T, 2> operator-(const T addend) const;
	Vec<T, 2> operator*(const T addend) const;
	Vec<T, 2> operator/(const T addend) const;

	bool operator==(const Vec<T, 2> &otherVec) const;

	friend std::ostream &operator<<<T, 2>(std::ostream &outputStream, const Vec<T, 2> &vec);

	T dot(const Vec<T, 2> &otherVec) const;
	T magSquared() const;
	T mag() const;

	T get(size_t index) const;
	T getX() const;
	T getY() const;
	T getS() const;
	T getT() const;
	void set(size_t index, T newElement);
	void setX(T newElement);
	void setY(T newElement);
	void setS(T newElement);
	void setT(T newElement);
};

#include "../../source/math/Vec_2.inl"
