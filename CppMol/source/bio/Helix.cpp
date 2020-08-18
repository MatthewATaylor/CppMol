#include "bio/Helix.h"

const std::string Helix::typeNames[10] = {
	"Right-handed alpha",
	"Right-handed omega",
	"Right-handed pi",
	"Right-handed gamma",
	"Right-handed 3/10",
	"Left-handed alpha",
	"Left-handed omega",
	"Left-handed gamma",
	"2/7 ribbon/helix",
	"Polyproline"
};

std::string Helix::getName() {
	return typeNames[type - 1];
}
