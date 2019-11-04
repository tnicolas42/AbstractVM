#pragma once

#include <stdexcept>
#include <string>

class AvmError : public std::exception {
	public:
		virtual char const * what() const throw();
};

class ConvertError : public AvmError {
	public:
		virtual char const * what() const throw();
};
