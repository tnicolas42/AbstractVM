#include <limits>
#include "Avm.hpp"
#include "Exceptions.hpp"

Avm::Avm() :
_error(nullptr),
_isExit(false),
_listInstr(),
_stack() {
}

Avm::Avm(Avm const &src) {
	*this = src;
}

Avm::~Avm() {
}

Avm &Avm::operator=(Avm const &rhs) {
	if (this != &rhs) {
		_isExit = rhs.getExitStatus();
	}
	return *this;
}

void Avm::saveInstr(Instruction const *instr) {
	_listInstr.push(instr);
}

void Avm::clearInstr() {
    while (!_listInstr.empty()) {
		delete _listInstr.front();
        _listInstr.pop();
	}
    while (!_stack.empty()) {
		delete _stack.top();
        _stack.pop();
	}
	_isExit = false;
}

void Avm::_stackEmptyError(Instruction const *instr) {
	if (_stack.empty()) {
		_error = new Error(instr->lineNbr, instr->lineStr,
			"The stack is empty");
		throw StackEmptyError();
	}
}

void Avm::_execDump() {
	std::stack<IOperand const *> tmp;
	while (!_stack.empty()) {
		std::cout << _stack.top()->toString() << std::endl;
		tmp.push(_stack.top());
		_stack.pop();
	}
	while (!tmp.empty()) {
		_stack.push(tmp.top());
		tmp.pop();
	}
}

void Avm::_execAssert(Instruction const *instr) {
	_stackEmptyError(instr);

	IOperand const *cmp = createOperand(instr->operandType, instr->value);

	if (!(*cmp == *_stack.top())) {
		std::cout << "AssertError: " << cmp->toString() << " is not egal to "
			<< _stack.top()->toString() << std::endl;
		_isExit = true;
	}

	delete cmp;
}

void Avm::_execCalc(Instruction const * instr) {
	_stackEmptyError(instr);

	IOperand const * first = _stack.top();
	_stack.pop();

	try {
		_stackEmptyError(instr);
	}
	catch (StackEmptyError &e) {
		delete first;
		throw StackEmptyError();
	}
	IOperand const * second = _stack.top();
	_stack.pop();

	IOperand const * res;
	try {
		switch (instr->instrType) {
			case InstrAdd:
				res = *first + *second;
				break;
			case InstrSub:
				res = *first - *second;
				break;
			case InstrMul:
				res = *first * *second;
				break;
			case InstrDiv:
				res = *first / *second;
				break;
			case InstrMod:
				res = *first % *second;
				break;
			default:
				_stack.push(second);
				_stack.push(first);
				throw InvalidInstructionError();
				break;
		}
	}
	catch (OverflowError &e) {
		_stack.push(second);
		_stack.push(first);
		throw OverflowError();
	}
	catch (ModOnDoubleError &e) {
		_stack.push(second);
		_stack.push(first);
		throw ModOnDoubleError();
	}
	delete first;
	delete second;
	_stack.push(res);
}


void Avm::_execOneInstr(Instruction const *instr) {
	switch (instr->instrType) {
		case InstrPush:
			_stack.push(createOperand(instr->operandType, instr->value));
			break;
		case InstrPop:
			_stackEmptyError(instr);
			delete _stack.top();
			_stack.pop();
			break;
		case InstrDump:
			_execDump();
			break;
		case InstrAssert:
			_execAssert(instr);
			break;
		case InstrPrint:
			std::cout << _stack.top()->toString() << std::endl;
			break;
		case InstrExit:
			_isExit = true;
			break;
		default:
			_execCalc(instr);
			break;
	}
}

void Avm::exec() {
    while (!_listInstr.empty()) {
		try {
			_execOneInstr(_listInstr.front());
		}
		catch (AvmError &e) {
			if (_error == nullptr) {
				_error = new Error(_listInstr.front()->lineNbr,
					_listInstr.front()->lineStr,
					e.what());
			}
		}
		if (_error != nullptr) {
			std::cout << *_error;
			delete _error;
			_error = nullptr;
		}
		if (_isExit) {  // exit if recv exit instr
			break;
		}
		delete _listInstr.front();
		_listInstr.pop();
	}
}

bool Avm::getExitStatus() const { return _isExit; }

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

Avm::Instruction::Instruction() :
instrType(InstrExit), operandType(Int8), value(""),
lineStr(""), lineNbr(-1) {}
Avm::Instruction::~Instruction() {}

std::regex const Avm::_regexFloat = std::regex(
	"^[ \n\t\r]*([-+]?\\d+\\.?\\d*f?|[-+][iI][nN][fF]|[nN][aA][nN])[ \n\t\r]*$");
std::regex const Avm::_regexInt = std::regex(
	"^[ \n\t\r]*[-+]?\\d+[ \n\t\r]*$");
