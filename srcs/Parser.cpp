#include "Parser.hpp"
#include "Exceptions.hpp"

Parser::Parser() {
}

Parser::Parser(Parser const &src) {
	*this = src;
}

Parser::~Parser() {
}

Parser &Parser::operator=(Parser const &rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}

bool Parser::parseInstr(Avm *avm, Avm::Instruction * instr) const {
	std::string errorMsg = "";
	try {
		if (instr->instrType == InstrPush || instr->instrType == InstrAssert) {
			if (instr->value == "") {
				errorMsg = "need on valid argument";
				throw InvalidInstructionError();
			}
			IOperand const * val;
			try {
				val = avm->createOperand(instr->operandType, instr->value);
			}
			catch (AvmError & e) {
				errorMsg = e.what();
				throw AvmError();
			}

			if (val == nullptr) {
				errorMsg = std::string("invalid value: ") + instr->value;
			}
			delete val;
		} else {
			if (instr->value != "") {
				throw InvalidInstructionError();
			}
		}
	}
	catch (AvmError & e) {
		if (errorMsg == "")
			errorMsg = e.what();
	}
	if (errorMsg != "") {
		std::cout << Error(instr->lineNbr,
						   instr->lineStr,
						   errorMsg);
		return false;
	}
	avm->saveInstr(instr);
	return true;
}
