#pragma once

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS>::Mat() {
	forAllElementsMutable([&](size_t row, size_t col) -> void {
		elements[row][col] = 0;
	});
}
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS>::Mat(T elements[ROWS][COLS]) {
	forAllElementsMutable([&](size_t row, size_t col) -> void {
		this->elements[row][col] = elements[row][col];
	});
}
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS>::Mat(std::initializer_list<std::initializer_list<T>> elements) {
	size_t i = 0, j = 0;
	for (const auto &row : elements) {
		for (const auto &element : row) {
			this->elements[i][j] = element;
			++j;
		}
		j = 0;
		++i;
	}
}

template<typename T, size_t ROWS, size_t COLS>
template<typename LambdaType>
void Mat<T, ROWS, COLS>::forAllElements(LambdaType lambda) const {
	for (size_t i = 0; i < ROWS; ++i) {
		for (size_t j = 0; j < COLS; ++j) {
			lambda(i, j);
		}
	}
}
template<typename T, size_t ROWS, size_t COLS>
template<typename LambdaType>
void Mat<T, ROWS, COLS>::forAllElementsMutable(LambdaType lambda) {
	for (size_t i = 0; i < ROWS; ++i) {
		for (size_t j = 0; j < COLS; ++j) {
			lambda(i, j);
		}
	}
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> &Mat<T, ROWS, COLS>::operator=(const Mat<T, ROWS, COLS> &otherMat) {
	forAllElementsMutable([&](size_t row, size_t col) -> void {
		elements[row][col] = otherMat.elements[row][col];
	});
	return *this;
}
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> &Mat<T, ROWS, COLS>::operator+=(const Mat<T, ROWS, COLS> &otherMat) {
	forAllElementsMutable([&](size_t row, size_t col) -> void {
		elements[row][col] += otherMat.elements[row][col];
	});
	return *this;
}
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> &Mat<T, ROWS, COLS>::operator-=(const Mat<T, ROWS, COLS> &otherMat) {
	forAllElementsMutable([&](size_t row, size_t col) -> void {
		elements[row][col] -= otherMat.elements[row][col];
	});
	return *this;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> Mat<T, ROWS, COLS>::operator+(const Mat<T, ROWS, COLS> &otherMat) const {
	Mat<T, ROWS, COLS> newMat = *this;
	return newMat += otherMat;
}
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> Mat<T, ROWS, COLS>::operator-(const Mat<T, ROWS, COLS> &otherMat) const {
	Mat<T, ROWS, COLS> newMat = *this;
	return newMat -= otherMat;
}
template<typename T, size_t ROWS, size_t COLS>
template<size_t OTHER_COLS>
Mat<T, ROWS, OTHER_COLS> Mat<T, ROWS, COLS>::operator*(const Mat<T, COLS, OTHER_COLS> &otherMat) const {
	Mat<T, ROWS, OTHER_COLS> newMat;
	for (size_t i = 0; i < ROWS; ++i) {
		for (size_t j = 0; j < OTHER_COLS; ++j) {
			T newElement = 0;
			for (size_t k = 0; k < COLS; ++k) {
				newElement += elements[i][k] * otherMat.elements[k][j];
			}
			newMat.elements[i][j] = newElement;
		}
	}
	return newMat;
}
template<typename T, size_t ROWS, size_t COLS>
Vec<T, ROWS> Mat<T, ROWS, COLS>::operator*(Vec<T, COLS> &vec) const {
	Vec<T, ROWS> newVec;
	for (size_t i = 0; i < ROWS; ++i) {
		T newElement = 0;
		for (size_t j = 0; j < COLS; ++j) {
			newElement += elements[i][j] * vec.get(j);
		}
		newVec.set(i, newElement);
	}
	return newVec;
}

template<typename T, size_t ROWS, size_t COLS>
bool Mat<T, ROWS, COLS>::operator==(const Mat<T, ROWS, COLS> &otherMat) const {
	for (size_t i = 0; i < ROWS; ++i) {
		for (size_t j = 0; j < COLS; ++j) {
			if (elements[i][j] != otherMat.elements[i][j]) {
				return false;
			}
		}
	}
	return true;
}

template<typename T, size_t ROWS, size_t COLS>
std::ostream &operator<<(std::ostream &outputStream, const Mat<T, ROWS, COLS> &mat) {
	outputStream << "[";
	for (size_t i = 1; i <= ROWS; ++i) {
		for (size_t j = 1; j <= COLS; ++j) {
			outputStream << mat.get(i, j);
			if (j < COLS) {
				outputStream << "  ";
			}
		}
		if (i < ROWS) {
			outputStream << std::endl << " ";
		}
	}
	outputStream << "]";
	return outputStream;
}

template <typename T, size_t ROWS, size_t COLS>
T Mat<T, ROWS, COLS>::get(size_t row, size_t col) const {
	return elements[row - 1][col - 1];
}
template <typename T, size_t ROWS, size_t COLS>
const T *Mat<T, ROWS, COLS>::getPtr() const {
	return &(elements[0][0]);
}
template<typename T, size_t ROWS, size_t COLS>
void Mat<T, ROWS, COLS>::set(size_t row, size_t col, T newElement) {
	elements[row - 1][col - 1] = newElement;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, COLS, ROWS> Mat<T, ROWS, COLS>::transpose() const {
	T newElements[COLS][ROWS];
	for (size_t i = 0; i < ROWS; ++i) {
		for (size_t j = 0; j < COLS; ++j) {
			newElements[j][i] = elements[i][j];
		}
	}
	Mat<T, COLS, ROWS> newMat(newElements);
	return newMat;
}
