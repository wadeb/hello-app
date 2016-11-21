#include "GL_program.h"
#include "Utility.h"

#include <iostream>
#include <fstream>
#include <iterator>

using std::basic_string;
using std::string;
using std::ifstream;
using std::istream_iterator;
using std::cerr;
using std::endl;
using std::ios_base;

//
// Compile both of the shaders that comprise the GLSL program
// and link them together.
// Throws Error on compilation or linking failure.
//
GL_program::GL_program(
	const char * const v_shader_filename,
	const char * const f_shader_filename
)
{
	// Compile the shaders that comprise the program.
	GLuint v_shader = make_shader(GL_VERTEX_SHADER, v_shader_filename),
		f_shader = make_shader(GL_FRAGMENT_SHADER, f_shader_filename);

	program = glCreateProgram();
	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	glLinkProgram(program);

	// Check to see that linking went okay.
	GLint program_ok;
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		throw Error("Failed to link shader program");
	}
}

//
// Free the program handle.
//
GL_program::~GL_program()
{
	glDeleteProgram(program);
}

//
// Return an OpenGL handle that represents the location of the
// named variable in the GLSL program.
//
GLint GL_program::uniform_location(const char * const uniform_name)
{
	return glGetUniformLocation(program, uniform_name);
}

//
// Return an OpenGL handle that represents the location of the
// named variable in the GLSL program.
//
GLint GL_program::attribute_location(const char * const attribute_name)
{
	return glGetAttribLocation(program, attribute_name);
}

//
// Lets the OpenGL library know that this the shader program
// to use currently.
//
void GL_program::use_program()
{
	glUseProgram(program);
}

//
// Helper function to read and compile a shader.
// Throws Error on compilation failure.
//
GLuint GL_program::make_shader(GLenum type, const char * const filename)
{
	basic_string<GLchar> source = read_shader_file(filename);
	GLuint shader = glCreateShader(type);
	GLint length = source.length();
	const GLchar *source_str = source.c_str();
	glShaderSource(shader, 1, &source_str, &length);
	glCompileShader(shader);
	GLint shader_ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		throw Error("Failed to compile: " + string(filename));
	}

	return shader;
}

//
// Helper function for reading the source of a GLSL shader file.
//
basic_string<GLchar> GL_program::read_shader_file(const char * const filename)
{
	ifstream shader_input(filename);
	// Make sure that no whitespace is skipped.
	shader_input.unsetf(ios_base::skipws);

	// Create the basic string using an input iterator.
	return basic_string<GLchar>(
		istream_iterator<GLchar>(shader_input),
		istream_iterator<GLchar>()
	);
}

//
// Prints information from the shader compilation log.
//
void GL_program::show_info_log(
	GLuint object,
	PFNGLGETSHADERIVPROC glGet__iv,
	PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
	GLint log_length;
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	char *log = new char[log_length];
	glGet__InfoLog(object, log_length, nullptr, log);
	cerr << *log << endl;
	delete[] log;
}
