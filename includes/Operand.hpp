#pragma once

#warning this class must be a template
class Operand {
	public:
		Operand();
		Operand(Operand const &src);
		virtual ~Operand();

		Operand &operator=(Operand const &rhs);
	protected:
	private:
};
