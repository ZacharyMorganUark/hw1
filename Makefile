#-------------------------------------------------
# sample Makefile for OpenGL
#-------------------------------------------------

# define OpenGL libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif

# define compiler options
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3

hw6: hw6.cpp
	$(CC) -o hw6 hw6.cpp $(LIB)
