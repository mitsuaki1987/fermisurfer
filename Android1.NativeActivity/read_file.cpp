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
@brief Read .frmsf file
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <GLES/gl.h>

#include "basic_math.hpp"
#include "variable.hpp"
/*
 Allocation of Kohn-Sham energies $ matrix elements
*/
static void allocate_griddata() 
{
  int i, j, ib, i0, i1, i2;

  for (i = 0; i < 3; i++) ng[i] = ng0[i];

  ntri = new int[nb];
  ntri_th = new int* [nb];
  for (ib = 0; ib < nb; ib++) ntri_th[ib] = new int[nthreads];
  nnl = new int[nb];
  n2d = new int[nb];
  nequator = new int[nb];
  draw_band = new int[nb];
  for (ib = 0; ib < nb; ib++) draw_band[ib] = 1;

  scl /= sqrtf(bvec[0][0] * bvec[0][0] + bvec[0][1] * bvec[0][1] + bvec[0][2] * bvec[0][2]);
  linewidth /= scl;
  /*
   Direct lattice vector
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) avec[i][j] = 0.0f;
    avec[i][i] = 1.0f;
    solve3(bvec, avec[i]);
  }/*for (i = 0; i < 3; ++i)*/
  for (i = 0; i < 3; ++i) {
    secvec[i] = bvec[2][i];
    eqvec[i] = bvec[2][i];
    eqvec_fr[i] = 0.0;
    secvec_fr[i] = 0.0;
  }
  eqvec_fr[2] = 1.0;
  secvec_fr[2] = 1.0;
  secscale = 0.0;

  eig0 = new GLfloat * **[nb];
  eig = new GLfloat * **[nb];
  mat0 = new GLfloat * ***[nb];
  mat = new GLfloat * ***[nb];
  vf = new GLfloat * ***[nb];
  for (ib = 0; ib < nb; ib++) {
    eig0[ib] = new GLfloat * *[ng0[0]];
    eig[ib] = new GLfloat * *[ng0[0]];
    mat0[ib] = new GLfloat * **[ng0[0]];
    mat[ib] = new GLfloat * **[ng0[0]];
    vf[ib] = new GLfloat * **[ng0[0]];
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib][i0] = new GLfloat * [ng0[1]];
      eig[ib][i0] = new GLfloat * [ng0[1]];
      mat0[ib][i0] = new GLfloat * *[ng0[1]];
      mat[ib][i0] = new GLfloat * *[ng0[1]];
      vf[ib][i0] = new GLfloat * *[ng0[1]];
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0][i1] = new GLfloat[ng0[2]];
        eig[ib][i0][i1] = new GLfloat[ng0[2]];
        mat0[ib][i0][i1] = new GLfloat * [ng0[2]];
        mat[ib][i0][i1] = new GLfloat * [ng0[2]];
        vf[ib][i0][i1] = new GLfloat * [ng0[2]];
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          mat0[ib][i0][i1][i2] = new GLfloat[3];
          mat[ib][i0][i1][i2] = new GLfloat[3];
          vf[ib][i0][i1][i2] = new GLfloat[3];
        }
      }
    }
  }
}
/**
 @brief Input from Fermi surface file
*/
int read_file()
{
  int ib, i, i0, i1, i2, ii0, ii1, ii2, ierr, iaxis;
  FILE *fp;
  char* ctemp1;
  char ctemp2[256];
  int lshift; //!< Switch for shifted Brillouin zone
  /*
   Open input file.
  */
  fp = fopen("/sdcard/Download/vfermi.frmsf", "r");
  /*
   k-point grid
  */
  ctemp1 = fgets(ctemp2, 256, fp);
  ierr = sscanf(ctemp2, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);
  /*
   Shift of k-point grid
  */
  ierr = fscanf(fp, "%d", &lshift);
  if (lshift == 0) {
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  /*
   Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
  }/*for (i = 0; i < 3; ++i)*/
  allocate_griddata();
  /*
   Kohn-Sham energies
  */
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      if (lshift != 0) ii0 = i0;
      else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        if (lshift != 0) ii1 = i1;
        else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (lshift != 0) ii2 = i2;
          else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
          ierr = fscanf(fp, "%e", &eig0[ib][ii0][ii1][ii2]);
        }
      }
    }
  }
  /*
   Matrix elements
  */
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < ng0[0]; ++i0) {
        if (lshift != 0) ii0 = i0;
        else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
        for (i1 = 0; i1 < ng0[1]; ++i1) {
          if (lshift != 0) ii1 = i1;
          else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
          for (i2 = 0; i2 < ng0[2]; ++i2) {
            if (lshift != 0) ii2 = i2;
            else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
            ierr = fscanf(fp, "%e", &mat0[ib][ii0][ii1][ii2][iaxis]);
            if (ierr == EOF) {
              fclose(fp);
              return iaxis;
            }
          }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
        }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
      }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }
  fclose(fp);
  return 3;
} /* read_file */
