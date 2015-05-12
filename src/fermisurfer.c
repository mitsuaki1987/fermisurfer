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
#include <GL/glut.h>
#include <omp.h>
/*
  Input variables
*/
int ng[3];          //!<BZ grids
int lshift;         //!<Switch for shifted Brillouin zone
int nb;             //!<The number of Bands                               
GLfloat bvec[3][3]; //!<Resiplocal lattice vector                
GLfloat ****eig;    //!<Eigenvalues    [nb][ng[0]][ng[1]][ng[2]] 
GLfloat ****mat;    //!<Matrix element [nb][ng[0]][ng[1]][ng[2]] 
/*
  Switch for some modes
*/
int blackback = 1; //!<Switch for black background        
int fcscl = 1;     //!<Switch for full color scale mode   
int fbz = 1;       //!<Switch for 1st Brillouin zone mode 
int nodeline = 0;  //!<Switch for node lines              
int lcolorbar = 1; //!<Switch for colorbar                
int lstereo = 1;   //!<Switch for the stereogram
int lmouse = 1;    //!<Switch for the mouse function
/*
  Variables for Brillouin zone boundaries
*/
int nbzl;             //!<The number of Lines of 1st Brillouin zone 
GLfloat ***bzl;       //!<Lines of 1st BZ [nbzl][2][3]     
GLfloat bragg[26][3]; //!<Bragg plane vectors              
GLfloat brnrm[26];    //!<Norms of Bragg plane vectors     
/*
  Variables for patchs
*/
int *ntri;          //!<The number of triangle patch       [nb]             
int *draw_band;     //!<Switch for drawn bands    [nb]             
GLfloat ***nmlp;    //!<Normal vector of patchs   [nb][ntri][3]    
GLfloat ****kvp;    //!<K-vectors of points       [nb][ntri][3][3] 
GLfloat ***matp;    //!<Matrix elements of points [nb][ntri][3]    
GLfloat ****clr;    //!<Colors of points          [nb][ntri][3][4] 
int itet = 0;
/*
  Variables for nodeline
*/
int *nnl; //!<The number of nodeline 
GLfloat ****kvnl; //!<K-vector of nodeline [nb][nnl][2][3] 
/*
  Variables for mouse  & cursorkey
*/
GLfloat sx;                            //!<Scale of mouse movement
GLfloat sy;                            //!<Scale of mouse movement
int cx;                                //!<Starting point of drug  
int cy;                                //!<Starting point of drug  
GLfloat scl = 1.0;                     //!<Initial scale           
GLfloat trans[3] = {0.0, 0.0, 0.0};    //!<Translation             
GLfloat rot[3][3] = {{1.0, 0.0, 0.0},  //!<Rotation matrix         
		     {0.0, 1.0, 0.0},   
		     {0.0, 0.0, 1.0}};
/*
  Colors
 */
GLfloat   black[] = {0.0, 0.0, 0.0, 1.0}; //!< Black color code
GLfloat   white[] = {1.0, 1.0, 1.0, 1.0}; //!< White color code
GLfloat    cyan[] = {0.0, 1.0, 1.0, 1.0}; //!< Cyan color code
GLfloat magenta[] = {1.0, 0.0, 1.0, 1.0}; //!< Magenta color code
GLfloat  yellow[] = {1.0, 1.0, 0.0, 1.0}; //!< Yellow color code
GLfloat     red[] = {1.0, 0.0, 0.0, 1.0}; //!< Red color code
GLfloat   green[] = {0.0, 1.0, 0.0, 1.0}; //!< Green color code
GLfloat    blue[] = {0.0, 0.0, 1.0, 1.0}; //!< Blue color code
/*
  Others
*/
int query;             //!<Query switch           
int corner[6][4];      //!<Corners of tetrahedron 
GLfloat def = 0.0;     //!<Shift of Fermi energy 
enum
  {
    MOUSE_SCROLL_UP     = 3, //!<Mouse wheel up
    MOUSE_SCROLL_DOWN   = 4  //!<Mouse wheel down
  };
/*!
  Input from Fermi surface file
 @param[in] fname  Input file name
*/
void read_file(char *fname){
  int ib, i, i1, i2, i3, ierr;
  FILE *fp;
  /*
    Open input file.
   */
  if ((fp = fopen(fname, "r")) == NULL) {
    printf("file open error!!\n");
    exit(EXIT_FAILURE);
  }
  printf("\n#####  Brillouin zone informations  ##### \n\n");
  /* 
     k-point grid 
  */
  ierr = fscanf(fp, "%d%d%d", &ng[0], &ng[1], &ng[2]);
  if(ierr == 0) printf("error ! reading ng");
  printf("k point grid : %d %d %d \n", ng[0], ng[1], ng[2]);
  /* 
     Shift of k-point grid 
  */
  ierr = fscanf(fp, "%d", &lshift);
  if(ierr == 0) printf("error ! reading lshift");
  if(lshift == 0){
    printf("k point grid is not shifted \n");
  }
  else if(lshift == 1){
    printf("k point grid is shifted on 0.5, 0.5, 0.5 . \n");
  }
  else{
    exit(0);
  }
  /* 
     # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  if(ierr == 0) printf("error ! reading nb");
  printf("# of bands : %d \n", nb);
  ntri = (int*)malloc(nb * sizeof(int));
  nnl  = (int*)malloc(nb * sizeof(int));
  draw_band = (int*)malloc(nb * sizeof(int));
  for (ib = 0; ib < nb; ib++) draw_band[ib] = 1;
  /*
    Reciplocal lattice vectors
   */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
    if(ierr == 0) printf("error ! reading bvec");
    printf("bvec %d : %f %f %f \n", i + 1, bvec[i][0], bvec[i][1], bvec[i][2]);
  }
  /*
    Allocation of Kohn-Sham energies $ matrix elements
  */
  eig = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  mat = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  for (ib = 0; ib < nb; ib++){
    eig[ib] = (GLfloat***)malloc(ng[0] * sizeof(GLfloat**));
    mat[ib] = (GLfloat***)malloc(ng[0] * sizeof(GLfloat**));
    for (i1 = 0; i1 < ng[0]; i1++){
      eig[ib][i1] = (GLfloat**)malloc(ng[1] * sizeof(GLfloat*));
      mat[ib][i1] = (GLfloat**)malloc(ng[1] * sizeof(GLfloat*));
      for (i2 = 0; i2 < ng[1]; i2++){ 
	eig[ib][i1][i2] = (GLfloat*)malloc(ng[2] * sizeof(GLfloat));
	mat[ib][i1][i2] = (GLfloat*)malloc(ng[2] * sizeof(GLfloat));
      }
    }
  }
  /*
    Kohn-Sham energies
  */
  for (ib = 0; ib < nb; ++ib) {  
    for (i1 = 0; i1 < ng[0]; ++i1) {
      for (i2 = 0; i2 < ng[1]; ++i2) {
	for (i3 = 0; i3 < ng[2]; ++i3) {
	  ierr = fscanf(fp, "%e", &eig[ib][i1][i2][i3]);
	}
      }
    }
  }
  /*
    Matrix elements
  */
  for (ib = 0; ib < nb; ++ib) {  
    for (i1 = 0; i1 < ng[0]; ++i1) {
      for (i2 = 0; i2 < ng[1]; ++i2) {
	for (i3 = 0; i3 < ng[2]; ++i3) {
	  ierr = fscanf(fp, "%e", &mat[ib][i1][i2][i3]);
	}
      }
    }
  }
  fclose(fp);
  /**/
} /* read_file */
/*!
  Initialize corners of tetrahedron
*/
void init_corner(){
  int i, j;
  int corner1[16][6][4] = {
    /*  
	[0] min = 0-7  
    */
    {{0, 1, 3, 7}, 
     {0, 1, 5, 7},
     {0, 2, 3, 7},
     {0, 2, 6, 7},
     {0, 4, 5, 7},
     {0, 4, 6, 7}},
    /*
      [1] min = 1-6
     */
    {{1, 0, 2, 6},
     {1, 0, 4, 6},
     {1, 3, 2, 6},
     {1, 3, 7, 6},
     {1, 5, 4, 6},
     {1, 5, 7, 6}},
    /*
      [2] min = 2-5
     */
    {{2, 0, 1, 5},
     {2, 0, 4, 5},
     {2, 3, 1, 5},
     {2, 3, 7, 5},
     {2, 6, 4, 5},
     {2, 6, 7, 5}},
    /*
      [3] min = 3-4
     */
    {{4, 0, 1, 3},
     {4, 0, 2, 3},
     {4, 5, 1, 3},
     {4, 5, 7, 3},
     {4, 6, 2, 3},
     {4, 6, 7, 3}},
    /*
      [4] min = 0-7, max = 3-4
     */
    {{0, 4, 5, 6},
     {1, 2, 3, 7},
     {0, 7, 2, 6},
     {0, 7, 1, 5},
     {0, 7, 1, 2},
     {0, 7, 6, 5}},
    /*
      [5] min = 1-6, max = 3-4
     */
    {{0, 4, 5, 6},
     {1, 2, 3, 7},
     {1, 6, 5, 7},
     {1, 6, 7, 2},
     {1, 6, 2, 0},
     {1, 6, 0, 5}},
    /*
      [6] min = 2-5, max = 3-4
     */
    {{0, 4, 5, 6},
     {1, 2, 3, 7},
     {2, 5, 7, 6},
     {2, 5, 6, 0},
     {2, 5, 0, 1},
     {2, 5, 1, 7}},
    /*
      [7] min = 3-4, max = 0-7
     */
    {{0, 1, 2, 4},
     {7, 3, 5, 6},
     {3, 4, 1, 5},
     {3, 4, 5, 6},
     {3, 4, 6, 2},
     {3, 4, 2, 1}},
    /*
      [8] min = 2-5, max = 0-7
     */
    {{0, 1, 2, 4},
     {7, 3, 5, 6},
     {2, 5, 1, 3},
     {2, 5, 3, 6},
     {2, 5, 6, 4},
     {2, 5, 4, 1}},
    /*
      [9] min = 1-6, max = 0-7
     */
    {{0, 1, 2, 4},
     {7, 3, 5, 6},
     {1, 6, 2, 3},
     {1, 6, 3, 5},
     {1, 6, 5, 4},
     {1, 6, 4, 2}},
    /*
      [10] min = 0-7, max = 1-6
     */
    {{1, 0, 3, 5},
     {6, 2, 4, 7},
     {0, 7, 2, 3},
     {0, 7, 3, 5},
     {0, 7, 5, 4},
     {0, 7, 4, 2}},
    /*
      [11] min = 3-4, max = 1-6
     */
    {{1, 0, 3, 5},
     {6, 2, 4, 7},
     {3, 4, 0, 2},
     {3, 4, 2, 7},
     {3, 4, 7, 5},
     {3, 4, 5, 0}},
    /*
      [12] min = 2-5, max = 1-6
     */
    {{1, 0, 3, 5},
     {6, 2, 4, 7},
     {2, 5, 0, 3},
     {2, 5, 3, 7},
     {2, 5, 7, 4},
     {2, 5, 4, 0}},
    /*
      [13] min = 0-7, max = 2-5
     */
    {{2, 0, 3, 6},
     {5, 1, 4, 7},
     {0, 7, 1, 3},
     {0, 7, 3, 6},
     {0, 7, 6, 4},
     {0, 7, 4, 1}},
    /*
      [14] min = 1-6, max = 2-5
     */
    {{2, 0, 3, 6},
     {5, 1, 4, 7},
     {1, 6, 0, 3},
     {1, 6, 3, 7},
     {1, 6, 7, 4},
     {1, 6, 4, 0}},
    /*
      [15] min = 3-4, max = 2-5
     */
    {{2, 0, 3, 6},
     {5, 1, 4, 7},
     {3, 4, 0, 1},
     {3, 4, 1, 7},
     {3, 4, 7, 6},
     {3, 4, 6, 0}},
  };
  /**/
  for (i = 0; i < 6; ++i){
    for (j = 0; j < 4; ++j){
      corner[i][j] = corner1[itet][i][j];
    }
  }
}
/*!
  Compute Bragg vetor
*/
void bragg_vector(){  
  int i1, i2, i3, i, ibr;
  /**/
  ibr = 0;
  /**/
  for(i1 = -1; i1 <= 1; ++i1){
    for(i2 = -1; i2 <= 1; ++i2){
      for(i3 = -1; i3 <= 1; ++i3){
	/**/	
	if(i1 == 0 && i2 == 0 && i3 ==0){
	}
	else {
	  for(i = 0; i < 3; ++i) bragg[ibr][i] = ((  GLfloat)i1 * bvec[0][i] 
						  + (GLfloat)i2 * bvec[1][i] 
						  + (GLfloat)i3 * bvec[2][i]) * 0.5;
	  /**/
	  brnrm[ibr] = bragg[ibr][0] * bragg[ibr][0]
	    +          bragg[ibr][1] * bragg[ibr][1]
	    +          bragg[ibr][2] * bragg[ibr][2];
	  /**/
	  ibr = ibr + 1;
	}
      }
    }
  }
} /* bragg_vector */
/*!
  Solve linear system
  @param[in] a Matix
  @param[inout] b Right hand side vector
*/
GLfloat solve3(GLfloat a[3][3], GLfloat b[3]){
  int i;
  GLfloat det, c[3];
  /**/
  det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) 
    +   a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2]) 
    +   a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
  /**/  
  c[0] = b[0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) 
    +    b[1] * (a[0][2] * a[2][1] - a[0][1] * a[2][2]) 
    +    b[2] * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);
  /**/
  c[1] = b[0] * (a[1][2] * a[2][0] - a[1][0] * a[2][2]) 
    +    b[1] * (a[0][0] * a[2][2] - a[0][2] * a[2][0]) 
    +    b[2] * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);
  /**/
  c[2] = b[0] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]) 
    +    b[1] * (a[0][1] * a[2][0] - a[0][0] * a[2][1])
    +    b[2] * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
  /**/
  for(i=0;i<3;++i) b[i] = c[i] / det;
  return det;
  /**/
}
/*!
  Judge wheser this line is the edge of 1st BZ
  @param[in] ibr Index of a Bragg plane
  @param[in] jbr Index of a Bragg plane
  @param[in] nbr
  @param[in] vert start point of line
  @param[in] vert2 end point of line
*/
int bragg_vert(int ibr, int jbr, int nbr, GLfloat vert[3], GLfloat vert2[3]){
  int kbr, i, lbr, nbr0;
  GLfloat bmat[3][3], rhs[3], prod, thr = 0.0001, det;
  /**/
  nbr0 = nbr;
  /**/
  for(kbr = nbr0; kbr < 26; ++kbr){
    /**/
    for(i=0;i<3;++i) bmat[0][i] = bragg[ibr][i];
    for(i=0;i<3;++i) bmat[1][i] = bragg[jbr][i];
    for(i=0;i<3;++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = brnrm[ibr];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    /*
      if Bragg planes do not cross, roop next kbr
     */
    det = solve3(bmat, rhs);
    if(fabsf(det) < thr) continue;
    /*
      if vert0 = vert1, roop next kbr
     */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
      +    (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
      +    (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if(prod < thr) continue;
    /*
      is corner really in 1st BZ ?
     */
    i = 0;
    for(lbr = 0; lbr < 26; ++lbr){
      prod = bragg[lbr][0] * rhs[0]
	+    bragg[lbr][1] * rhs[1]
	+    bragg[lbr][2] * rhs[2];
      /**/
      if(prod > brnrm[lbr] + thr){
	i = 1;
	break;
      }
    }
    if(i == 1) {
    }
    else{
      for(i=0;i<3;++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }
  /*
    this line is not a BZ boundary
   */
  return 0;
  /**/
}/* bragg_vert */
/*!
  Compute Brillouin zone boundariy lines
*/
void bz_lines(){
  /**/
  int ibr, jbr, nbr, ibzl, i, j, lvert;
  GLfloat vert[2][3];
  /**/
  ibzl = 0;
  /**/
  for(ibr = 0; ibr < 26; ++ibr){
    for(jbr = 0; jbr < 26; ++jbr){
      /**/
      for(i=0;i<3;++i) vert[1][i] = 0.0;
      nbr = 0;
      lvert = bragg_vert(ibr, jbr, nbr, vert[0], vert[1]);
      if(lvert == 0) continue;
      nbr = lvert;
      /**/
      lvert = bragg_vert(ibr, jbr, nbr, vert[1], vert[0]);
      if(lvert == 0) continue;
      /**/
      if(query == 1){
	nbzl = nbzl + 1;
      }
      else{
	for(i=0;i<2;++i) for(j=0;j<3;++j) bzl[ibzl][i][j] = vert[i][j];
	ibzl = ibzl + 1;
      }
      /**/
    }
  }
  /**/
  if(query == 1){
    printf("# of lines for BZ : %d \n", nbzl);
    /**/
    bzl = (GLfloat***)malloc(nbzl * sizeof(GLfloat*));
    for(ibzl = 0; ibzl < nbzl; ++ibzl){
      bzl[ibzl] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
      for(i = 0; i < 2; ++i){
	bzl[ibzl][i] = (GLfloat*)malloc(3 * sizeof(GLfloat));
      }
    }
    /**/
  }
  /**/
} /* bz_lines */
/*!
  Max and Minimum in Brillouine zone
*/
void max_and_min_bz(){
  int ib, i1, i2, i3;
  GLfloat eigmin, eigmax, matmin, matmax;
  /**/
  printf("\n#####  Max. and Min. of each bands  ##### \n\n");
  printf("Band   Eig_Min.      Eig_Max      Mat_Min      Mat_Max \n");
  for(ib = 0; ib < nb; ib++){
    eigmax = - 100000000.0000;
    eigmin =   100000000.0000;
    matmax = - 100000000.0000;
    matmin =   100000000.0000;
    for (i1 = 0; i1 < ng[0]; ++i1) {
      for (i2 = 0; i2 < ng[1]; ++i2) {
	for (i3 = 0; i3 < ng[2]; ++i3) {
	  if(eig[ib][i1][i2][i3] > eigmax) eigmax = eig[ib][i1][i2][i3];
	  if(eig[ib][i1][i2][i3] < eigmin) eigmin = eig[ib][i1][i2][i3];
	  if(mat[ib][i1][i2][i3] > matmax) matmax = mat[ib][i1][i2][i3];
	  if(mat[ib][i1][i2][i3] < matmin) matmin = mat[ib][i1][i2][i3];
	}
      }
    }
    printf("%d     %f     %f     %f     %f \n", ib + 1, eigmin, eigmax, matmin, matmax);
  }
  /**/
}/* max_and_min_bz */
/*!
  Sort eigenvalues
  @param[in] n the number of components
  @param[inout] eig2 the orbital energy
  @param[inout] mat2 the matrix element
  @param[inout] kvec2 of corners
*/
void eigsort(int n, GLfloat* eig2, GLfloat* mat2, GLfloat kvec2[][3]){  
  int i, j, k;
  GLfloat tmp;
  /**/
  for (i = 0; i < n - 1; ++i){
    for (j = i + 1; j < n; ++j){
      if(eig2[j] < eig2[i]) {
	tmp = eig2[j];
	eig2[j] = eig2[i];
	eig2[i] = tmp;
	/**/
	tmp = mat2[j];
	mat2[j] = mat2[i];
	mat2[i] = tmp;
	/**/
	for (k = 0; k < 3; ++k){
	  tmp = kvec2[j][k];
	  kvec2[j][k] = kvec2[i][k];
	  kvec2[i][k] = tmp;
	}
      }
    }
  }
} /* eigsort */
/*!
  Calculate normal vector
  @param[in] in1 Corner 1
  @param[in] in2 Corner 2
  @param[in] in3 Corner 3
  @param[out] out The normal vector
*/
void normal_vec(GLfloat in1[3], GLfloat in2[3], GLfloat in3[3], GLfloat out[3]){
  int i;
  GLfloat norm;
  out[0] = in1[1] * in2[2] - in1[2] * in2[1] 
    +      in2[1] * in3[2] - in2[2] * in3[1]
    +      in3[1] * in1[2] - in3[2] * in1[1];
  out[1] = in1[2] * in2[0] - in1[0] * in2[2] 
    +      in2[2] * in3[0] - in2[0] * in3[2]
    +      in3[2] * in1[0] - in3[0] * in1[2];
  out[2] = in1[0] * in2[1] - in1[1] * in2[0] 
    +      in2[0] * in3[1] - in2[1] * in3[0]
    +      in3[0] * in1[1] - in3[1] * in1[0];
  norm = sqrtf(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
  for(i=0;i<3;i++) out[i] = out[i] / norm;
} /* normal_vec */
/*!
  Store triangle patch
  @param[in] ib The band index
  @param[in] nbr Bragg plane
  @param[in] mat1 The matrix element
  @param[in] kvec1 k vector of corners
*/
void triangle(int ib, int nbr, GLfloat mat1[3], GLfloat kvec1[3][3]){
  /**/  
  int ibr, i, j;
  GLfloat prod[3], thr = 0.0000, mat2[3], kvec2[3][3];
  /**/
  if(fbz == 1){
    /**/
    for(ibr = nbr; ibr < 26; ++ibr){
      /**/
      for(i = 0; i < 3; ++i){
	prod[i] = bragg[ibr][0] * kvec1[i][0] 
	  +       bragg[ibr][1] * kvec1[i][1] 
	  +       bragg[ibr][2] * kvec1[i][2];
      }
      eigsort(3, prod, mat1, kvec1);
      /**/    
      if(brnrm[ibr] + thr < prod[0]) {
	return;
      }
      else if(brnrm[ibr] + thr < prod[1]){
	mat2[0] = mat1[0];
	mat2[1] = mat1[0] * (brnrm[ibr] - prod[1]) / (prod[0] - prod[1])
	  +       mat1[1] * (brnrm[ibr] - prod[0]) / (prod[1] - prod[0]);
	mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
	  +       mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	for(i=0;i<3;++i) kvec2[0][i] = kvec1[0][i];
	for(i=0;i<3;++i) kvec2[1][i] = kvec1[0][i] * (brnrm[ibr] - prod[1]) / (prod[0] - prod[1])
			   +           kvec1[1][i] * (brnrm[ibr] - prod[0]) / (prod[1] - prod[0]);
	for(i=0;i<3;++i) kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
			   +           kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	triangle(ib, ibr + 1, mat2, kvec2);
	return;
      }
      else if(brnrm[ibr] + thr < prod[2]){
	mat2[0] = mat1[0];
	mat2[1] = mat1[1];
	mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
	  +       mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	for(i=0;i<3;++i) kvec2[0][i] = kvec1[0][i];
	for(i=0;i<3;++i) kvec2[1][i] = kvec1[1][i];
	for(i=0;i<3;++i) kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
			   +           kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	triangle(ib, ibr + 1, mat2, kvec2);
	/**/
	mat2[0] = mat1[1] * (brnrm[ibr] - prod[2]) / (prod[1] - prod[2])
	  +       mat1[2] * (brnrm[ibr] - prod[1]) / (prod[2] - prod[1]);
	mat2[1] = mat1[1];
	mat2[2] = mat1[0] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
	  +       mat1[2] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	for(i=0;i<3;++i) kvec2[0][i] = kvec1[1][i] * (brnrm[ibr] - prod[2]) / (prod[1] - prod[2])
			   +           kvec1[2][i] * (brnrm[ibr] - prod[1]) / (prod[2] - prod[1]);
	for(i=0;i<3;++i) kvec2[1][i] = kvec1[1][i];
	for(i=0;i<3;++i) kvec2[2][i] = kvec1[0][i] * (brnrm[ibr] - prod[2]) / (prod[0] - prod[2])
			   +           kvec1[2][i] * (brnrm[ibr] - prod[0]) / (prod[2] - prod[0]);
	triangle(ib, ibr + 1, mat2, kvec2);
	return;
      }
      else{
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < 26*/
  } /* if fbz == 1 */
  /**/
  if(query == 1){
    ntri[ib] = ntri[ib] + 1;
  }
  else{
    normal_vec(kvec1[0], kvec1[1], kvec1[2], nmlp[ib][ntri[ib]]);
    for(i = 0; i < 3; ++i){
      matp[ib][ntri[ib]][i] = mat1[i];
      for(j = 0; j < 3; ++j){
	kvp[ib][ntri[ib]][i][j] = kvec1[i][j];
      }
    }
    ntri[ib] = ntri[ib] + 1;
  }
  /**/
}/* triangle */
/*!
  Tetrahedrron method
  @param[in] ib The band index
  @param[in] eig1 orbital energies
  @param[in] mat1 Matrix elements
  @param[in] kvec1 k vectors
*/
void tetrahedron(int ib, GLfloat eig1[8], GLfloat mat1[8], GLfloat kvec1[8][3]){
  /**/
  int it, i, j;
  GLfloat eig2[4], mat2[4], kvec2[4][3], a[4][4], kvec3[3][3], mat3[3];
  /**/
  for (it = 0; it < 6; ++it) {
    /*
      Define corners of the tetrahedron
     */
    for (i = 0; i < 4; ++i) { 
      eig2[ i] = eig1[corner[it][i]];
      mat2[ i] = mat1[corner[it][i]];
      /**/
      for(j = 0; j < 3; ++j) kvec2[i][j] = bvec[0][j] * kvec1[corner[it][i]][0]
      				  +        bvec[1][j] * kvec1[corner[it][i]][1]
      				  +        bvec[2][j] * kvec1[corner[it][i]][2];
    }
    /*
      Sort of eig1
     */
    eigsort(4, eig2, mat2, kvec2);
    for (i = 0; i < 4; ++i){
      for (j = 0; j < 4; ++j){
	a[i][j] = (0.00 - eig2[j]) / (eig2[i] - eig2[j]);
      }
    }
    /*
      Draw triangle in each cases
     */
    if(eig2[0] <= 0.00 && 0.00 < eig2[1]) {
      for(i = 0; i < 3; ++i) kvec3[0][i] = kvec2[0][i] * a[0][1] + kvec2[1][i] * a[1][0];
      for(i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][2] + kvec2[2][i] * a[2][0];
      for(i = 0; i < 3; ++i) kvec3[2][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      mat3[0] = mat2[0] * a[0][1] + mat2[1] * a[1][0];
      mat3[1] = mat2[0] * a[0][2] + mat2[2] * a[2][0];
      mat3[2] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      triangle(ib, 0, mat3, kvec3);
    }
    else if(eig2[1] <= 0.00 && 0.00 < eig2[2]){
      for(i = 0; i < 3; ++i) kvec3[0][i] = kvec2[0][i] * a[0][2] + kvec2[2][i] * a[2][0];
      for(i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      for(i = 0; i < 3; ++i) kvec3[2][i] = kvec2[1][i] * a[1][2] + kvec2[2][i] * a[2][1];
      mat3[0] = mat2[0] * a[0][2] + mat2[2] * a[2][0];
      mat3[1] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      mat3[2] = mat2[1] * a[1][2] + mat2[2] * a[2][1];
      triangle(ib, 0, mat3, kvec3);
      /**/
      for(i = 0; i < 3; ++i) kvec3[0][i] = kvec2[1][i] * a[1][3] + kvec2[3][i] * a[3][1];
      for(i = 0; i < 3; ++i) kvec3[1][i] = kvec2[0][i] * a[0][3] + kvec2[3][i] * a[3][0];
      for(i = 0; i < 3; ++i) kvec3[2][i] = kvec2[1][i] * a[1][2] + kvec2[2][i] * a[2][1];
      mat3[0] = mat2[1] * a[1][3] + mat2[3] * a[3][1];
      mat3[1] = mat2[0] * a[0][3] + mat2[3] * a[3][0];
      mat3[2] = mat2[1] * a[1][2] + mat2[2] * a[2][1];
      triangle(ib, 0, mat3, kvec3);
    }
    else if(eig2[2] <= 0.00 && 0.00 < eig2[3]){
      for(i = 0; i < 3; ++i) kvec3[0][i] = kvec2[3][i] * a[3][0] + kvec2[0][i] * a[0][3];
      for(i = 0; i < 3; ++i) kvec3[1][i] = kvec2[3][i] * a[3][1] + kvec2[1][i] * a[1][3];
      for(i = 0; i < 3; ++i) kvec3[2][i] = kvec2[3][i] * a[3][2] + kvec2[2][i] * a[2][3];
      mat3[0] = mat2[3] * a[3][0] + mat2[0] * a[0][3];
      mat3[1] = mat2[3] * a[3][1] + mat2[1] * a[1][3];
      mat3[2] = mat2[3] * a[3][2] + mat2[2] * a[2][3];
      triangle(ib, 0, mat3, kvec3);
    }
    else{
    }
  }
}/* tetrahedron */
/*!
  Patches for FSs
*/
void fermi_patch()
{
  int ib, i1, i2, i3, ii1, ii2, ii3, j1, j2, j3, start[3], i, j;
  GLfloat kvec1[8][3], eig1[8], mat1[8];
  /**/
  if(fbz == 1){
    for(i1 = 0; i1 < 3;++i1) start[i1] = - ng[i1];
  }
  else{
    for(i1 = 0; i1 < 3;++i1) start[i1] = 0;
  }
  /**/
#pragma omp parallel default(none) \
  shared(nb,ntri,start,ng,eig,mat,lshift) \
  private(ib,j1,j2,j3,i1,i2,i3,ii1,ii2,ii3,kvec1,eig1,mat1,i,j)
  {
#pragma omp for nowait
  for (ib = 0; ib < nb; ++ib) {
    ntri[ib] = 0; 
    for (j1 = start[0]; j1 < ng[0]; ++j1) {
      for (j2 = start[1]; j2 < ng[1]; ++j2) {
	for (j3 = start[2]; j3 < ng[2]; ++j3) {
	  /**/
	  i1  = j1;
	  i2  = j2;
	  i3  = j3;
	  ii1 = j1 + 1;
	  ii2 = j2 + 1;
	  ii3 = j3 + 1;
	  /**/
	  kvec1[0][0] = (GLfloat)i1  / (GLfloat)ng[0];
	  kvec1[1][0] = (GLfloat)i1  / (GLfloat)ng[0];
	  kvec1[2][0] = (GLfloat)i1  / (GLfloat)ng[0];
	  kvec1[3][0] = (GLfloat)i1  / (GLfloat)ng[0];
	  kvec1[4][0] = (GLfloat)ii1 / (GLfloat)ng[0];
	  kvec1[5][0] = (GLfloat)ii1 / (GLfloat)ng[0];
	  kvec1[6][0] = (GLfloat)ii1 / (GLfloat)ng[0];
	  kvec1[7][0] = (GLfloat)ii1 / (GLfloat)ng[0];
	  /**/	  
	  kvec1[0][1] = (GLfloat)i2  / (GLfloat)ng[1];
	  kvec1[1][1] = (GLfloat)i2  / (GLfloat)ng[1];
	  kvec1[2][1] = (GLfloat)ii2 / (GLfloat)ng[1];
	  kvec1[3][1] = (GLfloat)ii2 / (GLfloat)ng[1];
	  kvec1[4][1] = (GLfloat)i2  / (GLfloat)ng[1];
	  kvec1[5][1] = (GLfloat)i2  / (GLfloat)ng[1];
	  kvec1[6][1] = (GLfloat)ii2 / (GLfloat)ng[1];
	  kvec1[7][1] = (GLfloat)ii2 / (GLfloat)ng[1];
	  /**/	  
	  kvec1[0][2] = (GLfloat)i3  / (GLfloat)ng[2];
	  kvec1[1][2] = (GLfloat)ii3 / (GLfloat)ng[2];
	  kvec1[2][2] = (GLfloat)i3  / (GLfloat)ng[2];
	  kvec1[3][2] = (GLfloat)ii3 / (GLfloat)ng[2];
	  kvec1[4][2] = (GLfloat)i3  / (GLfloat)ng[2];
	  kvec1[5][2] = (GLfloat)ii3 / (GLfloat)ng[2];
	  kvec1[6][2] = (GLfloat)i3  / (GLfloat)ng[2];
	  kvec1[7][2] = (GLfloat)ii3 / (GLfloat)ng[2];
	  /**/
	  if(lshift == 1){
	    for(i=0;i<8;i++)for(j=0;j<3;j++) kvec1[i][j] = kvec1[i][j] + 0.50 / (GLfloat)ng[j];
	  }
	  /**/
	  if(i1  < 0) i1  = i1  + ng[0];
	  if(i2  < 0) i2  = i2  + ng[1];
	  if(i3  < 0) i3  = i3  + ng[2];
	  if(ii1 < 0) ii1 = ii1 + ng[0];
	  if(ii2 < 0) ii2 = ii2 + ng[1];
	  if(ii3 < 0) ii3 = ii3 + ng[2];
	  /**/
	  if(ii1 >= ng[0]) ii1 = 0;
	  if(ii2 >= ng[1]) ii2 = 0;
	  if(ii3 >= ng[2]) ii3 = 0;
	  /**/	  
	  eig1[0] = eig[ib][ i1][ i2][ i3];
	  eig1[1] = eig[ib][ i1][ i2][ii3];
	  eig1[2] = eig[ib][ i1][ii2][ i3];
	  eig1[3] = eig[ib][ i1][ii2][ii3];
	  eig1[4] = eig[ib][ii1][ i2][ i3];
	  eig1[5] = eig[ib][ii1][ i2][ii3];
	  eig1[6] = eig[ib][ii1][ii2][ i3];
	  eig1[7] = eig[ib][ii1][ii2][ii3];
	  /**/
	  mat1[0] = mat[ib][ i1][ i2][ i3];
	  mat1[1] = mat[ib][ i1][ i2][ii3];
	  mat1[2] = mat[ib][ i1][ii2][ i3];
	  mat1[3] = mat[ib][ i1][ii2][ii3];
	  mat1[4] = mat[ib][ii1][ i2][ i3];
	  mat1[5] = mat[ib][ii1][ i2][ii3];
	  mat1[6] = mat[ib][ii1][ii2][ i3];
	  mat1[7] = mat[ib][ii1][ii2][ii3];
	  /**/	  
	  tetrahedron(ib, eig1, mat1, kvec1);
	}
      }
    }
  }
 } /* End of parallel region */
  /**/
  if(query == 1){
    printf("band   # of patchs \n");
    for(ib =0; ib < nb; ib++){
      printf("%d       %d \n", ib + 1, ntri[ib]);
    }
    printf("\n");
    /*
      Allocation of triangler patches
    */
    nmlp = (GLfloat***)malloc(nb * sizeof(GLfloat**));
    matp = (GLfloat***)malloc(nb * sizeof(GLfloat**));
    clr = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    kvp = (GLfloat****)malloc(nb * sizeof(GLfloat***));
    for (ib = 0; ib < nb; ++ib) {
      nmlp[ib] = (GLfloat**)malloc(ntri[ib] * sizeof(GLfloat*));
      matp[ib] = (GLfloat**)malloc(ntri[ib] * sizeof(GLfloat*));
      clr[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      kvp[ib] = (GLfloat***)malloc(ntri[ib] * sizeof(GLfloat**));
      for (i1 = 0; i1 < ntri[ib]; ++i1){
	nmlp[ib][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
	matp[ib][i1] = (GLfloat*)malloc(3 * sizeof(GLfloat));
	clr[ib][i1] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
	kvp[ib][i1] = (GLfloat**)malloc(3 * sizeof(GLfloat*));
	for (i2 = 0; i2 < 3; ++i2){
	  kvp[ib][i1][i2] = (GLfloat*)malloc(3 * sizeof(GLfloat));
	  clr[ib][i1][i2] = (GLfloat*)malloc(4 * sizeof(GLfloat));
	}
      }
    }
    /**/
  }
  /**/
} /* fermi_patch */
/*!
  Max. & Min. of matrix elements.
*/
void max_and_min(){
  int ib, itri, i, j, ierr;
  GLfloat matmax, matmin, mat2;
  /**/
  matmax = - 100000000.0000;
  matmin =   100000000.0000;
  /**/
  for(ib = 0; ib < nb; ib++){
    for(itri = 0; itri < ntri[ib]; ++itri){
      for(i = 0; i < 3; ++i){
	if(matp[ib][itri][i] > matmax) matmax = matp[ib][itri][i];
	if(matp[ib][itri][i] < matmin) matmin = matp[ib][itri][i];
      }
    }
  }
  /**/  
  printf("Max. value : %f \n", matmax);
  printf("Min. value : %f \n \n", matmin);
  /**/  
  if(fcscl == 2){
    printf("Set min. value : ");
    ierr = scanf("%f", &matmin);
    if(ierr == 0) printf("error ! reading min");
    printf("Set max. value : ");
    ierr = scanf("%f", &matmax);
    if(ierr == 0) printf("error ! reading max");
  }
  /**/
  if(fcscl == 1 || fcscl == 2){
    for(ib = 0; ib < nb; ib++){
      for(itri = 0; itri < ntri[ib]; ++itri){
	for (i = 0; i < 3; ++i){
	  /**/
	  mat2 = (matp[ib][itri][i] - matmin) / (matmax - matmin);
	  mat2 = mat2 * 4.0;
	  /**/
	  if(mat2 <= 1.0) {
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = cyan[j] * mat2 + blue[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 2.0){
	    mat2 = mat2 - 1.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = green[j] * mat2 + cyan[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 3.0){
	    mat2 = mat2 - 2.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = yellow[j] * mat2 + green[j] * (1.0 - mat2);
	  }
	  else{
	    mat2 = mat2 - 3.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = red[j] * mat2 + yellow[j] * (1.0 - mat2);
	  }
	}
      }
    }
  }
  else if(fcscl == 4){
    for(ib = 0; ib < nb; ib++){
      for(itri = 0; itri < ntri[ib]; ++itri){
	for (i = 0; i < 3; ++i){
	  /**/
	  mat2 = matp[ib][itri][i] / 6.283185307;
	  mat2 = mat2 - floorf(mat2);
	  mat2 = mat2 * 6.0;
	  /**/
	  if(mat2 <= 1.0) {
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 2.0){
	    mat2 = mat2 - 1.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 3.0){
	    mat2 = mat2 - 2.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 4.0){
	    mat2 = mat2 - 3.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
	  }
	  else if(mat2 <= 5.0){
	    mat2 = mat2 - 4.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
	  }
	  else{
	    mat2 = mat2 - 5.0;
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
	  }
	}
      }
    }
  }
  else{
    for(ib = 0; ib < nb; ib++){
      /**/
      mat2 = 1.0 / (GLfloat)(nb - 1) * (GLfloat)ib;
      mat2 = mat2 * 4.0;
      /**/
      if(mat2 <= 1.0) {
	for(itri = 0; itri < ntri[ib]; ++itri){
	  for (i = 0; i < 3; ++i){
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = cyan[j] * mat2 + blue[j] * (1.0 - mat2);
	  }
	}
      }
      else if(mat2 <= 2.0){
	mat2 = mat2 - 1.0;
	for(itri = 0; itri < ntri[ib]; ++itri){
	  for (i = 0; i < 3; ++i){
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = green[j] * mat2 + cyan[j] * (1.0 - mat2);
	  }
	}
      }
      else if(mat2 <= 3.0){
	mat2 = mat2 - 2.0;
	for(itri = 0; itri < ntri[ib]; ++itri){
	  for (i = 0; i < 3; ++i){
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = yellow[j] * mat2 + green[j] * (1.0 - mat2);
	  }
	}
      }
      else{
	mat2 = mat2 - 3.0;
	for(itri = 0; itri < ntri[ib]; ++itri){
	  for (i = 0; i < 3; ++i){
	    for(j=0;j<4;++j) clr[ib][itri][i][j] = red[j] * mat2 + yellow[j] * (1.0 - mat2);
	  }
	}
      }
    }
  }
  /**/
} /* max_and_min */
/*!
  Node line
*/
void calc_nodeline(){
  int ib, itri, i, j;
  GLfloat mprod[2];
  /*
    Query
   */
#pragma omp parallel default(none)	 \
  shared(nb,nnl,matp,ntri)			 \
  private(ib,itri,mprod)
  {
#pragma omp for
    for(ib = 0; ib < nb; ib++){
      nnl[ib] = 0;
      for(itri = 0; itri < ntri[ib]; ++itri){
	/**/
	mprod[0] = matp[ib][itri][0] * matp[ib][itri][1];
	mprod[1] = matp[ib][itri][1] * matp[ib][itri][2];
	/**/
	if(     fabsf(matp[ib][itri][0]) < 0.00001 &&  fabsf(matp[ib][itri][1]) < 0.00001){
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(fabsf(matp[ib][itri][0]) < 0.00001 &&  fabsf(matp[ib][itri][2]) < 0.00001){
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(fabsf(matp[ib][itri][1]) < 0.00001 &&  fabsf(matp[ib][itri][2]) < 0.00001){
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(mprod[0] < 0.0){
	  if(mprod[1] < 0.0){
	    nnl[ib] = nnl[ib] + 1;
	  }
	  else{
	    nnl[ib] = nnl[ib] + 1;
	  }
	}
	else if(mprod[1] < 0.0){
	  nnl[ib] = nnl[ib] + 1;
	}
      }
    }
  } /* End of parallel region */
  /**/
  printf("band   # of nodeline \n");
  for(ib =0; ib < nb; ib++){
    printf("%d       %d \n", ib + 1, nnl[ib]);
  }
  printf("\n");
  /*
    Allocation of nodeline
  */
  kvnl = (GLfloat****)malloc(nb * sizeof(GLfloat***));
  for (ib = 0; ib < nb; ++ib) {
    kvnl[ib] = (GLfloat***)malloc(nnl[ib] * sizeof(GLfloat**));
    for (i = 0; i < nnl[ib]; ++i){
      kvnl[ib][i] = (GLfloat**)malloc(2 * sizeof(GLfloat*));
      for (j = 0; j < 2; ++j){
	kvnl[ib][i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
      }
    }
  }
  /**/
#pragma omp parallel default(none)		\
  shared(nb,nnl,matp,kvnl,kvp,ntri)			\
  private(ib,itri,mprod,i)
  {
#pragma omp for
    for(ib = 0; ib < nb; ib++){
      nnl[ib] = 0;
      for(itri = 0; itri < ntri[ib]; ++itri){
	/**/
	mprod[0] = matp[ib][itri][0] * matp[ib][itri][1];
	mprod[1] = matp[ib][itri][1] * matp[ib][itri][2];
	/**/
	if(     fabsf(matp[ib][itri][0]) < 0.00001 && fabsf(matp[ib][itri][1]) < 0.00001){
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = kvp[ib][itri][0][i];
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = kvp[ib][itri][1][i];
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(fabsf(matp[ib][itri][0]) < 0.00001 && fabsf(matp[ib][itri][2]) < 0.00001){
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = kvp[ib][itri][0][i];
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = kvp[ib][itri][2][i];
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(fabsf(matp[ib][itri][1]) < 0.00001 && fabsf(matp[ib][itri][2]) < 0.00001){
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = kvp[ib][itri][1][i];
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = kvp[ib][itri][2][i];
	  nnl[ib] = nnl[ib] + 1;
	}
	else if(mprod[0] < 0.0){
	  if(mprod[1] < 0.0){
	    for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = 1.0 / (matp[ib][itri][0] - matp[ib][itri][1])
			      * ((  0.0 - matp[ib][itri][1]) * kvp[ib][itri][0][i]
				 + (matp[ib][itri][0] - 0.0) * kvp[ib][itri][1][i]);
	    for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = 1.0 / (matp[ib][itri][1] - matp[ib][itri][2])
			      * ((  0.0 - matp[ib][itri][2]) * kvp[ib][itri][1][i]
				 + (matp[ib][itri][1] - 0.0) * kvp[ib][itri][2][i]);
	    nnl[ib] = nnl[ib] + 1;
	  }
	  else{
	    for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = 1.0 / (matp[ib][itri][0] - matp[ib][itri][1])
			      * ((  0.0 - matp[ib][itri][1]) * kvp[ib][itri][0][i]
				 + (matp[ib][itri][0] - 0.0) * kvp[ib][itri][1][i]);
	    for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = 1.0 / (matp[ib][itri][0] - matp[ib][itri][2])
			      * ((  0.0 - matp[ib][itri][2]) * kvp[ib][itri][0][i]
				 + (matp[ib][itri][0] - 0.0) * kvp[ib][itri][2][i]);
	    nnl[ib] = nnl[ib] + 1;
	  }
	}
	else if(mprod[1] < 0.0){
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][0][i] = 1.0 / (matp[ib][itri][0] - matp[ib][itri][2])
			    * ((  0.0 - matp[ib][itri][2]) * kvp[ib][itri][0][i]
			       + (matp[ib][itri][0] - 0.0) * kvp[ib][itri][2][i]);
	  for(i=0;i<3;++i)kvnl[ib][nnl[ib]][1][i] = 1.0 / (matp[ib][itri][1] - matp[ib][itri][2])
			    * ((  0.0 - matp[ib][itri][2]) * kvp[ib][itri][1][i]
			       + (matp[ib][itri][1] - 0.0) * kvp[ib][itri][2][i]);
	  nnl[ib] = nnl[ib] + 1;
	}
      }
    }
  } /* End of parallel region */
}
/*!
  Draw Fermi surfaces
*/
void draw_fermi(){
  /**/
  int i, j, ib, itri;
  GLfloat vect[3];
  /*
    Triagle patchs
   */
  for(ib = 0;ib < nb; ib++){
    /**/
    if(draw_band[ib] == 1){
#pragma omp parallel default(none)	 \
  shared(ib,ntri,rot,trans,nmlp,kvp,clr) \
  private(itri,i,j,vect)
      glBegin(GL_TRIANGLES);
      {
#pragma omp for
	for(itri = 0; itri < ntri[ib]; ++itri){
	  /**/	
	  for(j = 0; j < 3; ++j) vect[j] = rot[j][0] * nmlp[ib][itri][0]
				   +       rot[j][1] * nmlp[ib][itri][1]
				   +       rot[j][2] * nmlp[ib][itri][2];
	  glNormal3fv(vect);
	  /**/	
	  for (i = 0; i < 3; ++i){
	    /**/	  
	    for(j = 0; j < 3; ++j) vect[j] = rot[j][0] * kvp[ib][itri][i][0]
				     +       rot[j][1] * kvp[ib][itri][i][1]
				     +       rot[j][2] * kvp[ib][itri][i][2]
				     + trans[j];
	    /**/	
	    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clr[ib][itri][i]);
	    glVertex3fv(vect);
	  }
	}
	glEnd();
      } /* End of parallel region */
    }
    /**/
  }
  /*
    Nodeline
   */
  if(nodeline == 1){
    glLineWidth(10.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    glBegin(GL_LINES);
    for(ib = 0;ib < nb; ib++){
      /**/
      if(draw_band[ib] == 1){
	for(itri = 0; itri < nnl[ib]; ++itri){
	  for (i = 0; i < 2; ++i){
	    /**/	  
	    for(j = 0; j < 3; ++j) vect[j] = rot[j][0] * kvnl[ib][itri][i][0]
				     +       rot[j][1] * kvnl[ib][itri][i][1]
				     +       rot[j][2] * kvnl[ib][itri][i][2]
				     + trans[j] + 0.001;
	    /**/	
	    glVertex3fv(vect);
	  }
	}
      }
      /**/
    }
    glEnd();
  }
  /**/
} /* draw_ferm */
/*!
  Draw lines of BZ boundaries
*/
void draw_bz_lines(){
  /**/
  int ibzl, i, j;
  GLfloat bzl2[3], bvec2[3][3], linecolor[4];
  /**/
  if(blackback == 1){
    for(i=0;i<4;i++) linecolor[i] = white[i];
  }
  else{
    for(i=0;i<4;i++) linecolor[i] = black[i];
  }
  /**/
  glLineWidth(2.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, linecolor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, linecolor);
  /**/
  if(fbz == 1){
    /**/
    glBegin(GL_LINES);
    for(ibzl = 0; ibzl < nbzl; ++ibzl){
      for(i = 0; i< 2; ++i){
	for(j = 0; j < 3; ++j) bzl2[j] = rot[j][0] * bzl[ibzl][i][0]
				 +       rot[j][1] * bzl[ibzl][i][1]
				 +       rot[j][2] * bzl[ibzl][i][2]
				 + trans[j];
	glVertex3fv(bzl2);
      }
    }
    glEnd();
    /**/  
  }
  else{
    /**/    
    for(i = 0; i < 3; ++i){
      for(j = 0; j < 3; ++j){
	bvec2[i][j] = rot[j][0] * bvec[i][0]
	  +           rot[j][1] * bvec[i][1]
	  +           rot[j][2] * bvec[i][2];
      }
    }
    /**/
    glBegin(GL_LINE_STRIP);
    for(i=0;i<3;++i) bzl2[i] = trans[i]                                          ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i]                            ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i]              ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[1][i]               + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]               + bvec2[1][i]              ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]                                          ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]                             + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i]               + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i]               + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i]                            ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]                                          ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]                             + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]               + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i]               + bvec2[1][i]              ; glVertex3fv(bzl2);
    for(i=0;i<3;++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i]              ; glVertex3fv(bzl2);
    glEnd();
    /**/
  }
  /**/
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
  /**/
} /* draw bz_lines */
/*!
  Draw color scale
*/
void draw_colorbar()
{
  int i, j;
  GLfloat mat2, barcolor[4];
  GLfloat colorbar[13][3] = {
    { 0.0, 0.0, 1.0 }, 
    { -1.0, -1.0,       0.0 }, 
    { -1.0, -1.0 - 0.1, 0.0 }, 
    { -0.5, -1.0,       0.0 }, 
    { -0.5, -1.0 - 0.1, 0.0 }, 
    {  0.0, -1.0,       0.0 }, 
    {  0.0, -1.0 - 0.1, 0.0 }, 
    {  0.5, -1.0,       0.0 }, 
    {  0.5, -1.0 - 0.1, 0.0 }, 
    {  1.0, -1.0,       0.0 }, 
    {  1.0, -1.0 - 0.1, 0.0 }, 
    {  0.0, -1.0,       0.00001 }, 
    {  0.0, -1.0 - 0.1, 0.00001 }
  };
  /**/
  if(fcscl == 1 || fcscl == 2){
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(colorbar[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    glVertex3fv(colorbar[1]);
    glVertex3fv(colorbar[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
    glVertex3fv(colorbar[3]);
    glVertex3fv(colorbar[4]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glVertex3fv(colorbar[5]);
    glVertex3fv(colorbar[6]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
    glVertex3fv(colorbar[7]);
    glVertex3fv(colorbar[8]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    glVertex3fv(colorbar[9]);
    glVertex3fv(colorbar[10]);
    glEnd();
    /**/
    /* if(nodeline == 1){ */
    /*   glLineWidth(2.0); */
    /*   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black); */
    /*   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black); */
    /*   glBegin(GL_LINES); */
    /*   glVertex3fv(colorbar[11]); */
    /*   glVertex3fv(colorbar[12]); */
    /*   glEnd(); */
    /* } */
  }
  else if(fcscl == 4){
    for(i = 0; i <= 60; i ++){
      /**/
      mat2 = (GLfloat)i / 60.0 * 6.0;
      /**/
      if(mat2 <= 1.0) {
	for(j=0;j<4;++j) barcolor[j] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
      }
      else if(mat2 <= 2.0){
	mat2 = mat2 - 1.0;
	for(j=0;j<4;++j) barcolor[j] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
      }
      else if(mat2 <= 3.0){
	mat2 = mat2 - 2.0;
	for(j=0;j<4;++j) barcolor[j] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
      }
      else if(mat2 <= 4.0){
	mat2 = mat2 - 3.0;
	for(j=0;j<4;++j) barcolor[j] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
      }
      else if(mat2 <= 5.0){
	mat2 = mat2 - 4.0;
	for(j=0;j<4;++j) barcolor[j] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
      }
      else{
	mat2 = mat2 - 5.0;
	for(j=0;j<4;++j) barcolor[j] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
      }
      /**/
    glBegin(GL_TRIANGLES);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, barcolor);
    glNormal3fv(colorbar[0]);
    glVertex3f(0.15 * cos((GLfloat)(i+1) / 60.0 * 6.283185307),
    	       0.15 * sin((GLfloat)(i+1) / 60.0 * 6.283185307) - 1.0, 0.0);
    glVertex3f(0.15 * cos((GLfloat)i / 60.0 * 6.283185307),
    	       0.15 * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
    glVertex3f(0.2  * cos((GLfloat)i / 60.0 * 6.283185307),
    	       0.2  * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
    glVertex3f(0.2  * cos((GLfloat)i / 60.0 * 6.283185307),
    	       0.2  * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
    glVertex3f(0.2 * cos((GLfloat)(i+1) / 60.0 * 6.283185307),
    	       0.2 * sin((GLfloat)(i+1) / 60.0 * 6.283185307) - 1.0, 0.0);
    glVertex3f(0.15 * cos((GLfloat)(i+1) / 60.0 * 6.283185307),
    	       0.15 * sin((GLfloat)(i+1) / 60.0 * 6.283185307) - 1.0, 0.0);
    glEnd();
    }
  }
  else{
  }
} /* draw_colorbar */
/*!
Draw points for the stereogram
*/
void draw_circles(){
  int i;
  GLfloat r;
  /**/
  r = 0.05;
  /**/
  if(blackback == 1){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
  }
  else{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
  }
  /**/
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(0.7, scl, 0.0);
  for(i = 0; i <= 20; i ++){
    glVertex3f(r * cos((GLfloat)i / 20.0 * 6.283185307) + 0.7, 
	       r * sin((GLfloat)i / 20.0 * 6.283185307) + scl, 0.0);
  }
  glEnd();
  /**/
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(-0.7, scl, 0.0);
  for(i = 0; i <= 20; i ++){
    glVertex3f(r * cos((GLfloat)i / 20.0 * 6.283185307) - 0.7, 
	       r * sin((GLfloat)i / 20.0 * 6.283185307) + scl, 0.0);
  }
  glEnd();
}
/*!
  Glut Display function
*/
void display()
{
  GLfloat pos[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat amb[] = {0.2, 0.2, 0.2, 0.0};
  double dx, theta, posz, phi;
  GLfloat pos1[4], pos2[4];
  /**/
  if(lstereo == 2){
    theta = 3.1416 /180.0 * 2.50;
    posz = 5.0;
    dx = 0.7;
    phi = atan(posz/dx) -theta;
    theta = (3.1416 * 0.50 - phi) / 3.1416 * 180.0;
    /**/
    pos1[0] = posz * cos(phi) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * sin(phi);
    pos1[3] = 1.0;
    /**/
    pos2[0] = - posz * cos(phi) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * sin(phi);
    pos2[3] = 1.0;
  }
  else if(lstereo == 3){
    theta = -3.1416 /180.0 * 2.0;
    posz = 5.0;
    dx = - 0.7;
    /**/
    pos1[0] = posz * sin(theta) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * cos(theta);
    pos1[3] = 1.0;
    /**/
    pos2[0] = - posz * sin(theta) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * cos(theta);
    pos2[3] = 1.0;
  }
  else{
    theta = 0.0;
    dx = 0.0;
  }
  /*
      Initialize
  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /* 
     Set view point & view line
  */
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /* 
     Set position of light 
  */
  if(lstereo == 1){
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    /*
      Draw color scale
    */
    if(lcolorbar == 1) draw_colorbar();
  }
  else{
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    draw_circles();
    glTranslated(-dx, 0.0, 0.0);
    glRotated(theta, 0.0, 1.0, 0.0);
  }
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  /* 
     Rotation & Zoom
  */
  glScaled(scl, scl, scl);
  /* 
     Draw Brillouin zone boundaries 
  */
  draw_bz_lines();
  /* 
     Draw Fermi surfaces 
  */
  draw_fermi();
  /*
    Draw the second
   */
  if(lstereo != 1){
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos2);
    /**/
    glTranslated(dx, 0.0, 0.0);
    glRotated(-theta, 0.0, 1.0, 0.0);
    /**/
    glScaled(scl, scl, scl);
    draw_bz_lines();
    draw_fermi();
    /**/
    glPopMatrix();
  }
  /**/
  glutSwapBuffers();
} /* display */
/*!
  Window resize
  @param[in] w Window width
  @param[in] h Window height
*/
void resize(int w, int h)
{
  /*
    Scale of translation of mousepointer
  */
  sx = 1.0 / (GLfloat)w;
  sy = 1.0 / (GLfloat)h;
  /**/
  glViewport(0, 0, w, h);
  /**/
  glMatrixMode(GL_PROJECTION);
  /**/
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
  /**/
  glMatrixMode(GL_MODELVIEW);
} /* end resize */
/*!
  Idling
*/
void idle(void)
{
  glutPostRedisplay();
} /* idle */
/*!
 Glut mouse function
 @param[in] button  pushed button
 @param[in] state   down or upor ?
 @param[in] x       position of mouse cursor
 @param[in] y       position of mouse cursor
*/
void mouse(int button, int state, int x, int y)
{
  switch (button)	 {
    /*	
      Drag 
     */	
  case GLUT_LEFT_BUTTON:
	  switch (state) {
	  case GLUT_DOWN:
		  /* Start of animation */
		  glutIdleFunc(idle);
		  /* Record drag start point */
		  cx = x;
		  cy = y;
		  break;
	  case GLUT_UP:
		  /* End of animation */
		  glutIdleFunc(0);
		  break;
	  default:
		  break;
	  }
    break;
    /*
      Zoom up
     */
  case MOUSE_SCROLL_UP:
    switch (state) {
    case GLUT_DOWN:
      scl = scl * 1.1;
      glutPostRedisplay();
      break;
    case GLUT_UP:
      break;
    default:
      break;
    }
    break;
    /*
      Zoom down
     */
  case MOUSE_SCROLL_DOWN:
    switch (state) {
    case GLUT_DOWN:
      scl = scl * 0.9;
      glutPostRedisplay();
      break;
    case GLUT_UP:
      break;
    default:
      break;
    }
    break;
    /*
      No pushing
     */
  default:
    break;
  }
} /* end mouse */
/*!
  Glut motion function
  @param[in] x position of cursor
  @param[in] y position of cursor
*/
void motion(int x, int y)
{
  int i, j;
  GLfloat dx, dy, a, rot0[3][3], rot1[3][3], ax, ay;
  /*
    Translation of mousepointer from starting point
  */
  dx = (x - cx) * sx;
  dy = (y - cy) * sy;
  /**/
  /*
    Distanse from starting point
  */
  a = sqrt(dx * dx + dy * dy);
  /**/
  if (lmouse == 1){
	  /**/
	  if (a != 0.0) {
		  /*
			Compute rotational matrix from translation of mousepointer
			*/
		  ax = -dy;
		  ay = dx;
		  /**/
		  a = a * 10.0;
		  /**/
		  rot0[0][0] = (ax * ax + ay * ay * cos(a)) / (ax * ax + ay * ay);
		  rot0[0][1] = ax * ay * (cos(a) - 1.0) / (ax * ax + ay * ay);
		  rot0[0][2] = ay * sin(a) / sqrtf(ax * ax + ay * ay);
		  rot0[1][0] = ax * ay * (cos(a) - 1.0) / (ax * ax + ay * ay);
		  rot0[1][1] = (ax * ax * cos(a) + ay * ay) / (ax * ax + ay * ay);
		  rot0[1][2] = ax * sin(a) / sqrtf(ax * ax + ay * ay);
		  rot0[2][0] = -ay * sin(a) / sqrtf(ax * ax + ay * ay);
		  rot0[2][1] = -ax * sin(a) / sqrtf(ax * ax + ay * ay);
		  rot0[2][2] = cos(a);
		  /**/
		  for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) rot1[i][j] = rot[i][j];
		  /**/
		  for (i = 0; i < 3; i++){
			  for (j = 0; j < 3; j++){
				  rot[i][j] = rot0[i][0] * rot1[0][j]
					  + rot0[i][1] * rot1[1][j]
					  + rot0[i][2] * rot1[2][j];
			  }
		  }
	  }
  }
  else if (lmouse == 2){
	  scl = scl * exp(- dy);
  }
  else{
	  trans[0] = trans[0] + dx;
	  trans[1] = trans[1] - dy;
  }
  cx = x;
  cy = y;
  /**/    
} /* motion */
/*!
  Glut keyboard function
  @param[in] key key Typed key
  @param[in] x
  @param[in] y
*/
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  }
} /* keyboard */
/*!
  Glut special key function
  @param[in] key typed special key
  @param[in] x
  @param[in] y
*/
void special_key(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    trans[1] = trans[1] + 0.1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    trans[1] = trans[1] - 0.1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    /**/
    trans[0] = trans[0] + 0.1;
    glutPostRedisplay();
    break;
    /**/
  case GLUT_KEY_LEFT:
    /**/
    trans[0] = trans[0] - 0.1;
    glutPostRedisplay();
    break;
    /**/
  }
} /* special_key */
/*!
  Main menu
  @param[in] value Selected menu
*/
void main_menu(int value){
  /**/
  int ib, i1, i2, i3, ierr;
  GLfloat emin, emax;
  /**/
  if(value == 2){
    /*
      Shift Fermi energy
    */
    emin =  100000.0;
    emax = -100000.0;
    for (ib = 0; ib < nb; ++ib) {  
      for (i1 = 0; i1 < ng[0]; ++i1) {
	for (i2 = 0; i2 < ng[1]; ++i2) {
	  for (i3 = 0; i3 < ng[2]; ++i3) {
	    eig[ib][i1][i2][i3] = eig[ib][i1][i2][i3] + def;
	    if(emin > eig[ib][i1][i2][i3]) emin = eig[ib][i1][i2][i3];
	    if(emax < eig[ib][i1][i2][i3]) emax = eig[ib][i1][i2][i3];
	  }
	}
      }
    }
    printf("Min  Max  E_F \n");
    printf("%f %f %f \n",emin, emax, def);
    printf("New Fermi energy : ");
    ierr = scanf("%f", &def);
    if(ierr == 0) printf("error ! reading ef");
    for (ib = 0; ib < nb; ++ib) {  
      for (i1 = 0; i1 < ng[0]; ++i1) {
	for (i2 = 0; i2 < ng[1]; ++i2) {
	  for (i3 = 0; i3 < ng[2]; ++i3) {
	    eig[ib][i1][i2][i3] = eig[ib][i1][i2][i3] - def;
	  }
	}
      }
    }
    query = 1;
    fermi_patch();
    query = 0;
    fermi_patch();
    calc_nodeline();
    /**/
    max_and_min();
    /**/
    glutPostRedisplay();
  }
  else if(value == 9){
    /*
      Exit
     */
    printf("\nExit. \n\n");
    free(eig);
    free(mat);
    free(ntri);
    free(draw_band);
    free(nmlp);
    free(kvp);
    free(matp);
    free(clr);
    free(bzl);
    free(nnl);
    free(kvnl);
    exit(0);
  }
}
/*!
 Change mouse function
 @param[in] value Selected menu
*/
void menu_mouse(int value){
	/**/
	if (value == 1 && lmouse != 1){
		printf("Mouse drag : Rotate \n\n");
		lmouse = 1;
		glutPostRedisplay();
	}
	if (value == 2 && lmouse != 2){
		printf("Mouse drag : Scale \n\n");
		lmouse = 2;
		glutPostRedisplay();
	}
	if (value == 3 && lmouse != 3){
		printf("Mouse drag : Translate \n\n");
		lmouse = 3;
		glutPostRedisplay();
	}
	/**/
} /* menu_band */
/*!
  On / Off band
  @param[in] value Selected menu
*/
void menu_band(int value){
  /**/
  if(draw_band[value] == 0){
    printf("band # %d : On \n", value + 1);
    draw_band[value] = 1;
  }
  else{
    printf("band # %d : Off \n", value + 1);
    draw_band[value] = 0;
  }
  glutPostRedisplay();
  /**/
} /* menu_band */
/*!
  Change background color
  @param[in] value Selected menu
*/
void menu_bgcolor(int value){
  if(value == 1 && blackback != 1){
    printf("Background color becomes BLACK. \n");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    blackback = 1;
    glutPostRedisplay();
  }
  else if(value == 2 && blackback != 0){
      printf("Background color becomes WHITE. \n");
      glClearColor(1.0, 1.0, 1.0, 0.0);
      blackback = 0;
  }
/**/
}/* bgcolor change*/
/*!
  Change color scale mode
  @param[in] value Selected menu
*/
void menu_colorscale(int value){
  /**/
  if(value == 1 && fcscl != 1){
    fcscl = 1;
    printf("#####  Automatic color scale mode  ##### \n");
    max_and_min();
    glutPostRedisplay();
  }
  else if(value == 2 && fcscl != 2){
    fcscl = 2;
    printf("#####  Manual color scale mode  ##### \n");
    max_and_min();
    glutPostRedisplay();
  }
  else if(value == 3 && fcscl != 3){
    fcscl = 3;
    printf("#####  Unicolor scale mode  ##### \n");
    max_and_min();
    glutPostRedisplay();
  }
  else if(value == 4 && fcscl != 4){
    fcscl = 4;
    printf("#####  Periodic color scale mode  ##### \n");
    max_and_min();
    glutPostRedisplay();
  }
  /**/
} /* menu_colorscale */
/*!
  Change Brillouin zone
  @param[in] value Selected menu
*/
void menu_bzmode(int value){
  if(value == 1 && fbz != 1){
    fbz = 1;
    printf("#####  First Brillouin zone mode  ##### \n\n");
    free(kvp);
    free(nmlp);
    free(clr);
    free(matp);
    free(kvnl);
    /**/
    query = 1;
    fermi_patch();
    query = 0;
    fermi_patch();
    calc_nodeline();
    /**/
    max_and_min();
    /**/
    glutPostRedisplay();
  }
  else if(value == 2 && fbz != -1){
    fbz = -1;
    printf("#####  Primitive Brillouin zone mode  ##### \n\n");
    free(kvp);
    free(nmlp);
    free(clr);
    free(matp);
    free(kvnl);
    /**/
    query = 1;
    fermi_patch();
    query = 0;
    fermi_patch();
    calc_nodeline();
    /**/
    max_and_min();
    /**/
    glutPostRedisplay();
  }
} /* menu_bzmode */
/*!
  On/Off Node line
  @param[in] value Selected menu
*/
void menu_nodeline(int value){
  if(value == 1 && nodeline != 1){
    printf("Nodeline : on \n\n");
    nodeline = 1;
    glutPostRedisplay();
  }
  else if(value == 2 && nodeline != 0){
    printf("Nodeline : off \n\n");
    nodeline = 0;
    glutPostRedisplay();
  }
  /**/
} /* menu_nodeline */
/*!
  Tern stereogram
  @param[in] value Selected menu
*/
void menu_stereo(int value){
  if(value == 1 && lstereo != 1){
    printf("Stereo : Off. \n\n");
    lstereo = 1;
    glutPostRedisplay();
  }
  if(value == 2 && lstereo != 2){
    printf("Stereo : On. Parallel eyes \n\n");
    lstereo = 2;
    glutPostRedisplay();
  }
  if(value == 3 && lstereo != 3){
    printf("Stereo : On. Crossed eyes \n\n");
    lstereo = 3;
    glutPostRedisplay();
  }
} /* menu_stereo */
/*!
  On/Off Colorbar
  @param[in] value Selected menu
*/
void menu_colorbar(int value){
  if(value == 1 && lcolorbar != 1){
    printf("Color bar : on \n\n");
    lcolorbar = 1;
    glutPostRedisplay();
  }
  if(value == 2 && lcolorbar != 0){
    printf("Color bar : off \n\n");
    lcolorbar = 0;
    glutPostRedisplay();
  }
} /* menu_colorbar */
/*!
  Change tetrahedron
  @param[in] value Selected menu
*/
void menu_tetra(int value){
  /**/
  if(value != itet){
    printf("Tetra patern %d \n", value + 1);
    itet = value;
    free(kvp);
    free(nmlp);
    free(clr);
    free(matp);
    free(kvnl);
    init_corner();
    query = 1;
    fermi_patch();
    query = 0;
    fermi_patch();
    calc_nodeline();
    max_and_min();
    glutPostRedisplay();
  }
} /* menu_tetra */
/*!
  Glut init function
*/
void init(void)
{
  int ib;
  int ibandmenu, ibgmenu, icsmenu, ibzmenu, inlmenu, 
    icbmenu, itetmenu, istereomenu, imousemenu;
  char ibstr[20]={0};
  /**/
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  /* Stereogram */
  imousemenu = glutCreateMenu(menu_mouse);
  glutAddMenuEntry("Rotate", 1);
  glutAddMenuEntry("Scale", 2);
  glutAddMenuEntry("Translate", 3);
  /* Band menu */
  ibandmenu = glutCreateMenu(menu_band);
  for(ib =0;ib < nb; ib++){
    sprintf(ibstr,"band # %d",ib + 1);
    glutAddMenuEntry(ibstr, ib);
  }
  /* Background color */
  ibgmenu = glutCreateMenu(menu_bgcolor);
  glutAddMenuEntry("Black",1);
  glutAddMenuEntry("White",2);
  /* Color scale mode */
  icsmenu = glutCreateMenu(menu_colorscale);
  glutAddMenuEntry("Auto",1);
  glutAddMenuEntry("Manual",2);
  glutAddMenuEntry("Unicolor",3);
  glutAddMenuEntry("Periodic",4);
  /* Brillouin zone */
  ibzmenu = glutCreateMenu(menu_bzmode);
  glutAddMenuEntry("First Brillouin zone",1);
  glutAddMenuEntry("Primitive Brillouin zone",2);
  /* Nodeline on/off */
  inlmenu = glutCreateMenu(menu_nodeline);
  glutAddMenuEntry("On",1);
  glutAddMenuEntry("Off",2);
  /* Colorbar on/off */
  icbmenu = glutCreateMenu(menu_colorbar);
  glutAddMenuEntry("On",1);
  glutAddMenuEntry("Off",2);
  /* Stereogram */
  istereomenu = glutCreateMenu(menu_stereo);
  glutAddMenuEntry("None",1);
  glutAddMenuEntry("Parallel",2);
  glutAddMenuEntry("Cross",3);
  /* Tetrahedron */
  itetmenu = glutCreateMenu(menu_tetra);
  for(ib =0;ib < 16; ib++){
    sprintf(ibstr,"tetra # %d",ib + 1);
    glutAddMenuEntry(ibstr,ib);
  }
  /* 
     Main menu 
  */
  glutCreateMenu(main_menu);
  glutAddSubMenu("Band",ibandmenu);
  glutAddSubMenu("Mouse Drag", imousemenu);
  glutAddMenuEntry("Shift Fermi energy", 2);
  glutAddSubMenu("Background color",ibgmenu);
  glutAddSubMenu("Color scale mode",icsmenu);
  glutAddSubMenu("Brillouin zone",ibzmenu);
  glutAddSubMenu("Node lines",inlmenu);
  glutAddSubMenu("Color bar On/Off",icbmenu);
  glutAddSubMenu("Stereogram",istereomenu);
  glutAddSubMenu("Tetrahedron",itetmenu);
  glutAddMenuEntry("Exit",9);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
} /* init */
/*!
  Main routine
*/
int main(int argc, char *argv[])
{
  /**/
  read_file(argv[1]);
  init_corner();
  bragg_vector();
  /**/
  query = 1;
  bz_lines();
  query = 0;
  bz_lines();
  /**/
  max_and_min_bz();
  /**/
#pragma omp parallel 
  printf("Threads : %d \n", omp_get_num_threads());
  /**/
  printf("\n#####  First Brillouin zone mode  ##### \n\n");
  query = 1;
  fermi_patch();
  query = 0;
  fermi_patch();
  calc_nodeline();
  /**/
  printf("\n#####  Full color scale  ##### \n\n");
  max_and_min();
  /*
    Description
  */
  printf("\n#####  How to handle  ##### \n\n");
  printf("          mouse drag : Rotate objects \n");
  printf("          mousewheel : Resize objects \n");
  printf("           cursorkey : Move objects \n");
  printf("  mouse right button : Menu \n");
  printf("\n");
  /**/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE  | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_key);
  init();
  glutMainLoop();
  return 0;
} /* main */
