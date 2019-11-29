#include <limits>
#include "Avm.hpp"
#include "Exceptions.hpp"

std::map<eOperandType, Avm::createFunc> Avm::_createMap = {
	{Int8, &Avm::createInt8},
	{Int16, &Avm::createInt16},
	{Int32, &Avm::createInt32},
	{Float, &Avm::createFloat},
	{Double, &Avm::createDouble}
};

Avm::Avm() :
_error(nullptr),
_isExit(false),
_isVerbose(false),
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
		delete _stack.front();
        _stack.pop_front();
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
	for (auto it = _stack.begin(); it != _stack.end(); it++) {
		std::cout << (*it)->toString() << std::endl;
	}
}

void Avm::_execAssert(Instruction const *instr) {
	_stackEmptyError(instr);

	IOperand const *cmp = createOperand(instr->operandType, instr->value);

	if (!(*cmp == *_stack.front())) {
		std::cout << "AssertError: " << cmp->toString() << " is not egal to "
			<< _stack.front()->toString() << std::endl;
		_isExit = true;
	}

	delete cmp;
}

void Avm::_execMinMax(Instruction const *instr, std::string const &minMax) {
	_stackEmptyError(instr);

	IOperand const * op1 = _stack[0];
	IOperand const * op2 = nullptr;
	for (uint32_t i = 1; i < _stack.size(); i++) {
		op2 = _stack[i];
		if (minMax == "max") {
			if (op2 > op1) {
				delete op1;
				op1 = op2;
			}
			else {
				delete op2;
			}
		}
		else {
			if (op2 < op1) {
				delete op1;
				op1 = op2;
			}
			else {
				delete op2;
			}
		}
	}
	_stack.clear();
	_stack.push_front(op1);
}

void Avm::_execCalc(Instruction const * instr) {
	_stackEmptyError(instr);

	IOperand const * first = _stack.front();
	_stack.pop_front();

	try {
		_stackEmptyError(instr);
	}
	catch (StackEmptyError &e) {
		_stack.push_front(first);
		throw StackEmptyError();
	}
	IOperand const * second = _stack.front();
	_stack.pop_front();

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
				_stack.push_front(second);
				_stack.push_front(first);
				throw InvalidInstructionError();
				break;
		}
	}
	catch (AvmError &e) {
		_stack.push_front(second);
		_stack.push_front(first);
		if (_error == nullptr) {
			_error = new Error(instr->lineNbr,
				instr->lineStr,
				e.what());
		}
		throw AvmError();
	}
	delete first;
	delete second;
	_stack.push_front(res);
}


void Avm::_execOneInstr(Instruction const *instr) {
	switch (instr->instrType) {
		case InstrPush:
			_stack.push_front(createOperand(instr->operandType, instr->value));
			break;
		case InstrPop:
			_stackEmptyError(instr);
			delete _stack.front();
			_stack.pop_front();
			break;
		case InstrDump:
			_execDump();
			break;
		case InstrAssert:
			_execAssert(instr);
			break;
		case InstrPrint:
			_stackEmptyError(instr);
			std::cout << _stack.front()->toChar() << std::flush;
			break;
		case InstrExit:
			_isExit = true;
			break;
		case InstrMax:
			_execMinMax(instr, "max");
			break;
		case InstrMin:
			_execMinMax(instr, "min");
			break;
		case InstrPrintAll:
			_stackEmptyError(instr);
			for (auto it = _stack.begin(); it != _stack.end(); it++) {
				std::cout << (*it)->toChar() << std::flush;
			}
			break;
		default:
			_execCalc(instr);
			break;
	}
}

void Avm::exec(bool instantExec) {
    while (!_listInstr.empty()) {
		if (_isVerbose) {
			std::cout << BOLD  << ">>> " << EOC << "[" << _listInstr.front()->lineNbr << "] "
				<< _listInstr.front()->lineStr << std::endl;
		}
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
	if (!instantExec) {
		if (!_isExit) {
			std::cout << YELLOW << BOLD << "Warning:" << EOC << BOLD << " missing exit instruction" << EOC << std::endl;
		}
	}
}

void Avm::setVerbose(bool verbose) { _isVerbose = verbose; }
bool Avm::getExitStatus() const { return _isExit; }
bool Avm::isVerbose() const { return _isVerbose; }

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
	return _createMap[type](value);
}

Avm::Instruction::Instruction() :
instrType(InstrExit), operandType(Int8), value(""),
lineStr(""), lineNbr(-1) {}
Avm::Instruction::~Instruction() {}

std::regex const Avm::_regexFloat = std::regex(
	"^[ \n\t\r]*([-+]?\\d+\\.?\\d*f?|[-+][iI][nN][fF]|[nN][aA][nN])[ \n\t\r]*$");
std::regex const Avm::_regexInt = std::regex(
	"^[ \n\t\r]*[-+]?\\d+[ \n\t\r]*$");
