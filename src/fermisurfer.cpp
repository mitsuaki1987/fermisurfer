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
 @brief Main routine
*/
/**@mainpage FermiSurfer Main Page

Fermisurfer displays Fermi surfaces 
with a color-plot of the arbitraly matrix element

@section Notation
 
- @f$\varepsilon_{n k}@f$ : Energy
- @f$\Delta_{n k}@f$ : Any @f$(n, k)@f$-dependent value for the color-plot.
- @f$N_b@f$ : Number of bands
 
@section sec_routine Important routines
- Main routine: main()
- Right click menu : FS_CreateMenu(), FS_ModifyMenu(int status)

@section sec_file Important files
- Main routine : fermisurfer.cpp
- Global valiable : variable.hpp

@section sec_flow Flow

- main() :
  - read_file() : Read .frmsf file
  - compute_patch_segment() : Compute patch and segment
    - fermi_patch() : Compute patches constructing Fermi surface
  - display() : Display figures with OpenGL

*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "read_file.hpp"
#include "menu.hpp"
#include "operation.hpp"
#include "initialize.hpp"
#include "draw.hpp"
#include "fermi_patch.hpp"
#include "calc_nodeline.hpp"
#include "bz_lines.hpp"
#include "free_patch.hpp"
#include "section.hpp"
#include "kumo.hpp"
#include "equator.hpp"
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <wx/wx.h>
#include "wx/cmdline.h"

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
};

IMPLEMENT_APP(MyApp)

/*
 Input variables
*/
int ng0[3];         //!< @f$k@f$-point grid in the input file
int shiftk[3];      //!< Wherether @f$k@f$-grid is shifted or not
int nb;             //!< The number of Bands
GLfloat avec[3][3]; //!< Direct lattice vector
GLfloat bvec[3][3]; //!< Reciprocal lattice vector
GLfloat ****eig0;   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
GLfloat *****mat0;   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]][3]
/*
 Interpolation
*/
int ng[3];        //!< @b Interpolated @f$k@f$-grids
GLfloat ****eig;  //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
GLfloat *****mat; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
GLfloat *****vf;  //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
int interpol = 1;     //!< Ratio of interpolation
/*
 Switch for some modes
*/
int blackback = 1;   //!< Switch for black background
int color_scale = 1; //!< Switch for full color scale mode
int fbz = 1;         //!< Switch for 1st Brillouin zone mode
int nodeline = 0;    //!< Switch for node lines
int lcolorbar = 1;   //!< Switch for colorbar
int lstereo = 1;     //!< Switch for the stereogram
int lmouse = 1;      //!< Switch for the mouse function
int lsection = 0;    //!< Switch for the 2D Fermi lines
int lside = 1;      //!< Switch for lighting
int lequator = 0;    //!< Switch for equator
/*
 Variables for Brillouin zone boundaries
*/
int nbzl;               //!< The number of Lines of 1st Brillouin zone
GLfloat bzl[676][2][3]; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
GLfloat bragg[26][3];   //!< Bragg plane vectors
GLfloat brnrm[26];      //!< Norms of Bragg plane vectors
GLfloat brnrm_min;     //!< Minimum scale of the reciplocal space
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
GLfloat ****matp;    //!< Matrix elements of points [::nb][::ntri][3][3]
GLfloat **clr;      //!< Colors of points [::nb][::ntri*3*4]
int itet = 0;           //!< Counter for tetrahedron
GLfloat side = 1.0;       //!< Which side is lighted
GLfloat patch_max[3];  //!< Max value across patch
GLfloat patch_min[3];  //!< Max value across patch
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
GLfloat scl = 1.0;       //!< Initial scale
GLfloat trans[3] = { 0.0, 0.0, 0.0 };  //!< Translation
GLfloat rot[3][3] = { { 1.0, 0.0, 0.0 },
                      { 0.0, 1.0, 0.0 },
                      { 0.0, 0.0, 1.0 } }; //!< Rotation matrix
GLfloat thetax = 0.0;    //!< Rotation angle
GLfloat thetay = 0.0;    //!< Rotation angle
GLfloat thetaz = 0.0;    //!< Rotation angle
GLfloat linewidth = 3.0; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
GLfloat   black[4] = { 0.0, 0.0, 0.0, 1.0 }; //!< Black color code
GLfloat    gray[4] = { 0.5f, 0.5f, 0.5f, 1.0 }; //!< Gray color code
GLfloat   wgray[4] = { 0.9f, 0.9f, 0.9f, 1.0 }; //!< Gray color code
GLfloat   bgray[4] = { 0.1f, 0.1f, 0.1f, 1.0 }; //!< Gray color code
GLfloat   white[4] = { 1.0, 1.0, 1.0, 1.0 }; //!< White color code
GLfloat    cyan[4] = { 0.0, 1.0, 1.0, 1.0 }; //!< Cyan color code
GLfloat magenta[4] = { 1.0, 0.0, 1.0, 1.0 }; //!< Magenta color code
GLfloat  yellow[4] = { 1.0, 1.0, 0.0, 1.0 }; //!< Yellow color code
GLfloat     red[4] = { 1.0, 0.0, 0.0, 1.0 }; //!< Red color code
GLfloat   green[4] = { 0.0, 1.0, 0.0, 1.0 }; //!< Green color code
GLfloat    blue[4] = { 0.0, 0.0, 1.0, 1.0 }; //!< Blue color code
/*
 Others
*/
int query;        //!< Query switch
int corner[6][4]; //!< Corners of tetrahedron
GLfloat EF = 0.0;       //!< Fermi energy
enum
{
  MOUSE_SCROLL_UP = 3,  //!< Mouse wheel up
  MOUSE_SCROLL_DOWN = 4 //!< Mouse wheel down
};
int nthreads;//!< Number of OpenMP threads
/*
Batch mode
*/
wxString window_name;
wxString batch_name;
wxString frmsf_file_name;
int lbatch;
/**
  @brief Glut Display function
  called by glutDisplayFunc
*/
void batch_draw()
{
  int iminmax;
  GLfloat minmax[3][2];

  printf("\n  Batch mode.\n");

  iminmax = read_batch(minmax);
  refresh_patch_segment();
  if (iminmax == 1) {
    if (color_scale == 3) {
      patch_min[0] = minmax[0][0];
      patch_max[0] = minmax[0][1];
      patch_min[1] = minmax[1][0];
      patch_max[1] = minmax[1][1];
      patch_min[2] = minmax[2][0];
      patch_max[2] = minmax[2][1];
    }
    else if (color_scale == 2) {
      patch_min[0] = minmax[0][0];
      patch_max[0] = minmax[0][1];
      patch_min[1] = minmax[1][0];
      patch_max[1] = minmax[1][1];
    }
    else {
      patch_min[0] = minmax[0][0];
      patch_max[0] = minmax[0][1];
    }
    paint();
  }
}
/**
 @brief Main routine of FermiSurfer

 Refer: ::query

 Modify: ::query, ::nthreads
*/
bool MyApp::OnInit()
{
  int ierr;

  if (!wxApp::OnInit()) return false;

  printf("\n");
  printf("###########################################\n");
  printf("##                                       ##\n");
  printf("##  Welocome to FermiSurfer ver. %s  ##\n", VERSION);
  printf("##                                       ##\n");
  printf("###########################################\n");
  printf("\n");
  if (argc < 2) {
    printf("\n");
    printf("  Input file is not specified !\n");
    printf("    Press any key to exit.\n");
    ierr = getchar();
    exit(-1);
  }
  /**/
#if defined(_OPENMP)
#pragma omp parallel 
#pragma omp master
  nthreads = omp_get_num_threads();
#else
  nthreads = 1;
#endif
  printf("  Number of threads : %d\n", nthreads);
  printf("\n");
  printf("  Initialize variables ...\n");
  printf("\n");
  /**/
  color_scale = read_file();
  if (color_scale == 0)color_scale = 4;
  interpol_energy();
  init_corner();
  bragg_vector();
  /*
   Brillouin zone
  */
  bz_lines();
  calc_2dbz();
  /**/
  max_and_min_bz();
  /**/
  compute_patch_segment();
  /*
    Description
  */
  printf("\n");
  printf("  ##  How to handle  ###################\n");
  printf("\n");
  printf("              mouse drag : Rotate objects\n");
  printf("              mousewheel : Resize objects\n");
  printf("               cursorkey : Move objects\n");
  printf("      mouse right button : Menu\n");
  printf("\n");
  /**/
  //if (argc > 4)glutInitWindowSize(atoi(argv[3]), atoi(argv[4]));
  new MyFrame(NULL, argv[1]);
  lbatch = 0;
  if (argc > 2) {
    lbatch = 1;
    window_name = argv[1];
    batch_name = argv[2];
    batch_draw();
  }
  //MyFrame::m_canvas->PostSizeEventToParent();
  return true;
} /* main */

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  wxApp::OnInitCmdLine(parser);

  parser.AddParam("FRMSF file to plot.",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  if (parser.GetParamCount())
    frmsf_file_name = parser.GetParam(0);

  return wxApp::OnCmdLineParsed(parser);
}