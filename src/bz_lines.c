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
#include <math.h>
#include "variable.h"
#include "basic_math.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
* Judge wheser this line is the edge of 1st BZ
*/
int bragg_vert(
  int ibr /**< [in] Index of a Bragg plane*/,
  int jbr /**< [in] Index of a Bragg plane*/,
  int nbr /**< [in] */,
  GLfloat vert[3] /**< [in] start point of line*/,
  GLfloat vert2[3] /**< [in] end point of line*/)
{
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3], rhs[3], prod, thr = 0.0001, det;
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
}/* bragg_vert */
 /**
 * Compute Brillouin zone boundariy lines
 */
void bz_lines() {
  /**/
  int ibr, jbr, nbr, ibzl, i, j, lvert;
  GLfloat vert[2][3];
  /**/
  ibzl = 0;
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
      if (query == 1) {
        nbzl = nbzl + 1;
      }
      else {
        for (i = 0; i<2; ++i) for (j = 0; j<3; ++j) bzl[ibzl][i][j] = vert[i][j];
        ibzl = ibzl + 1;
      }
      /**/
    }
  }
  /**/
  if (query == 1) {
    printf("# of lines for BZ : %d \n", nbzl);
    /**/
    bzl = (GLfloat***)malloc(nbzl * sizeof(GLfloat*));
    for (ibzl = 0; ibzl < nbzl; ++ibzl) {
      bzl[ibzl] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
      for (i = 0; i < 2; ++i) {
        bzl[ibzl][i] = (GLfloat*)malloc(3 * sizeof(GLfloat));
      }
    }
    /**/
  }
  /**/
} /* bz_lines */
