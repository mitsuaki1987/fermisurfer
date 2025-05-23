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
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <wx/wx.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "basic_math.hpp"
#include "variable.hpp"
/**
 @brief Project 3D \f$k\f$-vector into 2D plane. 

 Modify: ::kv2d
*/
static void proj_2d(
  GLfloat axis2d[2][3],
  GLfloat vec[3] //!< [in,out] Line ends to be projected
) 
{
  int ii, kk;
  GLfloat vec0[3] = {};

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
static void set2daxis(
  GLfloat secvec[3],
  GLfloat axis2d[2][3]
) {
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
  int nbragg,
  GLfloat bragg[26][3],
  GLfloat brnrm[26],
  GLfloat secvec[3],
  GLfloat secscale,
  int jbr, //!< [in] Index of a Bragg plane
  int nbr, //!< [in]
  GLfloat vert[3], //!< [inout] start point of line
  GLfloat vert2[3], //!< [in] end point of line
  GLfloat kshift[3] //!< [in] Reciplocal lattice vector
)
{
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3] = {}, rhs[3] = {}, prod, thr, det;
  //
  nbr0 = nbr;
  //
  for (kbr = nbr0; kbr < nbragg; ++kbr) {
    //
    for (i = 0; i<3; ++i) bmat[0][i] = secvec[i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    //
    rhs[0] = 0.0;
    for (i = 0; i < 3; ++i)rhs[0] += secvec[i] * secvec[i];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = sqrtf(rhs[0] * rhs[1] * rhs[2]) * 0.001f;
    rhs[0] *= secscale;
    for (i = 0; i < 3; ++i)rhs[1] += bragg[jbr][i] * kshift[i];
    for (i = 0; i < 3; ++i)rhs[2] += bragg[kbr][i] * kshift[i];
    /*
    if Bragg planes do not cross, loop next kbr
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
    //
    // is corner really in 1st BZ ?
    //
    i = 0;
    for (lbr = 0; lbr < nbragg; ++lbr) {
      prod = bragg[lbr][0] * (rhs[0] - kshift[0])
           + bragg[lbr][1] * (rhs[1] - kshift[1])
           + bragg[lbr][2] * (rhs[2] - kshift[2]);
      //
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }//for (lbr = 0; lbr < nbragg; ++lbr)
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }//for (kbr = nbr0; kbr < nbragg; ++kbr)
  //
  // this line is not a BZ boundary
  //
  return 0;
}/* bragg_vert2d */
/**
 @brief Compute boundary of 2D BZ

 Modify : ::nbzl2d, ::bzl2d_proj
*/
void calc_2dbz() {
  int jbr, nbr, i, j, lvert, ibzl, a0, a1, a2, ia, iBZ;
  GLfloat vert[2][3] = {}, vec[26][2][3] = {}, prod, thr, kshift[3] = {};

  if (fbz != 1)return;
  /*
   Set Projection axis for 2D plane
  */
  set2daxis(secvec, axis2d);
  //
  //
  //
  nnbzl2d = BZ_number[0] * BZ_number[1] * BZ_number[2];
  nbzl2d = new int [nnbzl2d];
  bzl2d = new GLfloat * *[nnbzl2d];
  bzl2d_proj = new GLfloat * *[nnbzl2d];
  for (i = 0; i < nnbzl2d; i++) {
    bzl2d[i] = new GLfloat *[26];
    bzl2d_proj[i] = new GLfloat *[26];
    for (j = 0; j < 26; j++) {
      bzl2d[i][j] = new GLfloat [3];
      bzl2d_proj[i][j] = new GLfloat [3];
    }
  }
  //
  iBZ = 0;
  for (a0 = -BZ_number[0] / 2; a0 < -BZ_number[0] / 2 + BZ_number[0]; a0++) {
    for (a1 = -BZ_number[1] / 2; a1 < -BZ_number[1] / 2 + BZ_number[1]; a1++) {
      for (a2 = -BZ_number[2] / 2; a2 < -BZ_number[2] / 2 + BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;

        nbzl2d[iBZ] = 0;

        for (jbr = 0; jbr < nbragg; ++jbr) {
          //
          for (i = 0; i < 3; ++i) vert[1][i] = 0.0;
          nbr = 0;
          lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[0], vert[1], kshift);
          if (lvert == 0) continue;
          nbr = lvert;
          //
          lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[1], vert[0], kshift);
          if (lvert == 0) continue;
          //
          for (i = 0; i < 2; ++i) for (j = 0; j < 3; ++j) vec[nbzl2d[iBZ]][i][j] = vert[i][j];
          nbzl2d[iBZ] += 1;
        }//for (jbr = 0; jbr < nbragg; ++jbr)
        //
        // Order bz lines
        //
        for (i = 0; i < 3; i++) bzl2d[iBZ][0][i] = vec[0][0][i];
        for (i = 0; i < 3; i++) bzl2d[iBZ][1][i] = vec[0][1][i];
        for (ibzl = 0; ibzl < nbzl2d[iBZ]; ibzl++) {

          thr = 0.0f;
          for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[iBZ][j][i] * bzl2d[iBZ][j][i];
          thr *= 0.001f;

          prod = 0.0;
          for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
            prod += (bzl2d[iBZ][j][i] - vec[ibzl][j][i]) * (bzl2d[iBZ][j][i] - vec[ibzl][j][i]);
          if (prod < thr)
            for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;

          prod = 0.0;
          for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
            prod += (bzl2d[iBZ][1 - j][i] - vec[ibzl][j][i]) * (bzl2d[iBZ][1 - j][i] - vec[ibzl][j][i]);
          if (prod < thr)
            for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
        }//for (ibzl = 1; ibzl < *nbzl2d; ibzl++)

        for (jbr = 1; jbr < nbzl2d[iBZ] - 1; jbr++) {

          thr = 0.0f;
          for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[iBZ][jbr][i] * bzl2d[iBZ][jbr][i];
          thr *= 0.001f;

          prod = 0.0;
          for (ibzl = 0; ibzl < nbzl2d[iBZ]; ibzl++) for (i = 0; i < 3; i++)
            prod += vec[ibzl][0][i] * vec[ibzl][0][i];
          if (prod < thr) {
            nbzl2d[iBZ] = jbr + 1;
            break;
          }

          for (ibzl = 1; ibzl < nbzl2d[iBZ]; ibzl++) {
            prod = (bzl2d[iBZ][jbr][0] - vec[ibzl][0][0]) * (bzl2d[iBZ][jbr][0] - vec[ibzl][0][0])
                 + (bzl2d[iBZ][jbr][1] - vec[ibzl][0][1]) * (bzl2d[iBZ][jbr][1] - vec[ibzl][0][1])
                 + (bzl2d[iBZ][jbr][2] - vec[ibzl][0][2]) * (bzl2d[iBZ][jbr][2] - vec[ibzl][0][2]);
            if (prod < thr) {
              for (i = 0; i < 3; i++) bzl2d[iBZ][jbr + 1][i] = vec[ibzl][1][i];
              for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
            }

            prod = (bzl2d[iBZ][jbr][0] - vec[ibzl][1][0]) * (bzl2d[iBZ][jbr][0] - vec[ibzl][1][0])
                 + (bzl2d[iBZ][jbr][1] - vec[ibzl][1][1]) * (bzl2d[iBZ][jbr][1] - vec[ibzl][1][1])
                 + (bzl2d[iBZ][jbr][2] - vec[ibzl][1][2]) * (bzl2d[iBZ][jbr][2] - vec[ibzl][1][2]);
            if (prod < thr) {
              for (i = 0; i < 3; i++) bzl2d[iBZ][jbr + 1][i] = vec[ibzl][0][i];
              for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
            }
          }//for (ibzl = 1; ibzl < *nbzl2d; ibzl++)
        }//for (ibzl = 1; ibzl < nbzl; ibzl++)
        //
        // Project into 2D plane
        //
        for (ibzl = 0; ibzl < nbzl2d[iBZ]; ibzl++) {
          for (i = 0; i < 3; i++) bzl2d_proj[iBZ][ibzl][i] = bzl2d[iBZ][ibzl][i];
          proj_2d(axis2d, bzl2d_proj[iBZ][ibzl]);
        }//for (ibzl = 0; ibzl < *nbzl2d; ibzl++)

        iBZ += 1;
      }//for (a2
    }//for (a1
  }//for (a0
}/*calc_2dbz()*/
/**
 @brief Compute Fermi-line 

 Modify : ::n2d, ::clr2d, ::kv2d
*/
void calc_section() {
  int i, j, ib, itri, ithread, n2d0;
  std::vector<std::vector<std::vector<std::vector<GLfloat> > > > kv2d_v, clr2d_v;

  kv2d_v.resize(nthreads);
  clr2d_v.resize(nthreads);

  if (fbz != 1)return;

  *terminal << wxT("    band   # of Fermi-line\n");
  for (ib = 0; ib < nb; ib++) {

#pragma omp parallel default(none) \
shared(nb,ib,clr,clr2d_v,kvp,kv2d_v,ntri,secvec,secscale,axis2d,bvec,BZ_number) \
private(itri,i,j,ithread)
    {
      int sw[3], a0, a1, a2, ia;
      GLfloat norm[3] = {}, a[3][3] = {}, kshift[3] = {}, kvp2[3][3] = {};
      std::vector<std::vector<GLfloat> > kv2d_0, clr2d_0;

      kv2d_0.resize(2);
      clr2d_0.resize(2);
      for (i = 0; i < 2; i++) {
        kv2d_0.at(i).resize(3);
        clr2d_0.at(i).resize(4);
      }

      ithread = get_thread();
      kv2d_v.at(ithread).resize(0);
      clr2d_v.at(ithread).resize(0);
      for (a0 = -BZ_number[0] / 2; a0 < -BZ_number[0] / 2 + BZ_number[0]; a0++) {
        for (a1 = -BZ_number[1] / 2; a1 < -BZ_number[1] / 2 + BZ_number[1]; a1++) {
          for (a2 = -BZ_number[2] / 2; a2 < -BZ_number[2] / 2 + BZ_number[2]; a2++) {
            for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;

#pragma omp for
            for (itri = 0; itri < ntri[ib]; ++itri) {
              /**/
              for (i = 0; i < 3; i++)for (j = 0; j < 3; j++)kvp2[i][j] = kvp[ib][itri][i][j] + kshift[j];

              for (i = 0; i < 3; i++) {
                norm[i] = 0.0;
                for (j = 0; j < 3; j++) norm[i] += secvec[j] * (kvp2[i][j] - secscale * secvec[j]);
              }
              eigsort(3, norm, sw);
              for (i = 0; i < 3; ++i) {
                for (j = 0; j < 3; ++j) {
                  a[i][j] = (0.0f - norm[sw[j]]) / (norm[sw[i]] - norm[sw[j]]);
                }/*for (j = 0; j < 3; ++j)*/
              }/*for (i = 0; i < 3; ++i)*/
               /**/
              if ((norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]]) || (norm[sw[0]] <= 0.0 && 0.0 < norm[sw[1]])) {
                for (i = 0; i < 3; ++i) {
                  kv2d_0.at(0).at(i)
                    = kvp2[sw[1]][i] * a[1][0] + kvp2[sw[0]][i] * a[0][1];
                  kv2d_0.at(1).at(i)
                    = kvp2[sw[2]][i] * a[2][0] + kvp2[sw[0]][i] * a[0][2];
                }/*for (i = 0; i < 3; ++i)*/
                for (i = 0; i < 4; ++i) {
                  clr2d_0.at(0).at(i)
                    = clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][0]
                    + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][1];
                  clr2d_0.at(1).at(i)
                    = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
                    + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
                }/*for (i = 0; i < 4; ++i)*/
                proj_2d(axis2d, &kv2d_0.at(0).at(0));
                proj_2d(axis2d, &kv2d_0.at(1).at(0));
                kv2d_v.at(ithread).push_back(kv2d_0);
                clr2d_v.at(ithread).push_back(clr2d_0);
              }/*else if (norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]])*/
              else if ((norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]]) || (norm[sw[1]] <= 0.0 && 0.0 < norm[sw[2]])) {
                for (i = 0; i < 3; ++i) {
                  kv2d_0.at(0).at(i)
                    = kvp2[sw[2]][i] * a[2][0] + kvp2[sw[0]][i] * a[0][2];
                  kv2d_0.at(1).at(i)
                    = kvp2[sw[2]][i] * a[2][1] + kvp2[sw[1]][i] * a[1][2];
                }/*for (i = 0; i < 3; ++i)*/
                for (i = 0; i < 4; ++i) {
                  clr2d_0.at(0).at(i)
                    = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
                    + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
                  clr2d_0.at(1).at(i)
                    = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][1]
                    + clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][2];
                }/*for (i = 0; i < 4; ++i)*/
                proj_2d(axis2d, &kv2d_0.at(0).at(0));
                proj_2d(axis2d, &kv2d_0.at(1).at(0));
                kv2d_v.at(ithread).push_back(kv2d_0);
                clr2d_v.at(ithread).push_back(clr2d_0);
              }/*else if (norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]])*/
            }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
          }//a2
        }//a1
      }//a0
    }/* End of parallel region */
    /*
     Allocation of Fermi-lines
    */
    n2d[ib] = 0;
    for (ithread = 0; ithread < nthreads; ithread++)
      n2d[ib] += kv2d_v.at(ithread).size();

    *terminal << wxString::Format(wxT("    %d       %d\n"), ib + 1, n2d[ib]);
    kv2d[ib] = new GLfloat[6 * n2d[ib]];
    kv2d_fat[ib] = new GLfloat[12 * n2d[ib]];
    clr2d[ib] = new GLfloat[16 * n2d[ib]];

    n2d0 = 0;
    for (ithread = 0; ithread < nthreads; ithread++) {
      for (itri = 0; itri < kv2d_v.at(ithread).size(); itri++) {
        for (i = 0; i < 2; i++) {
          for (j = 0; j < 3; j++) {
            kv2d[ib][j + i * 3 + 6 * n2d0] = kv2d_v.at(ithread).at(itri).at(i).at(j);
          }
          for (j = 0; j < 3; j++) {
            clr2d[ib][j + i * 8 + 0 + 16 * n2d0] = clr2d_v.at(ithread).at(itri).at(i).at(j);
            clr2d[ib][j + i * 8 + 4 + 16 * n2d0] = clr2d_v.at(ithread).at(itri).at(i).at(j);
          }
        }
        n2d0 += 1;
      }
    }
  }/*for (ib = 0; ib < nb; ib++)*/
}/*void calc_nodeline()*/
