
CC = gcc

CFLAGS=-g -O0 -lglut -lGLU -lGL -lm -fopenmp -g
# for mac
#CFLAGS=-D MAC -O3 -framework OpenGL -framework GLUT -lm

OBJS= \
basic_math.o \
bz_lines.o \
calc_nodeline.o \
draw.o \
fermisurfer.o \
fermi_patch.o \
free_patch.o \
initialize.o \
kumo.o \
menu.o \
operation.o \
read_file.o \
section.o

HEADERS= \
basic_math.h \
bz_lines.h \
calc_nodeline.h \
draw.h \
fermi_patch.h \
free_patch.h \
initialize.h \
kumo.h \
menu.h \
operation.h \
read_file.h \
section.h \
variable.h

all:fermisurfer bxsf2frmsf

SUFFIXES: .o .c

.c.o:
	$(CC) $(CFLAGS) -c $<

fermisurfer:$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

bxsf2frmsf:bxsf2frmsf.o
	$(CC) $< $(CFLAGS) -o $@

install:
	sudo cp fermisurfer /usr/local/bin/fermisurfer

uninstall:
	sudo rm /usr/local/bin/fermisurfer

clean:
	rm -rf *.o fermisurfer bxsf2frmsf

basic_math.o:$(HEADERS)
bz_lines.o:$(HEADERS)
calc_nodeline.o:$(HEADERS)
draw.o:$(HEADERS)
fermisurfer.o:$(HEADERS)
fermi_patch.o:$(HEADERS)
free_patch.o:$(HEADERS)
initialize.o:$(HEADERS)
kumo.o:$(HEADERS)
menu.o:$(HEADERS)
operation.o:$(HEADERS)
read_file.o:$(HEADERS)
section.o:$(HEADERS)
