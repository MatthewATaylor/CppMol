#pragma once

template<typename T>
Vec<T, 4>::Vec() {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] = 0;
	}
}
template<typename T>
Vec<T, 4>::Vec(T elements[4]) {
	for (size_t i = 0; i < 4; ++i) {
		this->elements[i] = elements[i];
	}
}
template<typename T>
Vec<T, 4>::Vec(T x, T y, T z, T w) {
	elements[0] = x;
	elements[1] = y;
	elements[2] = z;
	elements[3] = w;
}
template<typename T>
Vec<T, 4>::Vec(T element) {
	for (size_t i = 0; i < 4; ++i) {
		this->elements[i] = element;
	}
}

template<typename T>
Vec<T, 4> &Vec<T, 4>::operator=(const Vec<T, 4> &otherVec) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] = otherVec.get(i);
	}
	return *this;
}
template<typename T>
Vec<T, 4> &Vec<T, 4>::operator+=(const Vec<T, 4> &otherVec) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] += otherVec.get(i);
	}
	return *this;
}
template<typename T>
Vec<T, 4> &Vec<T, 4>::operator-=(const Vec<T, 4> &otherVec) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] -= otherVec.get(i);
	}
	return *this;
}

template<typename T>
Vec<T, 4> &Vec<T, 4>::operator+=(const T addend) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] += addend;
	}
	return *this;
}
template<typename T>
Vec<T, 4> &Vec<T, 4>::operator-=(const T addend) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] -= addend;
	}
	return *this;
}
template<typename T>
Vec<T, 4> &Vec<T, 4>::operator*=(const T addend) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] *= addend;
	}
	return *this;
}
template<typename T>
Vec<T, 4> &Vec<T, 4>::operator/=(const T addend) {
	for (size_t i = 0; i < 4; ++i) {
		elements[i] /= addend;
	}
	return *this;
}

template<typename T>
Vec<T, 4> Vec<T, 4>::operator+(const Vec<T, 4> &otherVec) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult += otherVec;
}
template<typename T>
Vec<T, 4> Vec<T, 4>::operator-(const Vec<T, 4> &otherVec) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult -= otherVec;
}

template<typename T>
Vec<T, 4> Vec<T, 4>::operator+(const T addend) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult += addend;
}
template<typename T>
Vec<T, 4> Vec<T, 4>::operator-(const T addend) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult -= addend;
}
template<typename T>
Vec<T, 4> Vec<T, 4>::operator*(const T addend) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult *= addend;
}
template<typename T>
Vec<T, 4> Vec<T, 4>::operator/(const T addend) const {
	Vec<T, 4> vectorResult = *this;
	return vectorResult /= addend;
}

template<typename T>
bool Vec<T, 4>::operator==(const Vec<T, 4> &otherVec) const {
	for (size_t i = 0; i < 4; ++i) {
		if (elements[i] != otherVec.get(i)) {
			return false;
		}
	}
	return true;
}

template<typename T>
std::ostream &operator<<(std::ostream &outputStream, const Vec<T, 4> &vec) {
	outputStream << "<";
	for (size_t i = 0; i < 4; ++i) {
		outputStream << vec.get(i);
		if (i < 4 - 1) {
			outputStream << ", ";
		}
	}
	outputStream << ">";
	return outputStream;
}

template<typename T>
T Vec<T, 4>::dot(const Vec<T, 4> &otherVec) const {
	T result = 0;
	for (size_t i = 0; i < 4; ++i) {
		result += elements[i] * otherVec.get(i);
	}
	return result;
}
template<typename T>
T Vec<T, 4>::magSquared() const {
	T result = 0;
	for (size_t i = 0; i < 4; ++i) {
		result += elements[i] * elements[i];
	}
	return result;
}
template<typename T>
T Vec<T, 4>::mag() const {
	return sqrt(magSquared());
}

template<typename T>
T Vec<T, 4>::get(size_t index) const {
	return elements[index];
}
template<typename T>
T Vec<T, 4>::getX() const {
	return elements[0];
}
template<typename T>
T Vec<T, 4>::getY() const {
	return elements[1];
}
template<typename T>
T Vec<T, 4>::getZ() const {
	return elements[2];
}
template<typename T>
T Vec<T, 4>::getW() const {
	return elements[3];
}
template<typename T>
void Vec<T, 4>::set(size_t index, T newElement) {
	elements[index] = newElement;
}
template<typename T>
void Vec<T, 4>::setX(T newElement) {
	elements[0] = newElement;
}
template<typename T>
void Vec<T, 4>::setY(T newElement) {
	elements[1] = newElement;
}
template<typename T>
void Vec<T, 4>::setZ(T newElement) {
	elements[2] = newElement;
}
template<typename T>
void Vec<T, 4>::setW(T newElement) {
	elements[3] = newElement;
}
