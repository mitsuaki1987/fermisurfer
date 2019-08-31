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
/**
@file
@brief Functions for the 2D Fermi line
*/
#include <GLES/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "basic_math.hpp"
#include "variable.hpp"
/**
 @brief Project 3D \f$k\f$-vector into 2D plane. 

 Modify: ::kv2d
*/
static void proj_2d(
  GLfloat vec[3] //!< [in,out] Line ends to be projected
) 
{
  int ii, kk;
  GLfloat vec0[3];

  for (kk = 0; kk < 2; kk++) {
    vec0[kk] = 0.0;
    for (ii = 0; ii < 3; ii++)
      vec0[kk] += axis2d[kk][ii] * vec[ii];
  }/*for (kk = 0; kk < 2; kk++)*/
  vec0[2] = 0.0;
  for (kk = 0; kk < 3; kk++)vec[kk] = vec0[kk];
}/*proj_2d*/
/**
 @brief Set Projection axis for 2D plane

 Modify : ::axis2d
*/
static void set2daxis() {
  int ii, jj;
  GLfloat snorm, norm, thr = 0.001f;

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
    norm = sqrtf(norm);
    if (norm > thr) {
      for (jj = 0; jj < 3; jj++) axis2d[0][jj] /= norm;
      break;
    }/*if (norm > 0.thr)*/
  }/*for (ii = 0; ii < 3; ii++)*/
  /*
   Define the second axis with outor product
  */
  axis2d[1][0] = secvec[1] * axis2d[0][2] - secvec[2] * axis2d[0][1];
  axis2d[1][1] = secvec[2] * axis2d[0][0] - secvec[0] * axis2d[0][2];
  axis2d[1][2] = secvec[0] * axis2d[0][1] - secvec[1] * axis2d[0][0];
  norm = 0.0;
  for (jj = 0; jj < 3; jj++) norm += axis2d[1][jj] * axis2d[1][jj];
  norm = sqrtf(norm);
  for (jj = 0; jj < 3; jj++) axis2d[1][jj] /= norm;
}/*static void set_2daxis*/
/**
 @brief Judge wheser this line is the edge of 1st BZ (or the premitive BZ)
*/
int bragg_vert2d(
  int jbr, //!< [in] Index of a Bragg plane
  int nbr, //!< [in]
  GLfloat vert[3], //!< [inout] start point of line
  GLfloat vert2[3] //!< [in] end point of line
)
{
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3], rhs[3], prod, thr, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < nbragg; ++kbr) {
    /**/
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = secvec[i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = 0.0;
    for (i = 0; i < 3; ++i)rhs[0] += secvec[i] * secvec[i];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = sqrtf(rhs[0] * rhs[1] * rhs[2]) * 0.001f;
    rhs[0] *= secscale;
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
    for (lbr = 0; lbr < nbragg; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }/*for (lbr = 0; lbr < nbragg; ++lbr)*/
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }/*for (kbr = nbr0; kbr < nbragg; ++kbr)*/
  /*
  this line is not a BZ boundary
  */
  return 0;
}/* bragg_vert2d */
/**
 @brief Compute boundary of 2D BZ

 Modify : ::nbzl2d, ::bzl2d_proj
*/
void calc_2dbz() {
  int jbr, nbr, i, j, lvert, ibzl;
  GLfloat vert[2][3], vec[26][2][3], prod, thr;

  if (fbz != 1)return;
  /*
   Set Projection axis for 2D plane
  */
  set2daxis();

  nbzl2d = 0;

  for (jbr = 0; jbr < nbragg; ++jbr) {
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
  }/*for (jbr = 0; jbr < nbragg; ++jbr)*/
  /*
   Order bz lines
  */
  for (i = 0; i < 3; i++) bzl2d[0][i] = vec[0][0][i];
  for (i = 0; i < 3; i++) bzl2d[1][i] = vec[0][1][i];
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {

    thr = 0.0f;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[j][i] * bzl2d[j][i];
    thr *= 0.001f;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[j][i] - vec[ibzl][j][i]) * (bzl2d[j][i] - vec[ibzl][j][i]);
    if (prod < thr) 
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[1 - j][i] - vec[ibzl][j][i]) * (bzl2d[1 - j][i] - vec[ibzl][j][i]);
    if (prod < thr)
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
  }/*for (ibzl = 1; ibzl < nbzl2d; ibzl++)*/

  for (jbr = 1; jbr < nbzl2d - 1; jbr++) {

    thr = 0.0f;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[jbr][i] * bzl2d[jbr][i];
    thr *= 0.001f;

    prod = 0.0;
    for (ibzl = 0; ibzl < nbzl2d; ibzl++) for (i = 0; i < 3; i++)
      prod += vec[ibzl][0][i] * vec[ibzl][0][i];
    if (prod < thr) {
      nbzl2d = jbr + 1;
      break;
    }

    for (ibzl = 1; ibzl < nbzl2d; ibzl++) {
      prod = (bzl2d[jbr][0] - vec[ibzl][0][0]) * (bzl2d[jbr][0] - vec[ibzl][0][0])
           + (bzl2d[jbr][1] - vec[ibzl][0][1]) * (bzl2d[jbr][1] - vec[ibzl][0][1])
           + (bzl2d[jbr][2] - vec[ibzl][0][2]) * (bzl2d[jbr][2] - vec[ibzl][0][2]);
      if (prod < thr) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][1][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }

      prod = (bzl2d[jbr][0] - vec[ibzl][1][0]) * (bzl2d[jbr][0] - vec[ibzl][1][0])
           + (bzl2d[jbr][1] - vec[ibzl][1][1]) * (bzl2d[jbr][1] - vec[ibzl][1][1])
           + (bzl2d[jbr][2] - vec[ibzl][1][2]) * (bzl2d[jbr][2] - vec[ibzl][1][2]);
      if (prod < thr) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][0][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }
    }/*for (ibzl = 1; ibzl < nbzl2d; ibzl++)*/
  }/*for (ibzl = 1; ibzl < nbzl; ibzl++)*/
  /*
   Project into 2D plane
  */
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {
    for (i = 0; i < 3; i++) bzl2d_proj[ibzl][i] = bzl2d[ibzl][i];
    proj_2d(bzl2d_proj[ibzl]);
  }/*for (ibzl = 0; ibzl < nbzl2d; ibzl++)*/
}/*calc_2dbz()*/
/**
 @brief Compute Fermi-line 

 Modify : ::ntri_th, ::n2d, ::clr2d, ::kv2d

 If ::query = 1, this routine only compute the number of line segment and
 malloc variables.

 If ::query = 0, actually compute lines and store them.
*/
void calc_section() {
  int i, ib, itri, ithread;

  if (fbz != 1)return;

#pragma omp parallel default(none) \
  shared(nb,n2d,clr,clr2d,kvp,kv2d,ntri,ntri_th,secvec,secscale,query) \
  private(ib,itri,i,ithread)
  {
    int j, n2d0, sw[3];
    GLfloat norm[3], a[3][3];

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
        eigsort(3, norm, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.0f - norm[sw[j]]) / (norm[sw[i]] - norm[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/
         /**/
        if ((norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]]) || (norm[sw[0]] <= 0.0 && 0.0 < norm[sw[1]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kv2d[ib][i + 0 * 3 + 6 * n2d0]
                = kvp[ib][itri][sw[1]][i] * a[1][0] + kvp[ib][itri][sw[0]][i] * a[0][1];
              kv2d[ib][i + 1 * 3 + 6 * n2d0]
                = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
            }/*for (i = 0; i < 3; ++i)*/
            for (i = 0; i < 4; ++i) {
              clr2d[ib][i + 0 * 4 + 8 * n2d0]
                = clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][0]
                + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][1];
              clr2d[ib][i + 1 * 4 + 8 * n2d0]
                = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
                + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
            }/*for (i = 0; i < 4; ++i)*/
            proj_2d(&kv2d[ib][0 * 3 + 6 * n2d0]);
            proj_2d(&kv2d[ib][1 * 3 + 6 * n2d0]);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]])*/
        else if ((norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]]) || (norm[sw[1]] <= 0.0 && 0.0 < norm[sw[2]])) {
          if (query == 0) {
            for (i = 0; i < 3; ++i) {
              kv2d[ib][i + 0 * 3 + 6 * n2d0] 
                = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
              kv2d[ib][i + 1 * 3 + 6 * n2d0]
                = kvp[ib][itri][sw[2]][i] * a[2][1] + kvp[ib][itri][sw[1]][i] * a[1][2];
            }/*for (i = 0; i < 3; ++i)*/
            for (i = 0; i < 4; ++i) {
              clr2d[ib][i + 0 * 4 + 8 * n2d0]
                = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
                + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
              clr2d[ib][i + 1 * 4 + 8 * n2d0]
                = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][1]
                + clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][2];
            }/*for (i = 0; i < 4; ++i)*/
            proj_2d(&kv2d[ib][0 * 3 + 6 * n2d0]);
            proj_2d(&kv2d[ib][1 * 3 + 6 * n2d0]);
          }/*if (query == 0)*/
          n2d0 += 1;
        }/*else if (norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]])*/
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
    /*
    Allocation of Fermi-lines
    */
    kv2d = new GLfloat*[nb];
    clr2d = new GLfloat*[nb];
    for (ib = 0; ib < nb; ++ib) {
      kv2d[ib] = new GLfloat[6 * n2d[ib]];
      clr2d[ib] = new GLfloat[8 * n2d[ib]];
    }/*for (ib = 0; ib < nb; ++ib)*/
  }/*if (query == 0)*/
}/*void calc_nodeline()*/
