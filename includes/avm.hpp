#pragma once

#include <string>
#include <regex>
#include "Operand.hpp"

class Avm {
	public:
		Avm();
		Avm(Avm const &src);
		virtual ~Avm();

		Avm &operator=(Avm const &rhs);

		IOperand const * createInt8(std::string const & value) const;
		IOperand const * createInt16(std::string const & value) const;
		IOperand const * createInt32(std::string const & value) const;
		IOperand const * createFloat(std::string const & value) const;
		IOperand const * createDouble(std::string const & value) const;
		IOperand const * createOperand(eOperandType type, std::string const & value) const;

	protected:
	private:
		static std::regex regexFloat;
		static std::regex regexInt;
};
