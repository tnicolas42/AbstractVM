#include "Avm.hpp"

Avm::Avm() {
}

Avm::Avm(Avm const &src) {
	*this = src;
}

Avm::~Avm() {
}

Avm &Avm::operator=(Avm const &rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}
