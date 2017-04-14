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

#include <stdlib.h>
#include "basic_math.h"
#include "variable.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
*Compute coefficient for the Kumo interpolation
*/
void kumo_coef(int j, GLfloat *coef) {
  GLfloat x, mx;
  x = (GLfloat)j / (GLfloat)interpol;
  mx = 1.0 - x;
  coef[0] = -0.5 * x * mx * mx;
  coef[1] = mx * (mx*mx + 3.0*x*mx + 0.5*x*x);
  coef[2] = x * (x*x + 3.0*mx*x + 0.5*mx*mx);
  coef[3] = -0.5 * x * x * mx;
}
/**
* Interpolation of energy and matrix
*/
void interpol_energy() {
  int ib, i0, i1, ii, i2, j0, j1, j2;
  GLfloat coef[4];
  GLfloat eig1[4][4][4], mat1[4][4][4], eig2[4][4], mat2[4][4], eig3[4], mat3[4];
  /*
  Reallocate
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng[0]; i0++) {
      for (i1 = 0; i1 < ng[1]; i1++) {
        free(eig[ib][i0][i1]);
        free(mat[ib][i0][i1]);
      }
      free(eig[ib][i0]);
      free(mat[ib][i0]);
    }
    free(eig[ib]);
    free(mat[ib]);
  }
  for (ii = 0; ii < 3; ii++)ng[ii] = ng0[ii] * interpol;
  /**/
  for (ib = 0; ib < nb; ib++) {
    eig[ib] = (GLfloat***)malloc(ng[0] * sizeof(GLfloat**));
    mat[ib] = (GLfloat***)malloc(ng[0] * sizeof(GLfloat**));
    for (i0 = 0; i0 < ng[0]; i0++) {
      eig[ib][i0] = (GLfloat**)malloc(ng[1] * sizeof(GLfloat*));
      mat[ib][i0] = (GLfloat**)malloc(ng[1] * sizeof(GLfloat*));
      for (i1 = 0; i1 < ng[1]; i1++) {
        eig[ib][i0][i1] = (GLfloat*)malloc(ng[2] * sizeof(GLfloat));
        mat[ib][i0][i1] = (GLfloat*)malloc(ng[2] * sizeof(GLfloat));
      }
    }
  }
  /*
  3rd order - three dimensional Kumo interpolation
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng0[0]; i0++) {
      for (i1 = 0; i1 < ng0[1]; i1++) {
        for (i2 = 0; i2 < ng0[2]; i2++) {
          for (j0 = 0; j0 < 4; j0++) {
            for (j1 = 0; j1 < 4; j1++) {
              for (j2 = 0; j2 < 4; j2++) {
                eig1[j0][j1][j2] = eig0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                           [modulo(i1 + j1 - 1, ng0[1])]
                                           [modulo(i2 + j2 - 1, ng0[2])];
                mat1[j0][j1][j2] = mat0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                           [modulo(i1 + j1 - 1, ng0[1])]
                                           [modulo(i2 + j2 - 1, ng0[2])];
              }
            }
          }

          for (j0 = 0; j0 < interpol; j0++) {
            kumo_coef(j0, &coef[0]);
            for (j1 = 0; j1 < 4; j1++) {
              for (j2 = 0; j2 < 4; j2++) {
                eig2[j1][j2] = 0.0;
                mat2[j1][j2] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                  eig2[j1][j2] += coef[ii] * eig1[ii][j1][j2];
                  mat2[j1][j2] += coef[ii] * mat1[ii][j1][j2];
                }
              }
            }
            for (j1 = 0; j1 < interpol; j1++) {
              kumo_coef(j1, &coef[0]);
              for (j2 = 0; j2 < 4; j2++) {
                eig3[j2] = 0.0;
                mat3[j2] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                  eig3[j2] += coef[ii] * eig2[ii][j2];
                  mat3[j2] += coef[ii] * mat2[ii][j2];
                }
              }
              for (j2 = 0; j2 < interpol; j2++) {
                kumo_coef(j2, &coef[0]);
                eig[ib][i0*interpol + j0]
                       [i1*interpol + j1]
                       [i2*interpol + j2] = 0.0;
                mat[ib][i0*interpol + j0]
                       [i1*interpol + j1]
                       [i2*interpol + j2] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                  eig[ib][i0*interpol + j0]
                         [i1*interpol + j1]
                         [i2*interpol + j2] += coef[ii] * eig3[ii];
                  mat[ib][i0*interpol + j0]
                         [i1*interpol + j1]
                         [i2*interpol + j2] += coef[ii] * mat3[ii];
                }
              }
            }
          }
        }
      }
    }
  }
}/*void interpol_energy() */
