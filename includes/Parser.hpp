#pragma once

#include <string>
#include <vector>
#include "Avm.hpp"
#include "Exceptions.hpp"

class Parser {
	public:
		explicit Parser(Avm const & avm);
		Parser(Parser const &src);
		virtual ~Parser();
		Parser &operator=(Parser const &rhs);

		bool parseFromFile(std::string const &filename);
		bool parseFromString(std::string const &str);
		bool parseOneLine(std::string const &line, int lineNbr = -1);

		void printErrors() const;
		void clearErrors();

		Avm const	&getAvm() const;
		std::vector<Error> getErrors() const;

		bool		recvExecCommand;
	protected:
	private:
		Parser();

		std::vector<Error> _errors;
		Avm const	&_avm;
};
