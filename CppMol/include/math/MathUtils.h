#pragma once

#include <cmath>
#include <stdexcept>

#include "Vec.h"
#include "Mat.h"

struct MathUtils {
	static const float PI;

	template<typename T, size_t SIZE>
	static Vec<T, SIZE> normalized(const Vec<T, SIZE> &vecToNormalize);

	template<typename T>
	static T toRadians(T degrees);

	template<typename T>
	static T toDegrees(T radians);

	template<typename T>
	static T determinant(const Mat<T, 2, 2> &matrix);

	template<typename T>
	static T determinant(const Mat<T, 3, 3> &matrix);

	struct MatGen {
		template<typename T, size_t SIZE>
		static Mat<T, SIZE, SIZE> identity();

		template<typename T, size_t SIZE>
		static Mat<T, SIZE, SIZE> scale(const Vec<T, SIZE - 1> &scaleValues);

		template<typename T, size_t SIZE>
		static Mat<T, SIZE, SIZE> translation(const Vec<T, SIZE - 1> &translationValues);

		//2D rotation
		template<typename T>
		static Mat<T, 3, 3> rotation(T rotationRadians);

		//3D rotation
		template<typename T>
		static Mat<T, 4, 4> rotation(const Vec<T, 3> &rotationRadians);

		template<typename T>
		static Mat<T, 4, 4> lookAt(
			const Vec<T, 3> &cameraPosition, 
			const Vec<T, 3> &cameraForward, 
			const Vec<T, 3> &cameraUp
		);

		template<typename T>
		static Mat<T, 4, 4> perspective(T fovYRadians, T screenWidth, T screenHeight, T nearPlane, T farPlane);

		template<typename T>
		static Mat<T, 3, 3> normal(const Mat<T, 4, 4> &modelMatrix);

		template<typename T>
		static Mat<T, 2, 2> inverse(const Mat<T, 2, 2> &original);

		template<typename T>
		static Mat<T, 3, 3> inverse(const Mat<T, 3, 3> &original);

		template<typename T>
		static Mat<T, 4, 4> inverse(const Mat<T, 4, 4> &original);
	};
};

#include "../../source/math/MathUtils.inl"
