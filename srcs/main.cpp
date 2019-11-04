#include <iostream>
#include "Operand.hpp"
#include "Avm.hpp"

int		main(int ac, char **av) {
	(void)ac;
	(void)av;
	Avm avm;
	IOperand const *int8Var = avm.createOperand(Int8, "12");
	IOperand const *floatVar = avm.createOperand(Float, "12.0");

	std::cout << int8Var->toString() << std::endl;
	std::cout << floatVar->toString() << std::endl;
	std::cout << "float " << (*floatVar / *int8Var)->toString() << std::endl;
	std::cout << "int " << (*int8Var + *int8Var)->toString() << std::endl;

	return 0;
}
