#include "Glut_hello_app.h"
#include "Utility.h"

// Standard includes
#include <stdlib.h> // Included before GLUT due to bug in Visual Studio.

// GL includes.
#ifdef __APPLE__ // For the case of compilation on OS X.
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <cmath>

// Constants

// Vertex buffer data. Just need a vertex at each corner of the window.
const GLfloat vertex_buffer_data_c[] = {
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f
};

// Element buffer data. Each shingle triple denotes a triangle.
const GLushort element_buffer_data_c[] = {0, 1, 2, 3};

// Texture file names
const char * const texture_file_1_c = "hello1.tga";
const char * const texture_file_2_c = "hello2.tga";

// Shader filenames.
const char * const v_shader_filename_c = "glsl/hello-gl.v.glsl";
const char * const f_shader_filename_c = "glsl/hello-gl.f.glsl";

// Declaration/initialization of static member variables.
GLuint Glut_hello_app::vertex_buffer = 0;
GLuint Glut_hello_app::element_buffer = 0;
GL_texture *Glut_hello_app::textures[2] = {
	nullptr,
	nullptr
};

GL_program *Glut_hello_app::program = nullptr;

GLint Glut_hello_app::uniform_timer = 0;
GLint Glut_hello_app::uniform_textures[2] = {0, 0};
GLint Glut_hello_app::attribute_position = 0;
GLfloat Glut_hello_app::timer = 0.0;

//
// Initialize GLUT, GLEW, and create acquire all resources
// needed for rendering.
//
Glut_hello_app::Glut_hello_app(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Hello World");
	glutDisplayFunc(&Glut_hello_app::render);
	glutIdleFunc(&Glut_hello_app::update_timer);

	// GLEW initialization.
	glewInit();
	if (!GLEW_VERSION_2_0) {
		throw Error("OpenGL 2.0 not available");
	}

	// Setup. Pass in the vertex shader filename if there is one.
	make_resources(argc >= 2 ? argv[1] : v_shader_filename_c);
}

//
// Frees the program instance if there is one.
// NOTE: Check to see if there is anything that needs
// 	 to be freed for GLUT.
//
Glut_hello_app::~Glut_hello_app()
{
	// Okay to delete nullptrs here.
	delete program;
	delete textures[0];
	delete textures[1];
}

//
// Runs the render loop for the GLUT app.
//
void Glut_hello_app::main_loop()
{
	glutMainLoop();
}

//
// Setup all of the rendering resources for the application
//
void Glut_hello_app::make_resources(const char * const v_shader_filename)
{
	// Allocate and bind the vertex and element buffers.
	vertex_buffer = make_buffer(GL_ARRAY_BUFFER,
					vertex_buffer_data_c,
					sizeof(vertex_buffer_data_c));

	element_buffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER,
					element_buffer_data_c,
					sizeof(element_buffer_data_c));

	textures[0] = new GL_texture(texture_file_1_c);
	textures[1] = new GL_texture(texture_file_2_c);

	// Create the program.
	program = new GL_program(
		v_shader_filename,
		f_shader_filename_c
	);

	// Save the locations of the input variables to the GLSL program.
	uniform_timer = program->uniform_location("timer");
	uniform_textures[0] = program->uniform_location("textures[0]");
	uniform_textures[1] = program->uniform_location("textures[1]");
	attribute_position = program->attribute_location("position");
}

//
// Update the fade factor between the two images.
//
void Glut_hello_app::update_timer()
{
	int milliseconds = glutGet(GLUT_ELAPSED_TIME);
	// Use sin to make a smooth change of color.
	timer = (float)milliseconds * 0.001f;
	// Force the render function to be called for the next loop.
	glutPostRedisplay();
}

//
// Draw the current frame.
//
void Glut_hello_app::render()
{
	// Reset the default color of the frame buffer.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	program->use_program();

	// Set the fade factor for this frame.
	glUniform1f(uniform_timer, timer);

	// Rebind the textures
	GL_texture::reset_current_texture();
	textures[0]->bind(uniform_textures[0]);
	textures[1]->bind(uniform_textures[1]);

	// Bind the attributes in the vertex position attributes in the
	// vertex buffer to the uniform position variable in the shader program.
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(
		attribute_position, // attribute in the shader program.
		4, // size of the attribute (vec2 for 2D position)
		GL_FLOAT, // type of each attribute element.
		GL_FALSE, // normalized?
		sizeof(GLfloat) * 4, // stride (no interleaving)
		(void *)0 // Pointer representation of start offset.
	);

	glEnableVertexAttribArray(attribute_position);

	// Submit a rendering job to the GPU.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glDrawElements(
		GL_TRIANGLE_STRIP, // triangle mode.
		4, // number of vertices to make triangles from.
		GL_UNSIGNED_SHORT, // type for each element index.
		(void *)0 // pointer offset from the sstart of the buffer.
	);

	// Disable the vertex attribute array since we no longer need it.
	glDisableVertexAttribArray(attribute_position);

	// NOTE: Shouldn't need these any longer.
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

//
// Helper function to create and configure OpenGL buffers.
//
GLuint Glut_hello_app::make_buffer(GLenum target,
					const void *buffer_data,
					GLsizei buffer_size)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);

	return buffer;
}
