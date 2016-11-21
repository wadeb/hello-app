#include "GL_texture.h"
#include "Utility.h"
#include "tga.h"

// Number of texturej units available from OpenGL.
const unsigned char num_texture_units_c = 32;

// Start with the first texture unit.
unsigned char GL_texture::current_texture_unit = 0;

// Texture unit enums that are available from gl.h
// TODO: Make this a general contant, rather than a static member variable.
const GLenum GL_texture::texture_units[] = {
	GL_TEXTURE0,
	GL_TEXTURE1,
	GL_TEXTURE2,
	GL_TEXTURE3,
	GL_TEXTURE4,
	GL_TEXTURE5,
	GL_TEXTURE6,
	GL_TEXTURE7,
	GL_TEXTURE8,
	GL_TEXTURE9,
	GL_TEXTURE10,
	GL_TEXTURE11,
	GL_TEXTURE12,
	GL_TEXTURE13,
	GL_TEXTURE14,
	GL_TEXTURE15,
	GL_TEXTURE16,
	GL_TEXTURE17,
	GL_TEXTURE18,
	GL_TEXTURE19,
	GL_TEXTURE20,
	GL_TEXTURE21,
	GL_TEXTURE22,
	GL_TEXTURE23,
	GL_TEXTURE24,
	GL_TEXTURE25,
	GL_TEXTURE26,
	GL_TEXTURE27,
	GL_TEXTURE28,
	GL_TEXTURE29,
	GL_TEXTURE30,
	GL_TEXTURE31	
};

//
// Load the texture from TGA file.
// NOTE: Currently default to linear interpolation between texels
//       and clamping indices outside of the texture edge.
// TODO: Support other, more common, texture file types.
//
GL_texture::GL_texture(const char * const texture_filename)
{
	int width, height;
	void *pixels = read_tga(texture_filename, width, height);
	if (!pixels) {
		throw Error("Could not read TGA file");
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D, // target
		0, // LOD
		GL_RGB8, // Internal format
		width,
		height,
		0, // border
		GL_BGR, // External format
		GL_UNSIGNED_BYTE, // type
		pixels // Pixel data array
	);

	free(pixels);
}

//
// Bind and map this texture to the provided GLSL program variable location.
//
void GL_texture::bind(GLuint uniform_location)
{
	if (current_texture_unit >= num_texture_units_c) {
		throw Error("All texture units already taken");
	}

	glActiveTexture(texture_units[current_texture_unit]);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(uniform_location, current_texture_unit);
	current_texture_unit++;
}
