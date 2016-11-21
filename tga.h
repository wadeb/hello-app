#ifndef TGA_H
#define TGA_H

#include <string>

//
// Function that reads pixel, width, and height information from a TGA file.
//
void *read_tga(const std::string &filename, int &width, int &height);

#endif // TGA_H
