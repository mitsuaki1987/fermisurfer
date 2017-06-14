Re-distribution of this program
===============================

Contain Fermisurfer in your program
-----------------------------------

FermiSurfer is distributed with the :ref:`mitlicense`.
To summarize this, you can freely modify, copy and paste FermiSurfer to any program
such as a private program (in the research group, co-workers, etc.),
open-source, free, and commercial software.
Also, you can freely choose the license to distribute your program.

Build FermiSurfer without Autoconf
----------------------------------

In this package, FermiSurfer is built with Autotools (Autoconf, Automake, Libtool).
If you do not want to use Autotools for your distributed program with FermiSurfer's source,
you can use the following simple Makefile (please care about TAB).

.. code-block:: makefile

   CC = gcc
   CFLAGS = -g -O2 -lglut -lGLU -lGL -lm -fopenmp -DHAVE_GL_GLUT_H
   
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

.. _mitlicense:

MIT License
-----------

| Copyright (c) 2014 Mitsuaki Kawamura
|
| Permission is hereby granted, free of charge, to any person obtaining a
| copy of this software and associated documentation files (the
| "Software"), to deal in the Software without restriction, including
| without limitation the rights to use, copy, modify, merge, publish,
| distribute, sublicense, and/or sell copies of the Software, and to
| permit persons to whom the Software is furnished to do so, subject to
| the following conditions:
| 
| The above copyright notice and this permission notice shall be included
| in all copies or substantial portions of the Software.
|
| THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
| OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
| MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
| IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
| CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
| TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
| SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
