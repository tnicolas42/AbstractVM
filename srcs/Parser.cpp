#include <sstream>
#include <fstream>
#include <algorithm>
#include "Parser.hpp"

Parser::Parser(Avm const & avm) :
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


	_errors.push_back(Error(lineNbr, line, "invalid syntax"));
	return false;

	return true;
}

void Parser::printErrors() const {
	for (auto it = _errors.begin(); it != _errors.end(); it++) {
		std::cout << *it;
	}
}
void Parser::clearErrors() {
	_errors.empty();
}

Avm const	&Parser::getAvm() const { return _avm; }
std::vector<Error> Parser::getErrors() const { return _errors; }
