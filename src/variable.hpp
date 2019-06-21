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
#if ! defined(FERMISURFER_VARIABLE_H)
#define FERMISURFER_VARIABLE_H

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <wx/wx.h>
#include "menu.hpp"

/*
 Input variables
*/
extern int ng0[3];         //!< @f$k@f$-point grid in the input file
extern int shiftk[3];      //!< Wherether @f$k@f$-grid is shifted or not
extern int nb;             //!< The number of Bands
extern GLfloat avec[3][3]; //!< Direct lattice vector
extern GLfloat bvec[3][3]; //!< Reciprocal lattice vector
extern GLfloat ****eig0;   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
extern GLfloat *****mat0;   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]][3]
/*
 Interpolation
*/
extern int ng[3];        //!< @b Interpolated @f$k@f$-grids
extern GLfloat ****eig;  //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
extern GLfloat *****mat; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
extern GLfloat *****vf;  //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
extern int interpol;     //!< Ratio of interpolation
/*
 Switch for some modes
*/
extern int blackback;   //!< Switch for black background
extern int color_scale; //!< Switch for full color scale mode
extern int fbz;         //!< Switch for 1st Brillouin zone mode
extern int nodeline;    //!< Switch for node lines
extern int lcolorbar;   //!< Switch for colorbar
extern int lstereo;     //!< Switch for the stereogram
extern int lmouse;      //!< Switch for the mouse function
extern int lsection;    //!< Switch for the 2D Fermi lines
extern int lequator;    //!< Switch for equator
/*
 Variables for Brillouin zone boundaries
*/
extern int nbzl;               //!< The number of Lines of 1st Brillouin zone
extern GLfloat bzl[676][2][3]; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
extern GLfloat bragg[26][3];   //!< Bragg plane vectors
extern GLfloat brnrm[26];      //!< Norms of Bragg plane vectors
extern GLfloat brnrm_min;     //!< Minimum scale of the reciplocal space
extern int nbragg;             //!< Number of Bragg plane og 1st BZ
/*
 Variables for patchs
*/
extern int *ntri;          //!< The number of triangle patch [::nb]
extern int **ntri_th;      //!< The number of triangle patch in each thread [::nb]
extern int *draw_band;     //!< Switch for drawn bands [::nb]
extern GLfloat ****nmlp;    //!< Normal vector of patchs [::nb][::ntri][3][3]
extern GLfloat***** arw;
extern GLfloat ****kvp;    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
extern GLfloat **nmlp_rot; //!< Normal vector of patchs [::nb][::ntri*3*3]
extern GLfloat **kvp_rot;  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
extern GLfloat **arw_rot;
extern GLfloat ****matp;    //!< Matrix elements of points [::nb][::ntri][3][3]
extern GLfloat **clr;      //!< Colors of points [::nb][::ntri*3*4]
extern int itet;           //!< Counter for tetrahedron
extern GLfloat side;       //!< Which side is lighted
extern GLfloat patch_max;  //!< Max value across patch
extern GLfloat patch_min;  //!< Max value across patch
/*
  Variables for nodeline
*/
extern int *nnl;             //!< The number of nodeline
extern GLfloat ****kvnl;     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
extern GLfloat **kvnl_rot; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
/*
 2D Fermi line
*/
extern GLfloat secvec[3];         //!< @f$k@f$-vector to define section
extern GLfloat secvec_fr[3];         //!< @f$k@f$-vector to define section
extern GLfloat secscale;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
extern GLfloat axis2d[2][3];      //!< @f$k@f$-vector to define section
extern int *n2d;                  //!< Number of line segment
extern GLfloat **kv2d;          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
extern GLfloat **clr2d;         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
extern int nbzl2d;                //!< The number of Lines of 1st Brillouin zone
extern GLfloat bzl2d[26][3];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
extern GLfloat bzl2d_proj[26][3]; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
extern GLfloat eqvec[3]; //!<  @f$k@f$-vector for equator
extern GLfloat eqvec_fr[3]; //!<  @f$k@f$-vector for equator
extern int *nequator;             //!< The number of equator
extern GLfloat ****kveq;     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
extern GLfloat **kveq_rot; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
/*
  Variables for mouse  & cursorkey
*/
extern GLfloat sx;        //!< Scale of mouse movement
extern GLfloat sy;        //!< Scale of mouse movement
extern int cx;            //!< Starting point of drug
extern int cy;            //!< Starting point of drug
extern GLfloat scl;       //!< Initial scale
extern GLfloat trans[3];  //!< Translation
extern GLfloat rot[3][3]; //!< Rotation matrix
extern GLfloat thetax;    //!< Rotation angle
extern GLfloat thetay;    //!< Rotation angle
extern GLfloat thetaz;    //!< Rotation angle
extern GLfloat linewidth; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
extern GLfloat   black[4]; //!< Black color code
extern GLfloat    gray[4]; //!< Gray color code
extern GLfloat   wgray[4]; //!< Gray color code
extern GLfloat   bgray[4]; //!< Gray color code
extern GLfloat   white[4]; //!< White color code
extern GLfloat    cyan[4]; //!< Cyan color code
extern GLfloat magenta[4]; //!< Magenta color code
extern GLfloat  yellow[4]; //!< Yellow color code
extern GLfloat     red[4]; //!< Red color code
extern GLfloat   green[4]; //!< Green color code
extern GLfloat    blue[4]; //!< Blue color code
/*
 Others
*/
extern int query;        //!< Query switch
extern int corner[6][4]; //!< Corners of tetrahedron
extern GLfloat EF;       //!< Fermi energy
enum
{
  MOUSE_SCROLL_UP = 3,  //!< Mouse wheel up
  MOUSE_SCROLL_DOWN = 4 //!< Mouse wheel down
};
extern int nthreads;//!< Number of OpenMP threads
/*
Batch mode
*/
extern wxString batch_name;
extern wxString frmsf_file_name;
extern int lbatch;

extern wxTextCtrl* terminal;
extern int refresh_interpol;
extern int refresh_patch;
extern int refresh_color;
extern int refresh_nodeline;
extern int refresh_equator;
extern int refresh_section;
extern int skip_minmax;
extern MyFrame* myf;

#endif
