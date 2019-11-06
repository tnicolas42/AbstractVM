#pragma once

#include <string>
#include "Avm.hpp"

class Parser {
	public:
		explicit Parser(Avm const & avm);
		Parser(Parser const &src);
		virtual ~Parser();
		Parser &operator=(Parser const &rhs);

		bool parseFromFile(std::string const &filename);
		bool parseFromString(std::string const &str);
		bool parseOneLine(std::string const &str, int lineNbr = -1);

		Avm const	&getAvm() const;
	protected:
	private:
		Parser();

		Avm const	&_avm;
};
