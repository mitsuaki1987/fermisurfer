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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "basic_math.hpp"
#include "variable.hpp"
/**
@brief Compute equator \f$\{\bf v}_{n k} \cdot {\bf k} = 0\f$

 Modify : ::ntri_th, ::nequator, ::kveq, ::kveq_rot

 If ::query = 1, this routine only compute the number of 
 line segmants and malloc variables.
*/
void equator() {
  int ib, itri, i, j, ithread;

#pragma omp parallel default(none) \
  shared(nb,nequator,nmlp,kveq,kvp,ntri,ntri_th,query,eqvec) \
  private(ib,itri,i,j,ithread)
  {
    int sw[3], nequator0;
    GLfloat a[3][3], prod[3];

    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      if(query == 1) nequator0 = 0;
      else nequator0 = ntri_th[ib][ithread];
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {

        for (i = 0; i < 3; ++i) {
          prod[i] = 0.0f;
          for (j = 0; j < 3; ++j) prod[i] += eqvec[j] * nmlp[ib][itri][i][j];
        }/*for (i = 0; i < 3; ++i)*/

        eigsort(3, prod, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.f - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if ((prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])
          || (prod[sw[0]] <= 0.0 && 0.0 < prod[sw[1]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kveq[ib][nequator0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
              kveq[ib][nequator0][1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nequator0 += 1;
        }/*else if (prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])*/
        else if ((prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])
          || (prod[sw[1]] <= 0.0 && 0.0 < prod[sw[2]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kveq[ib][nequator0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
              kveq[ib][nequator0][1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nequator0 += 1;
        }/*else if (prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      if (query == 1) ntri_th[ib][ithread] = nequator0;
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
      nequator[ib] = ntri_th[ib][nthreads - 1];
      for (ithread = nthreads - 1; ithread > 0; ithread--) {
        ntri_th[ib][ithread] = ntri_th[ib][ithread - 1];
      }
      ntri_th[ib][0] = 0;
    }/*for (ib = 0; ib < nb; ib++)*/
    printf("    band   # of nodeline\n");
    for (ib = 0; ib < nb; ib++) {
      printf("    %d       %d\n", ib + 1, nequator[ib]);
    }/*for (ib = 0; ib < nb; ib++)*/
    /*
     Allocation of nodeline
    */
    kveq = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    kveq_rot = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    nmleq = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    clreq = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    for (ib = 0; ib < nb; ++ib) {
      kveq[ib] = (GLfloat***)malloc(nequator[ib] * sizeof(GLfloat**));
      kveq_rot[ib] = (GLfloat*)malloc(6 * nequator[ib] * sizeof(GLfloat));
      nmleq[ib] = (GLfloat*)malloc(6 * nequator[ib] * sizeof(GLfloat));
      clreq[ib] = (GLfloat*)malloc(8 * nequator[ib] * sizeof(GLfloat));
      for (itri = 0; itri < nequator[ib]; ++itri) {
        kveq[ib][itri] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
        for (i = 0; i < 2; ++i) {
          kveq[ib][itri][i] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          for (j = 0; j < 2; ++j)nmleq[ib][j + 3 * i + 6 * itri] = 0.0f;
          nmleq[ib][2 + 3 * i + 6 * itri] = 1.0f;
          for (j = 0; j < 4; ++j)clreq[ib][j + 4 * i + 8 * itri] = black[j];
        }/*for (j = 0; j < 2; ++j)*/
      }/*for (i = 0; i < nequator[ib]; ++i)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 1)*/
}/*void equator()*/
