#pragma once

template<typename T>
Vec<T, 2>::Vec() {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] = 0;
	}
}
template<typename T>
Vec<T, 2>::Vec(T elements[2]) {
	for (size_t i = 0; i < 2; ++i) {
		this->elements[i] = elements[i];
	}
}
template<typename T>
Vec<T, 2>::Vec(T x, T y) {
	elements[0] = x;
	elements[1] = y;
}
template<typename T>
Vec<T, 2>::Vec(T element) {
	for (size_t i = 0; i < 2; ++i) {
		this->elements[i] = element;
	}
}

template<typename T>
Vec<T, 2> &Vec<T, 2>::operator=(const Vec<T, 2> &otherVec) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] = otherVec.get(i);
	}
	return *this;
}
template<typename T>
Vec<T, 2> &Vec<T, 2>::operator+=(const Vec<T, 2> &otherVec) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] += otherVec.get(i);
	}
	return *this;
}
template<typename T>
Vec<T, 2> &Vec<T, 2>::operator-=(const Vec<T, 2> &otherVec) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] -= otherVec.get(i);
	}
	return *this;
}

template<typename T>
Vec<T, 2> &Vec<T, 2>::operator+=(const T addend) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] += addend;
	}
	return *this;
}
template<typename T>
Vec<T, 2> &Vec<T, 2>::operator-=(const T addend) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] -= addend;
	}
	return *this;
}
template<typename T>
Vec<T, 2> &Vec<T, 2>::operator*=(const T addend) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] *= addend;
	}
	return *this;
}
template<typename T>
Vec<T, 2> &Vec<T, 2>::operator/=(const T addend) {
	for (size_t i = 0; i < 2; ++i) {
		elements[i] /= addend;
	}
	return *this;
}

template<typename T>
Vec<T, 2> Vec<T, 2>::operator+(const Vec<T, 2> &otherVec) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult += otherVec;
}
template<typename T>
Vec<T, 2> Vec<T, 2>::operator-(const Vec<T, 2> &otherVec) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult -= otherVec;
}

template<typename T>
Vec<T, 2> Vec<T, 2>::operator+(const T addend) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult += addend;
}
template<typename T>
Vec<T, 2> Vec<T, 2>::operator-(const T addend) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult -= addend;
}
template<typename T>
Vec<T, 2> Vec<T, 2>::operator*(const T addend) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult *= addend;
}
template<typename T>
Vec<T, 2> Vec<T, 2>::operator/(const T addend) const {
	Vec<T, 2> vectorResult = *this;
	return vectorResult /= addend;
}

template<typename T>
bool Vec<T, 2>::operator==(const Vec<T, 2> &otherVec) const {
	for (size_t i = 0; i < 2; ++i) {
		if (elements[i] != otherVec.get(i)) {
			return false;
		}
	}
	return true;
}

template<typename T>
std::ostream &operator<<(std::ostream &outputStream, const Vec<T, 2> &vec) {
	outputStream << "<";
	for (size_t i = 0; i < 2; ++i) {
		outputStream << vec.get(i);
		if (i < 2 - 1) {
			outputStream << ", ";
		}
	}
	outputStream << ">";
	return outputStream;
}

template<typename T>
T Vec<T, 2>::dot(const Vec<T, 2> &otherVec) const {
	T result = 0;
	for (size_t i = 0; i < 2; ++i) {
		result += elements[i] * otherVec.get(i);
	}
	return result;
}
template<typename T>
T Vec<T, 2>::magSquared() const {
	T result = 0;
	for (size_t i = 0; i < 2; ++i) {
		result += elements[i] * elements[i];
	}
	return result;
}
template<typename T>
T Vec<T, 2>::mag() const {
	return sqrt(magSquared());
}

template<typename T>
T Vec<T, 2>::get(size_t index) const {
	return elements[index];
}
template<typename T>
T Vec<T, 2>::getX() const {
	return elements[0];
}
template<typename T>
T Vec<T, 2>::getY() const {
	return elements[1];
}
template<typename T>
T Vec<T, 2>::getS() const {
	return elements[0];
}
template<typename T>
T Vec<T, 2>::getT() const {
	return elements[1];
}
template<typename T>
void Vec<T, 2>::set(size_t index, T newElement) {
	elements[index] = newElement;
}
template<typename T>
void Vec<T, 2>::setX(T newElement) {
	elements[0] = newElement;
}
template<typename T>
void Vec<T, 2>::setY(T newElement) {
	elements[1] = newElement;
}
template<typename T>
void Vec<T, 2>::setS(T newElement) {
	elements[0] = newElement;
}
template<typename T>
void Vec<T, 2>::setT(T newElement) {
	elements[1] = newElement;
}
