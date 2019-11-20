#pragma once

#include <string>

enum eOperandType {
	Int8, Int16, Int32, Float, Double
};

class IOperand {
	public:
		virtual int getPrecision() const = 0;  // Precision of the type of the instance
		virtual eOperandType getType() const = 0;  // Type of the instance
		virtual IOperand const * operator+(IOperand const & rhs) const = 0;  // Sum
		virtual IOperand const * operator-(IOperand const & rhs) const = 0;  // Difference
		virtual IOperand const * operator*(IOperand const & rhs) const = 0;  // Product
		virtual IOperand const * operator/(IOperand const & rhs) const = 0;  // Quotient
		virtual IOperand const * operator%(IOperand const & rhs) const = 0;  // Modulo
		virtual bool operator==(IOperand const & rhs) const = 0;  // Modulo
		virtual std::string const toString() const = 0;  // String representation of the instance
		virtual char toChar() const = 0;  // Char representation
		virtual double toDouble() const = 0;  // String representation of the instance
		virtual int64_t toInt64() const = 0;  // String representation of the instance
		virtual ~IOperand();
};
