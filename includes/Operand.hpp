#pragma once

#include <string>
#include <iostream>
#include <limits>
#include "IOperand.hpp"
#include "Avm.hpp"
#include "Exceptions.hpp"

enum eOperator {
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD
};

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

		int getPrecision() const {
			return static_cast<int>(type);
		}
		eOperandType getType() const {
			return type;
		}
		IOperand const * operator+(IOperand const & rhs) const {
			return operation(rhs, OP_ADD);
		}
		IOperand const * operator-(IOperand const & rhs) const {
			return operation(rhs, OP_SUB);
		}
		IOperand const * operator*(IOperand const & rhs) const {
			return operation(rhs, OP_MUL);
		}
		IOperand const * operator/(IOperand const & rhs) const {
			return operation(rhs, OP_DIV);
		}
		IOperand const * operator%(IOperand const & rhs) const {
			return operation(rhs, OP_MOD);
		}
		bool operator==(IOperand const & rhs) const {
			eOperandType	retType = (getType() > rhs.getType()) ? getType() : rhs.getType();

			if (retType >= Float) {  // float comparison
				return toDouble() == rhs.toDouble();
			}
			// int comparison
			return toInt64() == rhs.toInt64();
		}
		std::string const toString() const {
			return std::to_string(_value);
		}
		double toDouble() const {
			return static_cast<double>(_value);
		}
		int64_t toInt64() const {
			return static_cast<int64_t>(_value);
		}

	private:
		T	_value;

		IOperand const * operation(IOperand const & rhs, eOperator op) const {
			IOperand		*res = nullptr;
			eOperandType	retType = (getType() > rhs.getType()) ? getType() : rhs.getType();
			bool			floatType = false;
			double			dval = 0;
			int64_t			ival = 0;

			if (retType >= Float) {
				floatType = true;
			}

			switch (op) {
				case OP_ADD:
					if (floatType)
						dval = this->toDouble() + rhs.toDouble();
					else
						ival = this->toInt64() + rhs.toInt64();
					break;
				case OP_SUB:
					if (floatType)
						dval = this->toDouble() - rhs.toDouble();
					else
						ival = this->toInt64() - rhs.toInt64();
					break;
				case OP_MUL:
					if (floatType)
						dval = this->toDouble() * rhs.toDouble();
					else
						ival = this->toInt64() * rhs.toInt64();
					break;
				case OP_DIV:
					if (floatType)
						dval = this->toDouble() / rhs.toDouble();
					else
						ival = this->toInt64() / rhs.toInt64();
					break;
				case OP_MOD:
					if (floatType)
						dval = 0;
					else
						ival = this->toInt64() + rhs.toInt64();
					break;
			}
			switch (retType) {
				case Int8:
					if (ival > static_cast<int64_t>(std::numeric_limits<int8_t>::max())
					|| ival < static_cast<int64_t>(std::numeric_limits<int8_t>::min()))
						throw OverflowError();
					res = new Operand<int8_t, Int8>(static_cast<int8_t>(ival));
					break;
				case Int16:
					if (ival > static_cast<int64_t>(std::numeric_limits<int16_t>::max())
					|| ival < static_cast<int64_t>(std::numeric_limits<int16_t>::min()))
						throw OverflowError();
					res = new Operand<int16_t, Int16>(static_cast<int16_t>(ival));
					break;
				case Int32:
					if (ival > static_cast<int64_t>(std::numeric_limits<int32_t>::max())
					|| ival < static_cast<int64_t>(std::numeric_limits<int32_t>::min()))
						throw OverflowError();
					res = new Operand<int32_t, Int32>(ival);
					break;
				case Float:
					res = new Operand<float, Float>(static_cast<float>(dval));
					break;
				case Double:
					res = new Operand<double, Double>(dval);
					break;
			}
			return res;
		}
};

typedef Operand<int8_t, Int8>		OperandInt8;
typedef Operand<int16_t, Int16>	OperandInt16;
typedef Operand<int32_t, Int32>	OperandInt32;
typedef Operand<float, Float>		OperandFloat;
typedef Operand<double, Double>	OperandDouble;
