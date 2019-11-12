#pragma once

#include <stdexcept>
#include <string>
#include <ostream>

#define EOC	"\x1B[0m"

#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

#define F_RED  "\x1B[41m"
#define F_GREEN  "\x1B[42m"
#define F_YELLOW	"\x1B[43m"
#define F_BLUE  "\x1B[44m"
#define F_MAGENTA  "\x1B[45m"
#define F_CYAN  "\x1B[46m"
#define F_WHITE  "\x1B[47m"

#define CLEAR "\033[H\033[2J"
#define BOLD "\e[1m"
#define LIGHT "\e[2m"
#define ITALIC "\e[3m"
#define ULINE "\e[4m"

struct Error {
	int			lineNbr;
	std::string	line;
	std::string	errorMsg;

	Error();
	Error(int ln, std::string lineStr, std::string msg);
};
std::ostream &operator<<(std::ostream &o, const Error &other);

class AvmError : public std::exception {
	public:
		virtual char const * what() const throw();
};

class ConvertError : public AvmError {
	public:
		virtual char const * what() const throw();
};

class OverflowError : public AvmError {
	public:
		virtual char const * what() const throw();
};

class StackEmptyError : public AvmError {
	public:
		virtual char const * what() const throw();
};

class InvalidInstructionError : public AvmError {
	public:
		virtual char const * what() const throw();
};

class ModOnDoubleError : public AvmError {
	public:
		virtual char const * what() const throw();
};

class DivByZeroError : public AvmError {
	public:
		virtual char const * what() const throw();
};
