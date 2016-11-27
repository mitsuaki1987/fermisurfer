
CC = gcc

CFLAGS=-O3 -lglut -lGLU -lGL -lm -g
# for mac
#CFLAGS=-D MAC -O3 -framework OpenGL -framework GLUT -lm

all:fermisurfer bxsf2frmsf

fermisurfer:fermisurfer.c
	$(CC) $< $(CFLAGS) -o $@

bxsf2frmsf:bxsf2frmsf.c
	$(CC) $< $(CFLAGS) -o $@

install:
	sudo cp fermisurfer /usr/local/bin/fermisurfer

uninstall:
	sudo rm /usr/local/bin/fermisurfer

clean:
	rm -rf fermisurfer bxsf2frmsf

