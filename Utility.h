#ifndef UTILITY_H
#define UTILITY_H

//
// Header for utility classes and functions that are shared
// throughout the GLUT application.
//

#include <exception>
#include <string>

//
// Generic Error exception class to be used throughout the application.
//
class Error : public std::exception {
public:
	// Initialize the message for the exception.
	Error(const std::string &in_msg = "")
		: msg(in_msg) {}

	// Override description function.
	const char *what() const noexcept override { return msg.c_str(); }

private:
	// Error msg to be shown.
	const std::string msg;

};

#endif // UTILITY_H
