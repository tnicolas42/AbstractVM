#include "Exceptions.hpp"

char const * AvmError::what() const throw() {
	return "avm error";
}
char const * ConvertError::what() const throw() {
	return "conversion error";
}
