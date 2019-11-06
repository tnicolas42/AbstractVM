#include <sstream>
#include <fstream>
#include <algorithm>
#include "Parser.hpp"

Parser::Parser(Avm const & avm) : _avm(avm) {
}

Parser::Parser(Parser const &src) :
_avm(getAvm()) {
	*this = src;
}

Parser::~Parser() {
}

Parser &Parser::operator=(Parser const &rhs) {
	(void)rhs;
	// if (this != &rhs) {}
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
bool Parser::parseOneLine(std::string const &str, int lineNbr) {
	std::cout << lineNbr << ": " << str << std::endl;
	#warning call Avm.saveInstr()
	return true;
}

Avm const	&Parser::getAvm() const { return _avm; }
