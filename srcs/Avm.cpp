#include "Avm.hpp"

Avm::Avm() {
}

Avm::Avm(Avm const &src) {
	*this = src;
}

Avm::~Avm() {
}

Avm &Avm::operator=(Avm const &rhs) {
	if (this != &rhs) {
		;
	}
	return *this;
}

#warning need to add a factory to implement add operator
