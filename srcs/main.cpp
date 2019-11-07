#include <iostream>
#include <string>
#include "Operand.hpp"
#include "Avm.hpp"
#include "Parser.hpp"
#include "Exceptions.hpp"

int		main(int ac, char **av) {
	(void)ac;
	(void)av;
	Avm		avm;
	Parser	parser(&avm);

	if (ac >= 2) {
		for (int i=1; i < ac; i++) {
			if (parser.parseFromFile(av[i]))
				avm.exec();
			else
				parser.printErrors();
			avm.clearInstr();
			parser.clearErrors();
		}
	} else {
		std::string line;
		std::vector<Error> errors;
		int i = 1;
		bool isError = false;
		while(std::cin) {
			std::getline(std::cin, line);
			if (parser.parseOneLine(line, i) == false)
				isError = true;
			if (parser.recvExecCommand)
				break;
			i++;
		}
		if (isError) {
			parser.printErrors();
		} else {
			avm.exec();
		}
			avm.clearInstr();
			parser.clearErrors();
	}
	return 0;
}
