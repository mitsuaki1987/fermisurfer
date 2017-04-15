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
#include <stdio.h>
#include "variable.h"
#include "basic_math.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
* Store triangle patch
*/
void triangle(
  int ib /**<[in] The band index*/,
  int *ntri0, /**<[inout] Index of triangle patch*/
  int nbr /**<[in] Bragg plane*/,
  GLfloat mat1[3] /**<[in] The matrix element*/,
  GLfloat kvec1[3][3] /**<[in] k-vector of corners*/)
{
  /**/
  int ibr, i, j;
  GLfloat prod[3], thr = 0.0000, mat2[3], kvec2[3][3];
  /**/
  if (fbz == 1) {
    /**/
    for (ibr = nbr; ibr < 26; ++ibr) {
      /**/
      for (i = 0; i < 3; ++i) {
        prod[i] = bragg[ibr][0] * kvec1[i][0]
                + bragg[ibr][1] * kvec1[i][1]
                + bragg[ibr][2] * kvec1[i][2];
      }
      eigsort(3, prod, mat1, kvec1);
      /**/
      if (brnrm[ibr] + thr < prod[0]) {
        return;
      }
      else if (brnrm[ibr] + thr < prod[1]) {
        mat2[0] = mat1[0];
        mat2[1] = mat1[0] * (brnrm[ibr] - prod[1]) / (prod[0] - prod[1])
                + mat1[1] * (brnrm[ibr] - prod[0]) / (prod[1] - prod[0]);
        mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                + mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        for (i = 0; i<3; ++i) kvec2[0][i] = kvec1[0][i];
        for (i = 0; i<3; ++i) 
          kvec2[1][i] = kvec1[0][i] * (brnrm[ibr] - prod[1]) / (prod[0] - prod[1])
                      + kvec1[1][i] * (brnrm[ibr] - prod[0]) / (prod[1] - prod[0]);
        for (i = 0; i<3; ++i)
          kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                      + kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        return;
      }
      else if (brnrm[ibr] + thr < prod[2]) {
        mat2[0] = mat1[0];
        mat2[1] = mat1[1];
        mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                + mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        for (i = 0; i<3; ++i) kvec2[0][i] = kvec1[0][i];
        for (i = 0; i<3; ++i) kvec2[1][i] = kvec1[1][i];
        for (i = 0; i<3; ++i) 
          kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                      + kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        /**/
        mat2[0] = mat1[1] * (brnrm[ibr] - prod[2]) / (prod[1] - prod[2])
                + mat1[2] * (brnrm[ibr] - prod[1]) / (prod[2] - prod[1]);
        mat2[1] = mat1[1];
        mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                + mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        for (i = 0; i<3; ++i)
          kvec2[0][i] = kvec1[1][i] * (brnrm[ibr] - prod[2]) / (prod[1] - prod[2])
                      + kvec1[2][i] * (brnrm[ibr] - prod[1]) / (prod[2] - prod[1]);
        for (i = 0; i<3; ++i) kvec2[1][i] = kvec1[1][i];
        for (i = 0; i<3; ++i)
          kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
                      + kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
        triangle(ib, ntri0, ibr + 1, mat2, kvec2);
        return;
      }
      else {
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < 26*/
  } /* if fbz == 1 */
    /**/
  if (query == 1) {
    *ntri0 += 1;
  }
  else {
    normal_vec(kvec1[0], kvec1[1], kvec1[2], nmlp[ib][*ntri0]);
    for (i = 0; i < 3; ++i) {
      matp[ib][*ntri0][i] = mat1[i];
      for (j = 0; j < 3; ++j) {
        kvp[ib][*ntri0][i][j] = kvec1[i][j];
      }
    }
    *ntri0 += 1;
  }
  /**/
}/* triangle */
 /**
 * Tetrahedrron method
 */
void tetrahedron(
  int ib /**< [in] The band index*/,
  int *ntri0, /**< [inout] index of trixngle*/
  GLfloat eig1[8] /**< [in] Orbital energies*/,
  GLfloat mat1[8] /**< [in] Matrix elements*/,
  GLfloat kvec1[8][3] /**< [in] k vectors*/)
{
  /**/
  int it, i, j;
  GLfloat eig2[4], mat2[4], kvec2[4][3], a[4][4], kvec3[3][3], mat3[3];
  /**/
  for (it = 0; it < 6; ++it) {
    /*
    Define corners of the tetrahedron
    */
    for (i = 0; i < 4; ++i) {
      eig2[i] = eig1[corner[it][i]];
      mat2[i] = mat1[corner[it][i]];
      /**/
      for (j = 0; j < 3; ++j) 
        kvec2[i][j] = bvec[0][j] * kvec1[corner[it][i]][0]
                    + bvec[1][j] * kvec1[corner[it][i]][1]
                    + bvec[2][j] * kvec1[corner[it][i]][2];
    }
    /*
    Sort of eig1
    */
    eigsort(4, eig2, mat2, kvec2);
    for (i = 0; i < 4; ++i) {
      for (j = 0; j < 4; ++j) {
        a[i][j] = (0.00 - eig2[j]) / (eig2[i] - eig2[j]);
      }
    }
    /*
    Draw triangle in each cases
    */
    if (eig2[0] <= 0.00 && 0.00 < eig2[1]) {
      for (i = 0; i < 3; ++i) kvec3[0][i] = kvec2[0][i] * a[0][1] + kvec2[1][i] * a[1][0];
      for (i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][2] + kvec2[2][i] * a[2][0];
      for (i = 0; i < 3; ++i) kvec3[2][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      mat3[0] = mat2[0] * a[0][1] + mat2[1] * a[1][0];
      mat3[1] = mat2[0] * a[0][2] + mat2[2] * a[2][0];
      mat3[2] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      triangle(ib, ntri0, 0, mat3, kvec3);
    }
    else if (eig2[1] <= 0.00 && 0.00 < eig2[2]) {
      for (i = 0; i < 3; ++i) kvec3[0][i] = kvec2[0][i] * a[0][2] + kvec2[2][i] * a[2][0];
      for (i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      for (i = 0; i < 3; ++i) kvec3[2][i] = kvec2[1][i] * a[1][2] + kvec2[2][i] * a[2][1];
      mat3[0] = mat2[0] * a[0][2] + mat2[2] * a[2][0];
      mat3[1] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      mat3[2] = mat2[1] * a[1][2] + mat2[2] * a[2][1];
      triangle(ib, ntri0, 0, mat3, kvec3);
      /**/
      for (i = 0; i < 3; ++i) kvec3[0][i] = kvec2[1][i] * a[1][3] + kvec2[3][i] * a[3][1];
      for (i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      for (i = 0; i < 3; ++i) kvec3[2][i] = kvec2[1][i] * a[1][2] + kvec2[2][i] * a[2][1];
      mat3[0] = mat2[1] * a[1][3] + mat2[3] * a[3][1];
      mat3[1] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      mat3[2] = mat2[1] * a[1][2] + mat2[2] * a[2][1];
      triangle(ib, ntri0, 0, mat3, kvec3);
    }
    else if (eig2[2] <= 0.00 && 0.00 < eig2[3]) {
      for (i = 0; i < 3; ++i) kvec3[0][i] = kvec2[3][i] * a[3][0] + kvec2[0][i] * a[0][3];
      for (i = 0; i < 3; ++i) kvec3[1][i] = kvec2[3][i] * a[3][1] + kvec2[1][i] * a[1][3];
      for (i = 0; i < 3; ++i) kvec3[2][i] = kvec2[3][i] * a[3][2] + kvec2[2][i] * a[2][3];
      mat3[0] = mat2[3] * a[3][0] + mat2[0] * a[0][3];
      mat3[1] = mat2[3] * a[3][1] + mat2[1] * a[1][3];
      mat3[2] = mat2[3] * a[3][2] + mat2[2] * a[2][3];
      triangle(ib, ntri0, 0, mat3, kvec3);
    }
    else {
    }
  }
}/* tetrahedron */
 /**
 * Patches for FSs
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
  /**
   * If this run is workspace query, malloc arrays
   */
  if (query == 1) {
    /*
     * Sum patches in all threads 
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
    /**
     * Allocation of triangler patches
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
