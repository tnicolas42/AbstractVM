#pragma once

#include <string>
#include "IOperand.hpp"

template<typename T, eOperandType type>
class Operand : public IOperand {
	public:
		Operand() : _value(0) {}
		explicit Operand(T const value) : _value(value) {}
		Operand(Operand const &src) { *this = src; }
		virtual ~Operand() {}

		Operand &operator=(Operand const &rhs) {
			(void)rhs;
			// if (this != &rhs) {}
			return *this;
		}

		virtual int getPrecision() const {
			return static_cast<int>(type);
		}
		virtual eOperandType getType() const {
			return type;
		}
		// virtual IOperand const * operator+(IOperand const & rhs) const {

		// }
		// virtual IOperand const * operator-(IOperand const & rhs) const {

		// }
		// virtual IOperand const * operator*(IOperand const & rhs) const {

		// }
		// virtual IOperand const * operator/(IOperand const & rhs) const {

		// }
		// virtual IOperand const * operator%(IOperand const & rhs) const {

		// }
		virtual std::string const toString() const {
			return std::to_string(_value);
		}

	private:
		T	_value;
};

typedef Operand<int8_t, Int8>		OperandInt8;
typedef Operand<int16_t, Int16>	OperandInt16;
typedef Operand<int32_t, Int32>	OperandInt32;
typedef Operand<float, Float>		OperandFloat;
typedef Operand<double, Double>	OperandDouble;
