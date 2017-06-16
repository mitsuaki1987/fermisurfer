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
- Main routine : fermisurfer.c
- Global valiable : variable.h

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
#if defined(HAVE_GL_GLUT_H)
#include <GL/glut.h>
#elif defined(HAVE_GLUT_GLUT_H)
#include <GLUT/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "variable.h"
#include "read_file.h"
#include "menu.h"
#include "operation.h"
#include "initialize.h"
#include "draw.h"
#include "fermi_patch.h"
#include "calc_nodeline.h"
#include "bz_lines.h"
#include "free_patch.h"
#include "section.h"

#if defined(_OPENMP)
#include <omp.h>
#endif

/**
 @brief Initialize GLUT canvas
*/
void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  /* Menu */
  FS_CreateMenu();
} /* init */
/**
 @brief Main routine of FermiSurfer

 Refer: ::query

 Modify: ::query, ::nthreads
*/
int main(
  int argc, //!< [in]
  char *argv[] //!< [in] Input file name
)
{
  printf("\n");
  printf("##########################################\n");
  printf("##                                      ##\n");
  printf("##  Welocome to FermiSurfer ver. %s  ##\n", VERSION);
  printf("##                                      ##\n");
  printf("##########################################\n");
  printf("\n");
  if (argc < 2) {
    printf("\n");
    printf("  Input file is not specified !\n");
    printf("    Press any key to exit.\n");
    getchar();
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
  initialize_val();
  /**/
  read_file(argv[1]);
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
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE  | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_key);
  glutMenuStateFunc(FS_ModifyMenu);
  init();
  glutMainLoop();
  return 0;
} /* main */
