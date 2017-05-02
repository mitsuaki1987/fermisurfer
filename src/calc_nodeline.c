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

#include <stdio.h>
#include <math.h>
#include "variable.h"
#include "basic_math.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
 Compute node-line where \f$\Delta_{n k} = 0\f$
*/
void calc_nodeline() {
  int ib, itri, i, j, nnl0, ithread;
  GLfloat mprod[2];
  /*
   Query
  */
#pragma omp parallel default(none) \
  shared(nb,nnl,matp,ntri,ntri_th) \
  private(ib,itri,mprod,nnl0,ithread)
  {
    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      nnl0 = 0;
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {
        /**/
        mprod[0] = matp[ib][itri][0] * matp[ib][itri][1];
        mprod[1] = matp[ib][itri][1] * matp[ib][itri][2];
        /**/
        if (fabsf(matp[ib][itri][0]) < 0.00001 &&  fabsf(matp[ib][itri][1]) < 0.00001) {
          nnl0 += 1;
        }
        else if (fabsf(matp[ib][itri][0]) < 0.00001 &&  fabsf(matp[ib][itri][2]) < 0.00001) {
          nnl0 += 1;
        }
        else if (fabsf(matp[ib][itri][1]) < 0.00001 &&  fabsf(matp[ib][itri][2]) < 0.00001) {
          nnl0 += 1;
        }
        else if (mprod[0] < 0.0) {
          if (mprod[1] < 0.0) {
            nnl0 += 1;
          }
          else {
            nnl0 += 1;
          }
        }
        else if (mprod[1] < 0.0) {
          nnl0 += 1;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      ntri_th[ib][ithread] = nnl0;
    }/*for (ib = 0; ib < nb; ib++)*/
  }/* End of parallel region */
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
  }
  printf("    band   # of nodeline\n");
  for (ib = 0; ib < nb; ib++) {
    printf("    %d       %d\n", ib + 1, nnl[ib]);
  }
  /*
   Allocation of nodeline
  */
  kvnl = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  kvnl_rot = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  for (ib = 0; ib < nb; ++ib) {
    kvnl[ib] = (GLfloat***)malloc(nnl[ib] * sizeof(GLfloat**));
    kvnl_rot[ib] = (GLfloat***)malloc(nnl[ib] * sizeof(GLfloat**));
    for (i = 0; i < nnl[ib]; ++i) {
      kvnl[ib][i] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
      kvnl_rot[ib][i] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
      for (j = 0; j < 2; ++j) {
        kvnl[ib][i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        kvnl_rot[ib][i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
      }
    }
  }
  /**/
#pragma omp parallel default(none) \
  shared(nb,nnl,matp,kvnl,kvp,ntri,ntri_th) \
  private(ib,itri,mprod,i,nnl0,ithread)
  {
    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      nnl0 = ntri_th[ib][ithread];
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {
        /**/
        mprod[0] = matp[ib][itri][0] * matp[ib][itri][1];
        mprod[1] = matp[ib][itri][1] * matp[ib][itri][2];
        /**/
        if (fabsf(matp[ib][itri][0]) < 0.00001 && fabsf(matp[ib][itri][1]) < 0.00001) {
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] = kvp[ib][itri][0][i];
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] = kvp[ib][itri][1][i];
          nnl0 += 1;
        }
        else if (fabsf(matp[ib][itri][0]) < 0.00001 && fabsf(matp[ib][itri][2]) < 0.00001) {
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] = kvp[ib][itri][0][i];
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] = kvp[ib][itri][2][i];
          nnl0 += 1;
        }
        else if (fabsf(matp[ib][itri][1]) < 0.00001 && fabsf(matp[ib][itri][2]) < 0.00001) {
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] = kvp[ib][itri][1][i];
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] = kvp[ib][itri][2][i];
          nnl0 += 1;
        }
        else if (mprod[0] < 0.0) {
          if (mprod[1] < 0.0) {
            for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] =
              (matp[ib][itri][0] * kvp[ib][itri][1][i] - matp[ib][itri][1] * kvp[ib][itri][0][i]) 
              / (matp[ib][itri][0] - matp[ib][itri][1]);
            for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] =
              (matp[ib][itri][1] * kvp[ib][itri][2][i] - matp[ib][itri][2] * kvp[ib][itri][1][i])
              / (matp[ib][itri][1] - matp[ib][itri][2]);
            nnl0 += 1;
          }
          else {
            for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] =
              (matp[ib][itri][0] * kvp[ib][itri][1][i] - matp[ib][itri][1] * kvp[ib][itri][0][i])
              / (matp[ib][itri][0] - matp[ib][itri][1]);
            for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] =
              (matp[ib][itri][0] * kvp[ib][itri][2][i] - matp[ib][itri][2] * kvp[ib][itri][0][i])
              / (matp[ib][itri][0] - matp[ib][itri][2]);
            nnl0 += 1;
          }
        }
        else if (mprod[1] < 0.0) {
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][0][i] =
            (matp[ib][itri][0] * kvp[ib][itri][2][i] - matp[ib][itri][2] * kvp[ib][itri][0][i])
            / (matp[ib][itri][0] - matp[ib][itri][2]);
          for (i = 0; i<3; ++i)kvnl[ib][nnl0][1][i] =
            (matp[ib][itri][1] * kvp[ib][itri][2][i] - matp[ib][itri][2] * kvp[ib][itri][1][i])
            / (matp[ib][itri][1] - matp[ib][itri][2]);
          nnl0 += 1;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/* End of parallel region */
}/*void calc_nodeline()*/
