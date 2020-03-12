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
@brief Functions that initilize variables.
*/
#include <wx/wx.h>
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include "variable.hpp"
/**
 @brief Specify corners of tetrahedron

 Modify : ::corner
*/
void init_corner()
{
  int i, j;
  int corner1[16][6][4] = {
    /*
    [0] min = 0-7
    */
    { { 0, 1, 3, 7 },
    { 0, 1, 5, 7 },
    { 0, 2, 3, 7 },
    { 0, 2, 6, 7 },
    { 0, 4, 5, 7 },
    { 0, 4, 6, 7 } },
    /*
    [1] min = 1-6
    */
    { { 1, 0, 2, 6 },
    { 1, 0, 4, 6 },
    { 1, 3, 2, 6 },
    { 1, 3, 7, 6 },
    { 1, 5, 4, 6 },
    { 1, 5, 7, 6 } },
    /*
    [2] min = 2-5
    */
    { { 2, 0, 1, 5 },
    { 2, 0, 4, 5 },
    { 2, 3, 1, 5 },
    { 2, 3, 7, 5 },
    { 2, 6, 4, 5 },
    { 2, 6, 7, 5 } },
    /*
    [3] min = 3-4
    */
    { { 4, 0, 1, 3 },
    { 4, 0, 2, 3 },
    { 4, 5, 1, 3 },
    { 4, 5, 7, 3 },
    { 4, 6, 2, 3 },
    { 4, 6, 7, 3 } },
    /*
    [4] min = 0-7, max = 3-4
    */
    { { 0, 4, 5, 6 },
    { 1, 2, 3, 7 },
    { 0, 7, 2, 6 },
    { 0, 7, 1, 5 },
    { 0, 7, 1, 2 },
    { 0, 7, 6, 5 } },
    /*
    [5] min = 1-6, max = 3-4
    */
    { { 0, 4, 5, 6 },
    { 1, 2, 3, 7 },
    { 1, 6, 5, 7 },
    { 1, 6, 7, 2 },
    { 1, 6, 2, 0 },
    { 1, 6, 0, 5 } },
    /*
    [6] min = 2-5, max = 3-4
    */
    { { 0, 4, 5, 6 },
    { 1, 2, 3, 7 },
    { 2, 5, 7, 6 },
    { 2, 5, 6, 0 },
    { 2, 5, 0, 1 },
    { 2, 5, 1, 7 } },
    /*
    [7] min = 3-4, max = 0-7
    */
    { { 0, 1, 2, 4 },
    { 7, 3, 5, 6 },
    { 3, 4, 1, 5 },
    { 3, 4, 5, 6 },
    { 3, 4, 6, 2 },
    { 3, 4, 2, 1 } },
    /*
    [8] min = 2-5, max = 0-7
    */
    { { 0, 1, 2, 4 },
    { 7, 3, 5, 6 },
    { 2, 5, 1, 3 },
    { 2, 5, 3, 6 },
    { 2, 5, 6, 4 },
    { 2, 5, 4, 1 } },
    /*
    [9] min = 1-6, max = 0-7
    */
    { { 0, 1, 2, 4 },
    { 7, 3, 5, 6 },
    { 1, 6, 2, 3 },
    { 1, 6, 3, 5 },
    { 1, 6, 5, 4 },
    { 1, 6, 4, 2 } },
    /*
    [10] min = 0-7, max = 1-6
    */
    { { 1, 0, 3, 5 },
    { 6, 2, 4, 7 },
    { 0, 7, 2, 3 },
    { 0, 7, 3, 5 },
    { 0, 7, 5, 4 },
    { 0, 7, 4, 2 } },
    /*
    [11] min = 3-4, max = 1-6
    */
    { { 1, 0, 3, 5 },
    { 6, 2, 4, 7 },
    { 3, 4, 0, 2 },
    { 3, 4, 2, 7 },
    { 3, 4, 7, 5 },
    { 3, 4, 5, 0 } },
    /*
    [12] min = 2-5, max = 1-6
    */
    { { 1, 0, 3, 5 },
    { 6, 2, 4, 7 },
    { 2, 5, 0, 3 },
    { 2, 5, 3, 7 },
    { 2, 5, 7, 4 },
    { 2, 5, 4, 0 } },
    /*
    [13] min = 0-7, max = 2-5
    */
    { { 2, 0, 3, 6 },
    { 5, 1, 4, 7 },
    { 0, 7, 1, 3 },
    { 0, 7, 3, 6 },
    { 0, 7, 6, 4 },
    { 0, 7, 4, 1 } },
    /*
    [14] min = 1-6, max = 2-5
    */
    { { 2, 0, 3, 6 },
    { 5, 1, 4, 7 },
    { 1, 6, 0, 3 },
    { 1, 6, 3, 7 },
    { 1, 6, 7, 4 },
    { 1, 6, 4, 0 } },
    /*
    [15] min = 3-4, max = 2-5
    */
    { { 2, 0, 3, 6 },
    { 5, 1, 4, 7 },
    { 3, 4, 0, 1 },
    { 3, 4, 1, 7 },
    { 3, 4, 7, 6 },
    { 3, 4, 6, 0 } },
  };
  /**/
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 4; ++j) {
      corner[i][j] = corner1[itet][i][j];
    }
  }
}
/**
 @brief Compute Bragg vetor

 Modify : ::bragg, ::brnrm
*/
void bragg_vector()
{
  int i0, i1, i2, i, ibr;
  /**/
  ibr = 0;
  /**/
  for (i0 = -1; i0 <= 1; ++i0) {
    for (i1 = -1; i1 <= 1; ++i1) {
      for (i2 = -1; i2 <= 1; ++i2) {
        /*
         Excepte Gamma points
        */
        if (i0 == 0 && i1 == 0 && i2 == 0) continue;
        /*
         Fractional -> Cartecian
        */
        for (i = 0; i < 3; ++i)
          bragg[ibr][i] = ((GLfloat)i0 * bvec[0][i]
                        +  (GLfloat)i1 * bvec[1][i]
                        +  (GLfloat)i2 * bvec[2][i]) * 0.5f;
        /*
         And its norm
        */
        brnrm[ibr] = bragg[ibr][0] * bragg[ibr][0]
                   + bragg[ibr][1] * bragg[ibr][1]
                   + bragg[ibr][2] * bragg[ibr][2];
        /**/
        ibr = ibr + 1;
      }/*for (i2 = -1; i2 <= 1; ++i2)*/
    }/*for (i1 = -1; i1 <= 1; ++i1)*/
  }/*for (i0 = -1; i0 <= 1; ++i0)*/
  /*
   Search min. of brnrm
  */
  brnrm_min = brnrm[0];
  for (ibr = 1; ibr < 26; ibr++) {
    if (brnrm_min > brnrm[ibr]) brnrm_min = brnrm[ibr];
  }
  *terminal << wxString::Format(wxT("    Minimum Bragg norm : %f\n"), brnrm_min);
}/* bragg_vector */
/**
 @brief Print max and minimum @f$\varepsilon_{n k}, \Delta_{n k}@f$
        in the whole Brillouine zone
*/
void max_and_min_bz()
{
  int ib, i0, i1, i2;
  GLfloat eigmin, eigmax, matmin, matmax;
  /**/
  *terminal << wxT("\n");
  *terminal << wxT("  ##  Max. and Min. of each bands  #######################\n");
  *terminal << wxT("\n");
  *terminal << wxT("    Band   Eig_Min.      Eig_Max      Mat_Min      Mat_Max\n");
  for (ib = 0; ib < nb; ib++) {
    eigmax = eig0[0][0][0][0];
    eigmin = eig0[0][0][0][0];
    matmax = mat0[0][0][0][0][0];
    matmin = mat0[0][0][0][0][0];
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (eig0[ib][i0][i1][i2] > eigmax) eigmax = eig0[ib][i0][i1][i2];
          if (eig0[ib][i0][i1][i2] < eigmin) eigmin = eig0[ib][i0][i1][i2];
          if (mat0[ib][i0][i1][i2][0] > matmax) matmax = mat0[ib][i0][i1][i2][0];
          if (mat0[ib][i0][i1][i2][0] < matmin) matmin = mat0[ib][i0][i1][i2][0];
        }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
      }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
    }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    *terminal << wxString::Format(wxT("    %d     %f     %f     %f     %f\n"),
      ib + 1, eigmin, eigmax, matmin, matmax);
  }/*for (ib = 0; ib < nb; ib++)*/
}/* max_and_min_bz */
