#pragma once

template<typename T, size_t SIZE>
Vec<T, SIZE>::Vec() {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] = 0;
	}
}
template<typename T, size_t SIZE>
Vec<T, SIZE>::Vec(T elements[SIZE]) {
	for (size_t i = 0; i < SIZE; ++i) {
		this->elements[i] = elements[i];
	}
}
template<typename T, size_t SIZE>
Vec<T, SIZE>::Vec(T element) {
	for (size_t i = 0; i < SIZE; ++i) {
		this->elements[i] = element;
	}
}

template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator=(const Vec<T, SIZE> &otherVec) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] = otherVec.elements[i];
	}
	return *this;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator+=(const Vec<T, SIZE> &otherVec) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] += otherVec.elements[i];
	}
	return *this;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator-=(const Vec<T, SIZE> &otherVec) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] -= otherVec.elements[i];
	}
	return *this;
}

template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator+=(const T addend) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] += addend;
	}
	return *this;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator-=(const T addend) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] -= addend;
	}
	return *this;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator*=(const T addend) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] *= addend;
	}
	return *this;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> &Vec<T, SIZE>::operator/=(const T addend) {
	for (size_t i = 0; i < SIZE; ++i) {
		elements[i] /= addend;
	}
	return *this;
}

template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator+(const Vec<T, SIZE> &otherVec) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult += otherVec;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator-(const Vec<T, SIZE> &otherVec) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult -= otherVec;
}

template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator+(const T addend) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult += addend;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator-(const T addend) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult -= addend;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator*(const T addend) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult *= addend;
}
template<typename T, size_t SIZE>
Vec<T, SIZE> Vec<T, SIZE>::operator/(const T addend) const {
	Vec<T, SIZE> vectorResult = *this;
	return vectorResult /= addend;
}

template<typename T, size_t SIZE>
bool Vec<T, SIZE>::operator==(const Vec<T, SIZE> &otherVec) const {
	for (size_t i = 0; i < SIZE; ++i) {
		if (elements[i] != otherVec.elements[i]) {
			return false;
		}
	}
	return true;
}

template<typename T, size_t SIZE>
std::ostream &operator<<(std::ostream &outputStream, const Vec<T, SIZE> &vec) {
	outputStream << "<";
	for (size_t i = 0; i < SIZE; ++i) {
		outputStream << vec.elements[i];
		if (i < SIZE - 1) {
			outputStream << ", ";
		}
	}
	outputStream << ">";
	return outputStream;
}

template<typename T, size_t SIZE>
T Vec<T, SIZE>::dot(const Vec<T, SIZE> &otherVec) const {
	T result = 0;
	for (size_t i = 0; i < SIZE; ++i) {
		result += elements[i] * otherVec.elements[i];
	}
	return result;
}
template<typename T, size_t SIZE>
T Vec<T, SIZE>::magSquared() const {
	T result = 0;
	for (size_t i = 0; i < SIZE; ++i) {
		result += elements[i] * elements[i];
	}
	return result;
}
template<typename T, size_t SIZE>
T Vec<T, SIZE>::mag() const {
	return sqrt(magSquared());
}

template<typename T, size_t SIZE>
T Vec<T, SIZE>::get(size_t index) const {
	return elements[index];
}
template<typename T, size_t SIZE>
void Vec<T, SIZE>::set(size_t index, T newElement) {
	elements[index] = newElement;
}
