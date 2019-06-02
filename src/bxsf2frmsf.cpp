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
@brief Convert BXSF format to FRMSF format and compute Fermi velocity
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int Nb;//!< Number of bands
int Ng[3];//!< @f$k@f$-grid along each direction

double Bvec[3][3];//!< Reciprocal lattice vector

double ****Eig;//!< Eigenvalue @f$\varepsilon_{n k}@f$ [::Nb][::Ng[0]][::Ng[1]][::Ng[2]]
double *****vf;//!< Fermi velocity @f${\bf v}_{{\rm F} n k}@f$ [::Nb][::Ng[0]][::Ng[1]][::Ng[2]][7]
/*
@brief Read BXSF file
*/
static void read_dat(
  char *fname//!<[in] Input file name
) {
  FILE *fp;
  char ctmp[256], ctmpEF1[16], ctmpEF2[16];
  int ierr, ii, ib, ik0, ik1, ik2;
  char *cerr;
  double EF;

  if ((fp = fopen(fname, "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }
  printf("\n#####  Reading BXSF file %s  #####\n\n", fname);

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "Fermi Energy:") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  ierr = sscanf(ctmp, "%s %s %lf", ctmpEF1, ctmpEF2, &EF);
  printf("  Fermi energy : %le\n", EF);

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "BEGIN_BLOCK_BANDGRID_3D") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  cerr = fgets(ctmp, 256, fp);
  cerr = fgets(ctmp, 256, fp);

  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d", &Nb);
  printf("  Number of bands : %d \n", Nb);
  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d%d%d", &Ng[0], &Ng[1], &Ng[2]);
  for (ii = 0; ii < 3; ii++) Ng[ii] -= 1;
  printf("  k point grid : %d %d %d \n", Ng[0], Ng[1], Ng[2]);

  cerr = fgets(ctmp, 256, fp);
  for (ii = 0; ii < 3; ++ii) {
    cerr = fgets(ctmp, 256, fp);
    ierr = sscanf(ctmp, "%le%le%le", &Bvec[0][ii], &Bvec[1][ii], &Bvec[2][ii]);
    printf("  Bvec %d : %f %f %f \n", ii + 1, Bvec[0][ii], Bvec[1][ii], Bvec[2][ii]);
  }
  
  Eig = new double***[Nb];
  for (ib = 0; ib < Nb; ib++) {
    Eig[ib] = new double**[Ng[0] + 1];
    for (ik0 = 0; ik0 <= Ng[0]; ik0++) {
      Eig[ib][ik0] = new double*[Ng[1] + 1];
      for (ik1 = 0; ik1 <= Ng[1]; ik1++) {
        Eig[ib][ik0][ik1] = new double[Ng[2] + 1];
      }/*for (ik1 = 0; ik1 < Ng[1]; ik1++)*/
    }/*for (ik0 = 0; ik0 < Ng[0]; ik0++)*/
  }/*for (ib = 0; ib < Nb; ib++)*/

  for (ib = 0; ib < Nb; ib++) {
    cerr = fgets(ctmp, 256, fp);
    printf("  Reading %s", ctmp);

    for (ik0 = 0; ik0 <= Ng[0]; ik0++) {
      for (ik1 = 0; ik1 <= Ng[1]; ik1++) {
        for (ik2 = 0; ik2 <= Ng[2]; ik2++) {
          ierr = fscanf(fp, "%le", &Eig[ib][ik0][ik1][ik2]);
          Eig[ib][ik0][ik1][ik2] -= EF;
        }/*for (ik2 = 0; ik2 < Ng[2]; ik2++)*/
      }/*for (ik1 = 0; ik1 < Ng[1]; ik1++)*/
    }/*for (ik0 = 0; ik0 < Ng[0]; ik0++)*/

    cerr = fgets(ctmp, 256, fp);

  }/*for (ib = 0; ib < Nb; ib++)*/
}
/**
@brief This routine compute the Fermi verocity from differentiate of e_{n k}.
*/
static void fvel() {
  int ik0, ik1, ik2, ib, ii, jj, ikv[3], ikp[3], ikm[3];
  double de[3];
  double det, avec[3][3], alength[3];

  printf("\n#####  Compute the Fermi velocity  #####\n\n");

  /*
   Compute direct lattice vectors
  */
  det = -Bvec[0][2] * Bvec[1][1] * Bvec[2][0]
      + Bvec[0][1] * Bvec[1][2] * Bvec[2][0]
      + Bvec[0][2] * Bvec[1][0] * Bvec[2][1]
      - Bvec[0][0] * Bvec[1][2] * Bvec[2][1]
      - Bvec[0][1] * Bvec[1][0] * Bvec[2][2]
      + Bvec[0][0] * Bvec[1][1] * Bvec[2][2];

  avec[0][0] = -Bvec[1][2] * Bvec[2][1] + Bvec[1][1] * Bvec[2][2];
  avec[0][1] = Bvec[0][2] * Bvec[2][1] - Bvec[0][1] * Bvec[2][2];
  avec[0][2] = -Bvec[0][2] * Bvec[1][1] + Bvec[0][1] * Bvec[1][2];
  avec[1][0] = Bvec[1][2] * Bvec[2][0] - Bvec[1][0] * Bvec[2][2];
  avec[1][1] = -Bvec[0][2] * Bvec[2][0] + Bvec[0][0] * Bvec[2][2];
  avec[1][2] = Bvec[0][2] * Bvec[1][0] - Bvec[0][0] * Bvec[1][2];
  avec[2][0] = -Bvec[1][1] * Bvec[2][0] + Bvec[1][0] * Bvec[2][1];
  avec[2][1] = Bvec[0][1] * Bvec[2][0] - Bvec[0][0] * Bvec[2][1];
  avec[2][2] = -Bvec[0][1] * Bvec[1][0] + Bvec[0][0] * Bvec[1][1];
  for (ii = 0; ii < 3; ++ii) {
    for (jj = 0; jj < 3; ++jj) avec[ii][jj] /= det;
      printf("  avec %d : %f %f %f \n", ii + 1, avec[ii][0], avec[ii][1], avec[ii][2]);
  }

  for (ii = 0; ii < 3; ii++)
    alength[ii] = sqrt(
      avec[0][ii] * avec[0][ii] + avec[1][ii] * avec[1][ii] + avec[2][ii] * avec[2][ii]);
  printf("  alength : %f %f %f \n", alength[0], alength[1], alength[2]);
  /*
   malloc fermi velocity
  */
  vf = new double****[Nb];
  for (ib = 0; ib < Nb; ib++) {
    vf[ib] = new double***[Ng[0]];
    for (ik0 = 0; ik0 < Ng[0]; ik0++) {
      vf[ib][ik0] = new double**[Ng[1]];
      for (ik1 = 0; ik1 < Ng[1]; ik1++) {
        vf[ib][ik0][ik1] = new double*[Ng[2]];
        for (ik2 = 0; ik2 < Ng[2]; ik2++) {
          vf[ib][ik0][ik1][ik2] = new double[7];
        }
      }/*for (ik1 = 0; ik1 < Ng[1]; ik1++)*/
    }/*for (ik0 = 0; ik0 < Ng[0]; ik0++)*/
  }/*for (ib = 0; ib < Nb; ib++)*/

  for (ik0 = 0; ik0 < Ng[0]; ik0++) {
    ikv[0] = ik0;
    for (ik1 = 0; ik1 < Ng[1]; ik1++) {
      ikv[1] = ik1;
      for (ik2 = 0; ik2 < Ng[2]; ik2++) {
        ikv[2] = ik2;
 
        for (ib = 0; ib < Nb; ib++) {
          for (ii = 0; ii < 3; ii++) {

            for (jj = 0; jj < 3; jj++) {
              ikp[jj] = ikv[jj];
              ikm[jj] = ikv[jj];
            }
            if(ikv[ii] == Ng[ii] - 1) ikp[ii] = 0;
            else ikp[ii] += 1;

            if (ikv[ii] == 0) ikm[ii] = Ng[ii] - 1;
            else ikm[ii] -= 1;

            de[ii] = Eig[ib][ikp[0]][ikp[1]][ikp[2]]
                   - Eig[ib][ikm[0]][ikm[1]][ikm[2]];
            de[ii] *= 0.5 * (double)Ng[ii];
          }/*for (ii = 0; ii < 3; ii++)*/

          for (ii = 0; ii < 3; ii++) {
            vf[ib][ik0][ik1][ik2][ii] = de[ii] * alength[ii];
            vf[ib][ik0][ik1][ik2][ii + 3] =
              avec[0][ii] * de[0] + avec[1][ii] * de[1] + avec[2][ii] * de[2];
          }/*for (ii = 0; ii < 3; ii++)*/
          vf[ib][ik0][ik1][ik2][6] = sqrt(
              vf[ib][ik0][ik1][ik2][3] * vf[ib][ik0][ik1][ik2][3]
            + vf[ib][ik0][ik1][ik2][4] * vf[ib][ik0][ik1][ik2][4]
            + vf[ib][ik0][ik1][ik2][5] * vf[ib][ik0][ik1][ik2][5]);
   
        }/*for (ib = 0; ib < Nb; ib++)*/
      }/* for (ik2 = 0; ik2 < Ng[2]; ik2++)*/
    }/*for (ik1 = 0; ik1 < Ng[1]; ik1++)*/
  }/* for (ik0 = 0; ik0 < Ng[0]; ik0++)*/
}/*void fvel()*/
/**
@brief Write FRMSF file
*/
static void write_file(
  char *fname1, //!< [in] File name without 
  const char *ext, //!< [in] File tail
  int component //!< [in] Component of the Fermi velocity
) 
{
  FILE* fo;
  int ierr, ibnd, ik0, ik1, ik2, ii;
  char fname2[256];
  char *p;

  ii = 0;
  if (strstr(fname1, ".bxsf") != NULL) {
    for (p = &fname1[0]; p < strstr(fname1, ".bxsf"); p++) fname2[ii++] = *p;
  }
  else if (strstr(fname1, ".BXSF") != NULL) {
    for (p = &fname1[0]; p < strstr(fname1, ".BXSF"); p++) fname2[ii++] = *p;
  }
  else if (strstr(fname1, ".Bxsf") != NULL) {
    for (p = &fname1[0]; p < strstr(fname1, ".Bxsf"); p++) fname2[ii++] = *p;
  }
  else {
    printf("ERROR! Extension shoud be .bxsf, .BXSF, or .Bxsf !\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(-1);
  }
  sprintf(&fname2[ii], "%s", ext);

  printf("  %s\n", fname2);

  fo = fopen(fname2, "w");
  ierr = fprintf(fo, "%d %d %d\n", Ng[0], Ng[1], Ng[2]);
  ierr = fprintf(fo, "%d\n", 1);
  ierr = fprintf(fo, "%d\n", Nb);
  ierr = fprintf(fo, "%e %e %e\n", Bvec[0][0], Bvec[1][0], Bvec[2][0]);
  ierr = fprintf(fo, "%e %e %e\n", Bvec[0][1], Bvec[1][1], Bvec[2][1]);
  ierr = fprintf(fo, "%e %e %e\n", Bvec[0][2], Bvec[1][2], Bvec[2][2]);

  for (ibnd = 0; ibnd < Nb; ++ibnd) {
    for (ik0 = 0; ik0 < Ng[0]; ++ik0) {
      for (ik1 = 0; ik1 < Ng[1]; ++ik1) {
        for (ik2 = 0; ik2 < Ng[2]; ++ik2) {
          ierr = fprintf(fo, "%e\n", Eig[ibnd][ik0][ik1][ik2]);
        }
      }
    }
  }
  for (ibnd = 0; ibnd < Nb; ++ibnd) {
    for (ik0 = 0; ik0 < Ng[0]; ++ik0) {
      for (ik1 = 0; ik1 < Ng[1]; ++ik1) {
        for (ik2 = 0; ik2 < Ng[2]; ++ik2) {
          ierr = fprintf(fo, "%e\n", vf[ibnd][ik0][ik1][ik2][component]);
        }
      }
    }
  }
  fclose(fo);

}/*void write_file()*/
/*
 @brief Main routine of BXSF2FRMSF
*/
int main(
  int argc, //!< [in]
  char *argv[] //!< [in] Input file name
)
{
  int ierr;

  if (argc < 2) {
    printf("\n\nInput file is not specified !\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(-1);
  }

  read_dat(argv[1]);
  
  fvel();

  printf("\n#####  Output files  #####\n\n");

  write_file(argv[1], "_vfa1.frmsf\0", 0);
  write_file(argv[1], "_vfa2.frmsf\0", 1);
  write_file(argv[1], "_vfa3.frmsf\0", 2);
  write_file(argv[1], "_vfx.frmsf\0", 3);
  write_file(argv[1], "_vfy.frmsf\0", 4);
  write_file(argv[1], "_vfz.frmsf\0", 5);
  write_file(argv[1], "_vf.frmsf\0", 6);

  printf("\n#####  DONE  #####\n");
  printf("  Press any key to exit.\n");
  ierr = getchar();
}