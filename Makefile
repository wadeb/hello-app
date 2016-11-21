CC = g++
LD = g++

CFLAGS = -c -g -I/usr/X11R6/include -std=c++14 -Wall -Werror -Wextra
CFLAGS += -pedantic-errors

LDFLAGS = -g -L/usr/X11R6/lib -lGL -lGLEW -lglut

OBJS = Glut_hello_app.o tga.o GL_program.o GL_texture.o main.o

all: hello_app

hello_app: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o hello_app

main.o: main.cpp Glut_hello_app.h
	$(CC) $(CFLAGS) main.cpp

Glut_hello_app.o: Glut_hello_app.cpp Glut_hello_app.h Utility.h GL_texture.h GL_program.h
	$(CC) $(CFLAGS) Glut_hello_app.cpp

tga.o: tga.cpp tga.h Utility.h
	$(CC) $(CFLAGS) tga.cpp

GL_program.o: GL_program.cpp GL_program.h Utility.h
	$(CC) $(CFLAGS) GL_program.cpp

GL_texture.o: GL_texture.cpp GL_texture.h tga.h Utility.h
	$(CC) $(CFLAGS) GL_texture.cpp

clean:
	rm -f *.o hello_app

.PHONY: all clean
