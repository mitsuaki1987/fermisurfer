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
@brief Functions for computing patch of Fermi surface
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
#include <math.h>
#include "variable.h"
#include "basic_math.h"
/**
 @brief Store triangle patch

 Modify : ::matp, ::kvp

 For the 1st BZ  mode, this routine cuts triangle recursivly at the
 BZ boundary (Bragg plane).
 If ::query == 1, this routine only increment the number of patch.
 If ::query == 0, store acutually the corner.

 - DO @f${\bf l}@f$ in Bragg vector
   - @f${p_i = {\bf l}\cdot{\bf k}}@f$
   - Sort : @f$p_0<p_1<p_2@f$
   - @f[
       a_{i j} \equiv \frac{-p_j}{p_i - p_j}
     @f]
   - if (@f$|{\bf l}| < p_0@f$)
     - This patch is not in the 1st BZ
   - if (@f$p_0 < |{\bf l}| < p_1@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_1 < |{\bf l}| < p_2@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
     - and
     - @f${\bf k}'_0 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_2 < |{\bf l}| < p_3@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_2@f$
 - END DO
*/
static void triangle(
  int ib, //!<[in] The band index
  int *ntri0, //!<[inout] Index of triangle patch
  int nbr, //!<[in] Bragg plane
  GLfloat mat1[3][3], //!<[in] The matrix element
  GLfloat kvec1[3][3], //!<[in] @f$k@f$-vector of corners
  GLfloat vf1[3][3] //!<[in] @f$v_f@f$-vector of corners
)
{
  int ibr, i, j, sw[3];
  GLfloat prod[3], thr, thr2 = 0.001f, mat2[3][3], kvec2[3][3],
    vf2[3][3], a[3][3], bshift, vfave[3], norm[3];
  /*
   If the area is nearly 0, it is ignored.
  */
  for (i = 0; i < 3; i++)norm[i] = 0.0f;
  for (i = 0; i < 3; i++) {
    norm[0] += (kvec1[1][i] - kvec1[2][i])*(kvec1[1][i] - kvec1[2][i]);
    norm[1] += (kvec1[2][i] - kvec1[0][i])*(kvec1[2][i] - kvec1[0][i]);
    norm[2] += (kvec1[0][i] - kvec1[1][i])*(kvec1[0][i] - kvec1[1][i]);
  }
  for (i = 0; i < 3; i++) {
    if (norm[i] < 1.0e-10f*brnrm_min) return;
  }
  /*
   For 1st BZ, it is cut at the BZ boundary.
  */
  if (fbz == 1) {
    /**/
    for (ibr = 0; ibr < nbragg; ++ibr) {

      thr = brnrm[ibr] * 0.001f;
      /**/
      for (i = 0; i < 3; ++i) 
        prod[i] = bragg[ibr][0] * kvec1[i][0]
                + bragg[ibr][1] * kvec1[i][1]
                + bragg[ibr][2] * kvec1[i][2];
      eigsort(3, prod, sw);
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (brnrm[ibr] - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/
      i = (int)(0.5f * ((prod[sw[2]] / brnrm[ibr]) + 1.0f));
      bshift = -2.0f *(GLfloat)i;

      if (brnrm[ibr] + thr > prod[sw[2]]) continue;

      if (brnrm[ibr] < prod[sw[0]]) {
        /*
         All corners are outside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            kvec2[i][j] = kvec1[sw[i]][j] + bshift * bragg[ibr][j];
            mat2[i][j] = mat1[sw[i]][j];
            vf2[i][j] = vf1[sw[i]][j];
          }
        }
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);
        return;
      }
      else if (brnrm[ibr] < prod[sw[1]]) {
        /*
         Single corner (#0) is inside of the Bragg plane
        */
       for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          
          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[2]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[2]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[2]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);
        return;
      }
      else if (brnrm[ibr] < prod[sw[2]]) {
        /*
        Two corners (#0, #1) are inside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
          kvec2[1][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[2][i] = kvec1[sw[2]][i];

          mat2[0][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          mat2[1][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[2][i] = mat1[sw[2]][i];

          vf2[0][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
          vf2[1][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[2][i] = vf1[sw[2]][i];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2, vf2);
        return;
      }
      else {
        /*
        All corners are inside of the Bragg plane
        */
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < nbragg*/
  } /* if fbz == 1 */
  /*
   If it is not query, store.
   If query, only count the number of patches.
  */
  if (query != 1){
    normal_vec(kvec1[0], kvec1[1], kvec1[2], norm);
    for (i = 0; i < 3; ++i) {
      vfave[i] = 0.0f;
      for (j = 0; j < 3; ++j) vfave[i] += vf1[j][i];
    }
    prod[0] = 0.0f;
    for (i = 0; i < 3; ++i) prod[0] += vfave[i] * norm[i];

    if (prod[0] < 0.0f) {
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          kvp[ib][*ntri0][i][j] = kvec1[2-i][j];
          matp[ib][*ntri0][i][j] = mat1[2 - i][j];
          nmlp[ib][*ntri0][i][j] = vf1[2-i][j];
        }
      }/*for (i = 0; i < 3; ++i)*/
    }
    else {
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          kvp[ib][*ntri0][i][j] = kvec1[i][j];
          matp[ib][*ntri0][i][j] = mat1[i][j];
          nmlp[ib][*ntri0][i][j] = vf1[i][j];
        }
      }/*for (i = 0; i < 3; ++i)*/
    }
  }/*if (query != 1)*/
  *ntri0 += 1;
}/* triangle */
/**
@brief Cut triangle patch with the tetrahedron method.

 - Sort : @f$\varepsilon_0<\varepsilon_1<\varepsilon_2<\varepsilon_3@f$
 - @f[
     a_{i j} \equiv \frac{-\varepsilon_j}{\varepsilon_i - \varepsilon_j}
   @f]
 - if (@f$\varepsilon_0 < 0 < \varepsilon_1@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_2 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
 - if (@f$\varepsilon_1 < 0 < \varepsilon_2@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
   - and
   - @f${\bf k}'_0 = {\bf k}_1 a_{1 3} + {\bf k}_3 a_{3 1}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
 - if (@f$\varepsilon_2 < 0 < \varepsilon_3@f$)
   - @f${\bf k}'_0 = {\bf k}_3 a_{3 0} + {\bf k}_0 a_{0 3}@f$
   - @f${\bf k}'_1 = {\bf k}_3 a_{3 1} + {\bf k}_1 a_{1 3}@f$
   - @f${\bf k}'_2 = {\bf k}_3 a_{3 2} + {\bf k}_2 a_{2 3}@f$
*/
static void tetrahedron(
  int ib, //!< [in] The band index
  int *ntri0, //!< [in,out] Counter of trixngle
  GLfloat eig1[8], //!< [in] Orbital energies @f$\varepsilon_{n k}@f$
  GLfloat mat1[8][3], //!< [in] Matrix elements @f$\Delta_{n k}@f$
  GLfloat kvec1[8][3], //!< [in] @f$k@f$-vectors
  GLfloat vf1[8][3] //!< [in] @f$v_f@f$-vectors
)
{
  int it, i, j, sw[4];
  GLfloat eig2[4], mat2[4][3], kvec2[4][3], vf2[4][3], a[4][4], 
    kvec3[3][3], mat3[3][3], vf3[3][3], vol, thr = 0.000f;

  for (it = 0; it < 6; ++it) {
    /*
     Define corners of the tetrahedron
    */
    for (i = 0; i < 4; ++i) {
      eig2[i] = eig1[corner[it][i]];
      for (j = 0; j < 3; ++j) {
        mat2[i][j] = mat1[corner[it][i]][j];
        vf2[i][j] = vf1[corner[it][i]][j];
      }
      /*
       Fractional -> Cartecian
      */
      for (j = 0; j < 3; ++j) 
        kvec2[i][j] = bvec[0][j] * kvec1[corner[it][i]][0]
                    + bvec[1][j] * kvec1[corner[it][i]][1]
                    + bvec[2][j] * kvec1[corner[it][i]][2];
    }/*for (i = 0; i < 4; ++i)*/
    eigsort(4, eig2, sw);

    for (i = 0; i < 4; ++i) {
      for (j = 0; j < 4; ++j) {
        a[i][j] = (0.0f - eig2[sw[j]]) / (eig2[sw[i]] - eig2[sw[j]]);
      }/*for (j = 0; j < 4; ++j)*/
    }/*for (i = 0; i < 4; ++i)*/
    /*
     Draw triangle in each cases
    */
    if (eig2[sw[0]] <= 0.0 && 0.0 < eig2[sw[1]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][1] + kvec2[sw[1]][i] * a[1][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[2][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];

        mat3[0][i] = mat2[sw[0]][i] * a[0][1] + mat2[sw[1]][i] * a[1][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[2][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];

        vf3[0][i] = vf2[sw[0]][i] * a[0][1] + vf2[sw[1]][i] * a[1][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[2][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
      }
      
      vol = a[1][0] * a[2][0] * a[3][0];
      triangle(ib, ntri0, 0, mat3, kvec3, vf3);
    }
    else if (eig2[sw[1]] <= 0.0 && 0.0 < eig2[sw[2]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }
      
      vol = a[1][2] * a[2][0] * a[3][0];
      triangle(ib, ntri0, 0, mat3, kvec3, vf3);
      /**/
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[1]][i] * a[1][3] + kvec2[sw[3]][i] * a[3][1];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[1]][i] * a[1][3] + mat2[sw[3]][i] * a[3][1];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[1]][i] * a[1][3] + vf2[sw[3]][i] * a[3][1];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }

      vol = a[1][3] * a[3][0] * a[2][1];
      triangle(ib, ntri0, 0, mat3, kvec3, vf3);
    }
    else if (eig2[sw[2]] <= 0.0 && 0.0 < eig2[sw[3]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[3]][i] * a[3][0] + kvec2[sw[0]][i] * a[0][3];
        kvec3[1][i] = kvec2[sw[3]][i] * a[3][1] + kvec2[sw[1]][i] * a[1][3];
        kvec3[2][i] = kvec2[sw[3]][i] * a[3][2] + kvec2[sw[2]][i] * a[2][3];

        mat3[0][i] = mat2[sw[3]][i] * a[3][0] + mat2[sw[0]][i] * a[0][3];
        mat3[1][i] = mat2[sw[3]][i] * a[3][1] + mat2[sw[1]][i] * a[1][3];
        mat3[2][i] = mat2[sw[3]][i] * a[3][2] + mat2[sw[2]][i] * a[2][3];

        vf3[0][i] = vf2[sw[3]][i] * a[3][0] + vf2[sw[0]][i] * a[0][3];
        vf3[1][i] = vf2[sw[3]][i] * a[3][1] + vf2[sw[1]][i] * a[1][3];
        vf3[2][i] = vf2[sw[3]][i] * a[3][2] + vf2[sw[2]][i] * a[2][3];
      }

      vol = a[0][3] * a[1][3] * a[2][3];
      triangle(ib, ntri0, 0, mat3, kvec3, vf3);
    }
    else {
    }
  }/*for (it = 0; it < 6; ++it)*/
}/* tetrahedron */
/**
 @brief Compute patches for Fermi surfaces

 Modify : ::ntri, ::ntri_th, nmlp, ::matp, ::kvp, ::clr, ::nmlp_rot, ::kvp_rot
 If ::query = 1, this routine compute the number of patches and malloc these variables.
*/
void fermi_patch()
{
  int ib, i0, i1, j0, start[3], last[3];
  int ithread;
  /**/
  if (fbz == 1) {
    if (query == 1) {
      printf("\n");
      printf("  ##  First Brillouin zone mode  #######\n");
    }
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = ng[i0] / 2 - ng[i0];
      last[i0] = ng[i0] / 2;
    }
  }
  else {
    if (query == 1) {
      printf("\n");
      printf("  ##  Premitive Brillouin zone mode  #######\n");
    }
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = 0;
      last[i0] = ng[i0];
    }
  }
  if (query == 1) printf("    Computing patch ...\n");
  /**/
#pragma omp parallel default(none) \
  shared(nb,ntri,ntri_th,start,last,ng,ng0,eig,vf,EF,mat,shiftk,query) \
  private(ib,j0,i0,i1,ithread)
  {
    int ntri0, i, j, i2, j1, j2, ii0, ii1, ii2;
    GLfloat kvec1[8][3], mat1[8][3], eig1[8], vf1[8][3];

    ithread = get_thread();
    for (ib = 0; ib < nb; ++ib) {

      if(query == 1) ntri0 = 0;
      else ntri0 = ntri_th[ib][ithread];

#pragma omp for nowait schedule(static, 1)
      for (j0 = start[0]; j0 < last[0]; ++j0) {
        for (j1 = start[1]; j1 < last[1]; ++j1) {
          for (j2 = start[2]; j2 < last[2]; ++j2) {
            /**/
            i0 = j0;
            i1 = j1;
            i2 = j2;
            ii0 = j0 + 1;
            ii1 = j1 + 1;
            ii2 = j2 + 1;
            /**/
            kvec1[0][0] = (GLfloat)i0 / (GLfloat)ng[0];
            kvec1[1][0] = (GLfloat)i0 / (GLfloat)ng[0];
            kvec1[2][0] = (GLfloat)i0 / (GLfloat)ng[0];
            kvec1[3][0] = (GLfloat)i0 / (GLfloat)ng[0];
            kvec1[4][0] = (GLfloat)ii0 / (GLfloat)ng[0];
            kvec1[5][0] = (GLfloat)ii0 / (GLfloat)ng[0];
            kvec1[6][0] = (GLfloat)ii0 / (GLfloat)ng[0];
            kvec1[7][0] = (GLfloat)ii0 / (GLfloat)ng[0];
            /**/
            kvec1[0][1] = (GLfloat)i1 / (GLfloat)ng[1];
            kvec1[1][1] = (GLfloat)i1 / (GLfloat)ng[1];
            kvec1[2][1] = (GLfloat)ii1 / (GLfloat)ng[1];
            kvec1[3][1] = (GLfloat)ii1 / (GLfloat)ng[1];
            kvec1[4][1] = (GLfloat)i1 / (GLfloat)ng[1];
            kvec1[5][1] = (GLfloat)i1 / (GLfloat)ng[1];
            kvec1[6][1] = (GLfloat)ii1 / (GLfloat)ng[1];
            kvec1[7][1] = (GLfloat)ii1 / (GLfloat)ng[1];
            /**/
            kvec1[0][2] = (GLfloat)i2 / (GLfloat)ng[2];
            kvec1[1][2] = (GLfloat)ii2 / (GLfloat)ng[2];
            kvec1[2][2] = (GLfloat)i2 / (GLfloat)ng[2];
            kvec1[3][2] = (GLfloat)ii2 / (GLfloat)ng[2];
            kvec1[4][2] = (GLfloat)i2 / (GLfloat)ng[2];
            kvec1[5][2] = (GLfloat)ii2 / (GLfloat)ng[2];
            kvec1[6][2] = (GLfloat)i2 / (GLfloat)ng[2];
            kvec1[7][2] = (GLfloat)ii2 / (GLfloat)ng[2];
            /**/
            for (i = 0; i < 8; i++)
              for (j = 0; j < 3; j++)
                kvec1[i][j] = kvec1[i][j] + (GLfloat)shiftk[j] / (GLfloat)(2 * ng0[j]);
            /**/
            i0 = modulo(i0, ng[0]);
            i1 = modulo(i1, ng[1]);
            i2 = modulo(i2, ng[2]);
            ii0 = modulo(ii0, ng[0]);
            ii1 = modulo(ii1, ng[1]);
            ii2 = modulo(ii2, ng[2]);
            /**/
            eig1[0] = eig[ib][i0][i1][i2] - EF;
            eig1[1] = eig[ib][i0][i1][ii2] - EF;
            eig1[2] = eig[ib][i0][ii1][i2] - EF;
            eig1[3] = eig[ib][i0][ii1][ii2] - EF;
            eig1[4] = eig[ib][ii0][i1][i2] - EF;
            eig1[5] = eig[ib][ii0][i1][ii2] - EF;
            eig1[6] = eig[ib][ii0][ii1][i2] - EF;
            eig1[7] = eig[ib][ii0][ii1][ii2] - EF;
            /**/
            for (j = 0; j < 3; j++) {
              mat1[0][j] = mat[ib][i0][i1][i2][j];
              mat1[1][j] = mat[ib][i0][i1][ii2][j];
              mat1[2][j] = mat[ib][i0][ii1][i2][j];
              mat1[3][j] = mat[ib][i0][ii1][ii2][j];
              mat1[4][j] = mat[ib][ii0][i1][i2][j];
              mat1[5][j] = mat[ib][ii0][i1][ii2][j];
              mat1[6][j] = mat[ib][ii0][ii1][i2][j];
              mat1[7][j] = mat[ib][ii0][ii1][ii2][j];
              /**/
              vf1[0][j] = vf[ib][i0][i1][i2][j];
              vf1[1][j] = vf[ib][i0][i1][ii2][j];
              vf1[2][j] = vf[ib][i0][ii1][i2][j];
              vf1[3][j] = vf[ib][i0][ii1][ii2][j];
              vf1[4][j] = vf[ib][ii0][i1][i2][j];
              vf1[5][j] = vf[ib][ii0][i1][ii2][j];
              vf1[6][j] = vf[ib][ii0][ii1][i2][j];
              vf1[7][j] = vf[ib][ii0][ii1][ii2][j];
            }/*for (j = 0; j < 3; j++)*/
            /**/
            tetrahedron(ib, &ntri0, eig1, mat1, kvec1, vf1);
          }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
        }/*for (j1 = start[1]; j1 < ng[1]; ++j1)*/
      }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
      if (query == 1) ntri_th[ib][ithread] = ntri0;
    }/*for (ib = 0; ib < nb; ++ib)*/
  } /* End of parallel region */
  /*
   If this run is workspace query, malloc arrays
  */
  if (query == 1) {
    /*
     Sum patches in all threads 
    */
    for (ib = 0; ib < nb; ib++) {
      for (ithread = 1; ithread < nthreads; ithread++) {
        ntri_th[ib][ithread] += ntri_th[ib][ithread - 1];
      }
      ntri[ib] = ntri_th[ib][nthreads - 1];
      for (ithread = nthreads - 1; ithread > 0; ithread--) {
        ntri_th[ib][ithread] = ntri_th[ib][ithread - 1];
      }
      ntri_th[ib][0] = 0;
    }
    /**/
    printf("      band   # of patchs\n");
    for (ib = 0; ib < nb; ib++) {
      printf("      %d       %d\n", ib + 1, ntri[ib]);
    }
    /*
     Allocation of triangler patches
    */
    matp = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    clr = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    kvp = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    nmlp = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    kvp_rot = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    nmlp_rot = (GLfloat**)malloc(nb * sizeof(GLfloat*));
    for (ib = 0; ib < nb; ++ib) {
      matp[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      clr[ib] = (GLfloat*)malloc(12 * ntri[ib] * sizeof(GLfloat));
      kvp[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      nmlp[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      kvp_rot[ib] = (GLfloat*)malloc(9 * ntri[ib] * sizeof(GLfloat));
      nmlp_rot[ib] = (GLfloat*)malloc(9 * ntri[ib] * sizeof(GLfloat));
      for (i0 = 0; i0 < ntri[ib]; ++i0) {
        matp[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        kvp[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        nmlp[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        for (i1 = 0; i1 < 3; ++i1) {
          matp[ib][i0][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          kvp[ib][i0][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          nmlp[ib][i0][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        }/*for (i1 = 0; i1 < 3; ++i1)*/
      }/*for (i0 = 0; i0 < ntri[ib]; ++i0)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 1)*/
  else {
    printf("    ... Done\n");
  }
} /* fermi_patch */
