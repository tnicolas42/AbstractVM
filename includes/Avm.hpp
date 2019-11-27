#pragma once

#include <string>
#include <regex>
#include <queue>
#include <deque>
#include <map>
#include "Operand.hpp"
#include "Exceptions.hpp"

#define WHITESPACE "\t "

enum eInstr {
	InstrPush,
	InstrPop,
	InstrDump,
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
			std::string		value;

			// for errors
			std::string		lineStr;
			int				lineNbr;

			Instruction();
			~Instruction();
		};

		void saveInstr(Instruction const *instr);
		void clearInstr();
		void exec(bool instantExec = false);

		bool getExitStatus() const;
		bool isVerbose() const;
		void setVerbose(bool verbose);

		/* factory to create operand */
		typedef IOperand const * (*createFunc)(std::string const &);
		static IOperand const * createInt8(std::string const & value);
		static IOperand const * createInt16(std::string const & value);
		static IOperand const * createInt32(std::string const & value);
		static IOperand const * createFloat(std::string const & value);
		static IOperand const * createDouble(std::string const & value);
		static IOperand const * createOperand(eOperandType type, std::string const & value);

	private:
		static std::regex const _regexFloat;
		static std::regex const _regexInt;

		Error	*_error;
		bool	_isExit;
		bool	_isVerbose;

		std::queue<Instruction const *>	_listInstr;
		std::deque<IOperand const *> _stack;

		/* used in factory */
		static std::map<eOperandType, createFunc> _createMap;

		void _stackEmptyError(Instruction const *instr);

		void _execDump();
		void _execAssert(Instruction const *instr);
		void _execCalc(Instruction const *instr);
		void _execOneInstr(Instruction const *instr);
};
