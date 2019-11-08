#include <sstream>
#include <fstream>
#include <algorithm>
#include "Parser.hpp"

const std::map<std::string, eInstr> Parser::_instrMap = {
	{"push", InstrPush},
	{"pop", InstrPop},
	{"dump", InstrDump},
	{"assert", InstrAssert},
	{"add", InstrAdd},
	{"sub", InstrSub},
	{"mul", InstrMul},
	{"div", InstrDiv},
	{"mod", InstrMod},
	{"print", InstrPrint},
	{"exit", InstrExit},
};

const std::map<std::string, eOperandType> Parser::_typeMap = {
	{"Int8", Int8},
	{"Int16", Int16},
	{"Int32", Int32},
	{"Float", Float},
	{"Double", Double},
};

Parser::Parser(Avm * avm) :
recvExecCommand(false),
_errors(),
_avm(avm) {
}

Parser::Parser(Parser const &src) :
_avm(getAvm()) {
	*this = src;
}

Parser::~Parser() {
}

Parser &Parser::operator=(Parser const &rhs) {
	if (this != &rhs) {
		_errors = rhs.getErrors();
		recvExecCommand = rhs.recvExecCommand;
	}
	return *this;
}

bool Parser::parseFromFile(std::string const &filename) {
	std::ifstream file(filename);
    if (file.fail()) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

	int i = 1;
    for (std::string line; std::getline(file, line);) {
		if (parseOneLine(line, i) == false) {
			file.close();
			return false;
		}
		i++;
    }
    file.close();
	return true;
}
bool Parser::parseFromString(std::string const &str) {
	std::stringstream ss(str);
    std::string line;
	int i = 1;
    while (std::getline(ss, line)) {
		if (parseOneLine(line, i) == false) {
			return false;
		}
		i++;
    }
	return true;
}

std::string trim(std::string const & str,
                 std::string const & whitespace = WHITESPACE) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";  // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool Parser::parseOneLine(std::string const &line, int lineNbr) {
	std::string str = trim(line);
	if (str.size() == 0)
		return true;  // empty line
	if (str.size() == 2 && str == ";;") {
		recvExecCommand = true;
		return true;  // exec program
	}
	if (str[0] == ';')
		return true;  // comment

	std::vector<std::string> words;
	std::istringstream stream(str);
	std::copy(std::istream_iterator<std::string>(stream),
			  std::istream_iterator<std::string>(),
			  std::back_inserter(words));
	if (words.size() == 0) {
		return true;
	}
	std::string command = words[0];
	// command == push, dump, ...
	words.erase(words.begin());

	Avm::Instruction *instr = new Avm::Instruction;

	instr->lineNbr = lineNbr;
	instr->lineStr = line;

	bool isValidCommand = false;
	for (auto it = _instrMap.begin(); it != _instrMap.end(); it++) {
		if (command == it->first) {
			instr->instrType = it->second;
			isValidCommand = true;
			break;
		}
	}
	if (isValidCommand == false) {
		_errors.push_back(Error(lineNbr, line, std::string("invalid command: ") + command));
		return false;
	}
	if (instr->instrType == InstrPush || instr->instrType == InstrAssert) {
		if (words.size() == 0) {
			_errors.push_back(Error(lineNbr, line, command + " need one argument"));
			return false;
		}
		std::string arg;
		for (auto it = words.begin(); it != words.end(); it++) {
			arg += *it;
		}

		if (arg[arg.size() - 1] != ')') {
			_errors.push_back(Error(lineNbr, line, "invalid syntax"));
			return false;
		}
		arg = arg.substr(0, arg.size()-1);
		std::string argType = arg.substr(0, arg.find("("));
		// argType == Int8, Float, ...


		bool isValidType = false;
		for (auto it = _typeMap.begin(); it != _typeMap.end(); it++) {
			if (argType == it->first) {
				instr->operandType = it->second;
				isValidType = true;
				break;
			}
		}
		if (isValidType == false) {
			_errors.push_back(Error(lineNbr, line, std::string("invalid type: ") + argType));
			return false;
		}

		std::string valueStr = arg.substr(arg.find("(")+1, arg.size()-1);

		IOperand const * val;
		try {
			val = _avm->createOperand(instr->operandType, valueStr);
		}
		catch (AvmError &e) {
			_errors.push_back(Error(lineNbr, line, e.what()));
			return false;
		}
		if (val == nullptr) {
			_errors.push_back(Error(lineNbr, line, std::string("invalid value: ") + valueStr));
			return false;
		}
		instr->value = valueStr;
		delete val;

	} else if (words.size() > 0) {
		_errors.push_back(Error(lineNbr, line, command + " need no arguments"));
		return false;
	}

	_avm->saveInstr(instr);
	return true;
}

void Parser::printErrors() const {
	for (auto it = _errors.begin(); it != _errors.end(); it++) {
		std::cout << *it;
	}
}
void Parser::clearErrors() {
	_errors.clear();
}

Avm					*Parser::getAvm() const { return _avm; }
std::vector<Error>	Parser::getErrors() const { return _errors; }
