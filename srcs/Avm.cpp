#include "Avm.hpp"
#include "Exceptions.hpp"

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
#warning changer le mode de check pour les float et les double (et check overflow)
IOperand const * Avm::createInt8(std::string const & value) const {
	int8_t val = static_cast<int8_t>(std::atoi(value.c_str()));
	if (std::to_string(val) != value) {
		throw ConvertError();
	}
	IOperand *res = new OperandInt8(val);
	return res;
}
IOperand const * Avm::createInt16(std::string const & value) const {
	int16_t val = static_cast<int16_t>(std::atoi(value.c_str()));
	if (std::to_string(val) != value) {
		throw ConvertError();
	}
	IOperand *res = new OperandInt16(val);
	return res;
}
IOperand const * Avm::createInt32(std::string const & value) const {
	int32_t val = static_cast<int32_t>(std::atoi(value.c_str()));
	if (std::to_string(val) != value) {
		throw ConvertError();
	}
	IOperand *res = new OperandInt32(val);
	return res;
}
IOperand const * Avm::createFloat(std::string const & value) const {
	float val = static_cast<float>(std::atof(value.c_str()));
	if (std::to_string(val) != value) {
		throw ConvertError();
	}
	IOperand *res = new OperandFloat(val);
	return res;
}
IOperand const * Avm::createDouble(std::string const & value) const {
	double val = std::atof(value.c_str());
	if (std::to_string(val) != value) {
		throw ConvertError();
	}
	IOperand *res = new OperandDouble(val);
	return res;
}
IOperand const * Avm::createOperand(eOperandType type, std::string const & value) const {
	return createInt8(value);
}
