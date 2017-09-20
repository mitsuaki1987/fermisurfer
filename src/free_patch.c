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
@brief Refresh patch
*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "variable.h"
#include "basic_math.h"

/**
 @brief Free variables for patch before new patch is computed

 Free : ::nmlp, ::matp, ::clr, ::kvp, ::nmlp_rot, ::kvp_rot,
        ::kvnl, ::kvnl_rot, ::kv2d, ::clr2d
*/
void free_patch() {
  int ib, i0, i1;
  /*
   Fermi patch
  */
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ntri[ib]; ++i0) {
      for (i1 = 0; i1 < 3; ++i1) {
        free(nmlp[ib][i0][i1]);
        free(kvp[ib][i0][i1]);
      }
      free(nmlp[ib][i0]);
      free(matp[ib][i0]);
      free(kvp[ib][i0]);
    }
    free(nmlp[ib]);
    free(matp[ib]);
    free(clr[ib]);
    free(kvp[ib]);
    free(nmlp_rot[ib]);
    free(kvp_rot[ib]);
  }
  free(nmlp);
  free(matp);
  free(clr);
  free(kvp);
  free(nmlp_rot);
  free(kvp_rot);
  /*
   Nodal line
  */
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < nnl[ib]; ++i0) {
      for (i1 = 0; i1 < 2; ++i1) {
        free(kvnl[ib][i0][i1]);
      }/*for (i1 = 0; i1 < 2; ++i1)*/
      free(kvnl[ib][i0]);
    }/*for (i0 = 0; i0 < nnl[ib]; ++i0)*/
    free(kvnl[ib]);
    free(kvnl_rot[ib]);
    free(nmlnl[ib]);
    free(clrnl[ib]);
  }/*for (ib = 0; ib < nb; ++ib)*/
  free(kvnl);
  free(kvnl_rot);
  free(nmlnl);
  free(clrnl);
  /*
   2D Fermi line
  */
  for (ib = 0; ib < nb; ++ib) {
    free(kv2d[ib]);
    free(clr2d[ib]);
    free(nml2d[ib]);
  }/*for (ib = 0; ib < nb; ++ib)*/
  free(kv2d);
  free(clr2d);
  free(nml2d);
}/*void free_patch()*/
/**
 @brief Compute Max. & Min. of matrix elements.
 Compute color of each patch 

 Modify : ::clr
*/
void max_and_min() {
  int itri, ierr, ithread;
  GLfloat matmax, matmin, *max_th, *min_th;
  
  max_th = (GLfloat*)malloc(nthreads * sizeof(GLfloat));
  min_th = (GLfloat*)malloc(nthreads * sizeof(GLfloat));

  printf("\n");
  if (fcscl == 1) printf("  ##  Full color scale mode #############\n");
  else if (fcscl == 2) printf("  ##  Manual color scale mode #############\n");
  else if (fcscl == 3) printf("  ##  Uni-color mode #############\n");
  else printf("  ##  Periodic color scale mode #############\n");
  printf("\n");

#pragma omp parallel default(none) \
  shared(nb,ntri,matp,max_th,min_th) private(itri,ithread)
  {
    int i, ib;

    ithread = get_thread();
    max_th[ithread] = -100000000.0000;
    min_th[ithread] = 100000000.0000;

    for (ib = 0; ib < nb; ib++) {
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          if (matp[ib][itri][i] > max_th[ithread]) max_th[ithread] = matp[ib][itri][i];
          if (matp[ib][itri][i] < min_th[ithread]) min_th[ithread] = matp[ib][itri][i];
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
  /**/
  matmax = max_th[0];
  matmin = min_th[0];
  for (ithread = 1; ithread < nthreads; ithread++) {
    if (max_th[ithread] > matmax) matmax = max_th[ithread];
    if (min_th[ithread] > matmin) matmin = min_th[ithread];
  }
  printf("    Max. value : %f\n", matmax);
  printf("    Min. value : %f\n\n", matmin);
  /**/
  if (fcscl == 2) {
    printf("    Set min. value : ");
    ierr = scanf("%f", &matmin);
    if (ierr == 0) printf("error ! reading min");
    printf("    Set max. value : ");
    ierr = scanf("%f", &matmax);
    if (ierr == 0) printf("error ! reading max");
  }
  /**/
  if (fcscl == 1 || fcscl == 2) {
#pragma omp parallel default(none) \
shared(nb,ntri,matp,clr,cyan,blue,green,yellow,red,matmax,matmin) \
private(itri)
    {
    int i, j, ib;
    GLfloat mat2;

      for (ib = 0; ib < nb; ib++) {
#pragma omp for nowait
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            mat2 = (matp[ib][itri][i] - matmin) / (matmax - matmin);
            mat2 = mat2 * 4.0f;
            /**/
            if (mat2 <= 1.0) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = cyan[j] * mat2 + blue[j] * (1.0f - mat2);
            }
            else if (mat2 <= 2.0) {
              mat2 = mat2 - 1.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = green[j] * mat2 + cyan[j] * (1.0f - mat2);
            }
            else if (mat2 <= 3.0) {
              mat2 = mat2 - 2.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = yellow[j] * mat2 + green[j] * (1.0f - mat2);
            }
            else {
              mat2 = mat2 - 3.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = red[j] * mat2 + yellow[j] * (1.0f - mat2);
            }
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
    }/*End of parallel region*/
  }
  else if (fcscl == 4) {
#pragma omp parallel default(none) \
shared(nb,ntri,matp,clr,cyan,blue,green,yellow,red,magenta) \
private(itri)
    {
    int i, j, ib;
    GLfloat mat2;

      for (ib = 0; ib < nb; ib++) {
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            mat2 = matp[ib][itri][i] / 6.283185307f;
            mat2 = mat2 - floorf(mat2);
            mat2 = mat2 * 6.0f;
            /**/
            if (mat2 <= 1.0) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = yellow[j] * mat2 + red[j] * (1.0f - mat2);
            }
            else if (mat2 <= 2.0) {
              mat2 = mat2 - 1.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = green[j] * mat2 + yellow[j] * (1.0f - mat2);
            }
            else if (mat2 <= 3.0) {
              mat2 = mat2 - 2.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = cyan[j] * mat2 + green[j] * (1.0f - mat2);
            }
            else if (mat2 <= 4.0) {
              mat2 = mat2 - 3.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = blue[j] * mat2 + cyan[j] * (1.0f - mat2);
            }
            else if (mat2 <= 5.0) {
              mat2 = mat2 - 4.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = magenta[j] * mat2 + blue[j] * (1.0f - mat2);
            }
            else {
              mat2 = mat2 - 5.0f;
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = red[j] * mat2 + magenta[j] * (1.0f - mat2);
            }
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
    }/*End of parallel region*/
  }
  else {
#pragma omp parallel default(none) \
shared(nb,ntri,matp,clr,cyan,blue,green,yellow,red) \
private(itri)
    {
    int i, j, ib;
    GLfloat mat2;

      for (ib = 0; ib < nb; ib++) {
        /**/
        mat2 = 1.0f / (GLfloat)(nb - 1) * (GLfloat)ib;
        mat2 = mat2 * 4.0f;
        /**/
        if (mat2 <= 1.0) {
#pragma omp for nowait
          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = cyan[j] * mat2 + blue[j] * (1.0f - mat2);
            }
          }
        }
        else if (mat2 <= 2.0) {
          mat2 = mat2 - 1.0f;
#pragma omp for nowait
          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = green[j] * mat2 + cyan[j] * (1.0f - mat2);
            }
          }
        }
        else if (mat2 <= 3.0) {
          mat2 = mat2 - 2.0f;
#pragma omp for nowait
          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = yellow[j] * mat2 + green[j] * (1.0f - mat2);
            }
          }
        }
        else {
          mat2 = mat2 - 3.0f;
#pragma omp for nowait
          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j+4*i+12*itri] = red[j] * mat2 + yellow[j] * (1.0f - mat2);
            }
          }
        }
      }
    }/*End of parallel region*/
  }
  free(max_th);
  free(min_th);
}/* max_and_min */
