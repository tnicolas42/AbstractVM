#pragma once

#include <string>
#include <regex>
#include <queue>
#include "Operand.hpp"

#define WHITESPACE "\t "

enum eInstr {
	InstrPush,
	InstrPop,
	InstrAssert,
	InstrAdd,
	InstrSub,
	InstrMul,
	InstrDiv,
	InstrMod,
	InstrPrint,
	InstrExit,
};

class Avm {
	public:
		Avm();
		Avm(Avm const &src);
		virtual ~Avm();

		Avm &operator=(Avm const &rhs);

		struct Instruction {
			eInstr			instrType;
			eOperandType	operandType;
			IOperand const	*operand;

			Instruction();
			~Instruction();
		};

		void saveInstr(Instruction const *instr);
		void clearInstr();
		void exec();

		/* factory to create operand */
		static IOperand const * createInt8(std::string const & value);
		static IOperand const * createInt16(std::string const & value);
		static IOperand const * createInt32(std::string const & value);
		static IOperand const * createFloat(std::string const & value);
		static IOperand const * createDouble(std::string const & value);
		static IOperand const * createOperand(eOperandType type, std::string const & value);

	private:
		static std::regex const _regexFloat;
		static std::regex const _regexInt;

		std::queue<Instruction const *>	_listInstr;
};
