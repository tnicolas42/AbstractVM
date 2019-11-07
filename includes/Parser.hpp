#pragma once

#include <string>
#include <vector>
#include <map>
#include "Avm.hpp"
#include "Exceptions.hpp"
#include "IOperand.hpp"

class Parser {
	public:
		explicit Parser(Avm * avm);
		Parser(Parser const &src);
		virtual ~Parser();
		Parser &operator=(Parser const &rhs);

		bool parseFromFile(std::string const &filename);
		bool parseFromString(std::string const &str);
		bool parseOneLine(std::string const &line, int lineNbr = -1);

		void printErrors() const;
		void clearErrors();

		Avm					*getAvm() const;
		std::vector<Error>	getErrors() const;

		bool		recvExecCommand;

	private:
		Parser();

		std::vector<Error>	_errors;
		Avm					*_avm;

		static const std::map<std::string, eInstr> _instrMap;
		static const std::map<std::string, eOperandType> _typeMap;
};
