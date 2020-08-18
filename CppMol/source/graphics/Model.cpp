#include "graphics/Model.h"

unsigned int Model::vertexArrayID;
unsigned int Model::vertexBufferID;
unsigned int Model::sphereBufferID;
Mat4 Model::modelMatrix(MathUtils::MatGen::identity<float, 4>());

std::vector<float> Model::atomSpheres;
std::vector<float> Model::connectorSpheres;
std::vector<Connector*> Model::connectors;

const SphereTemplate *Model::sphereTemplate = nullptr;
const ConnectorTemplate *Model::connectorTemplate = nullptr;

const MoleculeData *Model::moleculeData = nullptr;

void Model::setSphereBufferAttributes() {
	//Center
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	//Radius
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);

	//Color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
}
void Model::addSphere(
	const Vec3 &center,
	float radius,
	float r, float g, float b,
	std::vector<float> *vec
) {
	vec->reserve(vec->size() + 7);
	vec->push_back(center.getX());
	vec->push_back(center.getY());
	vec->push_back(center.getZ());
	vec->push_back(radius);
	vec->push_back(r);
	vec->push_back(g);
	vec->push_back(b);
}

bool Model::selectionIsValid(const Selection *selection) {
	if (!moleculeData || moleculeData->atoms.size() == 0) {
		std::cerr << "ERROR > No atoms found\n\n";
		return false;
	}
	return true;
}

void Model::reset() {
	atomSpheres.clear();
	connectorSpheres.clear();
	for (size_t i = 0; i < connectors.size(); ++i) {
		delete connectors[i];
	}
	connectors.clear();
	moleculeData = nullptr;
}

void Model::setSphereTemplate(const SphereTemplate *sphereTemplate) {
	Model::sphereTemplate = sphereTemplate;
}
void Model::setConnectorTemplate(const ConnectorTemplate *connectorTemplate) {
	Model::connectorTemplate = connectorTemplate;
}

void Model::loadMoleculeData(const MoleculeData *moleculeData) {
	reset();

	//Gather coordinate information to adjust model size and position
	Vec3 coordSums;
	float maxCoord = 0.0f;
	for (size_t i = 0; i < moleculeData->atoms.size(); ++i) {
		coordSums += moleculeData->atoms[i].coords;
		for (unsigned int j = 0; j < 3; j++) {
			float absCoord = std::abs(moleculeData->atoms[i].coords.get(j));
			if (absCoord > maxCoord) {
				maxCoord = absCoord;
			}
		}
	}
	float coordScale = 8.0f / maxCoord;
	Vec3 coordAverages = coordSums * coordScale / (float)moleculeData->atoms.size();

	std::vector<const Atom*> alphaCarbons;

	for (size_t i = 0; i < moleculeData->atoms.size(); ++i) {
		float r = 0.0f, g = 0.0f, b = 0.0f;
		if (moleculeData->atoms[i].element == "C") {
			r = 0.39f;
			g = 0.39f;
			b = 0.39f;
		}
		else if (moleculeData->atoms[i].element == "H") {
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
		}
		else if (moleculeData->atoms[i].element == "N") {
			b = 1.0f;
		}
		else if (moleculeData->atoms[i].element == "O") {
			r = 1.0f;
		}
		else if (moleculeData->atoms[i].element == "S") {
			r = 1.0f;
			g = 1.0f;
		}
		else if (moleculeData->atoms[i].element == "P") {
			r = 1.0f;
			g = 0.5f;
		}
		else {
			std::cerr << "ERROR > Unknown color for element: " << 
				moleculeData->atoms[i].element << "\n\n";
		}

		//Add to alpha carbon data for constructing backbone
		if (moleculeData->atoms[i].name == "CA") {
			alphaCarbons.push_back(&(moleculeData->atoms[i]));
		}

		addAtom(
			moleculeData->atoms[i].coords * coordScale - coordAverages,
			SphereTemplate::DEFAULT_RADIUS,
			r, g, b
		);
	}

	//Construct backbone using connectors
	if (alphaCarbons.size() >= 2) {
		for (size_t i = 0; i < alphaCarbons.size() - 1; ++i) {
			//Same chain, no residues skipped
			if (alphaCarbons[i]->chain == alphaCarbons[i + 1]->chain &&
				(alphaCarbons[i]->residueNum == alphaCarbons[i + 1]->residueNum ||
					alphaCarbons[i]->residueNum + 1 == alphaCarbons[i + 1]->residueNum)) {

				Vec3 coords1 = alphaCarbons[i]->coords * coordScale - coordAverages;
				Vec3 coords2 = alphaCarbons[i + 1]->coords * coordScale - coordAverages;

				addConnector(
					alphaCarbons[i], alphaCarbons[i + 1],
					ConnectorTemplate::DEFAULT_RADIUS,
					0.39f, 0.39f, 0.39f,
					coords1, coords2
				);
			}
		}
	}

	fillSphereBuffer();
	Model::moleculeData = moleculeData;
}

void Model::addAtom(const Vec3 &center, float radius, float r, float g, float b) {
	addSphere(center, radius, r, g, b, &atomSpheres);
}
void Model::addConnector(
	const Atom *atom1, const Atom *atom2,
	float radius,
	float r, float g, float b,
	const Vec3 &point1, const Vec3 &point2
) {
	connectors.push_back(new Connector(atom1, atom2, radius, r, g, b, point1, point2));
	addSphere(point1, radius, r, g, b, &connectorSpheres);
	addSphere(point2, radius, r, g, b, &connectorSpheres);
}

void Model::genBuffers() {
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &sphereBufferID);
}

void Model::fillSphereTemplateBuffer() {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
	}

	const size_t TEMPLATE_SIZE = sizeof(float) * SphereTemplate::NUM_VERTICES;

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, TEMPLATE_SIZE, sphereTemplate->getVerticesPtr(), GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void Model::allocateSphereBuffer() {
	const size_t ATOM_SPHERES_SIZE = sizeof(float) * atomSpheres.size();
	const size_t CONNECTOR_SPHERES_SIZE = sizeof(float) * connectorSpheres.size();
	const size_t SPHERES_SIZE = ATOM_SPHERES_SIZE + CONNECTOR_SPHERES_SIZE;

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferID);

	glBufferData(GL_ARRAY_BUFFER, SPHERES_SIZE, 0, GL_STATIC_DRAW);
	setSphereBufferAttributes();
}

void Model::syncSphereBuffer(bool syncAtomSpheres, bool syncConnectorSpheres) {
	const size_t ATOM_SPHERES_SIZE = sizeof(float) * atomSpheres.size();
	const size_t CONNECTOR_SPHERES_SIZE = sizeof(float) * connectorSpheres.size();

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferID);

	if (syncAtomSpheres) {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			ATOM_SPHERES_SIZE,
			atomSpheres.data()
		);
	}

	if (syncConnectorSpheres) {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			ATOM_SPHERES_SIZE,
			CONNECTOR_SPHERES_SIZE,
			connectorSpheres.data()
		);
	}
}

void Model::fillSphereBuffer() {
	const size_t ATOM_SPHERES_SIZE = sizeof(float) * atomSpheres.size();
	const size_t CONNECTOR_SPHERES_SIZE = sizeof(float) * connectorSpheres.size();
	const size_t SPHERES_SIZE = ATOM_SPHERES_SIZE + CONNECTOR_SPHERES_SIZE;

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferID);

	glBufferData(GL_ARRAY_BUFFER, SPHERES_SIZE, 0, GL_STATIC_DRAW);
	setSphereBufferAttributes();

	//Atom spheres
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		ATOM_SPHERES_SIZE,
		atomSpheres.data()
	);

	//Connector spheres
	glBufferSubData(
		GL_ARRAY_BUFFER,
		ATOM_SPHERES_SIZE,
		CONNECTOR_SPHERES_SIZE,
		connectorSpheres.data()
	);
}

void Model::render(
	const Shader *sphereShader,
	const Shader *connectorShader,
	const Window *window,
	const Camera *camera
) {
	Mat4 viewMatrix = camera->getViewMatrix();
	Mat4 projectionMatrix = camera->getProjectionMatrix(window);

	//Draw atom and connector spheres
	if (!sphereShader) {
		std::cerr << "ERROR > Unloaded sphere shaders\n\n";
		return;
	}

	glBindVertexArray(vertexArrayID);
	sphereShader->useProgram();

	sphereShader->setModelMatrix(modelMatrix);
	sphereShader->setNormalMatrix(MathUtils::MatGen::normal<float>(modelMatrix));
	sphereShader->setViewMatrix(viewMatrix);
	sphereShader->setProjectionMatrix(projectionMatrix);

	glDrawArraysInstanced(
		GL_TRIANGLES,
		0,
		GLsizei(SphereTemplate::NUM_VERTICES / 3),
		GLsizei((atomSpheres.size() + connectorSpheres.size()) / 7)
	);

	//Draw connectors
	if (!connectorShader) {
		std::cerr << "ERROR > Unloaded connector shaders\n\n";
		return;
	}
	if (!connectorTemplate) {
		std::cerr << "ERROR > Connector template not set\n\n";
		return;
	}

	connectorTemplate->bind();
	connectorShader->useProgram();

	connectorShader->setViewMatrix(viewMatrix);
	connectorShader->setProjectionMatrix(projectionMatrix);

	for(size_t i = 0; i < connectors.size(); ++i) {
		//Set connector's model and normal matrices, set shader's color, draw arrays
		connectors[i]->render(connectorShader, modelMatrix, connectorTemplate);
	}
}

void Model::setAtomRadius(float radius, const Selection *selection, bool reversed) {
	if (!selectionIsValid(selection)) {
		return;
	}
	for (size_t i = 3; i < atomSpheres.size(); i += 7) {
		if (selection->isMatch(&moleculeData->atoms[(i - 3) / 7], reversed)) {
			atomSpheres[i] = radius;
		}
	}
	syncSphereBuffer(true, false);
}
void Model::setConnectorRadius(float radius, const Selection *selection, bool reversed) {
	for (size_t i = 0; i < connectors.size(); ++i) {
		bool atom1IsMatch = selection->isMatch(connectors[i]->atom1, reversed);
		bool atom2IsMatch = selection->isMatch(connectors[i]->atom2, reversed);
		if ((!reversed &&  atom1IsMatch && atom2IsMatch) ||
			(reversed && (atom1IsMatch || atom2IsMatch))) {

			connectors[i]->setRadius(radius);

			//Set radius of connector spheres
			size_t sphereIndex1 = i * 2;
			size_t sphereIndex2 = i * 2 + 1;

			size_t sphereRadiusIndex1 = sphereIndex1 * 7 + 3;
			size_t sphereRadiusIndex2 = sphereIndex2 * 7 + 3;

			connectorSpheres[sphereRadiusIndex1] = radius;
			connectorSpheres[sphereRadiusIndex2] = radius;
		}
	}
	syncSphereBuffer(false, true);
}

void Model::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
