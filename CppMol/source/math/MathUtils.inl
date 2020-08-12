#pragma once

template<typename T, size_t SIZE>
Vec<T, SIZE> MathUtils::normalized(const Vec<T, SIZE> &vecToNormalize) {
	T magnitude = vecToNormalize.mag();
	return vecToNormalize / magnitude;
}

template<typename T>
T MathUtils::toRadians(T degrees) {
	return (degrees / 180) * PI;
}

template<typename T>
T MathUtils::toDegrees(T radians) {
	return (radians / PI) * 180;
}

template<typename T>
T MathUtils::determinant(const Mat<T, 2, 2> &matrix) {
	return
		matrix.get(1, 1) * matrix.get(2, 2) -
		matrix.get(1, 2) * matrix.get(2, 1);
}

template<typename T>
T MathUtils::determinant(const Mat<T, 3, 3> &matrix) {
	T det = 0;
	for (unsigned int i = 1; i <= 3; ++i) {
		unsigned int leftCol = (i == 1 ? 2 : 1);
		unsigned int rightCol = (i == 3 ? 2 : 3);

		T det2 =
			matrix.get(2, leftCol) * matrix.get(3, rightCol) -
			matrix.get(2, rightCol) * matrix.get(3, leftCol);

		if (i == 2) {
			det2 *= -1;
		}

		det += matrix.get(1, i) * det2;
	}
	return det;
}

template<typename T, size_t SIZE>
Mat<T, SIZE, SIZE> MathUtils::MatGen::identity() {
	Mat<T, SIZE, SIZE> outputMatrix;
	for (size_t i = 1; i <= SIZE; ++i) {
		outputMatrix.set(i, i, 1);
	}
	return outputMatrix;
}

template<typename T, size_t SIZE>
Mat<T, SIZE, SIZE> MathUtils::MatGen::scale(const Vec<T, SIZE - 1> &scaleValues) {
	Mat<T, SIZE, SIZE> outputMatrix;
	for (size_t i = 1; i <= SIZE - 1; ++i) {
		outputMatrix.set(i, i, scaleValues.get(i - 1));
	}
	outputMatrix.set(SIZE, SIZE, 1);
	return outputMatrix;
}

template<typename T, size_t SIZE>
Mat<T, SIZE, SIZE> MathUtils::MatGen::translation(const Vec<T, SIZE - 1> &translationValues) {
	Mat<T, SIZE, SIZE> outputMatrix = identity<T, SIZE>();
	for (size_t i = 1; i <= SIZE - 1; ++i) {
		outputMatrix.set(SIZE, i, translationValues.get(i - 1));
	}
	return outputMatrix;
}

template<typename T>
Mat<T, 3, 3> MathUtils::MatGen::rotation(T rotationRadians) {
	T sinTheta = std::sin(rotationRadians);
	T cosTheta = std::cos(rotationRadians);
	return Mat<T, 3, 3>(
		{
			{  cosTheta, sinTheta, 0 },
			{ -sinTheta, cosTheta, 0 },
			{  0,						  0,						 1 }
		}
	);
}

template<typename T>
Mat<T, 4, 4> MathUtils::MatGen::rotation(const Vec<T, 3> &rotationRadians) {
	T sinX = std::sin(rotationRadians.getX());
	T cosX = std::cos(rotationRadians.getX());
	T sinY = std::sin(rotationRadians.getY());
	T cosY = std::cos(rotationRadians.getY());
	T sinZ = std::sin(rotationRadians.getZ());
	T cosZ = std::cos(rotationRadians.getZ());

	//Rz * Ry * Rx
	return Mat<T, 4, 4>(
		{
			{  cosZ * cosY,                      -sinZ * cosY,                       sinY,        0 },
			{  cosZ * sinY * sinX + sinZ * cosX, -sinZ * sinY * sinX + cosZ * cosX, -cosY * sinX, 0 },
			{ -cosZ * sinY * cosX + sinZ * sinX,  sinZ * sinY * cosX + cosZ * sinX,  cosY * cosX, 0 },
			{  0,                                 0,                                 0,           1 }
		}
	);
}

template<typename T>
Mat<T, 4, 4> MathUtils::MatGen::rotationAboutAxis(const Vec<T, 3> &axis, T rotationRadians) {
	Vec<T, 3> normalizedAxis = axis / axis.mag();

	T sinTheta = std::sin(rotationRadians);
	T cosTheta = std::cos(rotationRadians);

	return Mat<T, 4, 4>(
		{
			{
				normalizedAxis.getX() * normalizedAxis.getX() * (1 - cosTheta) + cosTheta,
				normalizedAxis.getX() * normalizedAxis.getY() * (1 - cosTheta) + normalizedAxis.getZ() * sinTheta,
				normalizedAxis.getX() * normalizedAxis.getZ() * (1 - cosTheta) - normalizedAxis.getY() * sinTheta,
				0
			},
			{
				normalizedAxis.getX() * normalizedAxis.getY() * (1 - cosTheta) - normalizedAxis.getZ() * sinTheta,
				normalizedAxis.getY() * normalizedAxis.getY() * (1 - cosTheta) + cosTheta,
				normalizedAxis.getY() * normalizedAxis.getZ() * (1 - cosTheta) + normalizedAxis.getX() * sinTheta,
				0
			},
			{
				normalizedAxis.getX() * normalizedAxis.getZ() * (1 - cosTheta) + normalizedAxis.getY() * sinTheta,
				normalizedAxis.getY() * normalizedAxis.getZ() * (1 - cosTheta) - normalizedAxis.getX() * sinTheta,
				normalizedAxis.getZ() * normalizedAxis.getZ() * (1 - cosTheta) + cosTheta,
				0
			},
			{ 0, 0, 0, 1 }
		}
	);
}

template<typename T>
Mat<T, 4, 4> MathUtils::MatGen::lookAt(
	const Vec<T, 3> &cameraPosition, 
	const Vec<T, 3> &cameraForward, 
	const Vec<T, 3> &cameraUp
) {
	/*
	Perform 2 transformations:
		1.) Change basis vectors to camera coordinates
		2.) Translate points according to camera position

		Because we are going from coordinates in one coordinate space to the view space using the camera axes,
		the final transformation is equal to the inverse of the product of the two matrices.

	Below is the product of the above two transformation matrices
	*/
	Vec<T, 3> forward = normalized(cameraPosition - cameraForward);
	Vec<T, 3> right = normalized(cameraUp.cross(forward));
	Vec<T, 3> up = forward.cross(right);

	Mat<T, 4, 4> outputMatrix;

	outputMatrix.set(1, 1, right.getX());
	outputMatrix.set(2, 1, right.getY());
	outputMatrix.set(3, 1, right.getZ());

	outputMatrix.set(1, 2, up.getX());
	outputMatrix.set(2, 2, up.getY());
	outputMatrix.set(3, 2, up.getZ());

	outputMatrix.set(1, 3, forward.getX());
	outputMatrix.set(2, 3, forward.getY());
	outputMatrix.set(3, 3, forward.getZ());

	outputMatrix.set(4, 1, right.dot(cameraPosition) * -1);
	outputMatrix.set(4, 2, up.dot(cameraPosition) * -1);
	outputMatrix.set(4, 3, forward.dot(cameraPosition) * -1);
	outputMatrix.set(4, 4, 1);

	return outputMatrix;
}

template<typename T>
Mat<T, 4, 4> MathUtils::MatGen::perspective(T fovYRadians, T screenWidth, T screenHeight, T nearPlane, T farPlane) {
	T aspectRatio = screenWidth / screenHeight;
	T topOverNear = std::tan(fovYRadians / 2);
	T top = topOverNear * nearPlane;
	T bottom = -top;
	T right = top * aspectRatio;
	T left = -right;

	Mat<T, 4, 4> outputMatrix;
	outputMatrix.set(1, 1, 1 / (aspectRatio * topOverNear));
	outputMatrix.set(2, 2, 1 / topOverNear);
	outputMatrix.set(3, 3, -(farPlane + nearPlane) / (farPlane - nearPlane));
	outputMatrix.set(3, 4, -1);
	outputMatrix.set(4, 3, -2 * farPlane * nearPlane / (farPlane - nearPlane));

	return outputMatrix;
}

template<typename T>
Mat<T, 3, 3> MathUtils::MatGen::normal(const Mat<T, 4, 4> &modelMatrix) {
	Mat<T, 4, 4> transposedInverse = inverse(modelMatrix).transpose();
	return Mat<T, 3, 3>(
		{
			{ transposedInverse.get(1, 1), transposedInverse.get(1, 2), transposedInverse.get(1, 3) },
			{ transposedInverse.get(2, 1), transposedInverse.get(2, 2), transposedInverse.get(2, 3) },
			{ transposedInverse.get(3, 1), transposedInverse.get(3, 2), transposedInverse.get(3, 3) }
		}
	);
}

template<typename T>
Mat<T, 2, 2> MathUtils::MatGen::inverse(const Mat<T, 2, 2> &original) {
	T det = determinant(original);
	return Mat<T, 2, 2>(
		{
			{  original.get(2, 2) / det, -original.get(1, 2) / det },
			{ -original.get(2, 1) / det,  original.get(1, 1) / det }
		}
	);
}

template<typename T>
Mat<T, 3, 3> MathUtils::MatGen::inverse(const Mat<T, 3, 3> &original) {
	Mat<T, 3, 3> outputMatrix;

	//Create matrix of minors
	for (unsigned int i = 1; i <= 3; ++i) {
		for (unsigned int j = 1; j <= 3; ++j) {
			unsigned int topRow = (i == 1 ? 2 : 1);
			unsigned int bottomRow = (i == 3 ? 2 : 3);

			unsigned int leftCol = (j == 1 ? 2 : 1);
			unsigned int rightCol = (j == 3 ? 2 : 3);
			
			T det =
				original.get(topRow, leftCol) * original.get(bottomRow, rightCol) -
				original.get(topRow, rightCol) * original.get(bottomRow, leftCol);

			outputMatrix.set(i, j, det);
		}
	}

	//Create matrix of cofactors
	outputMatrix.set(1, 2, outputMatrix.get(1, 2) * -1);
	outputMatrix.set(2, 1, outputMatrix.get(2, 1) * -1);
	outputMatrix.set(2, 3, outputMatrix.get(2, 3) * -1);
	outputMatrix.set(3, 2, outputMatrix.get(3, 2) * -1);

	T det =
		original.get(1, 1) * outputMatrix.get(1, 1) +
		original.get(1, 2) * outputMatrix.get(1, 2) +
		original.get(1, 3) * outputMatrix.get(1, 3);

	outputMatrix = outputMatrix.transpose();
	for (unsigned int i = 1; i <= 3; ++i) {
		for (unsigned int j = 1; j <= 3; ++j) {
			outputMatrix.set(i, j, outputMatrix.get(i, j) / det);
		}
	}

	return outputMatrix;
}

template<typename T>
Mat<T, 4, 4> MathUtils::MatGen::inverse(const Mat<T, 4, 4> &original) {
	Mat<T, 4, 4> outputMatrix;

	//Create matrix of cofactors of minors
	for (unsigned int i = 1; i <= 4; ++i) {
		for (unsigned int j = 1; j <= 4; ++j) {
			unsigned int topRow = (i == 1 ? 2 : 1);
			unsigned int middleRow = (i <= 2 ? 3 : 2);
			unsigned int bottomRow = (i == 4 ? 3 : 4);

			unsigned int leftCol = (j == 1 ? 2 : 1);
			unsigned int middleCol = (j <= 2 ? 3 : 2);
			unsigned int rightCol = (j == 4 ? 3 : 4);

			T det = determinant(
				Mat<T, 3, 3>(
					{
						{
							original.get(topRow, leftCol),
							original.get(topRow, middleCol),
							original.get(topRow, rightCol)
						},
						{
							original.get(middleRow, leftCol),
							original.get(middleRow, middleCol),
							original.get(middleRow, rightCol)
						},
						{
							original.get(bottomRow, leftCol),
							original.get(bottomRow, middleCol),
							original.get(bottomRow, rightCol)
						}
					}
					)
			);

			//Cofactors
			if (
				((i == 1 || i == 3) && (j == 2 || j == 4)) ||
				((i == 2 || i == 4) && (j == 1 || j == 3))
				) {
				det *= -1;
			}

			outputMatrix.set(i, j, det);
		}
	}

	T det =
		original.get(1, 1) * outputMatrix.get(1, 1) +
		original.get(1, 2) * outputMatrix.get(1, 2) +
		original.get(1, 3) * outputMatrix.get(1, 3) +
		original.get(1, 4) * outputMatrix.get(1, 4);

	outputMatrix = outputMatrix.transpose();
	for (unsigned int i = 1; i <= 4; ++i) {
		for (unsigned int j = 1; j <= 4; ++j) {
			outputMatrix.set(i, j, outputMatrix.get(i, j) / det);
		}
	}

	return outputMatrix;
}
