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
#include <math.h>
#include <stdio.h>
#include "variable.h"

/**
* Free variables for patch
*/
void free_patch() {
  int ib, i0, i1;

  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ntri[ib]; ++i0) {
      for (i1 = 0; i1 < 3; ++i1) {
        free(kvp[ib][i0][i1]);
        free(clr[ib][i0][i1]);
      }
      free(nmlp[ib][i0]);
      free(matp[ib][i0]);
      free(clr[ib][i0]);
      free(kvp[ib][i0]);
    }
    free(nmlp[ib]);
    free(matp[ib]);
    free(clr[ib]);
    free(kvp[ib]);
  }
  free(nmlp);
  free(matp);
  free(clr);
  free(kvp);

  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < nnl[ib]; ++i0) {
      for (i1 = 0; i1 < 2; ++i1) {
        free(kvnl[ib][i0][i1]);
      }
      free(kvnl[ib][i0]);
    }
    free(kvnl[ib]);
  }
  free(kvnl);
}
/**
* Max. & Min. of matrix elements.
*/
void max_and_min() {
  int ib, itri, i, j, ierr;
  GLfloat matmax, matmin, mat2;
  /**/
  matmax = -100000000.0000;
  matmin = 100000000.0000;
  /**/
  for (ib = 0; ib < nb; ib++) {
    for (itri = 0; itri < ntri[ib]; ++itri) {
      for (i = 0; i < 3; ++i) {
        if (matp[ib][itri][i] > matmax) matmax = matp[ib][itri][i];
        if (matp[ib][itri][i] < matmin) matmin = matp[ib][itri][i];
      }
    }
  }
  /**/
  printf("Max. value : %f \n", matmax);
  printf("Min. value : %f \n \n", matmin);
  /**/
  if (fcscl == 2) {
    printf("Set min. value : ");
    ierr = scanf("%f", &matmin);
    if (ierr == 0) printf("error ! reading min");
    printf("Set max. value : ");
    ierr = scanf("%f", &matmax);
    if (ierr == 0) printf("error ! reading max");
  }
  /**/
  if (fcscl == 1 || fcscl == 2) {
    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = (matp[ib][itri][i] - matmin) / (matmax - matmin);
          mat2 = mat2 * 4.0;
          /**/
          if (mat2 <= 1.0) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = cyan[j] * mat2 + blue[j] * (1.0 - mat2);
          }
          else if (mat2 <= 2.0) {
            mat2 = mat2 - 1.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = green[j] * mat2 + cyan[j] * (1.0 - mat2);
          }
          else if (mat2 <= 3.0) {
            mat2 = mat2 - 2.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = yellow[j] * mat2 + green[j] * (1.0 - mat2);
          }
          else {
            mat2 = mat2 - 3.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = red[j] * mat2 + yellow[j] * (1.0 - mat2);
          }
        }
      }
    }
  }
  else if (fcscl == 4) {
    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = matp[ib][itri][i] / 6.283185307;
          mat2 = mat2 - floorf(mat2);
          mat2 = mat2 * 6.0;
          /**/
          if (mat2 <= 1.0) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
          }
          else if (mat2 <= 2.0) {
            mat2 = mat2 - 1.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
          }
          else if (mat2 <= 3.0) {
            mat2 = mat2 - 2.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
          }
          else if (mat2 <= 4.0) {
            mat2 = mat2 - 3.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
          }
          else if (mat2 <= 5.0) {
            mat2 = mat2 - 4.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
          }
          else {
            mat2 = mat2 - 5.0;
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
          }
        }
      }
    }
  }
  else {
    for (ib = 0; ib < nb; ib++) {
      /**/
      mat2 = 1.0 / (GLfloat)(nb - 1) * (GLfloat)ib;
      mat2 = mat2 * 4.0;
      /**/
      if (mat2 <= 1.0) {
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = cyan[j] * mat2 + blue[j] * (1.0 - mat2);
          }
        }
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = green[j] * mat2 + cyan[j] * (1.0 - mat2);
          }
        }
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = yellow[j] * mat2 + green[j] * (1.0 - mat2);
          }
        }
      }
      else {
        mat2 = mat2 - 3.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j<4; ++j) clr[ib][itri][i][j] = red[j] * mat2 + yellow[j] * (1.0 - mat2);
          }
        }
      }
    }
  }
} /* max_and_min */
