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
\mainpage FermiSurfer Main Page

Fermisurfer displays Fermi surfaces 
with a color plot of the arbitraly matrix element

\section Definition
 
- \f$\varepsilon_{n k}\f$ : Energy
- \f$\Delta_{n k}\f$ : Any \f$(n, k)\f$-dependent value for the color plot.
 
\section sec_routine Important routines
- Main routine: main()

\section file_sec Important files
- Main routine : fermisurfer.c
- Global valiable : variable.h
*/
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

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

/**
 Glut init function
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
 Main routine
*/
int main(
  int argc /**< [in] */, 
  char *argv[] /**< [in] */)
{
  printf("\n");
  printf("########################################\n");
  printf("##                                    ##\n");
  printf("##  Welocome to FermiSurfer ver. 1.8  ##\n");
  printf("##                                    ##\n");
  printf("########################################\n");
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
  /**/
  query = 1;
  bz_lines();
  query = 0;
  bz_lines();
  /**/
  max_and_min_bz();
  /**/
  query = 1;
  fermi_patch();
  query = 0;
  fermi_patch();
  calc_nodeline();
  /**/
  max_and_min();
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
