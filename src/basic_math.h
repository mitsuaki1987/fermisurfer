/*
The MIT License (MIT)

Copyright (c) 2014 Mitsuaki KAWAMURA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int modulo(int i, int n);
GLfloat solve3(
  GLfloat a[3][3] /**< [in] Matix*/,
  GLfloat b[3] /**< [inout] Right hand side vector*/);
void eigsort(
  int n /**< [in] the number of components*/,
  GLfloat* eig2 /**< [inout] the orbital energy*/,
  GLfloat* mat2 /**< [inout] the matrix element*/,
  GLfloat kvec2[][3] /**< [inout] k-vectors of corners*/);
void normal_vec(
  GLfloat in1[3] /**< [in] Corner 1*/,
  GLfloat in2[3] /**< [in] Corner 2*/,
  GLfloat in3[3] /**< [in] Corner 3*/,
  GLfloat out[3] /**< [out] The normal vector*/);
int get_thread();
