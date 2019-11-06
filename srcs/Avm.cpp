#include <limits>
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

void Avm::saveInstr(Instruction const &instr) {
	_listInstr.push(instr);
}

void Avm::clearInstr() {
	_listInstr.empty();
}

void Avm::exec() {
	std::cout << "exec function todo" << std::endl;
}

IOperand const * Avm::createInt8(std::string const & value) {
	if (std::regex_match(value, _regexInt) == false)
		throw ConvertError();
	int8_t val = static_cast<int8_t>(std::atoi(value.c_str()));
	if (std::atof(value.c_str()) > static_cast<double>(std::numeric_limits<int8_t>::max())
	|| std::atof(value.c_str()) < static_cast<double>(std::numeric_limits<int8_t>::min()))
		throw OverflowError();
	IOperand *res = new OperandInt8(val);
	return res;
}
IOperand const * Avm::createInt16(std::string const & value) {
	if (std::regex_match(value, _regexInt) == false)
		throw ConvertError();
	int16_t val = static_cast<int16_t>(std::atoi(value.c_str()));
	if (std::atof(value.c_str()) > static_cast<double>(std::numeric_limits<int16_t>::max())
	|| std::atof(value.c_str()) < static_cast<double>(std::numeric_limits<int16_t>::min()))
		throw OverflowError();
	IOperand *res = new OperandInt16(val);
	return res;
}
IOperand const * Avm::createInt32(std::string const & value) {
	if (std::regex_match(value, _regexInt) == false)
		throw ConvertError();
	int32_t val = static_cast<int32_t>(std::atoi(value.c_str()));
	if (std::atof(value.c_str()) > static_cast<double>(std::numeric_limits<int32_t>::max())
	|| std::atof(value.c_str()) < static_cast<double>(std::numeric_limits<int32_t>::min()))
		throw OverflowError();
	IOperand *res = new OperandInt32(val);
	return res;
}
IOperand const * Avm::createFloat(std::string const & value) {
	if (std::regex_match(value, _regexFloat) == false)
		throw ConvertError();
	float val = static_cast<float>(std::atof(value.c_str()));
	IOperand *res = new OperandFloat(val);
	return res;
}
IOperand const * Avm::createDouble(std::string const & value) {
	if (std::regex_match(value, _regexFloat) == false)
		throw ConvertError();
	double val = std::atof(value.c_str());
	IOperand *res = new OperandDouble(val);
	return res;
}
IOperand const * Avm::createOperand(eOperandType type, std::string const & value) {
	switch (type) {
		case Int8:
			return createInt8(value);
		case Int16:
			return createInt16(value);
		case Int32:
			return createInt32(value);
		case Float:
			return createFloat(value);
		case Double:
			return createDouble(value);
	}
	return nullptr;
}

std::regex const Avm::_regexFloat = std::regex(
	"^[ \n\t\r]*([-+]?\\d+\\.?\\d*f?|[-+][iI][nN][fF]|[nN][aA][nN])[ \n\t\r]*$");
std::regex const Avm::_regexInt = std::regex(
	"^[ \n\t\r]*[-+]?\\d+[ \n\t\r]*$");
