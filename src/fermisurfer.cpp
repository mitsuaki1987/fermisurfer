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
#include <cstdlib>
#include <cstdio>
#include <cstring>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <wx/wx.h>
#include <wx/cmdline.h>
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
int color_scale = 1; //!< Switch for full color scale mode
int fbz = 1;         //!< Switch for 1st Brillouin zone mode
int nodeline = 0;    //!< Switch for node lines
int lcolorbar = 1;   //!< Switch for colorbar
int lstereo = 1;     //!< Switch for the stereogram
int lmouse = 1;      //!< Switch for the mouse function
int lsection = 0;    //!< Switch for the 2D Fermi lines
int lequator = 0;    //!< Switch for equator
int BZ_number[3] = { 1, 1, 1 };
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
int *draw_band;     //!< Switch for drawn bands [::nb]
GLfloat**rgb_band;     //!< Switch for drawn bands [::nb]
GLfloat ****nmlp;    //!< Normal vector of patchs [::nb][::ntri][3][3]
GLfloat ****kvp;    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
GLfloat***** arw;
GLfloat **nmlp_rot; //!< Normal vector of patchs [::nb][::ntri*3*3]
GLfloat **kvp_rot;  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
GLfloat** arw_rot;
GLfloat ****matp;    //!< Matrix elements of points [::nb][::ntri][3][3]
GLfloat **clr;      //!< Colors of points [::nb][::ntri*3*4]
int itet = 0;           //!< Counter for tetrahedron
GLfloat side = 1.0;       //!< Which side is lighted
GLfloat patch_max;  //!< Max value across patch
GLfloat patch_min;  //!< Max value across patch
/*
  Variables for nodeline
*/
int *nnl;             //!< The number of nodeline
GLfloat ****kvnl;     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
GLfloat **kvnl_rot; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
/*
 2D Fermi line
*/
GLfloat secvec[3];         //!< @f$k@f$-vector to define section
GLfloat secvec_fr[3];         //!< @f$k@f$-vector to define section
GLfloat secscale;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
GLfloat axis2d[2][3];      //!< @f$k@f$-vector to define section
int *n2d;                  //!< Number of line segment
GLfloat **kv2d;          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
GLfloat **clr2d;         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
int nbzl2d;                //!< The number of Lines of 1st Brillouin zone
GLfloat bzl2d[26][3];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
GLfloat bzl2d_proj[26][3]; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
GLfloat eqvec[3]; //!<  @f$k@f$-vector for equator
GLfloat eqvec_fr[3]; //!<  @f$k@f$-vector for equator
int *nequator;             //!< The number of equator
GLfloat ****kveq;     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
GLfloat **kveq_rot; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
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
GLfloat BackGroundColor[4] = { 0.0, 0.0, 0.0, 1.0 };//!< BackGround color code
GLfloat LineColor[4] = { 1.0, 1.0, 1.0, 1.0 };//!< Line color code
GLfloat SectionColor[4] = { 0.5f, 0.5f, 0.5f, 1.0 };//!< Line color code
GLfloat BarColor[5][4] = { { 0.0, 0.0, 1.0, 1.0 } , { 0.0, 1.0, 1.0, 1.0 } ,
{ 0.0, 1.0, 0.0, 1.0 } ,{ 1.0, 1.0, 0.0, 1.0 } ,{ 1.0, 0.0, 0.0, 1.0 } };
GLfloat arrow_f[2][3];
GLfloat arrow_c[2][3];
/*
 Others
*/
int corner[6][4]; //!< Corners of tetrahedron
GLfloat EF = 0.0;       //!< Fermi energy
GLfloat sphere_v[190][2][3]; //!< Vertices for wisreflame sphere
GLfloat sphere_r;//!< Radius of wireflame sphere
GLfloat sphere_c[3];//!< Center of wireflame sphere
enum
{
  MOUSE_SCROLL_UP = 3,  //!< Mouse wheel up
  MOUSE_SCROLL_DOWN = 4 //!< Mouse wheel down
};
int nthreads;//!< Number of OpenMP threads
/*
Batch mode
*/
wxString batch_name;
wxString frmsf_file_name;
int lbatch = 0;

MyFrame* myf;

wxTextCtrl* terminal;
int refresh_interpol = 0;
int refresh_patch = 1;
int refresh_color = 1;
int refresh_nodeline = 1;
int refresh_equator = 1;
int refresh_section = 1;
int skip_minmax = 0;
int windowx = 1100;
int windowy = 850;
/**
  @brief Glut Display function
  called by glutDisplayFunc
*/
void batch_draw()
{
  int iminmax;
  GLfloat minmax[3][2];

  printf("\n  Batch mode.\n");

  read_batch();
  refresh_patch_segment();
}
/**
 @brief Main routine of FermiSurfer

*/
bool MyApp::OnInit()
{
  int ierr, i0, i1, i2;

  if (!wxApp::OnInit()) return false;

  myf = new MyFrame(NULL, argv[1], wxDefaultPosition, wxSize(windowx, windowy));

  *terminal << wxT("\n");
  *terminal << wxT("#####  Welocome to FermiSurfer ver. ") << 
    wxT(VERSION) << wxT("  #####\n");
  *terminal << wxT("\n");
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
  *terminal << wxT("  Number of threads : ") << nthreads << wxT("\n");
  *terminal << wxT("\n");
  *terminal << wxT("  Initialize variables ...\n");
  *terminal << wxT("\n");
  /*
  Input from BXSF or FRMSF file
  */
  if (frmsf_file_name.AfterLast(wxUniChar('.')).CmpNoCase(wxT("bxsf")) == 0) {
    read_bxsf();
  }
  else {
    color_scale = read_file();
    if (color_scale == 0)color_scale = 4;
  }
  /*
   Malloc only nb
  */
  kvp = new GLfloat ***[nb];
  kvp_rot = new GLfloat *[nb];
  nmlp = new GLfloat * **[nb];
  nmlp_rot = new GLfloat *[nb];
  matp = new GLfloat * **[nb];
  clr = new GLfloat *[nb];
  arw = new GLfloat * ***[nb];
  arw_rot = new GLfloat *[nb];
  kvnl = new GLfloat * **[nb];
  kvnl_rot = new GLfloat *[nb];
  kv2d = new GLfloat * [nb];
  clr2d = new GLfloat * [nb];
  kveq = new GLfloat *** [nb];
  kveq_rot = new GLfloat *[nb];
  /**/
  interpol_energy();
  bragg_vector();
  init_corner();
  myf->modify_band();
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
  *terminal << wxT("\n");
  *terminal << wxT("  ##  How to handle  ###################\n");
  *terminal << wxT("\n");
  *terminal << wxT("              mouse drag : Rotate objects\n");
  *terminal << wxT("              mousewheel : Resize objects\n");
  *terminal << wxT("    cursorkey or w,a,s,d : Move objects\n");
  *terminal << wxT("\n");
  //
  // Variables for wireflame sphere
  //
  for (i0 = 0; i0 < 190; i0++)
    for (i1 = 0; i1 < 2; i1++)
      for (i2 = 0; i2 < 3; i2++)
        sphere_v[i0][i1][i2] = 0.0f;
  for (i0 = 0; i0 < 3; i0++) sphere_c[i0] = 0.0f;
  sphere_r = 0.0f;
  //
  if (lbatch == 1) {
    batch_draw();
  }
  return true;
} /* main */

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  wxApp::OnInitCmdLine(parser);

  parser.AddParam("FRMSF file to plot.",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  parser.AddParam("Batch file",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddParam("Window Size x",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddParam("Window Size y",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  if (parser.GetParamCount() > 0) {
    frmsf_file_name = parser.GetParam(0);
    if (parser.GetParamCount() > 1) {
      batch_name = parser.GetParam(1);
      lbatch = 1;
      if (parser.GetParamCount() > 2) {
        windowx = wxAtoi(parser.GetParam(2));
        if (parser.GetParamCount() > 3) {
          windowy = wxAtoi(parser.GetParam(3));
        }
      }
    }
  }

  return wxApp::OnCmdLineParsed(parser);
}
