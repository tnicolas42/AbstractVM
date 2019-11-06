#pragma once

#include <string>
#include <regex>
#include <queue>
#include "Operand.hpp"

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
			union Data {
				int8_t	Int8;
				int16_t	Int16;
				int32_t	Int32;
				float	Float;
				double	Double;
			} data;
		};

		void saveInstr(Instruction const &instr);

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

		std::queue<Instruction const>	_listInstr;
};
