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

/**
* Input from Fermi surface file
*/
void read_file(char *fname/**<[in] fname  Input file name*/)
{
  int ib, i, i0, i1, i2, ii0, ii1, ii2, ierr;
  FILE *fp;
  /*
  Open input file.
  */
  if ((fp = fopen(fname, "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    getchar();
    exit(EXIT_FAILURE);
  }
  printf("\n#####  Brillouin zone informations  ##### \n\n");
  /*
  k-point grid
  */
  ierr = fscanf(fp, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);
  if (ierr == 0) printf("error ! reading ng");
  printf("k point grid : %d %d %d \n", ng0[0], ng0[1], ng0[2]);
  for (i = 0; i < 3; i++) ng[i] = ng0[i];
  /*
  Shift of k-point grid
  */
  ierr = fscanf(fp, "%d", &lshift);
  if (ierr == 0) printf("error ! reading lshift");

  if (lshift == 0) {
    printf("k point grid is the Monkhorst-Pack grid. \n");
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    printf("k point grid starts from Gamma. \n");
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    printf("k point grid starts from Gamma + a half grid. \n");
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   * # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  if (ierr == 0) printf("error ! reading nb");
  printf("# of bands : %d \n", nb);
  ntri = (int*)malloc(nb * sizeof(int));
  ntri_th = (int**)malloc(nb * sizeof(int*));
  for (ib = 0; ib < nb; ib++) ntri_th[ib] = (int*)malloc(nthreads * sizeof(int));
  nnl = (int*)malloc(nb * sizeof(int));
  draw_band = (int*)malloc(nb * sizeof(int));
  for (ib = 0; ib < nb; ib++) draw_band[ib] = 1;
  /*
  Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
    if (ierr == 0) printf("error ! reading bvec");
    printf("bvec %d : %f %f %f \n", i + 1, bvec[i][0], bvec[i][1], bvec[i][2]);
  }
  /*
  Allocation of Kohn-Sham energies $ matrix elements
  */
  eig0 = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  mat0 = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  eig = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  mat = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  for (ib = 0; ib < nb; ib++) {
    eig0[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    mat0[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    eig[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    mat[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      mat0[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      eig[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      mat[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
        mat0[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
        eig[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
        mat[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
      }
    }
  }
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
          eig[ib][ii0][ii1][ii2] = eig0[ib][ii0][ii1][ii2];
        }
      }
    }
  }
  /*
  Matrix elements
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
          ierr = fscanf(fp, "%e", &mat0[ib][ii0][ii1][ii2]);
          mat[ib][ii0][ii1][ii2] = mat0[ib][ii0][ii1][ii2];
        }
      }
    }
  }
  fclose(fp);
  /**/
} /* read_file */
