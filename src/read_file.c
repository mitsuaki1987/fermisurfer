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
#include "variable.h"
#include "basic_math.h"
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GLUT_H)
#include <GL/glut.h>
#elif defined(HAVE_GLUT_GLUT_H)
#include <GLUT/glut.h>
#endif
/**
 @brief Input from Fermi surface file
*/
void read_file(
  char *fname//!<[in] Input file name
)
{
  int ib, i, j, i0, i1, i2, ii0, ii1, ii2, ierr;
  FILE *fp;
  char* ctemp1;
  char ctemp2[256], rc;
  /*
   Open input file.
  */
  printf("  Openning %s ...\n", fname);
  if ((fp = fopen(fname, "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    getchar();
    exit(EXIT_FAILURE);
  }
  printf("\n");
  printf("  ##  Brillouin zone informations  ###########\n");
  printf("\n");
  /*
   k-point grid
  */
  ctemp1 = fgets(ctemp2, 256, fp);
  ierr = sscanf(ctemp2, "%d%d%d%s", &ng0[0], &ng0[1], &ng0[2], &rc);
  rc = tolower(rc);
  if (ierr <= 3) rc = 'r';
  if (rc == 'r')
    printf("    Real k-dependent quantity.\n");
  else if (rc == 'c')
    printf("    Complex k-dependent quantity.\n");
  else {
    printf("    Error! r or c is allowed. Input %c\n", rc);
    exit(-1);
  }

  if (ierr == 0) printf("error ! reading ng");
  printf("    k point grid : %d %d %d \n", ng0[0], ng0[1], ng0[2]);
  for (i = 0; i < 3; i++) ng[i] = ng0[i];
  /*
   Shift of k-point grid
  */
  ierr = fscanf(fp, "%d", &lshift);
  if (ierr == 0) printf("error ! reading lshift");

  if (lshift == 0) {
    printf("    k point grid is the Monkhorst-Pack grid. \n");
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    printf("    k point grid starts from Gamma. \n");
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    printf("    k point grid starts from Gamma + a half grid. \n");
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  if (ierr == 0) printf("error ! reading nb");
  printf("    # of bands : %d\n", nb);
  ntri = (int*)malloc(nb * sizeof(int));
  ntri_th = (int**)malloc(nb * sizeof(int*));
  for (ib = 0; ib < nb; ib++) ntri_th[ib] = (int*)malloc(nthreads * sizeof(int));
  nnl = (int*)malloc(nb * sizeof(int));
  n2d = (int*)malloc(nb * sizeof(int));
  nequator = (int*)malloc(nb * sizeof(int));
  draw_band = (int*)malloc(nb * sizeof(int));
  for (ib = 0; ib < nb; ib++) draw_band[ib] = 1;
  /*
   Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
    if (ierr == 0) printf("error ! reading bvec");
    printf("    bvec %d : %f %f %f \n", i + 1, bvec[i][0], bvec[i][1], bvec[i][2]);
  }/*for (i = 0; i < 3; ++i)*/
  /*
   Direct lattice vector
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) avec[i][j] = 0.0f;
    avec[i][i] = 1.0f;
    solve3(bvec, avec[i]);
    printf("    avec %d : %f %f %f \n", i + 1, avec[i][0], avec[i][1], avec[i][2]);
  }/*for (i = 0; i < 3; ++i)*/
  for (i = 0; i < 3; ++i) {
    secvec[i] = bvec[2][i];
    eqvec[i] = bvec[2][i];
  }
  secscale = 0.0;
  /*
   Allocation of Kohn-Sham energies $ matrix elements
  */
  eig0 = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  eig = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  mat0 = (GLfloat*****)malloc(nb * sizeof(GLfloat****));
  mat = (GLfloat*****)malloc(nb * sizeof(GLfloat****));
  vf = (GLfloat*****)malloc(nb * sizeof(GLfloat****));
  for (ib = 0; ib < nb; ib++) {
    eig0[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    eig[ib] = (GLfloat***)malloc(ng0[0] * sizeof(GLfloat**));
    mat0[ib] = (GLfloat****)malloc(ng0[0] * sizeof(GLfloat***));
    mat[ib] = (GLfloat****)malloc(ng0[0] * sizeof(GLfloat***));
    vf[ib] = (GLfloat****)malloc(ng0[0] * sizeof(GLfloat***));
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      eig[ib][i0] = (GLfloat**)malloc(ng0[1] * sizeof(GLfloat*));
      mat0[ib][i0] = (GLfloat***)malloc(ng0[1] * sizeof(GLfloat**));
      mat[ib][i0] = (GLfloat***)malloc(ng0[1] * sizeof(GLfloat**));
      vf[ib][i0] = (GLfloat***)malloc(ng0[1] * sizeof(GLfloat**));
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
        eig[ib][i0][i1] = (GLfloat*)malloc(ng0[2] * sizeof(GLfloat));
        mat0[ib][i0][i1] = (GLfloat**)malloc(ng0[2] * sizeof(GLfloat*));
        mat[ib][i0][i1] = (GLfloat**)malloc(ng0[2] * sizeof(GLfloat*));
        vf[ib][i0][i1] = (GLfloat**)malloc(ng0[2] * sizeof(GLfloat*));
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          mat0[ib][i0][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          mat[ib][i0][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
          vf[ib][i0][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
        }
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
          if (rc == 'r') {
            ierr = fscanf(fp, "%e", &mat0[ib][ii0][ii1][ii2][0]);
            mat0[ib][ii0][ii1][ii2][1] = 0.0;
          }
          else
            ierr = fscanf(fp, "%e%e", &mat0[ib][ii0][ii1][ii2][0], &mat0[ib][ii0][ii1][ii2][2]);
          mat0[ib][ii0][ii1][ii2][2] = 0.0;
        }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
      }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
    }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
  }/*for (ib = 0; ib < nb; ++ib)*/
  fclose(fp);
} /* read_file */

void read_batch(
  char *fname//!<[in] Input file name
)
{
  FILE *fp;
  int ierr, ib;

  printf("  Openning batch file %s ...\n", fname);
  if ((fp = fopen(fname, "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    getchar();
    exit(EXIT_FAILURE);
  }
  ierr = fscanf(fp, "%d", &blackback);
  for (ib = 0; ib < nb; ib++) 
    ierr = fscanf(fp, "%d", &draw_band[ib]);
  ierr = fscanf(fp, "%d", &lcolorbar);
  ierr = fscanf(fp, "%d", &fcscl);
  ierr = fscanf(fp, "%d", &lequator);
  if (lequator == 1) 
    ierr = fscanf(fp, "%f %f %f", &eqvec[0], &eqvec[1], &eqvec[2]);
  ierr = fscanf(fp, "%d", &interpol);
  ierr = fscanf(fp, "%d", &lside);
  ierr = fscanf(fp, "%f", &linewidth);
  ierr = fscanf(fp, "%d", &nodeline);
  ierr = fscanf(fp, "%d", &lsection);
  if (lsection == 1) {
    ierr = fscanf(fp, "%f", &secscale);
    ierr = fscanf(fp, "%f %f %f", &secvec[0], &secvec[1], &secvec[2]);
  }
  ierr = fscanf(fp, "%f", &EF);
  ierr = fscanf(fp, "%d", &lstereo);
  ierr = fscanf(fp, "%f", &scl);
  ierr = fscanf(fp, "%f %f %f", &trans[0], &trans[1], &trans[2]);
  ierr = fscanf(fp, "%f %f %f", &thetax, &thetay, &thetaz);
  fclose(fp);
  if (blackback == 1) glClearColor(0.0, 0.0, 0.0, 0.0);
  else glClearColor(1.0, 1.0, 1.0, 0.0);

  thetax = 3.14159265f / 180.0f * thetax;
  thetay = 3.14159265f / 180.0f * thetay;
  thetaz = 3.14159265f / 180.0f * thetaz;

  rot[0][0] = cosf(thetay)* cosf(thetaz);
  rot[0][1] = -cosf(thetay)* sinf(thetaz);
  rot[0][2] = sinf(thetay);
  rot[1][0] = cosf(thetaz)* sinf(thetax)* sinf(thetay) + cosf(thetax)* sinf(thetaz);
  rot[1][1] = cosf(thetax) * cosf(thetaz) - sinf(thetax)* sinf(thetay)* sinf(thetaz);
  rot[1][2] = -cosf(thetay)* sinf(thetax);
  rot[2][0] = -cosf(thetax)* cosf(thetaz)* sinf(thetay) + sinf(thetax)* sinf(thetaz);
  rot[2][1] = cosf(thetaz)* sinf(thetax) + cosf(thetax)* sinf(thetay)* sinf(thetaz);
  rot[2][2] = cosf(thetax)* cosf(thetay);
}
