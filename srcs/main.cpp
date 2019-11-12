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
	bool	hasFiles = false;
	bool	instantExec = false;

	if (ac >= 2) {
		for (int i=1; i < ac; i++) {
			if (strlen(av[i]) >= 2 && av[i][0] == '-' && av[i][1] == '-') {
				if (strcmp(av[i], "--verbose") == 0) {
					std::cout << BOLD << "[" << GREEN << "+" << EOC << BOLD << "] " << EOC
					"verbose mode activaed" << std::endl;
					avm.setVerbose(true);
				} else if (strcmp(av[i], "--instant-exec") == 0) {
					std::cout << BOLD << "[" << GREEN << "+" << EOC << BOLD << "] " << EOC
					"instant exec mode activaed" << std::endl;
					instantExec = true;
				} else if (strcmp(av[i], "--usage") == 0) {
					std::cout << "Usage: ./avm [--opt] [files]" << std::endl;
					std::cout << "\t--verbose: verbose mode" << std::endl;
					std::cout << "\t--instant-exec: (if stdin mode) exec instant all instructions" << std::endl;
					return 0;
				} else {
					std::cout << Error(-1, av[i], "invalid option");
				}
			} else {
				hasFiles = true;
			}
		}
	}

	if (hasFiles) {
		for (int i=1; i < ac; i++) {
			if (strlen(av[i]) >= 2 && av[i][0] == '-' && av[i][1] == '-')
				continue;
			if (avm.isVerbose()) {
				std::cout << BOLD << "exec " << EOC << av[i] << ":" << std::endl;
			}
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
			if (instantExec) {
				if (isError) {
					parser.printErrors();
				}
				avm.exec(true);
				parser.clearErrors();
				if (avm.getExitStatus())
					break;  // exit
			}
			i++;
		}
		if (isError) {
			parser.printErrors();
		}
		if (!instantExec)
			avm.exec();
		avm.clearInstr();
		parser.clearErrors();
	}
	return 0;
}
