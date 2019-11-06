#include "Exceptions.hpp"

Error::Error() : lineNbr(-1), line(""), errorMsg("") {}
Error::Error(int ln, std::string lineStr, std::string msg) :
lineNbr(ln), line(lineStr), errorMsg(msg) {}

std::ostream &operator<<(std::ostream &o, const Error &other) {
	o << "Error (line " << other.lineNbr << ") -> " << other.line << std::endl;
	o << "\t" << other.errorMsg << std::endl;
	return o;
}

char const * AvmError::what() const throw() {
	return "avm error";
}
char const * ConvertError::what() const throw() {
	return "conversion error";
}
char const * OverflowError::what() const throw() {
	return "overflow";
}
