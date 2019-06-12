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
    GLfloat a[3][3], matps[3];

    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      if(query == 1) nnl0 = 0;
      else nnl0 = ntri_th[ib][ithread];
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {

        for (i = 0; i < 3; ++i) matps[i] = matp[ib][itri][i][0];
        eigsort(3, matps, sw);

        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.f - matp[ib][itri][sw[j]][0]) 
              / (matp[ib][itri][sw[i]][0] - matp[ib][itri][sw[j]][0]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if (( matp[ib][itri][sw[0]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[1]][0])
          || (matp[ib][itri][sw[0]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[1]][0])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kvnl[ib][nnl0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
              kvnl[ib][nnl0][1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nnl0 += 1;
        }/*else if (matp[ib][itri][sw[0]] < 0.0 && 0.0 <= matp[ib][itri][sw[1]])*/
        else if ((matp[ib][itri][sw[1]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[2]][0])
          ||     (matp[ib][itri][sw[1]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[2]][0])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kvnl[ib][nnl0][0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
              kvnl[ib][nnl0][1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
            }/*for (i = 0; i < 3; ++i)*/
          }/*if (query == 0)*/
          nnl0 += 1;
        }/*else if (matp[ib][itri][sw[1]] < 0.0 && 0.0 <= matp[ib][itri][sw[2]])*/
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
    *terminal << wxT("    band   # of nodeline\n");
    for (ib = 0; ib < nb; ib++) {
      *terminal << wxString::Format(wxT("    %d       %d\n"), ib + 1, nnl[ib]);
    }/*for (ib = 0; ib < nb; ib++)*/
    /*
     Allocation of nodeline
    */
    kvnl = new GLfloat***[nb];
    kvnl_rot = new GLfloat*[nb];
    for (ib = 0; ib < nb; ++ib) {
      kvnl[ib] = new GLfloat**[nnl[ib]];
      kvnl_rot[ib] = new GLfloat[6*nnl[ib]];
      for (itri = 0; itri < nnl[ib]; ++itri) {
        kvnl[ib][itri] = new GLfloat * [2];
        for (i = 0; i < 2; ++i) {
          kvnl[ib][itri][i] = new GLfloat[3];
        }/*for (j = 0; j < 2; ++j)*/
      }/*for (i = 0; i < nnl[ib]; ++i)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 1)*/
}/*void calc_nodeline()*/
