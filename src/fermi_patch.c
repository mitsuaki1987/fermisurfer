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
#include <stdlib.h>
#include <stdio.h>
#include "variable.h"
#include "basic_math.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
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
  GLfloat mat1[3], //!<[in] The matrix element
  GLfloat kvec1[3][3] //!<[in] @f$k@f$-vector of corners
)
{
  int ibr, i, j, sw[3];
  GLfloat prod[3], thr = 0.0000, mat2[3], kvec2[3][3], a[3][3];

  if (fbz == 1) {
    /**/
    for (ibr = nbr; ibr < 26; ++ibr) {
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

      if (brnrm[ibr] + thr < prod[sw[0]]) {
        /*
         All corners are outside of the Bragg plane
        */
        return;
      }
      else if (brnrm[ibr] + thr < prod[sw[1]]) {
        /*
         Single corner (#0) is inside of the Bragg plane
        */
        mat2[0] = mat1[sw[0]];
        mat2[1] = mat1[sw[0]] * a[0][1] + mat1[sw[1]] * a[1][0];
        mat2[2] = mat1[sw[0]] * a[0][2] + mat1[sw[2]] * a[2][0];
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        return;
      }
      else if (brnrm[ibr] + thr < prod[sw[2]]) {
        /*
        Two corners (#0, #1) are inside of the Bragg plane
        */
        mat2[0] = mat1[sw[0]];
        mat2[1] = mat1[sw[1]];
        mat2[2] = mat1[sw[0]] * a[0][2] + mat1[sw[2]] * a[2][0];
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        /**/
        mat2[0] = mat1[sw[1]] * a[1][2] + mat1[sw[2]] * a[2][1];
        mat2[1] = mat1[sw[1]];
        mat2[2] = mat1[sw[0]] * a[0][2] + mat1[sw[2]] * a[2][0];
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        return;
      }
      else {
        /*
        All corners are inside of the Bragg plane
        */
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < 26*/
  } /* if fbz == 1 */
  /*
   If it is not query, store.
   If query, only count the number of patches.
  */
  if (query != 1){
    normal_vec(kvec1[0], kvec1[1], kvec1[2], nmlp[ib][*ntri0]);
    for (i = 0; i < 3; ++i) {
      matp[ib][*ntri0][i] = mat1[i];
      for (j = 0; j < 3; ++j)
        kvp[ib][*ntri0][i][j] = kvec1[i][j];
    }/*for (i = 0; i < 3; ++i)*/
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
  GLfloat mat1[8], //!< [in] Matrix elements @f$\Delta_{n k}@f$
  GLfloat kvec1[8][3] //!< [in] @f$k@f$-vectors
)
{
  int it, i, j, sw[4];
  GLfloat eig2[4], mat2[4], kvec2[4][3], a[4][4], kvec3[3][3], mat3[3];

  for (it = 0; it < 6; ++it) {
    /*
     Define corners of the tetrahedron
    */
    for (i = 0; i < 4; ++i) {
      eig2[i] = eig1[corner[it][i]];
      mat2[i] = mat1[corner[it][i]];
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
        a[i][j] = (0.00 - eig2[sw[j]]) / (eig2[sw[i]] - eig2[sw[j]]);
      }/*for (j = 0; j < 4; ++j)*/
    }/*for (i = 0; i < 4; ++i)*/
    /*
     Draw triangle in each cases
    */
    if (eig2[sw[0]] <= 0.00 && 0.00 < eig2[sw[1]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][1] + kvec2[sw[1]][i] * a[1][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[2][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
      }
      mat3[0] = mat2[sw[0]] * a[0][1] + mat2[sw[1]] * a[1][0];
      mat3[1] = mat2[sw[0]] * a[0][2] + mat2[sw[2]] * a[2][0];
      mat3[2] = mat2[sw[0]] * a[0][3] + mat2[sw[3]] * a[3][0];
      triangle(ib, ntri0, 0, mat3, kvec3);
    }
    else if (eig2[sw[1]] <= 0.00 && 0.00 < eig2[sw[2]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];
      }
      mat3[0] = mat2[sw[0]] * a[0][2] + mat2[sw[2]] * a[2][0];
      mat3[1] = mat2[sw[0]] * a[0][3] + mat2[sw[3]] * a[3][0];
      mat3[2] = mat2[sw[1]] * a[1][2] + mat2[sw[2]] * a[2][1];
      triangle(ib, ntri0, 0, mat3, kvec3);
      /**/
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[1]][i] * a[1][3] + kvec2[sw[3]][i] * a[3][1];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];
      }
      mat3[0] = mat2[sw[1]] * a[1][3] + mat2[sw[3]] * a[3][1];
      mat3[1] = mat2[sw[0]] * a[0][3] + mat2[sw[3]] * a[3][0];
      mat3[2] = mat2[sw[1]] * a[1][2] + mat2[sw[2]] * a[2][1];
      triangle(ib, ntri0, 0, mat3, kvec3);
    }
    else if (eig2[sw[2]] <= 0.00 && 0.00 < eig2[sw[3]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[3]][i] * a[3][0] + kvec2[sw[0]][i] * a[0][3];
        kvec3[1][i] = kvec2[sw[3]][i] * a[3][1] + kvec2[sw[1]][i] * a[1][3];
        kvec3[2][i] = kvec2[sw[3]][i] * a[3][2] + kvec2[sw[2]][i] * a[2][3];
      }
      mat3[0] = mat2[sw[3]] * a[3][0] + mat2[sw[0]] * a[0][3];
      mat3[1] = mat2[sw[3]] * a[3][1] + mat2[sw[1]] * a[1][3];
      mat3[2] = mat2[sw[3]] * a[3][2] + mat2[sw[2]] * a[2][3];
      triangle(ib, ntri0, 0, mat3, kvec3);
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
  int ib, i0, i1, i2, ii0, ii1, ii2, j0, j1, j2, start[3], i, j;
  int ntri0, ithread;
  GLfloat kvec1[8][3], eig1[8], mat1[8];
  /**/
  if (fbz == 1) {
    if (query == 1) {
      printf("\n");
      printf("  ##  First Brillouin zone mode  #######\n");
      printf("\n");
    }
    for (i0 = 0; i0 < 3; ++i0) start[i0] = - ng[i0];
  }
  else {
    if (query == 1) {
      printf("\n");
      printf("  ##  Premitive Brillouin zone mode  #######\n");
      printf("\n");
    }
    for (i0 = 0; i0 < 3; ++i0) start[i0] = 0;
  }
  /**/
#pragma omp parallel default(none) \
  shared(nb,ntri,ntri_th,start,ng,ng0,eig,EF,mat,shiftk,query) \
  private(ib,j0,j1,j2,i0,i1,i2,ii0,ii1,ii2,kvec1,eig1,mat1,i,j,ntri0,ithread)
  {
    ithread = get_thread();
    for (ib = 0; ib < nb; ++ib) {

      if(query == 1) ntri0 = 0;
      else ntri0 = ntri_th[ib][ithread];

#pragma omp for nowait schedule(static, 1)
      for (j0 = start[0]; j0 < ng[0]; ++j0) {
        for (j1 = start[1]; j1 < ng[1]; ++j1) {
          for (j2 = start[2]; j2 < ng[2]; ++j2) {
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
                kvec1[i][j] = kvec1[i][j] + (double)shiftk[j] / (GLfloat)(2 * ng0[j]);
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
            mat1[0] = mat[ib][i0][i1][i2];
            mat1[1] = mat[ib][i0][i1][ii2];
            mat1[2] = mat[ib][i0][ii1][i2];
            mat1[3] = mat[ib][i0][ii1][ii2];
            mat1[4] = mat[ib][ii0][i1][i2];
            mat1[5] = mat[ib][ii0][i1][ii2];
            mat1[6] = mat[ib][ii0][ii1][i2];
            mat1[7] = mat[ib][ii0][ii1][ii2];
            /**/
            tetrahedron(ib, &ntri0, eig1, mat1, kvec1);
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
    printf("    band   # of patchs\n");
    for (ib = 0; ib < nb; ib++) {
      printf("    %d       %d\n", ib + 1, ntri[ib]);
    }
    printf("\n");
    /*
     Allocation of triangler patches
    */
    nmlp = (GLfloat***)malloc(nb * sizeof(GLfloat**));
    matp = (GLfloat***)malloc(nb * sizeof(GLfloat**));
    clr = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    kvp = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    nmlp_rot = (GLfloat***)malloc(nb * sizeof(GLfloat**));
    kvp_rot = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    for (ib = 0; ib < nb; ++ib) {
      nmlp[ib] = (GLfloat**)malloc(ntri[ib] * sizeof(GLfloat*));
      matp[ib] = (GLfloat**)malloc(ntri[ib] * sizeof(GLfloat*));
      clr[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      kvp[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      nmlp_rot[ib] = (GLfloat**)malloc(ntri[ib] * sizeof(GLfloat*));
      kvp_rot[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      for (i0 = 0; i0 < ntri[ib]; ++i0) {
        nmlp[ib][i0] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        matp[ib][i0] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        clr[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        kvp[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        nmlp_rot[ib][i0] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        kvp_rot[ib][i0] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
        for (i1 = 0; i1 < 3; ++i1) {
          kvp[ib][i0][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          kvp_rot[ib][i0][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          clr[ib][i0][i1] = (GLfloat*)malloc(4 * sizeof(GLfloat));
        }/*for (i1 = 0; i1 < 3; ++i1)*/
      }/*for (i0 = 0; i0 < ntri[ib]; ++i0)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 1)*/
} /* fermi_patch */
