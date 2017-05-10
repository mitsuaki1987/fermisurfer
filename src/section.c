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
 Project 3D vector into 2D plane
*/
static void proj_2d(int ib, int n2d0, GLfloat vec[2][3]) {
  int ii, jj, kk;

  for (jj = 0; jj < 2; jj++) {
    for (kk = 0; kk < 2; kk++) {
      kv2d[ib][n2d0][jj][kk] = 0.0;
      for (ii = 0; ii < 3; ii++)
        kv2d[ib][n2d0][jj][kk] += axis2d[kk][ii] * vec[jj][ii];
    }/*for (kk = 0; kk < 2; kk++)*/
    kv2d[ib][n2d0][jj][2] = 0.0;
  }/*for (jj = 0; j < 2; jj++)*/
}/*proj_2d*/
/**
 Set Projection axis for 2D plane
*/
static void set2daxis() {
  int ii, jj;
  GLfloat snorm, norm;

  snorm = 0.0;
  for (ii = 0; ii < 3; ii++) snorm += secvec[ii] * secvec[ii];
  /*
   Define the first axis
  */
  for (ii = 0; ii < 3; ii++) {
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] = 0.0;
    axis2d[0][ii] = 1.0;
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] += - secvec[ii] * secvec[jj] / snorm;

    norm = 0.0;
    for (jj = 0; jj < 3; jj++) norm += axis2d[0][jj] * axis2d[0][jj];
    norm = sqrt(norm);
    if (norm > 0.000001) {
      for (jj = 0; jj < 3; jj++) axis2d[0][jj] /= norm;
      break;
    }/*if (norm > 0.000001)*/
  }/*for (ii = 0; ii < 3; ii++)*/
  /*
   Define the second axis with outor product
  */
  axis2d[1][0] = secvec[1] * axis2d[0][2] - secvec[2] * axis2d[0][1];
  axis2d[1][1] = secvec[2] * axis2d[0][0] - secvec[0] * axis2d[0][2];
  axis2d[1][2] = secvec[0] * axis2d[0][1] - secvec[1] * axis2d[0][0];
  norm = 0.0;
  for (jj = 0; jj < 3; jj++) norm += axis2d[1][jj] * axis2d[1][jj];
  norm = sqrt(norm);
  for (jj = 0; jj < 3; jj++) axis2d[1][jj] /= norm;
}/*static void set_2daxis*/
 /**
 Judge wheser this line is the edge of 1st BZ (or the premitive BZ)
 */
int bragg_vert2d(
  int jbr /**< [in] Index of a Bragg plane*/,
  int nbr /**< [in] */,
  GLfloat vert[3] /**< [in] start point of line*/,
  GLfloat vert2[3] /**< [in] end point of line*/)
{
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3], rhs[3], prod, thr = (GLfloat)0.0001, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < 26; ++kbr) {
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = secvec[i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = 0.0;
    for (i = 0; i < 3; ++i)rhs[0] += secscale * secvec[i] * secvec[i];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    /*
    if Bragg planes do not cross, roop next kbr
    */
    det = solve3(bmat, rhs);
    if (fabsf(det) < thr) continue;
    /*
    if vert0 = vert1, roop next kbr
    */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
         + (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
         + (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if (prod < thr) continue;
    /*
    is corner really in 1st BZ ?
    */
    i = 0;
    for (lbr = 0; lbr < 26; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }
  /*
  this line is not a BZ boundary
  */
  return 0;
  /**/
}/* bragg_vert2d */
/**
 Compute boundary of 2D BZ
*/
static void calc_2dbz() {
  int jbr, nbr, i, j, lvert, ibzl;
  GLfloat vert[2][3], vec[26][2][3], prod;

  nbzl2d = 0;

  for (jbr = 0; jbr < 26; ++jbr) {
    /**/
    for (i = 0; i < 3; ++i) vert[1][i] = 0.0;
    nbr = 0;
    lvert = bragg_vert2d(jbr, nbr, vert[0], vert[1]);
    if (lvert == 0) continue;
    nbr = lvert;
    /**/
    lvert = bragg_vert2d(jbr, nbr, vert[1], vert[0]);
    if (lvert == 0) continue;
    /**/
    for (i = 0; i < 2; ++i) for (j = 0; j < 3; ++j) vec[nbzl2d][i][j] = vert[i][j];
    nbzl2d = nbzl2d + 1;
  }/*for (jbr = 0; jbr < 26; ++jbr)*/
  /*
   Order bz lines
  */
  for (i = 0; i < 3; i++) bzl2d[0][i] = vec[0][0][i];
  for (i = 0; i < 3; i++) bzl2d[1][i] = vec[0][1][i];
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {
    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[j][i] - vec[ibzl][j][i]) * (bzl2d[j][i] - vec[ibzl][j][i]);
    if (prod < 0.00001) 
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[1 - j][i] - vec[ibzl][j][i]) * (bzl2d[1 - j][i] - vec[ibzl][j][i]);
    if (prod < 0.00001)
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
  }/*for (ibzl = 1; ibzl < nbzl2d; ibzl++)*/

  for (jbr = 1; jbr < nbzl2d - 1; jbr++) {

    prod = 0.0;
    for (ibzl = 0; ibzl < nbzl2d; ibzl++) for (i = 0; i < 3; i++)
      prod += vec[ibzl][0][i] * vec[ibzl][0][i];
    if (prod < 0.00001) {
      nbzl2d = jbr + 1;
      break;
    }

    for (ibzl = 1; ibzl < nbzl2d; ibzl++) {
      prod = (bzl2d[jbr][0] - vec[ibzl][0][0]) * (bzl2d[jbr][0] - vec[ibzl][0][0])
           + (bzl2d[jbr][1] - vec[ibzl][0][1]) * (bzl2d[jbr][1] - vec[ibzl][0][1])
           + (bzl2d[jbr][2] - vec[ibzl][0][2]) * (bzl2d[jbr][2] - vec[ibzl][0][2]);
      if (prod < 0.00001) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][1][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }

      prod = (bzl2d[jbr][0] - vec[ibzl][1][0]) * (bzl2d[jbr][0] - vec[ibzl][1][0])
           + (bzl2d[jbr][1] - vec[ibzl][1][1]) * (bzl2d[jbr][1] - vec[ibzl][1][1])
           + (bzl2d[jbr][2] - vec[ibzl][1][2]) * (bzl2d[jbr][2] - vec[ibzl][1][2]);
      if (prod < 0.00001) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][0][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }
    }/*for (ibzl = 1; ibzl < nbzl2d; ibzl++)*/
  }/*for (ibzl = 1; ibzl < nbzl; ibzl++)*/
  /*
   Project into 2D plane
  */
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {
    for (i = 0; i < 2; i++) {
      bzl2d_proj[ibzl][i] = 0.0;
      for (j = 0; j < 3; j++)
        bzl2d_proj[ibzl][i] += axis2d[i][j] * bzl2d[ibzl][j];
    }/*for (kk = 0; kk < 2; kk++)*/
    bzl2d_proj[ibzl][2] = 0.0;
  }/*for (ibzl = 0; ibzl < nbzl2d; ibzl++)*/
}/*calc_2dbz()*/
/**
 Compute Fermi-line 
*/
void calc_section() {
  int ib, itri, i, j, n2d0, ithread;
  GLfloat nprod[3], norm[3], vec[2][3];
  /*
   Set Projection axis for 2D plane
  */
  set2daxis();
  calc_2dbz();

#pragma omp parallel default(none) \
  shared(nb,n2d,clr,clr2d,kvp,ntri,ntri_th,secvec,secscale,query,kv2d) \
  private(ib,itri,norm,nprod,i,j,n2d0,ithread,vec)
  {
    ithread = get_thread();
    for (ib = 0; ib < nb; ib++) {
      if (query == 1) n2d0 = 0;
      else n2d0 = ntri_th[ib][ithread];
#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {
        /**/
        for (i = 0; i < 3; i++) {
          norm[i] = 0.0;
          for (j = 0; j < 3; j++) norm[i] += secvec[j] * (kvp[ib][itri][i][j] - secscale * secvec[j]);
        }
        nprod[0] = norm[1] * norm[2];
        nprod[1] = norm[2] * norm[0];
        nprod[2] = norm[0] * norm[1];
        /**/
        if (fabsf(norm[1]) < 0.00001 && fabsf(norm[2]) < 0.00001) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][1][i];
            for (i = 0; i < 3; ++i) vec[1][i] = kvp[ib][itri][2][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][1][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] = clr[ib][itri][2][i];
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (fabsf(norm[1]) < 0.00001 && fabsf(norm[2]) < 0.00001)*/
        else if (fabsf(norm[0]) < 0.00001 && fabsf(norm[2]) < 0.00001) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][0][i];
            for (i = 0; i < 3; ++i) vec[1][i] = kvp[ib][itri][2][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][0][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] = clr[ib][itri][2][i];
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (fabsf(norm[0]) < 0.00001 && fabsf(norm[2]) < 0.00001)*/
        else if (fabsf(norm[0]) < 0.00001 && fabsf(norm[1]) < 0.00001) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][0][i];
            for (i = 0; i < 3; ++i) vec[1][i] = kvp[ib][itri][1][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][0][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] = clr[ib][itri][1][i];
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*if (fabsf(norm[0]) < 0.00001 && fabsf(norm[1]) < 0.00001)*/
        else if (fabsf(norm[0]) < 0.00001 && nprod[0] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][0][i];
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[1] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][1][i]) / (norm[1] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][0][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[1] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][1][i]) / (norm[1] - norm[2]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (fabsf(norm[0]) < 0.00001 && nprod[0] < 0.00001)*/
        else if (fabsf(norm[1]) < 0.00001 && nprod[1] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][1][i];
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[0] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][0][i]) / (norm[0] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][1][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[0] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][0][i]) / (norm[0] - norm[2]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (fabsf(norm[1]) < 0.00001 && nprod[1] < 0.00001)*/
        else if (fabsf(norm[2]) < 0.00001 && nprod[2] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] = kvp[ib][itri][2][i];
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[0] * kvp[ib][itri][1][i] - norm[1] * kvp[ib][itri][0][i]) / (norm[0] - norm[1]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] = clr[ib][itri][2][i];
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[0] * clr[ib][itri][1][i] - norm[1] * clr[ib][itri][0][i]) / (norm[0] - norm[1]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (fabsf(norm[2]) < 0.00001 && nprod[2] < 0.00001)*/
        else if (nprod[1] < 0.0 && nprod[2] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] =
              (norm[0] * kvp[ib][itri][1][i] - norm[1] * kvp[ib][itri][0][i]) / (norm[0] - norm[1]);
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[0] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][0][i]) / (norm[0] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] =
              (norm[0] * clr[ib][itri][1][i] - norm[1] * clr[ib][itri][0][i]) / (norm[0] - norm[1]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[0] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][0][i]) / (norm[0] - norm[2]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (nprod[1] < 0.00001 && nprod[2] < 0.00001)*/
        else if (nprod[0] < 0.0 && nprod[2] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] =
              (norm[0] * kvp[ib][itri][1][i] - norm[1] * kvp[ib][itri][0][i]) / (norm[0] - norm[1]);
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[1] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][1][i]) / (norm[1] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] =
              (norm[0] * clr[ib][itri][1][i] - norm[1] * clr[ib][itri][0][i]) / (norm[0] - norm[1]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[1] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][1][i]) / (norm[1] - norm[2]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (nprod[0] < 0.00001 && nprod[2] < 0.00001)*/
        else if (nprod[1] < 0.0 && nprod[0] < 0.0) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) vec[0][i] =
              (norm[0] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][0][i]) / (norm[0] - norm[2]);
            for (i = 0; i < 3; ++i) vec[1][i] =
              (norm[1] * kvp[ib][itri][2][i] - norm[2] * kvp[ib][itri][1][i]) / (norm[1] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][0][i] =
              (norm[0] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][0][i]) / (norm[0] - norm[2]);
            for (i = 0; i < 4; ++i) clr2d[ib][n2d0][1][i] =
              (norm[1] * clr[ib][itri][2][i] - norm[2] * clr[ib][itri][1][i]) / (norm[1] - norm[2]);
            proj_2d(ib, n2d0, vec);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (nprod[1] < 0.00001 && nprod[0] < 0.00001)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      if(query == 1) ntri_th[ib][ithread] = n2d0;
    }/*for (ib = 0; ib < nb; ib++)*/
  }/* End of parallel region */
  /*
   Allocation of Fermi-lines
  */
  if (query == 1) {
    /*
    Sum Fermi-lines in all threads
    */
    for (ib = 0; ib < nb; ib++) {
      for (ithread = 1; ithread < nthreads; ithread++) {
        ntri_th[ib][ithread] += ntri_th[ib][ithread - 1];
      }
      n2d[ib] = ntri_th[ib][nthreads - 1];
      for (ithread = nthreads - 1; ithread > 0; ithread--) {
        ntri_th[ib][ithread] = ntri_th[ib][ithread - 1];
      }
      ntri_th[ib][0] = 0;
    }
    printf("    band   # of Fermi-line\n");
    for (ib = 0; ib < nb; ib++) {
      printf("    %d       %d\n", ib + 1, n2d[ib]);
    }
    /*
    Allocation of Fermi-lines
    */
    kv2d = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    clr2d = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    for (ib = 0; ib < nb; ++ib) {
      kv2d[ib] = (GLfloat***)malloc(n2d[ib] * sizeof(GLfloat**));
      clr2d[ib] = (GLfloat***)malloc(n2d[ib] * sizeof(GLfloat**));
      for (i = 0; i < n2d[ib]; ++i) {
        kv2d[ib][i] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
        clr2d[ib][i] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
        for (j = 0; j < 2; ++j) {
          kv2d[ib][i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          clr2d[ib][i][j] = (GLfloat*)malloc(4 * sizeof(GLfloat));
        }/*for (j = 0; j < 2; ++j)*/
      }/*for (i = 0; i < n2d[ib]; ++i)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 0)*/
}/*void calc_nodeline()*/
