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

#include <math.h>
#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
*Work as Modulo function of fortran
*/
int modulo(int i, int n) {
  int j;
  j = (i + 100 * n) % n;
  return j;
}/*modulo(int i, int n)*/
 /**
 * Solve linear system
 */
GLfloat solve3(
  GLfloat a[3][3] /**< [in] Matix*/,
  GLfloat b[3] /**< [inout] Right hand side vector*/)
{
  int i;
  GLfloat det, c[3];
  /**/
  det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
      + a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
      + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
  /**/
  c[0] = b[0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
       + b[1] * (a[0][2] * a[2][1] - a[0][1] * a[2][2])
       + b[2] * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);
  /**/
  c[1] = b[0] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
       + b[1] * (a[0][0] * a[2][2] - a[0][2] * a[2][0])
       + b[2] * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);
  /**/
  c[2] = b[0] * (a[1][0] * a[2][1] - a[1][1] * a[2][0])
       + b[1] * (a[0][1] * a[2][0] - a[0][0] * a[2][1])
       + b[2] * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
  /**/
  for (i = 0; i<3; ++i) b[i] = c[i] / det;
  return det;
  /**/
}
/**
* Sort eigenvalues
*/
void eigsort(
  int n /**< [in] the number of components*/,
  GLfloat* eig2 /**< [inout] the orbital energy*/,
  GLfloat* mat2 /**< [inout] the matrix element*/,
  GLfloat kvec2[][3] /**< [inout] k-vectors of corners*/)
{
  int i, j, k;
  GLfloat tmp;
  /**/
  for (i = 0; i < n - 1; ++i) {
    for (j = i + 1; j < n; ++j) {
      if (eig2[j] < eig2[i]) {
        tmp = eig2[j];
        eig2[j] = eig2[i];
        eig2[i] = tmp;
        /**/
        tmp = mat2[j];
        mat2[j] = mat2[i];
        mat2[i] = tmp;
        /**/
        for (k = 0; k < 3; ++k) {
          tmp = kvec2[j][k];
          kvec2[j][k] = kvec2[i][k];
          kvec2[i][k] = tmp;
        }
      }
    }
  }
} /* eigsort */
  /**
  * Calculate normal vector
  */
void normal_vec(
  GLfloat in1[3] /**< [in] Corner 1*/,
  GLfloat in2[3] /**< [in] Corner 2*/,
  GLfloat in3[3] /**< [in] Corner 3*/,
  GLfloat out[3] /**< [out] The normal vector*/)
{
  int i;
  GLfloat norm;
  out[0] = in1[1] * in2[2] - in1[2] * in2[1]
    + in2[1] * in3[2] - in2[2] * in3[1]
    + in3[1] * in1[2] - in3[2] * in1[1];
  out[1] = in1[2] * in2[0] - in1[0] * in2[2]
    + in2[2] * in3[0] - in2[0] * in3[2]
    + in3[2] * in1[0] - in3[0] * in1[2];
  out[2] = in1[0] * in2[1] - in1[1] * in2[0]
    + in2[0] * in3[1] - in2[1] * in3[0]
    + in3[0] * in1[1] - in3[1] * in1[0];
  norm = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
  for (i = 0; i<3; i++) out[i] = out[i] / norm;
} /* normal_vec */
