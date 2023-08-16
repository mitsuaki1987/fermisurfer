/*
 Copyright (c) 2014 Mitsuaki Kawamura

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "basic_math.hpp"
#include "variable.hpp"

void write_nesting(
  char* filename, 
  double *nesting
) {
  int ii, iq;
  FILE* fp;
  fp = fopen(filename, "w");
  fprintf(fp, "%d %d %d\n", ng0[0], ng0[1], ng0[2]);
  fprintf(fp, "1\n");
  fprintf(fp,"1\n");
  for (ii = 0; ii < 3; ii++) fprintf(fp, "%15.10f %15.10f %15.10f\n", bvec[ii][0], bvec[ii][1], bvec[ii][2]);
  for (iq=0;iq<ng0[0]*ng0[1]*ng0[2];iq++) fprintf(fp,"%15.10f\n", nesting[iq]);
  fclose(fp);
}

void libtetrabz_initialize(
    double wlsm[20][4],
    int ***ikv
    )
    {
  /*
  define shortest diagonal line & define type of tetragonal
  */
  int ii, i0, i1, i2, i3, it, itype, ivvec0[4] = {}, divvec[4][4] = {}, ivvec[6][20][3] = {}, nt;
  double bvec2[3][3] = {}, bvec3[4][3] = {}, bnorm[4] = {};
  double wlsm1[4][4] = { {1440.0, 0.0, 30.0, 0.0},
                         {0.0, 1440.0, 0.0, 30.0},
                         {30.0, 0.0, 1440.0, 0.0},
                         {0.0, 30.0, 0.0, 1440.0} },
  wlsm2[4][4] = {{-38.0, 7.0, 17.0, -28.0},
                 {-28.0, -38.0, 7.0, 17.0},
                 {17.0, -28.0, -38.0, 7.0},
                 {7.0, 17.0, -28.0, -38.0}},
  wlsm3[4][4] = {{-56.0, 9.0, -46.0, 9.0},
                 {9.0, -56.0, 9.0, -46.0},
                 {-46.0, 9.0, -56.0, 9.0},
                 {9.0, -46.0, 9.0, -56.0}},
  wlsm4[4][4] = {{-38.0, -28.0, 17.0, 7.0},
                 {7.0, -38.0, -28.0, 17.0},
                 {17.0, 7.0, -38.0, -28.0},
                 {-28.0, 17.0, 7.0, -38.0}},
  wlsm5[4][4] = {{-18.0, -18.0, 12.0, -18.0},
                 {-18.0, -18.0, -18.0, 12.0},
                 {12.0, -18.0, -18.0, -18.0},
                 {-18.0, 12.0, -18.0, -18.0}};

  for (i1 = 0; i1 < 3; i1++)
    for (i2 = 0; i2 < 3; i2++)
      bvec2[i1][i2] = (double)bvec[i1][i2] / (double) ng0[i1];

  for (i1 = 0; i1 < 3; i1++) {
    bvec3[0][i1] = -bvec2[0][i1] + bvec2[1][i1] + bvec2[2][i1];
    bvec3[1][i1] = bvec2[0][i1] - bvec2[1][i1] + bvec2[2][i1];
    bvec3[2][i1] = bvec2[0][i1] + bvec2[1][i1] - bvec2[2][i1];
    bvec3[3][i1] = bvec2[0][i1] + bvec2[1][i1] + bvec2[2][i1];
  }
  /*
  length of delta bvec
  */
  for (i1 = 0; i1 < 4; i1++) {
    bnorm[i1] = 0.0;
    for (i2 = 0; i2 < 3; i2++)
      bnorm[i1] += bvec3[i1][i2] * bvec3[i1][i2];
  }
  itype = 0;
  for (i1 = 1; i1 < 4; i1++)
    if (bnorm[i1] < bnorm[itype]) itype = i1;
  /*
  start & last
  */
  for (i0 = 0; i0 < 4; i0++) {
    ivvec0[i0] = 0;
    for (i1 = 0; i1 < 4; i1++)divvec[i0][i1] = 0;
    divvec[i0][i0] = 1;
  }
  ivvec0[itype] = 1;
  divvec[itype][itype] = -1;
  /*
  Corners of tetrahedron
  */
  it = 0;
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      if (i1 == i0) continue;
      for (i2 = 0; i2 < 3; i2++) {
        if (i2 == i1 || i2 == i0) continue;

        for (i3 = 0; i3 < 3; i3++) {
          ivvec[it][0][i3] = ivvec0[i3];
          ivvec[it][1][i3] = ivvec[it][0][i3] + divvec[i0][i3];
          ivvec[it][2][i3] = ivvec[it][1][i3] + divvec[i1][i3];
          ivvec[it][3][i3] = ivvec[it][2][i3] + divvec[i2][i3];
        }

        it += 1;
      }
    }
  }
  /*
  Additional points
  */
  for (i1 = 0; i1 < 6; i1++) {
    for (i2 = 0; i2 < 3; i2++) {
      ivvec[i1][4][i2] = 2 * ivvec[i1][0][i2] - ivvec[i1][1][i2];
      ivvec[i1][5][i2] = 2 * ivvec[i1][1][i2] - ivvec[i1][2][i2];
      ivvec[i1][6][i2] = 2 * ivvec[i1][2][i2] - ivvec[i1][3][i2];
      ivvec[i1][7][i2] = 2 * ivvec[i1][3][i2] - ivvec[i1][0][i2];

      ivvec[i1][8][i2] = 2 * ivvec[i1][0][i2] - ivvec[i1][2][i2];
      ivvec[i1][9][i2] = 2 * ivvec[i1][1][i2] - ivvec[i1][3][i2];
      ivvec[i1][10][i2] = 2 * ivvec[i1][2][i2] - ivvec[i1][0][i2];
      ivvec[i1][11][i2] = 2 * ivvec[i1][3][i2] - ivvec[i1][1][i2];

      ivvec[i1][12][i2] = 2 * ivvec[i1][0][i2] - ivvec[i1][3][i2];
      ivvec[i1][13][i2] = 2 * ivvec[i1][1][i2] - ivvec[i1][0][i2];
      ivvec[i1][14][i2] = 2 * ivvec[i1][2][i2] - ivvec[i1][1][i2];
      ivvec[i1][15][i2] = 2 * ivvec[i1][3][i2] - ivvec[i1][2][i2];

      ivvec[i1][16][i2] = ivvec[i1][3][i2] - ivvec[i1][0][i2] + ivvec[i1][1][i2];
      ivvec[i1][17][i2] = ivvec[i1][0][i2] - ivvec[i1][1][i2] + ivvec[i1][2][i2];
      ivvec[i1][18][i2] = ivvec[i1][1][i2] - ivvec[i1][2][i2] + ivvec[i1][3][i2];
      ivvec[i1][19][i2] = ivvec[i1][2][i2] - ivvec[i1][3][i2] + ivvec[i1][0][i2];
    }
  }

  for (i1 = 0; i1 < 4; i1++) {
    for (i2 = 0; i2 < 4; i2++) {
      wlsm[i2][i1] = wlsm1[i1][i2] /= 1260.0;
      wlsm[i2 + 4][i1] = wlsm2[i1][i2] /= 1260.0;
      wlsm[i2 + 8][i1] = wlsm3[i1][i2] /= 1260.0;
      wlsm[i2 + 12][i1] = wlsm4[i1][i2] /= 1260.0;
      wlsm[i2 + 16][i1] = wlsm5[i1][i2] /= 1260.0;
    }
  }
  /*
  k-index for energy
  */
  nt = 0;
  for (i2 = 0; i2 < ng0[2]; i2++) {
    for (i1 = 0; i1 < ng0[1]; i1++) {
      for (i0 = 0; i0 < ng0[0]; i0++) {

        for (it = 0; it < 6; it++) {

          for (ii = 0; ii < 20; ii++) {
            ikv[nt][ii][0] = modulo(i0 + ivvec[it][ii][0], ng0[0]);
            ikv[nt][ii][1] = modulo(i1 + ivvec[it][ii][1], ng0[1]);
            ikv[nt][ii][2] = modulo(i2 + ivvec[it][ii][2], ng0[2]);
          }
          nt += 1;
        }
      }
    }
  }
}
/*
Cut small tetrahedron A1
*/
void libtetrabz_tsmall_a1(
    double *e,
    double e0,
    double *v,
    double tsmall[4][4]
) {
  double a10, a20, a30;
  a10 = (e0 - e[0]) / (e[1] - e[0]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);
  a30 = (e0 - e[0]) / (e[3] - e[0]);

  *v = a10 * a20 * a30;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 1.0 - a10;
  tsmall[0][2] = 1.0 - a20;
  tsmall[0][3] = 1.0 - a30;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = a10;
  tsmall[1][2] = 0.0;
  tsmall[1][3] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = a20;
  tsmall[2][3] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = 0.0;
  tsmall[3][3] = a30;
}
/*
Cut small tetrahedron B1
*/
void libtetrabz_tsmall_b1(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a13, a20, a30;
  a13 = (e0 - e[3]) / (e[1] - e[3]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);
  a30 = (e0 - e[0]) / (e[3] - e[0]);

  *v = a20 * a30 * a13;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 1.0 - a20;
  tsmall[0][2] = 1.0 - a30;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 0.0;
  tsmall[1][2] = 0.0;
  tsmall[1][3] = a13;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = a20;
  tsmall[2][2] = 0.0;
  tsmall[2][3] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = a30;
  tsmall[3][3] = 1.0 - a13;
}
/*
Cut small tetrahedron B2
*/
void libtetrabz_tsmall_b2(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a21, a31;
  a21 = (e0 - e[1]) / (e[2] - e[1]);
  a31 = (e0 - e[1]) / (e[3] - e[1]);

  *v = a21 * a31;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 0.0;
  tsmall[0][2] = 0.0;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 1.0;
  tsmall[1][2] = 1.0 - a21;
  tsmall[1][3] = 1.0 - a31;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = a21;
  tsmall[2][3] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = 0.0;
  tsmall[3][3] = a31;
}
/*
Cut small tetrahedron B3
*/
void libtetrabz_tsmall_b3(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a12, a20, a31;
  a12 = (e0 - e[2]) / (e[1] - e[2]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);
  a31 = (e0 - e[1]) / (e[3] - e[1]);

  *v = a12 * a20 * a31;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 1.0 - a20;
  tsmall[0][2] = 0.0;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 0.0;
  tsmall[1][2] = a12;
  tsmall[1][3] = 1.0 - a31;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = a20;
  tsmall[2][2] = 1.0 - a12;
  tsmall[2][3] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = 0.0;
  tsmall[3][3] = a31;
}
/*
Cut small tetrahedron C1
*/
void libtetrabz_tsmall_c1(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a32;
  a32 = (e0 - e[2]) / (e[3] - e[2]);

  *v = a32;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 0.0;
  tsmall[0][2] = 0.0;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 1.0;
  tsmall[1][2] = 0.0;
  tsmall[1][3] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = 1.0;
  tsmall[2][3] = 1.0 - a32;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = 0.0;
  tsmall[3][3] = a32;
}
/*
Cut small tetrahedron C2
*/
void libtetrabz_tsmall_c2(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a23, a31;
  a23 = (e0 - e[3]) / (e[2] - e[3]);
  a31 = (e0 - e[1]) / (e[3] - e[1]);

  *v = a23 * a31;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 0.0;
  tsmall[0][2] = 0.0;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 1.0;
  tsmall[1][2] = 1.0 - a31;
  tsmall[1][3] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = 0.0;
  tsmall[2][3] = a23;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = a31;
  tsmall[3][3] = 1.0 - a23;
}
/*
Cut small tetrahedron C3
*/
void libtetrabz_tsmall_c3(
  double *e,
  double e0,
  double *v,
  double tsmall[4][4]
)
{
  double a23, a13, a30;
  a23 = (e0 - e[3]) / (e[2] - e[3]);
  a13 = (e0 - e[3]) / (e[1] - e[3]);
  a30 = (e0 - e[0]) / (e[3] - e[0]);

  *v = a23 * a13 * a30;

  tsmall[0][0] = 1.0;
  tsmall[0][1] = 1.0 - a30;
  tsmall[0][2] = 0.0;
  tsmall[0][3] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 0.0;
  tsmall[1][2] = a13;
  tsmall[1][3] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = 0.0;
  tsmall[2][3] = a23;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = a30;
  tsmall[3][2] = 1.0 - a13;
  tsmall[3][3] = 1.0 - a23;
}
/*
Cut triangle A1
*/
void libtetrabz_triangle_a1(
  double *e,
  double e0,
  double *v,
  double tsmall[4][3]
)
{
  double a10, a20, a30;
  a10 = (e0 - e[0]) / (e[1] - e[0]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);
  a30 = (e0 - e[0]) / (e[3] - e[0]);

  *v = 3.0 * a10 * a20 / (e[3] - e[0]);

  tsmall[0][0] = 1.0 - a10;
  tsmall[0][1] = 1.0 - a20;
  tsmall[0][2] = 1.0 - a30;
  tsmall[1][0] = a10;
  tsmall[1][1] = 0.0;
  tsmall[1][2] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = a20;
  tsmall[2][2] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2] = a30;
}
/*
Cut triangle B1
*/
void libtetrabz_triangle_b1(
  double *e,
  double e0,
  double *v,
  double tsmall[4][3]
) {
  double a30, a13, a20;
  a30 = (e0 - e[0]) / (e[3] - e[0]);
  a13 = (e0 - e[3]) / (e[1] - e[3]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);

  *v = 3.0 * a30 * a13 / (e[2] - e[0]);

  tsmall[0][0] = 1.0 - a20;
  tsmall[0][1] = 1.0 - a30;
  tsmall[0][2] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = 0.0;
  tsmall[1][2] = a13;
  tsmall[2][0] = a20;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = a30;
  tsmall[3][2] = 1.0 - a13;
}
/*
Cut triangle B2
*/
void libtetrabz_triangle_b2(
    double *e,
    double e0,
    double *v,
    double tsmall[4][3]
)
{
  double a12, a31, a20;
  a12 = (e0 - e[2]) / (e[1] - e[2]);
  a31 = (e0 - e[1]) / (e[3] - e[1]);
  a20 = (e0 - e[0]) / (e[2] - e[0]);

  *v = 3.0 * a12 * a31 / (e[2] - e[0]);

  tsmall[0][0] = 1.0 - a20;
  tsmall[0][1] = 0.0;
  tsmall[0][2] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = a12;
  tsmall[1][2] = 1.0 - a31;
  tsmall[2][0] = a20;
  tsmall[2][1] = 1.0 - a12;
  tsmall[2][2] = 0.0;
  tsmall[3][0] = 0.0;
  tsmall[3][1] = 0.0;
  tsmall[3][2]= a31;
}
/*
Cut triangle C1
*/
void libtetrabz_triangle_c1(
  double *e,
  double e0,
  double *v,
  double tsmall[4][3]
)
{
  double a03, a13, a23;
  a03 = (e0 - e[3]) / (e[0] - e[3]);
  a13 = (e0 - e[3]) / (e[1] - e[3]);
  a23 = (e0 - e[3]) / (e[2] - e[3]);

  *v = 3.0 * a03 * a13 / (e[3] - e[2]);

  tsmall[0][0] = a03;
  tsmall[0][1] = 0.0;
  tsmall[0][2] = 0.0;
  tsmall[1][0] = 0.0;
  tsmall[1][1] = a13;
  tsmall[1][2] = 0.0;
  tsmall[2][0] = 0.0;
  tsmall[2][1] = 0.0;
  tsmall[2][2] = a23;
  tsmall[3][0] = 1.0 - a03;
  tsmall[3][1] = 1.0 - a13;
  tsmall[3][2] = 1.0 - a23;
}
/*
Sort eigenvalues
*/
void eig_sort(
    int n, //!< [in] the number of components
    double *key, //!< [in] Variables to be sorted [n].
    int *swap //!< [out] Order of index (sorted)
)
{
  int i, j, k, min_loc;
  double min_val;

  for (i = 0; i < n; ++i) swap[i] = i;

  for (i = 0; i < n - 1; ++i) {
    min_val = key[i];
    min_loc = i;
    for (j = i + 1; j < n; ++j) {
      if (min_val > key[j]) {
        min_val = key[j];
        min_loc = j;
      }
    }
    if (key[i] > min_val) {
      /*
       Swap
      */
      key[min_loc] = key[i];
      key[i] = min_val;

      k = swap[min_loc];
      swap[min_loc] = swap[i];
      swap[i] = k;
    }
  }/*for (i = 0; i < n - 1; ++i)*/
}/*eig_sort*/
/*
2nd step of tetrahedron method.
*/
static double libtetrabz_dbldelta2(
    double **ej
) {
  int i3, ib, indx[3];
  double a10, a12, e[3] = {}, e_abs, nesting;

  nesting = 0.0;

  for (ib = 0; ib < nb; ib++) {

    e_abs = 0.0;
    for (i3 = 0; i3 < 3; i3++) {
      e[i3] = ej[ib][i3];
      if (e_abs < fabs(e[i3])) e_abs = fabs(e[i3]);
    }
    eig_sort(3, e, indx);

    if (e_abs < 1.0e-10) {
      printf("Nesting ##\n");
    }

    if ((e[0] < 0.0 && 0.0 <= e[1]) || (e[0] <= 0.0 && 0.0 < e[1])) {

      a10 = (0.0 - e[0]) / (e[1] - e[0]);

      nesting += 2.0 * a10 / (e[2] - e[0]);
    }
    else if ((e[1] <= 0.0 && 0.0 < e[2]) || (e[1] < 0.0 && 0.0 <= e[2])) {

      a12 = (0.0 - e[2]) / (e[1] - e[2]);

      nesting += 2.0 * a12 / (e[2] - e[0]);
    }
  }
  return nesting;
}
/*
Main SUBROUTINE for Delta(E1) * Delta(E2)
*/
void nesting_dbldelta()
{
  int it, i20, *** ikv, ** iqv, nk, iq, i0, i1, i2;
  double wlsm[20][4], * nesting, thr;

  nk = ng0[0] * ng0[1] * ng0[2];
  iqv = (int**)malloc(nk * sizeof(int*));
  iqv[0] = (int*)malloc(3 * nk * sizeof(int));
  for (iq = 0; iq < nk; iq++) iqv[iq] = iqv[0] + iq * 3;
  iq = 0;
  for (i0 = 0; i0 < ng0[0]; i0++) {
    for (i1 = 0; i1 < ng0[1]; i1++) {
      for (i2 = 0; i2 < ng0[2]; i2++) {
        iqv[iq][0] = i0;
        iqv[iq][1] = i1;
        iqv[iq][2] = i2;
        iq += 1;
      }
    }
  }

  thr = 1.0e-10;

  ikv = (int***)malloc(6 * nk * sizeof(int**));
  ikv[0] = (int**)malloc(6 * nk * 20 * sizeof(int*));
  ikv[0][0] = (int*)malloc(6 * nk * 20 * 3 * sizeof(int));
  for (it = 0; it < 6 * nk; it++) {
    ikv[it] = ikv[0] + it * 20;
    for (i20 = 0; i20 < 20; i20++) {
      ikv[it][i20] = ikv[0][0] + it * 20 * 3 + i20 * 3;
    }
  }

  nesting = (double*)malloc(nk * sizeof(double));

  libtetrabz_initialize(wlsm, ikv);

#pragma omp parallel default(none) \
private(iq,it,i20) \
shared(nb,nk,nesting,iqv,EF,eig0,ng0,thr,wlsm,ikv,nthreads)
  {
    int ib, ikqv[3] = {}, i4, jb, j3, indx[4], nth, myth;
    double** ei1, ** ej1, ** ej2, e[4] = {}, v, tsmall[4][3];

    ei1 = (double**)malloc(4 * sizeof(double*));
    ej1 = (double**)malloc(4 * sizeof(double*));
    ei1[0] = (double*)malloc(4 * nb * sizeof(double));
    ej1[0] = (double*)malloc(4 * nb * sizeof(double));
    for (i4 = 0; i4 < 4; i4++) {
      ei1[i4] = ei1[0] + i4 * nb;
      ej1[i4] = ej1[0] + i4 * nb;
    }

    ej2 = (double**)malloc(nb * sizeof(double*));
    ej2[0] = (double*)malloc(nb * 3 * sizeof(double));
    for (ib = 0; ib < nb; ib++) {
      ej2[ib] = ej2[0] + ib * 3;
    }

#pragma omp for
    for (iq = 0; iq < nk; iq++) {

      if(iq < nk / nthreads) {
        if (iq % (nk / (10*nthreads)) == 0) printf(" %d/10\n", iq / (nk / (10*nthreads)));
      }

      nesting[iq] = 0.0;
      if (iqv[iq][0] == 0 && iqv[iq][1] == 0 && iqv[iq][2] == 0) continue;
      for (it = 0; it < 6 * nk; it++) {

        for (i4 = 0; i4 < 4; i4++) {
          for (ib = 0; ib < nb; ib++) {
            ei1[i4][ib] = 0.0;
            ej1[i4][ib] = 0.0;
          }
        }
        for (i20 = 0; i20 < 20; i20++) {

          for (j3 = 0; j3 < 3; j3++) {
            ikqv[j3] = modulo(ikv[it][i20][j3] + iqv[iq][j3], ng0[j3]);
          }//j3
          for (i4 = 0; i4 < 4; i4++) {
            for (ib = 0; ib < nb; ib++) {
              ei1[i4][ib] += (double)(eig0[ib][ikv[it][i20][0]][ikv[it][i20][1]][ikv[it][i20][2]] * wlsm[i20][i4] - EF);
              ej1[i4][ib] += (double)(eig0[ib][        ikqv[0]][        ikqv[1]][        ikqv[2]] * wlsm[i20][i4] - EF);
            }//ib
          }//i4
        }//i20

        for (ib = 0; ib < nb; ib++) {

          for (i4 = 0; i4 < 4; i4++) e[i4] = ei1[i4][ib];
          eig_sort(4, e, indx);

          if (e[0] < 0.0 && 0.0 <= e[1]) {

            libtetrabz_triangle_a1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (jb = 0; jb < nb; jb++)
                for (j3 = 0; j3 < 3; j3++) ej2[jb][j3] = 0.0;
              for (i4 = 0; i4 < 4; i4++)
                for (jb = 0; jb < nb; jb++)
                  for (j3 = 0; j3 < 3; j3++)
                    ej2[jb][j3] += ej1[indx[i4]][jb] * tsmall[i4][j3];
              nesting[iq] += v * libtetrabz_dbldelta2(ej2);
            }
          }
          else if (e[1] < 0.0 && 0.0 <= e[2]) {

            libtetrabz_triangle_b1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (jb = 0; jb < nb; jb++)
                for (j3 = 0; j3 < 3; j3++) ej2[jb][j3] = 0.0;
              for (i4 = 0; i4 < 4; i4++)
                for (jb = 0; jb < nb; jb++)
                  for (j3 = 0; j3 < 3; j3++)
                    ej2[jb][j3] += ej1[indx[i4]][jb] * tsmall[i4][j3];
              nesting[iq] += v * libtetrabz_dbldelta2(ej2);
            }

            libtetrabz_triangle_b2(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (jb = 0; jb < nb; jb++)
                for (j3 = 0; j3 < 3; j3++) ej2[jb][j3] = 0.0;
              for (i4 = 0; i4 < 4; i4++)
                for (jb = 0; jb < nb; jb++)
                  for (j3 = 0; j3 < 3; j3++)
                    ej2[jb][j3] += ej1[indx[i4]][jb] * tsmall[i4][j3];
              nesting[iq] += v * libtetrabz_dbldelta2(ej2);
            }
          }
          else if (e[2] < 0.0 && 0.0 < e[3]) {

            libtetrabz_triangle_c1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (jb = 0; jb < nb; jb++)
                for (j3 = 0; j3 < 3; j3++) ej2[jb][j3] = 0.0;
              for (i4 = 0; i4 < 4; i4++)
                for (jb = 0; jb < nb; jb++)
                  for (j3 = 0; j3 < 3; j3++)
                    ej2[jb][j3] += ej1[indx[i4]][jb] * tsmall[i4][j3];
              nesting[iq] += v * libtetrabz_dbldelta2(ej2);
            }
          }
        }//nb
      }//it
      nesting[iq] /= (double)(nk * 6);
    }//iq

    free(ei1[0]);
    free(ei1);
    free(ej1[0]);
    free(ej1);
    free(ej2[0]);
    free(ej2);
  }//pragma omp

  write_nesting("doubledelta.frmsf", nesting);

  free(ikv[0]);
  free(ikv);
  free(nesting);
}
/*
 Results of Integration (1-x-y-z)/(g0+(g1-g0)x+(g2-g0)y+(g3-g0))
 for 0<x<1, 0<y<1-x, 0<z<1-x-y
*/

static double nesting_1234(
  double g1,
  double g2,
  double g3,
  double g4,
  double lng1,
  double lng2,
  double lng3,
  double lng4
)
{
  double w;
  double w2, w3, w4;
  w2 = (g2 * (lng2 - lng1)) / (g2 - g1);
  w3 = (g3 * (lng3 - lng1)) / (g3 - g1);
  w4 = (g4 * (lng4 - lng1)) / (g4 - g1);
  w2 = (g2 * (w2 - w3)) / (g2 - g3);
  w4 = (g4 * (w4 - w3)) / (g4 - g3);
  w = (3.0 * (w4 - w2)) / (g4 - g2);
  return w;
}//static double nesting_1234

static double nesting_1134(
  double g1,
  double g3,
  double g4,
  double lng1,
  double lng3,
  double lng4
)
{
  double w;
  double w3, w4;
  w3 = (g3 * (lng3 - lng1)) / (g3 - g1);
  w4 = (g4 * (lng4 - lng1)) / (g4 - g1);
  w3 = (g3 * (w3 - 1.0)) / (g3 - g1);
  w4 = (g4 * (w4 - 1.0)) / (g4 - g1);
  w = (3.0 * (w4 - w3)) / (g4 - g3);
  return w;
}//static double nesting_1134

static double nesting_1114(
  double g1,
  double g4,
  double lng1,
  double lng4
)
{
  double w;
  w = (g4 * (lng4 - lng1)) / (g4 - g1);
  w = (2.0 * g4 * (w - 1.0)) / (g4 - g1);
  w = (3.0 * (w - 1.0)) / (2.0 * (g4 - g1));
  return w;
}//static double nesting_1114

static double nesting_1144(
  double g1,
  double g4,
  double lng1,
  double lng4
)
{
  double w;
  w = (g1 * (lng4 - lng1)) / (g4 - g1);
  w = (2.0 * g4 * (1.0 - w)) / (g4 - g1);
  w = (3.0 * (w - 1.0)) / (g4 - g1);
  return w;
}//static double nesting_1144
/*
Tetrahedron method for delta(om - ep + e)
*/
static double libtetrabz_polstat3(
    double de[4]
)
{
  int i4, indx[4];
  double thr, thr2, e[4] = {}, ln[4] = {}, nesting;

  for (i4 = 0; i4 < 4; i4++) e[i4] = de[i4];
  eig_sort(4, e, indx);

  thr = e[3] * 1.0e-3;
  thr2 = 1.0e-8;

  for(i4 =0; i4 <4; i4++){
    if (e[i4] < thr2) {
      if (i4 == 3) {
        printf("  Nesting # \n");
      }
      ln[i4] = 0.0;
      e[i4] = 0.0;
    }
    else{
      ln[i4] = log(e[i4]);
    }
  }

  if (fabs(e[3] - e[2]) < thr) {
    if (fabs(e[3] - e[1]) < thr) {
      if (fabs(e[3] - e[0]) < thr) {
        /*
        e[3] = e[2] = e[1] = e[0]
        */
        nesting = 1.0 / e[3];
      }
      else {
        /*
        e[3] = e[2] = e[1]
        */
        nesting = nesting_1114(e[3],e[0],ln[3],ln[0]);
        if (nesting < 0.0) {
          printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
          printf("%f\n", nesting);
          printf("weighting 4=3=2\n");
        }
      }
    }
    else if (fabs(e[1] - e[0]) < thr) {
      /*
      e[3] = e[2], e[1] = e[0]
      */
      nesting = nesting_1144(e[1], e[3], ln[1], ln[3]);
      if (nesting < 0.0) {
        printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
        printf("%f\n", nesting);
        printf("weighting 4=3 2=1\n");
      }
    }
    else {
      /*
      e[3] = e[2]
      */
      nesting = nesting_1134(e[3], e[0], e[1], ln[3], ln[0], ln[1]);
      if (nesting < 0.0) {
        printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
        printf("%f\n", nesting);
        printf("weighting 4=3\n");
      }
    }
  }  
  else if (fabs(e[2] - e[1]) < thr) {
    if (fabs(e[2] - e[0]) < thr) {
      /*
      e[2] = e[1] = e[0]
      */
      nesting = nesting_1114(e[2], e[3], ln[2], ln[3]);
      if (nesting < 0.0) {
        printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
        printf("%f\n", nesting);
        printf("weighting 3=2=1\n");
      }
    }
    else {
      /*
      e[2] = e[1]
      */
      nesting = nesting_1134(e[2], e[0], e[3], ln[2], ln[0], ln[3]);
      if (nesting < 0.0) {
        printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
        printf("%f\n", nesting);
        printf("weighting 3=2\n");
      }
    }
  }  
  else if (fabs(e[1] - e[0]) < thr) {
    /*
    e[1] = e[0]
    */
    nesting = nesting_1134(e[1], e[2], e[3], ln[1], ln[2], ln[3]);
    if (nesting < 0.0) {
      printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
      printf("%f\n", nesting);
      printf("weighting 2=1\n");
    }
  }  
  else {
    /*
    Different each other.
    */
    nesting = nesting_1234(e[0], e[1], e[2], e[3], ln[0], ln[1], ln[2], ln[3]);
    if (nesting < 0.0) {
      printf("%f %f %f %f\n", e[0], e[1], e[2], e[3]);
      printf("%f\n", nesting);
      printf("weighting\n");
    }
  }
  return nesting;
}
/*
Tetrahedron method for theta( - E2)
*/
static double libtetrabz_polstat2(
    double *ei1,
    double **ej1
) {
  int i4, j4, ib, indx[4];
  double v, thr, e[4] = {}, de[4] = {}, tsmall[4][4], nesting;

  thr = 1.0e-8;
  nesting = 0.0;

  for (ib = 0; ib < nb; ib++) {

    for (i4 = 0; i4 < 4; i4++) e[i4] = -ej1[ib][i4];
    eig_sort(4, e, indx);

    if ((e[0] <= 0.0 && 0.0 < e[1]) || (e[0] < 0.0 && 0.0 <= e[1])) {

      libtetrabz_tsmall_a1(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }
    }
    else if ((e[1] <= 0.0 && 0.0 < e[2]) || (e[1] < 0.0 && 0.0 <= e[2])) {

      libtetrabz_tsmall_b1(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }

      libtetrabz_tsmall_b2(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }

      libtetrabz_tsmall_b3(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }
    }
    else if ((e[2] <= 0.0 && 0.0 < e[3]) || (e[2] < 0.0 && 0.0 <= e[3])) {

      libtetrabz_tsmall_c1(e, 0.0, &v, tsmall);
      
      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }

      libtetrabz_tsmall_c2(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }

      libtetrabz_tsmall_c3(e, 0.0, &v, tsmall);

      if (v > thr) {
        for (j4 = 0; j4 < 4; j4++) de[j4] = 0.0;
        for (i4 = 0; i4 < 4; i4++)
          for (j4 = 0; j4 < 4; j4++)
            de[j4] += (ej1[ib][indx[i4]] - ei1[indx[i4]]) * tsmall[i4][j4];
        nesting += v * libtetrabz_polstat3(de);
      }
    }
    else if (e[3] <= 0.0) {
      for (i4 = 0; i4 < 4; i4++)
        de[i4] = ej1[ib][i4] - ei1[i4];
      nesting += libtetrabz_polstat3(de);
    }
  }//ib
  return nesting;
}
/*
Compute Static polarization function
*/
void nesting_lindhard()
{
  int it, i20, *** ikv, ** iqv, nk, iq, i0, i1, i2;
  double wlsm[20][4], thr, *nesting, dos;

  nk = ng0[0] * ng0[1] * ng0[2];

  iqv = (int**)malloc(nk * sizeof(int*));
  iqv[0] = (int*)malloc(3 * nk * sizeof(int));
  for (iq = 0; iq < nk; iq++) iqv[iq] = iqv[0] + iq * 3;
  iq = 0;
  for (i0 = 0; i0 < ng0[0]; i0++) {
    for (i1 = 0; i1 < ng0[1]; i1++) {
      for (i2 = 0; i2 < ng0[2]; i2++) {
        iqv[iq][0] = i0;
        iqv[iq][1] = i1;
        iqv[iq][2] = i2;
        iq += 1;
      }
    }
  }
  
  thr = 1.0e-10;

  ikv = (int***)malloc(6 * nk * sizeof(int**));
  ikv[0] = (int**)malloc(6 * nk * 20 * sizeof(int*));
  ikv[0][0] = (int*)malloc(6 * nk * 20 * 3 * sizeof(int));
  for (it = 0; it < 6 * nk; it++) {
    ikv[it] = ikv[0] + it * 20;
    for (i20 = 0; i20 < 20; i20++) {
      ikv[it][i20] = ikv[0][0] + it * 20 * 3 + i20 * 3;
    }
  }

  nesting = (double*)malloc(nk * sizeof(double));

  libtetrabz_initialize(wlsm, ikv);

#pragma omp parallel default(none) \
private(it,i20) \
shared(eig0,ng0,nk,nb,ikv,iqv,wlsm,thr,nesting,EF,nthreads) \
reduction(+: dos)
  {
    int indx[4], ikqv[3] = {}, i4, j4, ib, jb;
    double** ei1, ** ej1, ei2[4], ** ej2, e[4], v, tsmall[4][4];

    ei1 = (double**)malloc(4 * sizeof(double*));
    ej1 = (double**)malloc(4 * sizeof(double*));
    ei1[0] = (double*)malloc(4 * nb * sizeof(double));
    ej1[0] = (double*)malloc(4 * nb * sizeof(double));
    for (i4 = 0; i4 < 4; i4++) {
      ei1[i4] = ei1[0] + i4 * nb;
      ej1[i4] = ej1[0] + i4 * nb;
    }

    ej2 = (double**)malloc(nb * sizeof(double*));
    ej2[0] = (double*)malloc(nb * 4 * sizeof(double));
    for (ib = 0; ib < nb; ib++)
      ej2[ib] = ej2[0] + ib * 4;

#pragma omp for
    for (iq = 0; iq < nk; iq++) {

      if (iq < nk / nthreads) {
        if (iq % (nk / (10 * nthreads)) == 0) printf(" %d/10\n", iq / (nk / (10 * nthreads)));
      }

      nesting[iq] = 0.0;
      for (it = 0; it < 6 * nk; it++) {

        for (i4 = 0; i4 < 4; i4++)
          for (ib = 0; ib < nb; ib++) {
            ei1[i4][ib] = 0.0;
            ej1[i4][ib] = 0.0;
          }
        for (i20 = 0; i20 < 20; i20++) {

          for (i4 = 0; i4 < 3; i4++) {
            ikqv[i4] = modulo(ikv[it][i20][i4] + iqv[iq][i4], ng0[i4]);
          }//i0

          for (i4 = 0; i4 < 4; i4++) {
            for (ib = 0; ib < nb; ib++) {
              ei1[i4][ib] += (double)(eig0[ib][ikv[it][i20][0]][ikv[it][i20][1]][ikv[it][i20][2]] * wlsm[i20][i4] - EF);
              ej1[i4][ib] += (double)(eig0[ib][ikqv[0]][ikqv[1]][ikqv[2]] * wlsm[i20][i4] - EF);
            }//ib
          }//i4
        }//i20

        for (ib = 0; ib < nb; ib++) {

          for (i4 = 0; i4 < 4; i4++) e[i4] = ei1[i4][ib];
          eig_sort(4, e, indx);

          if (e[0] <= 0.0 && 0.0 < e[1]) {

            libtetrabz_tsmall_a1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }
          }
          else if (e[1] <= 0.0 && 0.0 < e[2]) {

            libtetrabz_tsmall_b1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }

            libtetrabz_tsmall_b2(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }

            libtetrabz_tsmall_b3(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }
          }
          else if (e[2] <= 0.0 && 0.0 < e[3]) {

            libtetrabz_tsmall_c1(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }

            libtetrabz_tsmall_c2(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }

            libtetrabz_tsmall_c3(e, 0.0, &v, tsmall);

            if (v > thr) {
              for (j4 = 0; j4 < 4; j4++) {
                ei2[j4] = 0.0;
                for (jb = 0; jb < nb; jb++) ej2[jb][j4] = 0.0;
              }
              for (i4 = 0; i4 < 4; i4++)
                for (j4 = 0; j4 < 4; j4++) {
                  ei2[j4] += ei1[indx[i4]][ib] * tsmall[i4][j4];
                  for (jb = 0; jb < nb; jb++)
                    ej2[jb][j4] += ej1[indx[i4]][jb] * tsmall[i4][j4];
                }
              nesting[iq] += v * libtetrabz_polstat2(ei2, ej2);
            }
          }
          else if (e[3] <= 0.0) {
            for (i4 = 0; i4 < 4; i4++) {
              ei2[i4] = ei1[i4][ib];
              for (jb = 0; jb < nb; jb++)
                ej2[jb][i4] = ej1[i4][jb];
            }
            nesting[iq] += libtetrabz_polstat2(ei2, ej2);
          }
          else {
            continue;
          }
        }
      }//it
      nesting[iq] *= 2.0 / (double)(nk * 6);
    }//iq
    //
    // dos for the drude term
    //
    dos = 0.0;
    for (it = 0; it < 6 * nk; it++) {

      for (i4 = 0; i4 < 4; i4++)
        for (ib = 0; ib < nb; ib++) {
          ei1[i4][ib] = 0.0;
        }
      for (i20 = 0; i20 < 20; i20++) {

        for (i4 = 0; i4 < 4; i4++) {
          for (ib = 0; ib < nb; ib++) {
            ei1[i4][ib] += (double)(eig0[ib][ikv[it][i20][0]][ikv[it][i20][1]][ikv[it][i20][2]] * wlsm[i20][i4] - EF);
          }//ib
        }//i4
      }//i20

      for (ib = 0; ib < nb; ib++) {

        for (i4 = 0; i4 < 4; i4++) e[i4] = ei1[i4][ib];
        eig_sort(4, e, indx);

        if (e[0] <= 0.0 && 0.0 < e[1]) {
          dos += 3.0 * (0.0 - e[0]) * (0.0 - e[0])
            / ((e[1] - e[0]) * (e[2] - e[0]) * (e[3] - e[0]));
        }
        else if (e[1] <= 0.0 && 0.0 < e[2]) {
          dos += (3.0 * (e[1] - e[0])
            + 6.0 * (0.0 - e[1])
            - 3.0 * (e[2] - e[0] + e[3] - e[1]) / (e[2] - e[1]) / (e[3] - e[1]) * (0.0 - e[1]) * (0.0 - e[1]))
            / (e[2] - e[0]) / (e[3] - e[0]);
        }
        else if (e[2] <= 0.0 && 0.0 < e[3]) {
          dos += (3.0 * (e[3] - 0.0) * (e[3] - 0.0) / (e[3] - e[0]) / (e[3] - e[1]) / (e[3] - e[2]));
        }
      }//ib
    }//it

    free(ei1[0]);
    free(ei1);
    free(ej1[0]);
    free(ej1);
    free(ej2[0]);
    free(ej2);
  }//omp
  dos /= (double)(6 * nk);
  nesting[0] += dos;

  write_nesting("lindhard.frmsf", nesting);

  free(ikv[0]);
  free(ikv);
  free(nesting);
}
