#ifndef GLUT_HELLO_APP_H
#define GLUT_HELLO_APP_H

//
// Interface to first GLUT OpenGL application.
// Wade Bonkowski - 9/27/2016
//

// TODO: If this is going to be an object, it needs to not have
// 	 all static member variables. Which means that I need to find
// 	 a way to bind the instance of the object to its function in the
// 	 render loop.
// 	 It seems that the best way to do this might be to write my
// 	 own render loop.

#include "GL_program.h"
#include "GL_texture.h"

class Glut_hello_app {
public:
	// Ctor to initialize libraries and grab resources.
	Glut_hello_app(int argc, char **argv);

	// Dtor to clean up resources if need be.
	~Glut_hello_app();

	// No copying or moving.
	Glut_hello_app(const Glut_hello_app &) = delete;
	Glut_hello_app(Glut_hello_app &&) = delete;
	Glut_hello_app &operator=(const Glut_hello_app &) = delete;
	Glut_hello_app &operator=(Glut_hello_app &&) = delete;

	// Run the main application loop.
	void main_loop();

private:
	// Setup all application specific rendering resources.
	void make_resources(const char * const v_shader_filename);

	// Update the fade factor between the two images.
	static void update_timer();

	// Draw the current frame.
	static void render();

	// Helper function to make OpenGL buffers.
	static GLuint make_buffer(GLenum target,
					const void *buffer_data,
					GLsizei buffer_size);

	// GL state for the app.
	// NOTE: If I used my own main loop rather than GLUT
	// 	 these could member variables rather than class variables.
	static GLuint vertex_buffer, element_buffer;
	static GL_texture *textures[2];

	// Interface into the GLSL program.
	static GL_program *program; // A pointer to control the lifetime.

	// Variables for tracking variables in the OpenGL program.
	static GLint uniform_timer;
	static GLint uniform_textures[2];
	static GLint attribute_position;

	// Float that holds the fade factor for this frame.
	static GLfloat timer;

};

#endif // GLUT_HELLO_APP_H
