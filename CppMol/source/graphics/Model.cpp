#include "graphics/Model.h"

unsigned int Model::vertexArrayID;
unsigned int Model::vertexBufferID;
unsigned int Model::sphereBufferID;
Mat4 Model::modelMatrix(MathUtils::MatGen::identity<float, 4>());

std::vector<float> Model::atomSpheres;
std::vector<float> Model::backboneSpheres;
std::vector<float> Model::disulfideBondSpheres;

std::vector<Connector*> Model::backboneSegments;
std::vector<Connector*> Model::disulfideBonds;

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
	const Color *color,
	std::vector<float> *vec
) {
	vec->reserve(vec->size() + 7);
	vec->push_back(center.getX());
	vec->push_back(center.getY());
	vec->push_back(center.getZ());
	vec->push_back(radius);
	vec->push_back(color->r);
	vec->push_back(color->g);
	vec->push_back(color->b);
}

bool Model::selectionIsValid(const Selection *selection) {
	if (!moleculeData || moleculeData->atoms.size() == 0) {
		std::cerr << "ERROR > No atoms found\n\n";
		return false;
	}
	return true;
}

void Model::colorAtomsByFunc(
	std::function<Color(const Atom*)> func,
	const Selection *selection
) {
	if (!selectionIsValid(selection)) {
		return;
	}
	for (size_t i = 4; i <= atomSpheres.size() - 3; i += 7) {
		const Atom *atom = &moleculeData->atoms[(i - 4) / 7];
		if (selection->isMatch(atom)) {
			Color color = func(atom);
			atomSpheres[i] = color.r;
			atomSpheres[i + 1] = color.g;
			atomSpheres[i + 2] = color.b;
		}
	}
	syncSphereBuffer(true, false, false);
}

void Model::colorConnectorsByFunc(
	std::function<Color(const Connector*)> func,
	const Selection *selection,
	ConnectorType connectorType
) {
	std::vector<Connector*> *connectors = nullptr;
	std::vector<float> *spheres = nullptr;

	bool syncBackboneSpheres = false;
	bool syncDisulfideBondSpheres = false;

	if (connectorType == ConnectorType::BACKBONE) {
		connectors = &backboneSegments;
		spheres = &backboneSpheres;
		syncBackboneSpheres = true;
	}
	else {
		connectors = &disulfideBonds;
		spheres = &disulfideBondSpheres;
		syncDisulfideBondSpheres = true;
	}

	for (size_t i = 0; i < connectors->size(); ++i) {
		bool atom1IsMatch = selection->isMatch((*connectors)[i]->atom1);
		bool atom2IsMatch = selection->isMatch((*connectors)[i]->atom2);
		if (atom1IsMatch && atom2IsMatch) {
			Color color = func((*connectors)[i]);
			(*connectors)[i]->setColor(&color);

			//Set color of connector spheres
			size_t sphereIndex1 = i * 2;
			size_t sphereIndex2 = i * 2 + 1;

			size_t sphereColorIndex1 = sphereIndex1 * 7 + 4;
			size_t sphereColorIndex2 = sphereIndex2 * 7 + 4;

			(*spheres)[sphereColorIndex1] = color.r;
			(*spheres)[sphereColorIndex1 + 1] = color.g;
			(*spheres)[sphereColorIndex1 + 2] = color.b;

			(*spheres)[sphereColorIndex2] = color.r;
			(*spheres)[sphereColorIndex2 + 1] = color.g;
			(*spheres)[sphereColorIndex2 + 2] = color.b;
		}
	}
	syncSphereBuffer(false, syncBackboneSpheres, syncDisulfideBondSpheres);
}

void Model::reset() {
	atomSpheres.clear();
	backboneSpheres.clear();
	disulfideBondSpheres.clear();

	for (size_t i = 0; i < backboneSegments.size(); ++i) {
		delete backboneSegments[i];
	}
	backboneSegments.clear();

	for (size_t i = 0; i < disulfideBonds.size(); ++i) {
		delete disulfideBonds[i];
	}
	disulfideBonds.clear();

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
		//Add to alpha carbon data for constructing backbone
		if (moleculeData->atoms[i].name == "CA") {
			alphaCarbons.push_back(&(moleculeData->atoms[i]));
		}

		Color color = Color::fromElement(moleculeData->atoms[i].element);
		addAtom(
			moleculeData->atoms[i].coords * coordScale - coordAverages,
			SphereTemplate::DEFAULT_RADIUS,
			&color
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
				Color color = Color::fromElement("C");

				addConnector(
					alphaCarbons[i], alphaCarbons[i + 1],
					ConnectorTemplate::DEFAULT_RADIUS,
					&color, coords1, coords2,
					ConnectorType::BACKBONE
				);
			}
		}
	}

	//Add disulfide bonds
	for (size_t i = 0; i < moleculeData->disulfideBonds.size(); ++i) {
		//Start residue same as end residue
		if (moleculeData->disulfideBonds[i].chain1 == moleculeData->disulfideBonds[i].chain2 &&
			moleculeData->disulfideBonds[i].residue1 == moleculeData->disulfideBonds[i].residue2) {

			continue;
		}
		
		const Atom *atom1 = nullptr;
		const Atom *atom2 = nullptr;

		//Find sulfur atoms
		for (size_t j = 0; j < moleculeData->atoms.size(); ++j) {
			if (atom1 && atom2) {
				break;
			}
			
			if (!atom1 &&
				moleculeData->atoms[j].chain == moleculeData->disulfideBonds[i].chain1 &&
				moleculeData->atoms[j].residueNum == moleculeData->disulfideBonds[i].residue1 &&
				moleculeData->atoms[j].element == "S") {

				atom1 = &(moleculeData->atoms[j]);
			}

			if (!atom2 &&
				moleculeData->atoms[j].chain == moleculeData->disulfideBonds[i].chain2 &&
				moleculeData->atoms[j].residueNum == moleculeData->disulfideBonds[i].residue2 &&
				moleculeData->atoms[j].element == "S") {

				atom2 = &(moleculeData->atoms[j]);
			}
		}

		if (atom1 && atom2) {
			Vec3 coords1 = atom1->coords * coordScale - coordAverages;
			Vec3 coords2 = atom2->coords * coordScale - coordAverages;
			Color color = Color::fromElement("S");

			addConnector(
				atom1, atom2,
				0.0f, //Invisible by default
				&color, coords1, coords2,
				ConnectorType::DISULFIDE_BOND
			);
		}
	}

	fillSphereBuffer();
	Model::moleculeData = moleculeData;
}

void Model::addAtom(const Vec3 &center, float radius, const Color *color) {
	addSphere(center, radius, color, &atomSpheres);
}

void Model::addConnector(
	const Atom *atom1, const Atom *atom2,
	float radius,
	const Color *color,
	const Vec3 &point1, const Vec3 &point2,
	ConnectorType connectorType
) {
	std::vector<Connector*> *connectors = nullptr;
	std::vector<float> *spheres = nullptr;

	if (connectorType == ConnectorType::BACKBONE) {
		connectors = &backboneSegments;
		spheres = &backboneSpheres;
	}
	else {
		connectors = &disulfideBonds;
		spheres = &disulfideBondSpheres;
	}

	connectors->push_back(
		new Connector(
			atom1, atom2,
			radius, color,
			point1, point2
		)
	);
	addSphere(point1, radius, color, spheres);
	addSphere(point2, radius, color, spheres);
}

void Model::genBuffers() {
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &sphereBufferID);
}

void Model::fillSphereTemplateBuffer() {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
		return;
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
	const size_t BACKBONE_SPHERES_SIZE = sizeof(float) * backboneSpheres.size();
	const size_t DISULFIDE_BOND_SPHERES_SIZE = sizeof(float) * disulfideBondSpheres.size();
	const size_t SPHERES_SIZE = 
		ATOM_SPHERES_SIZE + BACKBONE_SPHERES_SIZE + DISULFIDE_BOND_SPHERES_SIZE;

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferID);

	glBufferData(GL_ARRAY_BUFFER, SPHERES_SIZE, 0, GL_STATIC_DRAW);
	setSphereBufferAttributes();
}

void Model::syncSphereBuffer(
	bool syncAtomSpheres, 
	bool syncBackboneSpheres,
	bool syncDisulfideBondSpheres
) {
	const size_t ATOM_SPHERES_SIZE = sizeof(float) * atomSpheres.size();
	const size_t BACKBONE_SPHERES_SIZE = sizeof(float) * backboneSpheres.size();
	const size_t DISULFIDE_BOND_SPHERES_SIZE = sizeof(float) * disulfideBondSpheres.size();

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

	if (syncBackboneSpheres) {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			ATOM_SPHERES_SIZE,
			BACKBONE_SPHERES_SIZE,
			backboneSpheres.data()
		);
	}

	if (syncDisulfideBondSpheres) {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			ATOM_SPHERES_SIZE + BACKBONE_SPHERES_SIZE,
			DISULFIDE_BOND_SPHERES_SIZE,
			disulfideBondSpheres.data()
		);
	}
}

void Model::fillSphereBuffer() {
	const size_t ATOM_SPHERES_SIZE = sizeof(float) * atomSpheres.size();
	const size_t BACKBONE_SPHERES_SIZE = sizeof(float) * backboneSpheres.size();
	const size_t DISULFIDE_BOND_SPHERES_SIZE = sizeof(float) * disulfideBondSpheres.size();
	const size_t SPHERES_SIZE = 
		ATOM_SPHERES_SIZE + BACKBONE_SPHERES_SIZE + DISULFIDE_BOND_SPHERES_SIZE;

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

	//Backbone spheres
	glBufferSubData(
		GL_ARRAY_BUFFER,
		ATOM_SPHERES_SIZE,
		BACKBONE_SPHERES_SIZE,
		backboneSpheres.data()
	);

	//Disulfide bond spheres
	glBufferSubData(
		GL_ARRAY_BUFFER,
		ATOM_SPHERES_SIZE + BACKBONE_SPHERES_SIZE,
		DISULFIDE_BOND_SPHERES_SIZE,
		disulfideBondSpheres.data()
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
		GLsizei(
			(atomSpheres.size() + backboneSpheres.size() + disulfideBondSpheres.size()) / 7
		)
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

	//Set connector model and normal matrices, set shader's color, draw arrays
	for(size_t i = 0; i < backboneSegments.size(); ++i) {
		backboneSegments[i]->render(connectorShader, modelMatrix, connectorTemplate);
	}
	for (size_t i = 0; i < disulfideBonds.size(); ++i) {
		disulfideBonds[i]->render(connectorShader, modelMatrix, connectorTemplate);
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
	syncSphereBuffer(true, false, false);
}
void Model::setConnectorRadius(
	float radius, const Selection *selection, ConnectorType connectorType,
	bool reversed
) {
	std::vector<Connector*> *connectors = nullptr;
	std::vector<float> *spheres = nullptr;
	
	bool syncBackboneSpheres = false;
	bool syncDisulfideBondSpheres = false;

	if (connectorType == ConnectorType::BACKBONE) {
		connectors = &backboneSegments;
		spheres = &backboneSpheres;
		syncBackboneSpheres = true;
	}
	else {
		connectors = &disulfideBonds;
		spheres = &disulfideBondSpheres;
		syncDisulfideBondSpheres = true;
	}

	for (size_t i = 0; i < connectors->size(); ++i) {
		bool atom1IsMatch = selection->isMatch((*connectors)[i]->atom1, reversed);
		bool atom2IsMatch = selection->isMatch((*connectors)[i]->atom2, reversed);
		if ((!reversed &&  atom1IsMatch && atom2IsMatch) ||
			(reversed && (atom1IsMatch || atom2IsMatch))) {

			(*connectors)[i]->setRadius(radius);

			//Set radius of connector spheres
			size_t sphereIndex1 = i * 2;
			size_t sphereIndex2 = i * 2 + 1;

			size_t sphereRadiusIndex1 = sphereIndex1 * 7 + 3;
			size_t sphereRadiusIndex2 = sphereIndex2 * 7 + 3;

			(*spheres)[sphereRadiusIndex1] = radius;
			(*spheres)[sphereRadiusIndex2] = radius;
		}
	}
	syncSphereBuffer(false, syncBackboneSpheres, syncDisulfideBondSpheres);
}

void Model::setAtomColor(const Color *color, const Selection *selection) {
	colorAtomsByFunc(
		[&color](const Atom *atom) -> Color {
			return *color;
		},
		selection
	);
}
void Model::colorAtomsDefault(const Selection *selection) {
	colorAtomsByFunc(
		[](const Atom *atom) -> Color {
			return Color::fromElement(atom->element);
		},
		selection
	);
}
void Model::colorAtomsByStructure(const Selection *selection) {
	colorAtomsByFunc(
		[&moleculeData = std::as_const(moleculeData)](const Atom *atom) -> Color {
			return Color::fromStructure(atom, moleculeData);
		},
		selection
	);
}
void Model::colorAtomsByChain(const Selection *selection) {

}

void Model::setConnectorColor(
	const Color *color, const Selection *selection,
	ConnectorType connectorType
) {
	colorConnectorsByFunc(
		[&color](const Connector *connector) -> Color {
			return *color;
		},
		selection, connectorType
	);
}
void Model::colorConnectorsDefault(
	const Selection *selection, ConnectorType connectorType
) {
	colorConnectorsByFunc(
		[&connectorType](const Connector *connector) -> Color {
			if (connectorType == ConnectorType::BACKBONE) {
				return Color::fromElement("C");
			}
			else {
				return Color::fromElement("S");
			}
		},
		selection, connectorType
	);
}
void Model::colorConnectorsByStructure(
	const Selection *selection, ConnectorType connectorType
) {
	colorConnectorsByFunc(
		[&moleculeData = std::as_const(moleculeData)](const Connector *connector) -> Color {
			return Color::fromConnectorStructure(
				connector->atom1, connector->atom2, moleculeData
			);
		},
		selection, connectorType
	);
}
void Model::colorConnectorsByChain(
	const Selection *selection, ConnectorType connectorType
) {

}

void Model::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
