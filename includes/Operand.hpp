#pragma once

#include <unistd.h>
#include "IOperand.hpp"

template<typename T>
class Operand : public IOperand {
	public:
		Operand() {}
		Operand(Operand const &src) { *this = src; }
		virtual ~Operand() {}

		Operand &operator=(Operand const &rhs) {
			if (this != &rhs) {}
			return *this;
		}
	protected:
	private:
};

typedef OperandInt8 Operand<int8_t>;
