#include <iostream>
#include "Operand.hpp"

int		main(int ac, char **av) {
	(void)ac;
	(void)av;
	OperandInt8 int8Var;
	OperandFloat floatVar;

	std::cout << int8Var.getType() << std::endl;
	std::cout << floatVar.getType() << std::endl;

	return 0;
}
