#include "Operand.hpp"

Operand::Operand() {
}

Operand::Operand(Operand const &src) {
	*this = src;
}

Operand::~Operand() {
}

Operand &Operand::operator=(Operand const &rhs) {
	if (this != &rhs) {
		;
	}
	return *this;
}
