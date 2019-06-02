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
 @brief Compute lines of BZ boundary
*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "basic_math.hpp"
#include "variable.hpp"

/**
 @brief Judge wheser this line is the edge of 1st BZ
*/
static int bragg_vert(
  int ibr, //!< [in] Index of a Bragg plane
  int jbr, //!< [in] Index of a Bragg plane
  int nbr, //!< [in]
  GLfloat vert[3], //!< [in] start point of line
  GLfloat vert2[3] //!< [in] end point of line
)
{
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3], rhs[3], prod, thr, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < 26; ++kbr) {
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = bragg[ibr][i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = brnrm[ibr];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = sqrtf(rhs[0] * rhs[1] * rhs[2]) * 0.001f;
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
}/* bragg_vert */
/**
 @brief Compute real number of Bragg plane at 1st BZ
*/
static void check_bragg() {
  int ibr, ibzl, ibzc;
  int ii, kk, bzflag, nbzcorner, nn;
  GLfloat thr = (GLfloat)0.0001, prod, bzc[676][3];
  /*
  First, compute real number of corners of 1st BZ
  */
  nbzcorner = 0;
  for (ibzl = 0; ibzl < nbzl; ibzl++) {
    for (ii = 0; ii < 2; ii++) {
      bzflag = 0;
      
      for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
        prod = 0.0f;
        for (kk = 0; kk < 3; kk++) prod += (bzl[ibzl][ii][kk] - bzc[ibzc][kk]) * (bzl[ibzl][ii][kk] - bzc[ibzc][kk]);
        if (prod < thr) bzflag = 1;
      }

      if (bzflag == 0) {
        for (kk = 0; kk < 3; kk++) bzc[nbzcorner][kk] = bzl[ibzl][ii][kk];
        nbzcorner += 1;
      }

    }/*for (ii = 0; ii < 2; ii++)*/
  }/*for (ibzl = 0; ibzl < nbzl; ibzl++)*/
  printf("    Number of corners of 1st BZ : %d\n", nbzcorner);
  /**@brief
  Then, compute real number Bragg plane of 1st BZ (::nbragg), 
  Re-order ::bragg and ::brnrm
  */
  nbragg = 0;
  for (ibr = 0; ibr < 26; ibr++) {
    nn = 0;

    for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
      prod = bragg[ibr][0] * bzc[ibzc][0] + bragg[ibr][1] * bzc[ibzc][1] + bragg[ibr][2] * bzc[ibzc][2];
      if (fabsf(prod - brnrm[ibr]) < thr) nn += 1;
    }
    if (nn >= 3) {
      for (kk = 0; kk < 3; kk++) bragg[nbragg][kk] = bragg[ibr][kk];
      brnrm[nbragg] = brnrm[ibr];
      nbragg += 1;
    }
  }
  printf("    Number of plane of 1st BZ : %d\n", nbragg);
}/*static void check_bragg*/
/**
 @brief Compute Brillouin zone boundariy lines

 Modify : ::nbzl, ::bzl
*/
void bz_lines() {
  /**/
  int ibr, jbr, nbr, i, j, lvert;
  GLfloat vert[2][3];
  /**/
  nbzl = 0;
  /**/
  for (ibr = 0; ibr < 26; ++ibr) {
    for (jbr = 0; jbr < 26; ++jbr) {
      /**/
      for (i = 0; i<3; ++i) vert[1][i] = 0.0;
      nbr = 0;
      lvert = bragg_vert(ibr, jbr, nbr, vert[0], vert[1]);
      if (lvert == 0) continue;
      nbr = lvert;
      /**/
      lvert = bragg_vert(ibr, jbr, nbr, vert[1], vert[0]);
      if (lvert == 0) continue;
      /**/
      for (i = 0; i < 2; ++i) for (j = 0; j < 3; ++j) bzl[nbzl][i][j] = vert[i][j];
      nbzl = nbzl + 1;

    }/*for (jbr = 0; jbr < 26; ++jbr)*/
  }/*for (ibr = 0; ibr < 26; ++ibr)*/

  check_bragg();

}/*bz_lines*/
