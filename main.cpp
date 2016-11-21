//
// Driver for the program.
// Wade Bonkowski - 9/28/2016
//

#include "Glut_hello_app.h"

#include <exception>
#include <iostream>

using std::exception;
using std::cerr;
using std::endl;

int main(int argc, char **argv)
{
	try {
		// Create and run the app.
		Glut_hello_app app(argc, argv);
		app.main_loop();
	} catch (exception &error) {
		// Output any errors during initialization and running.
		cerr << error.what() << endl;
		return 1;
	}

	return 0;
}
