#pragma once

#include "Avm.hpp"

class Parser {
	public:
		Parser();
		Parser(Parser const &src);
		virtual ~Parser();

		Parser &operator=(Parser const &rhs);

		bool parseInstr(Avm * avm, Avm::Instruction * instr) const;
	protected:
	private:
};
