#ifndef GLSL_PROGRAM_H
#define GLSL_PROGRAM_H

#include <GL/glew.h>
#include <string>

// TODO: In future OpenGL applications, make a class for each
// 	 opaque object that is part of the interface.

//
// Class that encapsulates functionality relating to the
// creation and usage of a GLSL program.
// Wade Bonkowski - 10/9/2016
//

class GL_program {
public:
	// Create the program from vertex and fragment shader source.
	GL_program(
		const char * const v_shader_filename,
		const char * const f_shader_filename
	);

	// Free the program handle resource.
	~GL_program();

	// Functions for getting variable locations from the GLSL program.
	// TODO: Make these functions return classes that encapsulate variables
	// 	 in the GLSL program.
	GLint uniform_location(const char * const uniform_name);
	GLint attribute_location(const char * const attribute_name);

	// Tell OpenGL that this program is the one to use currently.
	void use_program();

private:
	// Creates a shader object.
	GLuint make_shader(GLenum type, const char * const filename);

	// Reads the contents of a shader file.
	std::basic_string<GLchar> read_shader_file(const char * const filename);

	// Prints information from the shader compilation log.
	void show_info_log(
		GLuint object,
		PFNGLGETSHADERIVPROC glGet__iv,
		PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
	);

	GLuint program;

};

#endif // GLSL_PROGRAM_H
