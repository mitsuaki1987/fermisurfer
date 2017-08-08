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
@brief Compute nodal lines
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
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "variable.h"
#include "basic_math.h"
/**
@brief Compute node-line where \f$\Delta_{n k} = 0\f$

 Modify : ::ntri_th, ::nnl, ::kvnl, ::kvnl_rot

 If ::query = 1, this routine only compute the number of 
 line segmants and malloc variables.
*/
void calc_nodeline() {
  int ib, itri, i, j, ithread;

#pragma omp parallel default(none) \
  shared(nb,nnl,matp,kvnl,kvp,ntri,ntri_th,query) \
  private(ib,itri,i,j,ithread)
  {
    int sw[3], nnl0;
    GLfloat a[3][3];

    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      if(query == 1) nnl0 = 0;
      else nnl0 = ntri_th[ib][ithread];
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {

        eigsort(3, matp[ib][itri], sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.f - matp[ib][itri][sw[j]]) / (matp[ib][itri][sw[i]] - matp[ib][itri][sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if ((matp[ib][itri][sw[0]] < 0.0 && 0.0 <= matp[ib][itri][sw[1]])
          || (matp[ib][itri][sw[0]] <= 0.0 && 0.0 < matp[ib][itri][sw[1]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kvnl[ib][nnl0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
              kvnl[ib][nnl0][1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nnl0 += 1;
        }/*else if (mprod[2] < 0.00001 && mprod[0] < 0.00001)*/
        else if ((matp[ib][itri][sw[1]] < 0.0 && 0.0 <= matp[ib][itri][sw[2]])
          || (matp[ib][itri][sw[1]] <= 0.0 && 0.0 < matp[ib][itri][sw[2]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kvnl[ib][nnl0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
              kvnl[ib][nnl0][1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nnl0 += 1;
        }/*else if (mprod[1] < 0.00001 && mprod[0] < 0.00001)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      if (query == 1) ntri_th[ib][ithread] = nnl0;
    }/*for (ib = 0; ib < nb; ib++)*/
  }/* End of parallel region */
  /*
   Allocation of nodeline
  */
  if (query == 1) {
    /*
    Sum node-lines in all threads
    */
    for (ib = 0; ib < nb; ib++) {
      for (ithread = 1; ithread < nthreads; ithread++) {
        ntri_th[ib][ithread] += ntri_th[ib][ithread - 1];
      }
      nnl[ib] = ntri_th[ib][nthreads - 1];
      for (ithread = nthreads - 1; ithread > 0; ithread--) {
        ntri_th[ib][ithread] = ntri_th[ib][ithread - 1];
      }
      ntri_th[ib][0] = 0;
    }/*for (ib = 0; ib < nb; ib++)*/
    printf("    band   # of nodeline\n");
    for (ib = 0; ib < nb; ib++) {
      printf("    %d       %d\n", ib + 1, nnl[ib]);
    }/*for (ib = 0; ib < nb; ib++)*/
    /*
     Allocation of nodeline
    */
    kvnl = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    kvnl_rot = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    nmlnl = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    clrnl = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    for (ib = 0; ib < nb; ++ib) {
      kvnl[ib] = (GLfloat***)malloc(nnl[ib] * sizeof(GLfloat**));
      kvnl_rot[ib] = (GLfloat*)malloc(6 * nnl[ib] * sizeof(GLfloat));
      nmlnl[ib] = (GLfloat*)malloc(6 * nnl[ib] * sizeof(GLfloat));
      clrnl[ib] = (GLfloat*)malloc(8 * nnl[ib] * sizeof(GLfloat));
      for (itri = 0; itri < nnl[ib]; ++itri) {
        kvnl[ib][itri] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
        for (i = 0; i < 2; ++i) {
          kvnl[ib][itri][i] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          for (j = 0; j < 2; ++j)nmlnl[ib][j + 3 * i + 6 * itri] = 0.0f;
          nmlnl[ib][2 + 3 * i + 6 * itri] = 1.0f;
          for (j = 0; j < 4; ++j)clrnl[ib][j + 4 * i + 8 * itri] = black[j];
        }/*for (j = 0; j < 2; ++j)*/
      }/*for (i = 0; i < nnl[ib]; ++i)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 1)*/
}/*void calc_nodeline()*/
