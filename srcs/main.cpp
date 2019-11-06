#include <iostream>
#include "Operand.hpp"
#include "Avm.hpp"
#include "Parser.hpp"

int		main(int ac, char **av) {
	(void)ac;
	(void)av;
	Avm		avm;
	Parser	parser(avm);

	parser.parseFromString(std::string("line\nother line\nagain a line"));

	// IOperand const *int8Var = Avm::createOperand(Int32, av[1]);
	// IOperand const *floatVar = Avm::createOperand(Double, av[1]);

	// std::cout << int8Var->toString() << std::endl;
	// std::cout << floatVar->toString() << std::endl;
	// std::cout << "float " << (*floatVar / *int8Var)->toString() << std::endl;
	// std::cout << "int " << (*int8Var + *int8Var)->toString() << std::endl;

	return 0;
}
