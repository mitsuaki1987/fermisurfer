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
#pragma once

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
 Input variables
*/
int ng0[3];         /**< k-point grid in the input file*/
int lshift;         /**< Switch for shifted Brillouin zone */
int shiftk[3];      /**< Wherether k-grid is shifted or not*/
int nb;             /**< The number of Bands                       */
GLfloat bvec[3][3]; /**< Resiplocal lattice vector                 */
GLfloat ****eig0;    /**< Eigenvalues    [nb][ng0[0]][ng0[1]][ng0[2]]  */
GLfloat ****mat0;    /**< Matrix element [nb][ng0[0]][ng0[1]][ng0[2]]  */
/*
 Interpolation
*/
int ng[3];          /**< \b Interpolated k-grids */
GLfloat ****eig;    /**< Eigenvalues    [nb][ng[0]][ng[1]][ng[2]]  */
GLfloat ****mat;    /**< Matrix element [nb][ng[0]][ng[1]][ng[2]]  */
int interpol;       /**< Ration of interpolation*/
/*
 Switch for some modes
*/
int blackback; /**< Switch for black background */
int fcscl;     /**< Switch for full color scale mode */
int fbz;       /**< Switch for 1st Brillouin zone mode */
int nodeline;  /**< Switch for node lines */
int lcolorbar; /**< Switch for colorbar */
int lstereo;   /**< Switch for the stereogram */
int lmouse;    /**< Switch for the mouse function */
int lsection;    /**< Switch for the 2D Fermi lines*/
/*
 Menu
*/
int imenu_band, imenu_interpol, imenu_bgcolor, imenu_colorscale, imenu_bzmode, imenu_section, 
imenu_nodeline, imenu_colorbar, imenu_tetra, imenu_stereo, imenu_mouse, imenu;
/*
 Variables for Brillouin zone boundaries
*/
int nbzl;             /**< The number of Lines of 1st Brillouin zone */
GLfloat bzl[676][2][3];       /**< Lines of 1st BZ [nbzl(max:26*26=676)][2][3] */
GLfloat bragg[26][3]; /**< Bragg plane vectors */
GLfloat brnrm[26];    /**< Norms of Bragg plane vectors */
/*
 Variables for patchs
*/
int *ntri;          /**< The number of triangle patch       [nb] */
int **ntri_th;      /**< The number of triangle patch in each thread [nb] */
int *draw_band;     /**< Switch for drawn bands    [nb] */
GLfloat ***nmlp;    /**< Normal vector of patchs   [nb][ntri][3] */
GLfloat ****kvp;    /**< K-vectors of points       [nb][ntri][3][3] */
GLfloat ***nmlp_rot;    /**< Normal vector of patchs   [nb][ntri][3] */
GLfloat ****kvp_rot;    /**< K-vectors of points       [nb][ntri][3][3] */
GLfloat ***matp;    /**< Matrix elements of points [nb][ntri][3] */
GLfloat ****clr;    /**< Colors of points          [nb][ntri][3][4] */
int itet;       /**< Counter for tetrahedron */
/*
  Variables for nodeline
*/
int *nnl; /**< The number of nodeline */
GLfloat ****kvnl; /**< K-vector of nodeline [nb][nnl][2][3] */
GLfloat ****kvnl_rot; /**< K-vector of nodeline [nb][nnl][2][3] */
/*
 2D Fermi line
*/
GLfloat secvec[3]; /**< k-vector to define section*/
GLfloat secscale; /**0.0 or 1.0*/
GLfloat axis2d[2][3]; /**< k-vector to define section*/
int *n2d;
GLfloat ****kv2d;/**< k-vector for 2D plot[nb][n2d][2][2] */
GLfloat ****clr2d; /**< Matrix element for 2D plot[nb][n2d][2][4] */
int nbzl2d;             /**< The number of Lines of 1st Brillouin zone */
GLfloat bzl2d[26][3];       /**< Lines of 1st BZ [nbzl2d(max:26)][3] */
/*
  Variables for mouse  & cursorkey
*/
GLfloat sx; /**< Scale of mouse movement */
GLfloat sy; /**< Scale of mouse movement */
int cx; /**< Starting point of drug */
int cy; /**< Starting point of drug */
GLfloat scl; /**< Initial scale */
GLfloat trans[3];    /**< Translation */
GLfloat rot[3][3];  /**< Rotation matrix */
GLfloat thetax, thetay, thetaz;
/*
 Colors
*/
GLfloat   black[4]; /**<  Black color code */
GLfloat   white[4]; /**<  White color code */
GLfloat    cyan[4]; /**<  Cyan color code */
GLfloat magenta[4]; /**<  Magenta color code */
GLfloat  yellow[4]; /**<  Yellow color code */
GLfloat     red[4]; /**<  Red color code */
GLfloat   green[4]; /**<  Green color code */
GLfloat    blue[4]; /**<  Blue color code */
/*
 Others
*/
int query; /**< Query switch */
int corner[6][4]; /**< Corners of tetrahedron */
GLfloat EF; /**< Fermi energy */
enum
{
  MOUSE_SCROLL_UP = 3, /**< Mouse wheel up */
  MOUSE_SCROLL_DOWN = 4  /**< Mouse wheel down */
};
int nthreads;/**< Number of OpenMP threads */
