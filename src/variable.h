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
@brief Global variables
*/
#pragma once

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(WIN32)
#include <Windows.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif

/*
 Input variables
*/
int ng0[3];         //!< @f$k@f$-point grid in the input file
int lshift;         //!< Switch for shifted Brillouin zone
int shiftk[3];      //!< Wherether @f$k@f$-grid is shifted or not
int nb;             //!< The number of Bands
GLfloat avec[3][3]; //!< Direct lattice vector
GLfloat bvec[3][3]; //!< Reciprocal lattice vector
GLfloat ****eig0;   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
GLfloat ****mat0;   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]]
/*
 Interpolation
*/
int ng[3];       //!< @b Interpolated @f$k@f$-grids
GLfloat ****eig; //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
GLfloat ****mat; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
GLfloat *****vf; //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
int interpol;    //!< Ratio of interpolation
/*
 Switch for some modes
*/
int blackback; //!< Switch for black background
int fcscl;     //!< Switch for full color scale mode
int fbz;       //!< Switch for 1st Brillouin zone mode
int nodeline;  //!< Switch for node lines
int lcolorbar; //!< Switch for colorbar
int lstereo;   //!< Switch for the stereogram
int lmouse;    //!< Switch for the mouse function
int lsection;  //!< Switch for the 2D Fermi lines
int lside;     //!< Switch for lighting
int lequator;   //!< Switch for equator
/*
 Menu
*/
int imenu_band, imenu_interpol, imenu_background, imenu_colorscale, imenu_bzmode, imenu_section, 
imenu_nodeline, imenu_colorbar, imenu_tetra, imenu_stereo, imenu_mouse, imenu_light, 
imenu_line, imenu_shift, imenu_view, imenu_equator, imenu;
/*
 Variables for Brillouin zone boundaries
*/
int nbzl;               //!< The number of Lines of 1st Brillouin zone
GLfloat bzl[676][2][3]; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
GLfloat bragg[26][3];   //!< Bragg plane vectors
GLfloat brnrm[26];      //!< Norms of Bragg plane vectors
int nbragg;             //!< Number of Bragg plane og 1st BZ
/*
 Variables for patchs
*/
int *ntri;          //!< The number of triangle patch [::nb]
int **ntri_th;      //!< The number of triangle patch in each thread [::nb]
int *draw_band;     //!< Switch for drawn bands [::nb]
GLfloat ****nmlp;    //!< Normal vector of patchs [::nb][::ntri][3][3]
GLfloat ****kvp;    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
GLfloat **nmlp_rot; //!< Normal vector of patchs [::nb][::ntri*3*3]
GLfloat **kvp_rot;  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
GLfloat ***matp;    //!< Matrix elements of points [::nb][::ntri][3]
GLfloat **clr;      //!< Colors of points [::nb][::ntri*3*4]
int itet;           //!< Counter for tetrahedron
GLfloat side;       //!< Which side is lighted
/*
  Variables for nodeline
*/
int *nnl;             //!< The number of nodeline
GLfloat ****kvnl;     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
GLfloat **kvnl_rot; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
GLfloat **nmlnl;//!< Normal vector of nodeline [::nb][::nnl*2*3]
GLfloat **clrnl;//!< Color of nodeline [::nb][::nnl*2*4]
/*
 2D Fermi line
*/
GLfloat secvec[3];         //!< @f$k@f$-vector to define section
GLfloat secscale;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
GLfloat axis2d[2][3];      //!< @f$k@f$-vector to define section
int *n2d;                  //!< Number of line segment
GLfloat **kv2d;          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
GLfloat **clr2d;         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
GLfloat **nml2d;          //!< Normal-vector for 2D plot [::nb][::n2d*2*3]
int nbzl2d;                //!< The number of Lines of 1st Brillouin zone
GLfloat bzl2d[26][3];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
GLfloat bzl2d_proj[26][3]; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
GLfloat eqvec[3]; //!<  @f$k@f$-vector for equator
int *nequator;             //!< The number of equator
GLfloat ****kveq;     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
GLfloat **kveq_rot; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
GLfloat **nmleq;//!< Normal vector of equator [::nb][::nequator*2*3]
GLfloat **clreq;//!< Color of equator [::nb][::nequator*2*4]
/*
  Variables for mouse  & cursorkey
*/
GLfloat sx;        //!< Scale of mouse movement
GLfloat sy;        //!< Scale of mouse movement
int cx;            //!< Starting point of drug
int cy;            //!< Starting point of drug
GLfloat scl;       //!< Initial scale
GLfloat trans[3];  //!< Translation
GLfloat rot[3][3]; //!< Rotation matrix
GLfloat thetax;    //!< Rotation angle
GLfloat thetay;    //!< Rotation angle
GLfloat thetaz;    //!< Rotation angle
GLfloat linewidth; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
GLfloat   black[4]; //!< Black color code
GLfloat    gray[4]; //!< Gray color code
GLfloat   white[4]; //!< White color code
GLfloat    cyan[4]; //!< Cyan color code
GLfloat magenta[4]; //!< Magenta color code
GLfloat  yellow[4]; //!< Yellow color code
GLfloat     red[4]; //!< Red color code
GLfloat   green[4]; //!< Green color code
GLfloat    blue[4]; //!< Blue color code
/*
 Others
*/
int query;        //!< Query switch
int corner[6][4]; //!< Corners of tetrahedron
GLfloat EF;       //!< Fermi energy
enum
{
  MOUSE_SCROLL_UP = 3,  //!< Mouse wheel up
  MOUSE_SCROLL_DOWN = 4 //!< Mouse wheel down
};
int nthreads;//!< Number of OpenMP threads
