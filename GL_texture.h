#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <GL/glew.h>

//
// Class that represents an OpenGL texture object.
// Wade Bonkowski - 11/13/2016
// NOTE: Must be instantiated after GL context setup (GLUT).
//

class GL_texture {
public:
	// Load the texture from file.
	GL_texture(const char * const texture_filename);

	// Bind and map this texture to the provided GLSL program
	// variable location.
	void bind(GLuint uniform_location);

	// Sets the texture mapping back to the first unit.
	static void reset_current_texture() { current_texture_unit = 0; }

private:
	GLuint texture;

	static unsigned char current_texture_unit;
	static const GLenum texture_units[];

};

#endif // GL_TEXTURE_H
