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
/**@file
@brief Compute @f$\varepsilon_{n k}, \Delta_{n k}@f$ on
denser @f$k@f$-grid with French-curve (Kumo) interpolation
*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(WIN32)
#include <Windows.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "basic_math.hpp"
#include "variable.hpp"
/**
 @brief Compute coefficient for the French-curve (Kumo) interpolation
 @f[
  A^{\rm intp} = \sum_{i = 1}^4 C_i A_i^{\rm orig}
 @f]
*/
static void kumo_coef(
  int j, //!< [in] Interpolated grid index
  GLfloat *coef //!< [out] Coefficient of interpolation @f$C_i@f$
) {
  GLfloat x, mx;
  x = (GLfloat)j / (GLfloat)interpol;
  mx = 1.0f - x;
  coef[0] = -0.5f * x * mx * mx;
  coef[1] = mx * (mx*mx + 3.0f* x*mx + 0.5f* x* x);
  coef[2] =  x * ( x* x + 3.0f*mx* x + 0.5f*mx*mx);
  coef[3] = -0.5f * x *  x * mx;
}
/**
 @brief Interpolation of energy and matrix 
        with the French-curve (Kumo) interpolation.

 Modify : ::eig, ::mat
*/
void interpol_energy() {
  int ib, i0, i1, i2, ii;

  printf("    Interpolating ... ");
  /*
   Reallocate
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng[0]; i0++) {
      for (i1 = 0; i1 < ng[1]; i1++) {
        for (i2 = 0; i2 < ng[2]; i2++) {
          free(vf[ib][i0][i1][i2]);
          free(mat[ib][i0][i1][i2]);
        }
        free(eig[ib][i0][i1]);
        free(mat[ib][i0][i1]);
        free(vf[ib][i0][i1]);
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
      free(eig[ib][i0]);
      free(mat[ib][i0]);
      free(vf[ib][i0]);
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
    free(eig[ib]);
    free(mat[ib]);
    free(vf[ib]);
  }/*for (ib = 0; ib < nb; ib++)*/
  for (ii = 0; ii < 3; ii++)ng[ii] = ng0[ii] * interpol;
  /**/
  for (ib = 0; ib < nb; ib++) {
    eig[ib] = (GLfloat***)malloc(ng[0] * sizeof(GLfloat**));
    mat[ib] = (GLfloat****)malloc(ng[0] * sizeof(GLfloat***));
    vf[ib] = (GLfloat****)malloc(ng[0] * sizeof(GLfloat***));
    for (i0 = 0; i0 < ng[0]; i0++) {
      eig[ib][i0] = (GLfloat**)malloc(ng[1] * sizeof(GLfloat*));
      mat[ib][i0] = (GLfloat***)malloc(ng[1] * sizeof(GLfloat**));
      vf[ib][i0] = (GLfloat***)malloc(ng[1] * sizeof(GLfloat**));
      for (i1 = 0; i1 < ng[1]; i1++) {
        eig[ib][i0][i1] = (GLfloat*)malloc(ng[2] * sizeof(GLfloat));
        mat[ib][i0][i1] = (GLfloat**)malloc(ng[2] * sizeof(GLfloat*));
        vf[ib][i0][i1] = (GLfloat**)malloc(ng[2] * sizeof(GLfloat*));
        for (i2 = 0; i2 < ng[2]; i2++) {
          mat[ib][i0][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          vf[ib][i0][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        }
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  /*
   3rd order - three dimensional Kumo interpolation
  */
#pragma omp parallel default(none) \
  shared(nb,ng0,ng,eig,eig0,mat,mat0,interpol) \
  private (ib,i0,i1,i2,ii)
  {
    int j0, j1, j2, jj;
    GLfloat coef[4], 
      mat1[4][4][4][3], mat2[4][4][3], mat3[4][3],
      eig1[4][4][4], eig2[4][4], eig3[4];

    for (ib = 0; ib < nb; ib++) {
# pragma omp for nowait
      for (i0 = 0; i0 < ng0[0]; i0++) {
        //if (ith == 1) continue;
        for (i1 = 0; i1 < ng0[1]; i1++) {
          for (i2 = 0; i2 < ng0[2]; i2++) {
            for (j0 = 0; j0 < 4; j0++) {
              for (j1 = 0; j1 < 4; j1++) {
                for (j2 = 0; j2 < 4; j2++) {
                  eig1[j0][j1][j2] = eig0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                             [modulo(i1 + j1 - 1, ng0[1])]
                                             [modulo(i2 + j2 - 1, ng0[2])];
                  for (jj = 0; jj < 3; jj++) {
                    mat1[j0][j1][j2][jj] = mat0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                                   [modulo(i1 + j1 - 1, ng0[1])]
                                                   [modulo(i2 + j2 - 1, ng0[2])][jj];
                  }
                }/*for (j2 = 0; j2 < 4; j2++)*/
              }/*for (j1 = 0; j1 < 4; j1++)*/
            }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
            for (j0 = 0; j0 < interpol; j0++) {
              kumo_coef(j0, &coef[0]);
              for (j1 = 0; j1 < 4; j1++) {
                for (j2 = 0; j2 < 4; j2++) {
                  eig2[j1][j2] = 0.0;
                  for (jj = 0; jj < 3; jj++) mat2[j1][j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig2[j1][j2] += coef[ii] * eig1[ii][j1][j2];
                    for (jj = 0; jj < 3; jj++) 
                      mat2[j1][j2][jj] += coef[ii] * mat1[ii][j1][j2][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < 4; j2++)*/
              }/*for (j1 = 0; j1 < 4; j1++)*/
              for (j1 = 0; j1 < interpol; j1++) {
                kumo_coef(j1, &coef[0]);
                for (j2 = 0; j2 < 4; j2++) {
                  eig3[j2] = 0.0;
                  for (jj = 0; jj < 3; jj++) mat3[j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig3[j2] += coef[ii] * eig2[ii][j2];
                    for (jj = 0; jj < 3; jj++)
                      mat3[j2][jj] += coef[ii] * mat2[ii][j2][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < 4; j2++)*/
                for (j2 = 0; j2 < interpol; j2++) {
                  kumo_coef(j2, &coef[0]);
                  eig[ib][i0*interpol + j0]
                         [i1*interpol + j1]
                         [i2*interpol + j2] = 0.0;
                  for (jj = 0; jj < 3; jj++)
                    mat[ib][i0*interpol + j0]
                           [i1*interpol + j1]
                           [i2*interpol + j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig[ib][i0*interpol + j0]
                           [i1*interpol + j1]
                           [i2*interpol + j2] += coef[ii] * eig3[ii];
                    for (jj = 0; jj < 3; jj++)
                      mat[ib][i0*interpol + j0]
                             [i1*interpol + j1]
                             [i2*interpol + j2][jj] += coef[ii] * mat3[ii][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < interpol; j2++)*/
              }/*for (j1 = 0; j1 < interpol; j1++)*/
            }/*for (j0 = 0; j0 < interpol; j0++)*/
          }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
        }/*for (i1 = 0; i1 < ng0[1]; i1++)*/
      }/*for (i0 = 0; i0 < ng0[0]; i0++)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
  /*
   Fermi velocity
  */
#pragma omp parallel default(none) \
  shared(nb,ng,eig,vf,avec) \
  private (ib,i0,i1,i2,ii)
  {
    int i0p, i0m, i1p, i1m, i2p, i2m;
    GLfloat de[3];

    for (ib = 0; ib < nb; ib++) {
      for (i0 = 0; i0 < ng[0]; i0++) {
        i0p = modulo(i0 + 1, ng[0]);
        i0m = modulo(i0 - 1, ng[0]);
        for (i1 = 0; i1 < ng[1]; i1++) {
          i1p= modulo(i1 + 1, ng[1]);
          i1m = modulo(i1 - 1, ng[1]);
          for (i2 = 0; i2 < ng[2]; i2++) {
            i2p = modulo(i2 + 1, ng[2]);
            i2m = modulo(i2 - 1, ng[2]);

            de[0] = eig[ib][i0p][i1][i2] - eig[ib][i0m][i1][i2];
            de[1] = eig[ib][i0][i1p][i2] - eig[ib][i0][i1m][i2];
            de[2] = eig[ib][i0][i1][i2p] - eig[ib][i0][i1][i2m];
            for (ii = 0; ii < 3; ii++)de[ii] *= 0.5f * (GLfloat)ng[ii];
            for (ii = 0; ii < 3; ii++) vf[ib][i0][i1][i2][ii] =
              avec[0][ii] * de[0] + avec[1][ii] * de[1] + avec[2][ii] * de[2];

          }/*for (i2 = 0; i2 < ng[2]; i2++)*/
        }/*for (i1 = 0; i1 < ng[1]; i1++)*/
      }/*for (i0 = 0; i0 < ng[0]; i0++)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
  printf("Done\n\n");
}/*void interpol_energy() */
