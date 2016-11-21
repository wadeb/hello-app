#include "tga.h"
#include "Utility.h"

#include <cstdio>
#include <cstdlib>

using std::string;

//
// C File with C++ exceptions and references added that contains source
// for reading texture information from TGA files.
//

//
// Helper function for packing TGA width and height information
// into one integer.
//
static short le_short(unsigned char *bytes)
{
	return bytes[0] | ((char)bytes[1] << 8);
}

//
// Read pixel, width, and height information from a TGA file.
// Return the pixel information as a pointer to the buffer.
// Width and height information is returned via the reference parameters.
//
void *read_tga(const string &filename, int &width, int &height)
{
	struct tga_header {
		char id_length;
		char color_map_type;
		char data_type_code;
		unsigned char color_map_origin[2];
		unsigned char color_map_length[2];
		char color_map_depth;
		unsigned char x_origin[2];
		unsigned char y_origin[2];
		unsigned char width[2];
		unsigned char height[2];
		char bits_per_pixel;
		char image_descriptor;
	} header;

	int color_map_size;
	unsigned pixels_size;
	FILE *file;
	size_t read;
	void *pixels;

	file = fopen(filename.c_str(), "rb");
	if (!file)
		throw Error("Unable to open " + filename + " for reading");

	read = fread(&header, 1, sizeof(header), file);
	if (read != sizeof(header)) {
		fclose(file);
		throw Error(filename + " has incomplete tga header");
	}

	if (header.data_type_code != 2) {
		fclose(file);
		throw Error(filename + " is not an uncompressed RGB tga file");
	}

	if (header.bits_per_pixel != 24) {
		fclose(file);
		throw Error(filename + " is not a 24-bit uncompressed " +
				"RGB tga file");
	}

	// Verify header length information.
	for (int i = 0; i < header.id_length; i++) {
		if (getc(file) == EOF) {
			fclose(file);
			throw Error(filename + " has an incomplete id string");
		}
	}

	color_map_size = le_short(header.color_map_length) *
				(header.color_map_depth / 8);

	// Verify color map information.
	for (int i = 0; i < color_map_size; i++) {
		if (getc(file) == EOF) {
			fclose(file);
			throw Error(filename + " has incomplete color map");
		}
	}

	width = le_short(header.width);
	height = le_short(header.height);
	// Calculate the number of bytes that the pixel information occupies.
	pixels_size = width * height * (header.bits_per_pixel / 8);
	pixels = malloc(pixels_size);
	read = fread(pixels, 1, pixels_size, file);
	fclose(file);
	if (read != pixels_size) {
		free(pixels);
		throw Error(filename + " has incomplete image");
	}

	return pixels;
}
