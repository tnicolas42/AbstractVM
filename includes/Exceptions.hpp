#pragma once

#include <stdexcept>
#include <string>
#include <ostream>

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
