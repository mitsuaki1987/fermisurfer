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
@brief Create & modify right-click menu. And operate their function.
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
#include <math.h>
#include "variable.h"
#include "free_patch.h"
#include "fermi_patch.h"
#include "calc_nodeline.h"
#include "kumo.h"
#include "initialize.h"
#include "section.h"
#include "equator.h"

void compute_patch_segment() {
  query = 1; fermi_patch();
  query = 0; fermi_patch();
  max_and_min();
  paint();
  query = 1; calc_nodeline();
  query = 0; calc_nodeline();
  query = 1; calc_section();
  query = 0; calc_section();
  query = 1; equator();
  query = 0; equator();
}

void refresh_patch_segment() {
  free_patch();
  compute_patch_segment();
}

/**
 @brief handle main menu. 
*/
static void main_menu(
  int value //!< [in] Selected menu
) {
}
/**
@brief Change background color (::blackback)
*/
static void menu_background(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && blackback != 1) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    blackback = 1;
    if (color_scale == 4) paint();
    glutPostRedisplay();
  }
  else if (value == 0 && blackback != 0) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    blackback = 0;
    if (color_scale == 4) paint();
    glutPostRedisplay();
  }
}/* bgcolor change*/
 /**
 @brief Toggle the appearance of each band (::draw_band)
*/
static void menu_band(
  int value //!< [in] Selected menu
)
{
  if (draw_band[value] == 0) {
    draw_band[value] = 1;
  }
  else {
    draw_band[value] = 0;
  }
  glutPostRedisplay();
} /* menu_band */
/**
 @brief Change Brillouin zone (::fbz)
*/
static void menu_brillouinzone(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && fbz != 1) {
    fbz = 1;
    refresh_patch_segment();
    glutPostRedisplay();
  }
  else if (value == 2 && fbz != -1) {
    fbz = -1;
    lsection = 0;
    refresh_patch_segment();
    glutPostRedisplay();
  }
} /* menu_brillouinzone */
/**
 @brief Toggle Colorbar (::lcolorbar)
*/
static void menu_colorbar(
  int value //!<[in] Selected menu
)
{
  if (lcolorbar != 1)  lcolorbar = 1;
  else lcolorbar = 0;
  glutPostRedisplay();
} /* menu_colorbar */
/**
 @brief Change color scale mode (::color_scale)
*/
static void menu_colorscale(
  int value //!<[in] Selected menu
)
{
  int ierr;

  if (value == 0) {
    max_and_min();
    if (color_scale == 1 || color_scale == 3
      || color_scale == 5 || color_scale == 3) {
      printf("    Set min. value for scale : ");
      ierr = scanf("%f", &patch_min);
      if (ierr == 0) printf("error ! reading min");
      printf("    Set max. value for scale : ");
      ierr = scanf("%f", &patch_max);
      if (ierr == 0) printf("error ! reading max");
    }
    else if (color_scale == 2) {
      printf("    Set max. value for scale: ");
      ierr = scanf("%f", &patch_max);
      if (ierr == 0) printf("error ! reading max");
    }
    else {
      printf("  No color scale in this case.\n");
    }
    paint();
    glutPostRedisplay();
  }
  else if (value != color_scale) {
    color_scale = value;
    max_and_min();
    paint();
    glutPostRedisplay();
  }
} /* menu_colorscale */
/**
 @brief Modify and toggle appearance of equator (::lequator)
*/
static void menu_equator(
  int value //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  GLfloat vec[3];

  if (value == 1) {
    if (lequator != 1) lequator = 1;
    else lequator = 0;
    glutPostRedisplay();
  }/*if (value == 1)*/
  else {

    printf("    New Miller index : ");
    ierr = scanf("%f %f %f", &vec[0], &vec[1], &vec[2]);
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      eqvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        eqvec[ii] += vec[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
    /*
     Free variables for equator
    */
    for (ib = 0; ib < nb; ++ib) {
      for (ii = 0; ii < nequator[ib]; ++ii) {
        for (jj = 0; jj < 2; ++jj) {
          free(kveq[ib][ii][jj]);
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        free(kveq[ib][ii]);
      }/*for (i0 = 0; i0 < nequator[ib]; ++i0)*/
      free(kveq[ib]);
      free(kveq_rot[ib]);
      free(nmleq[ib]);
      free(clreq[ib]);
    }/*for (ib = 0; ib < nb; ++ib)*/
    free(kveq);
    free(kveq_rot);
    free(nmleq);
    free(clreq);

    query = 1; equator();
    query = 0; equator();
    glutPostRedisplay();
  }/*else if (value > 1)*/
} /*void menu_equator*/
/**
 @brief Modify interpolation ratio

 This routine modify interpolation ratio (::interpol) 
 then compute Fermi surfaces, etc.
*/
static void menu_interpol(
  int value //!< [in] Selected menu
)
{
  int ierr;

  if (value == 1) {
    printf("    Old interpolation ratio : %d\n", interpol);
    printf("    New interpolation ratio : ");
    //
    ierr = scanf("%d", &interpol);
    if (ierr != 1) printf("error ! reading interpol");
    /**/
    interpol_energy();
    refresh_patch_segment();
    /**/
    glutPostRedisplay();
  }
}/*static void menu_interpol*/
/**
 @brief Toggle Lighting (::lside)
*/
static void menu_lighting(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && lside != 1) {
    lside = 1;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glutPostRedisplay();
  }
  if (value == 2 && lside != 2) {
    lside = 2;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glutPostRedisplay();
  }
  if (value == 3 && lside != 3) {
    lside = 3;
    side = -1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glutPostRedisplay();
  }
} /* menu_lighting */
/**
 @brief Line width (::lside)
*/
static void menu_line(
  int value //!<[in] Selected menu
)
{
  int ierr;
  printf("    Current line width : %f\n", linewidth);
  printf("        New line width : ");
  ierr = scanf("%f", &linewidth);

  glutPostRedisplay();
} /* menu_line */
/**
 @brief Change the function associated to the mouse movement(::lmouse)
*/
static void menu_mouse(
  int value //!< [in] Selected menu
) 
{
  if (value == 1 && lmouse != 1) {
    lmouse = 1;
    glutPostRedisplay();
  }
  if (value == 2 && lmouse != 2) {
    lmouse = 2;
    glutPostRedisplay();
  }
  if (value == 3 && lmouse != 3) {
    lmouse = 3;
    glutPostRedisplay();
  }
} /* menu_mouse */
/**
 @brief Toggle apearance of nodale-line
*/
static void menu_nodeline(
  int value //!<[in] Selected menu
)
{
  if (nodeline != 1) nodeline = 1;
  else nodeline = 0;
  glutPostRedisplay();
}/*menu_nodeline*/
/**
 @brief Modify and toggle appearance of 2D Fermi lines (::lsection)
*/
static void menu_section(
  int value //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  GLfloat vec[3];

  if (value == 1) {
    if (lsection != 1) {
      lsection = 1;
      glutPostRedisplay();
    }
    else {
      lsection = 0;
      glutPostRedisplay();
    }
  }/*if (value == 1)*/
  else if (value > 1) {
    if (value == 2) secscale = 1.0;
    else secscale = 0.0;

    printf("    New Miller index : ");
    ierr = scanf("%f %f %f", &vec[0], &vec[1], &vec[2]);
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      secvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        secvec[ii] += vec[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
    /*
     Free variables for Fermi line
    */
    for (ib = 0; ib < nb; ++ib) {
      free(kv2d[ib]);
      free(clr2d[ib]);
      free(nml2d[ib]);
    }/*for (ib = 0; ib < nb; ++ib)*/
    free(kv2d);
    free(clr2d);
    free(nml2d);

    calc_2dbz();
    query = 1; calc_section();
    query = 0; calc_section();
    glutPostRedisplay();
  }/*else if (value > 1)*/
} /*void menu_section*/
/**
 @brief Shift Fermi energy
*/
static void menu_shift(
  int value //!<[in] Selected menu
)
{
  int ib, i0, i1, i2, ierr;
  GLfloat emin, emax;

  emin = 100000.0;
  emax = -100000.0;
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ng[0]; ++i0) {
      for (i1 = 0; i1 < ng[1]; ++i1) {
        for (i2 = 0; i2 < ng[2]; ++i2) {
          if (emin > eig[ib][i0][i1][i2]) emin = eig[ib][i0][i1][i2];
          if (emax < eig[ib][i0][i1][i2]) emax = eig[ib][i0][i1][i2];
        }
      }
    }
  }
  printf("    Min  Max  E_F \n");
  printf("    %f %f %f \n", emin, emax, EF);
  printf("    New Fermi energy : ");
  //
  ierr = scanf("%f", &EF);
  if (ierr != 1) printf("error ! reading ef");
  /**/
  refresh_patch_segment();
  /**/
  glutPostRedisplay();
} /* menu_shift */
/**
 @brief Tern stereogram (::lstereo)
*/
static void menu_stereo(
  int value //!<[in] Selected menu
) {
  if (value == 1 && lstereo != 1) {
    lstereo = 1;
    glutPostRedisplay();
  }
  if (value == 2 && lstereo != 2) {
    lstereo = 2;
    glutPostRedisplay();
  }
  if (value == 3 && lstereo != 3) {
    lstereo = 3;
    glutPostRedisplay();
  }
} /* menu_stereo */
/**
 @brief Change tetrahedron (::itet)
*/
static void menu_tetra(
  int value //!<[in] Selected menu
)
{
  if (value != itet) {
    printf("    Tetra patern %d \n", value + 1);
    itet = value;
    init_corner();
    refresh_patch_segment();
    glutPostRedisplay();
  }
}/*menu_tetra*/
 /**
 @brief Setting of view

 This modify scale (::scl) & tarnslation (::trans) &
 rotation (::thetax, ::thetay, ::thetaz, ::rot),
 */
static void menu_view(
  int value //!< [in] Selected menu
)
{
  int ierr;

  if (value == 1) {

    printf("    Current Scale : %f\n", scl);
    printf("        New Scale : ");
    ierr = scanf("%f", &scl);

  }
  else  if (value == 2) {

    printf("    Current Position(x y) : %f %f\n", trans[0], trans[1]);
    printf("        New Position(x y) : ");
    ierr = scanf("%f %f", &trans[0], &trans[1]);

  }
  else  if (value == 3) {

    /**/
    thetay = asinf(rot[0][2]);
    if (cosf(thetay) != 0.0) {
      if (-rot[1][2] / cosf(thetay) >= 0.0) thetax = acosf(rot[2][2] / cosf(thetay));
      else thetax = 6.283185307f - acosf(rot[2][2] / cosf(thetay));
      if (-rot[0][1] / cosf(thetay) >= 0.0) thetaz = acosf(rot[0][0] / cosf(thetay));
      else thetaz = 6.283185307f - acosf(rot[0][0] / cosf(thetay));
    }
    else {
      thetax = 0.0;
      if (rot[1][0] >= 0.0) thetaz = acosf(rot[1][1]);
      else thetaz = 6.283185307f - acosf(rot[1][1]);
    }
    thetax = 180.0f / 3.14159265f * thetax;
    thetay = 180.0f / 3.14159265f * thetay;
    thetaz = 180.0f / 3.14159265f * thetaz;
    printf("    Current Rotation (theta_x theta_y teta_z) in degree : %f %f %f\n", thetax, thetay, thetaz);
    printf("        New Rotation (theta_x theta_y teta_z) in degree : ");
    ierr = scanf("%f %f %f", &thetax, &thetay, &thetaz);
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

  glutPostRedisplay();

}
/**
 @brief Modify text in the right-click munu
*/
void FS_ModifyMenu(
  int status//!<[in]
)
{
  int ib;
  char menu_str[50] = { 0 };
  if (status == GLUT_MENU_IN_USE) {
    glutPostRedisplay();
  }
  else {
    /*
    Background color
    */
    glutSetMenu(imenu_background);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (blackback == 1) glutAddMenuEntry("[x] Black", 1);
    else glutAddMenuEntry("[ ] Black", 1);
    if (blackback == 0) glutAddMenuEntry("[x] White", 0);
    else glutAddMenuEntry("[ ] White", 0);
    /*
    Band menu
    */
    glutSetMenu(imenu_band);
    for (ib = 0; ib < nb; ib++) glutRemoveMenuItem(1);
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] == 1) sprintf(menu_str, "[x] band # %d", ib + 1);
      else sprintf(menu_str, "[ ] band # %d", ib + 1);
      glutAddMenuEntry(menu_str, ib);
    }
    /*
    Brillouin zone
    */
    glutSetMenu(imenu_bzmode);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (fbz == 1) glutAddMenuEntry("[x] First Brillouin zone", 1);
    else glutAddMenuEntry("[ ] First Brillouin zone", 1);
    if (fbz == -1) glutAddMenuEntry("[x] Primitive Brillouin zone", 2);
    else glutAddMenuEntry("[ ] Primitive Brillouin zone", 2);
    /*
    Colorbar on/off
    */
    glutSetMenu(imenu_colorbar);
    for (ib = 0; ib < 1; ib++) glutRemoveMenuItem(1);
    if (lcolorbar == 1) glutAddMenuEntry("[x] Color bar", 0);
    else glutAddMenuEntry("[ ] Color bar", 0);
    /*
    Color scale mode
    */
    glutSetMenu(imenu_colorscale);
    for (ib = 0; ib < 7; ib++) glutRemoveMenuItem(1);
    glutAddMenuEntry("Max/Min of Scale", 0);
    if (color_scale == 1) glutAddMenuEntry("[x] Input (Real)", 1);
    else glutAddMenuEntry("[ ] Input (Real)", 1);
    if (color_scale == 2) glutAddMenuEntry("[x] Input (Complex)", 2);
    else glutAddMenuEntry("[ ] Input (Complex)", 2);
    if (color_scale == 3) glutAddMenuEntry("[x] Fermi Velocity", 3);
    else glutAddMenuEntry("[ ] Fermi Velocity", 3);
    if (color_scale == 4) glutAddMenuEntry("[x] Band Index", 4);
    else glutAddMenuEntry("[ ] Band Index", 4);
    if (color_scale == 5) glutAddMenuEntry("[x] Input (Real, Gray Scale)", 5);
    else glutAddMenuEntry("[ ] Input (Real, Gray Scale)", 5);
    if (color_scale == 6) glutAddMenuEntry("[x] Fermi Velocity (Gray Scale)", 6);
    else glutAddMenuEntry("[ ] Fermi Velocity (Gray Scale)", 6);
    /*
    Equator
    */
    glutSetMenu(imenu_equator);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (lequator == 1) glutAddMenuEntry("[x] Equator", 1);
    else glutAddMenuEntry("[ ] Equator", 1);
    glutAddMenuEntry("Modify euqtor", 2);
    /*
    Interpolation ratio
    */
    glutSetMenu(imenu_interpol);
    glutRemoveMenuItem(1);
    sprintf(menu_str, "Ratio : %d", interpol);
    glutAddMenuEntry(menu_str, 1);
    /*
    Switch lighting
    */
    glutSetMenu(imenu_light);
    for (ib = 0; ib < 3; ib++) glutRemoveMenuItem(1);
    if (lside == 1) glutAddMenuEntry("[x] Both side", 1);
    else glutAddMenuEntry("[ ] Both side", 1);
    if (lside == 2) glutAddMenuEntry("[x] Unoccupied side", 2);
    else glutAddMenuEntry("[ ] Unoccupied side", 2);
    if (lside == 3) glutAddMenuEntry("[x] Occupied side", 3);
    else glutAddMenuEntry("[ ] Occupied side", 3);
    /*
    Line width
    */
    glutSetMenu(imenu_line);
    for (ib = 0; ib < 1; ib++) glutRemoveMenuItem(1);
    sprintf(menu_str, "Line width : %3.1f", linewidth);
    glutAddMenuEntry(menu_str, 1);
    /*
     Operation with mouse drag
    */
    glutSetMenu(imenu_mouse);
    for (ib = 0; ib < 3; ib++) glutRemoveMenuItem(1);
    if (lmouse == 1) glutAddMenuEntry("[x] Rotate", 1);
    else glutAddMenuEntry("[ ] Rotate", 1);
    if (lmouse == 2) glutAddMenuEntry("[x] Scale", 2);
    else glutAddMenuEntry("[ ] Scale", 2);
    if (lmouse == 3) glutAddMenuEntry("[x] Translate", 3);
    else glutAddMenuEntry("[ ] Translate", 3);
    /*
     Nodal line on/off
    */
    glutSetMenu(imenu_nodeline);
    for (ib = 0; ib < 1; ib++) glutRemoveMenuItem(1);
    if (nodeline == 1) glutAddMenuEntry("[x] Nodal line", 0);
    else glutAddMenuEntry("[ ] Nodal line", 0);
    /*
     2D Fermi lines
    */
    glutSetMenu(imenu_section);
    for (ib = 0; ib < 3; ib++) glutRemoveMenuItem(1);
    if (fbz == -1) glutAddMenuEntry("[Cannot] Section", 0);
    else if (lsection == 1) glutAddMenuEntry("[x] Section", 1);
    else glutAddMenuEntry("[ ] Section", 1);
    glutAddMenuEntry("Modify section", 2);
    glutAddMenuEntry("Modify section (across Gamma)", 3);
    /*
    Shift Fermi energy
    */
    glutSetMenu(imenu_shift);
    for (ib = 0; ib < 1; ib++) glutRemoveMenuItem(1);
    sprintf(menu_str, "Fermi energy :%9.5f", EF);
    glutAddMenuEntry(menu_str, 1);
    /*
    Stereogram
    */
    glutSetMenu(imenu_stereo);
    for (ib = 0; ib < 3; ib++) glutRemoveMenuItem(1);
    if (lstereo == 1) glutAddMenuEntry("[x] None", 1);
    else glutAddMenuEntry("[ ] None", 1);
    if (lstereo == 2) glutAddMenuEntry("[x] Parallel", 2);
    else glutAddMenuEntry("[ ] Parallel", 2);
    if (lstereo == 3) glutAddMenuEntry("[x] Cross", 3);
    else glutAddMenuEntry("[ ] Cross", 3);
    /*
     Tetrahedron
    */
    glutSetMenu(imenu_tetra);
    for (ib = 0; ib < 16; ib++) glutRemoveMenuItem(1);
    for (ib = 0; ib < 16; ib++) {
      if (itet == ib) sprintf(menu_str, "[x] tetra # %d", ib + 1);
      else sprintf(menu_str, "[ ] tetra # %d", ib + 1);
      glutAddMenuEntry(menu_str, ib);
    }
    /*
    Set view
    */
    glutSetMenu(imenu_view);
    for (ib = 0; ib < 3; ib++) glutRemoveMenuItem(1);
    sprintf(menu_str, "Scale");// :%6.2f", scl);
    glutAddMenuEntry(menu_str, 1);
    sprintf(menu_str, "Position");// :%6.2f %6.2f", trans[0], trans[1]);
    glutAddMenuEntry(menu_str, 2);
    sprintf(menu_str, "Rotation");// : %4.0f %4.0f %4.0f", thetax, thetay, thetaz);
    glutAddMenuEntry(menu_str, 3);
    
    glutPostRedisplay();
  }
}/*void FS_ModifyMenu*/
/**
 @brief Create Right-click Menu
*/
void FS_CreateMenu()
{
  int ib;
  char menu_str[50] = { 0 };
  /*
  Background color
  */
  imenu_background = glutCreateMenu(menu_background);
  if (blackback == 1) glutAddMenuEntry("[x] Black", 1);
  else glutAddMenuEntry("[ ] Black", 1);
  if (blackback == 0) glutAddMenuEntry("[x] White", 0);
  else glutAddMenuEntry("[ ] White", 0);
  /*
  On/Off  each band
  */
  imenu_band = glutCreateMenu(menu_band);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) sprintf(menu_str, "[x] band # %d", ib + 1);
    else sprintf(menu_str, "[ ] band # %d", ib + 1);
    glutAddMenuEntry(menu_str, ib);
  }
  /*
  Brillouin zone
  */
  imenu_bzmode = glutCreateMenu(menu_brillouinzone);
  if (fbz == 1) glutAddMenuEntry("[x] First Brillouin zone", 1);
  else glutAddMenuEntry("[ ] First Brillouin zone", 1);
  if (fbz == -1) glutAddMenuEntry("[x] Primitive Brillouin zone", 2);
  else glutAddMenuEntry("[ ] Primitive Brillouin zone", 2);
  /*
  Colorbar on/off
  */
  imenu_colorbar = glutCreateMenu(menu_colorbar);
  if (lcolorbar == 1) glutAddMenuEntry("[x] Color bar", 1);
  else glutAddMenuEntry("[ ] Color bar", 1);
  /*
  Color scale mode
  */
  imenu_colorscale = glutCreateMenu(menu_colorscale);
  glutAddMenuEntry("Max/Min of Scale", 0);
  if (color_scale == 1) glutAddMenuEntry("[x] Input (Real)", 1);
  else glutAddMenuEntry("[ ] Input (Real)", 1);
  if (color_scale == 2) glutAddMenuEntry("[x] Input (Complex)", 2);
  else glutAddMenuEntry("[ ] Input (Complex)", 2);
  if (color_scale == 3) glutAddMenuEntry("[x] Fermi Velocity", 3);
  else glutAddMenuEntry("[ ] Fermi Velocity", 3);
  if (color_scale == 4) glutAddMenuEntry("[x] Band Index", 4);
  else glutAddMenuEntry("[ ] Band Index", 4);
  if (color_scale == 5) glutAddMenuEntry("[x] Input (Real, Gray Scale)", 5);
  else glutAddMenuEntry("[ ] Input (Real, Gray Scale)", 5);
  if (color_scale == 6) glutAddMenuEntry("[x] Fermi Velocity (Gray Scale)", 6);
  else glutAddMenuEntry("[ ] Fermi Velocity (Gray Scale)", 6);
  /*
  Equator
  */
  imenu_equator = glutCreateMenu(menu_equator);
  if (lequator == 1) glutAddMenuEntry("[x] Equator", 1);
  else glutAddMenuEntry("[ ] Equator", 1);
  glutAddMenuEntry("Modify euqtor", 2);
  /*
  Interpolation ratio
  */
  sprintf(menu_str, "Ratio : %d", interpol);
  imenu_interpol = glutCreateMenu(menu_interpol);
  glutAddMenuEntry(menu_str, 1);
  /*
  Switch lighting
  */
  imenu_light = glutCreateMenu(menu_lighting);
  if (lside == 1) glutAddMenuEntry("[x] Both side", 1);
  else glutAddMenuEntry("[ ] Both side", 1);
  if (lside == 2) glutAddMenuEntry("[x] Unoccupied side", 2);
  else glutAddMenuEntry("[ ] Unoccupied side", 2);
  if (lside == 3) glutAddMenuEntry("[x] Occupied side", 3);
  else glutAddMenuEntry("[ ] Occupied side", 3);
  /*
  Line width
  */
  imenu_line = glutCreateMenu(menu_line);
  sprintf(menu_str, "Line width : %3.1f", linewidth);
  glutAddMenuEntry(menu_str, 1);
  /*
   Mouse drag works as ...
  */
  imenu_mouse = glutCreateMenu(menu_mouse);
  if (lmouse == 1) glutAddMenuEntry("[x] Rotate", 1);
  else glutAddMenuEntry("[ ] Rotate", 1);
  if (lmouse == 2) glutAddMenuEntry("[x] Scale", 2);
  else glutAddMenuEntry("[ ] Scale", 2);
  if (lmouse == 3) glutAddMenuEntry("[x] Translate", 3);
  else glutAddMenuEntry("[ ] Translate", 3);
  /*
  Nodeline on/off
  */
  imenu_nodeline = glutCreateMenu(menu_nodeline);
  if (nodeline == 1) glutAddMenuEntry("[x] Nodal line", 0);
  else glutAddMenuEntry("[ ] Nodal line", 0);
  /*
  2D Fermi lines
  */
  imenu_section = glutCreateMenu(menu_section);
  if (fbz == -1) glutAddMenuEntry("[Cannot] Section", 0);
  else if (lsection == 1) glutAddMenuEntry("[x] Section", 1);
  else glutAddMenuEntry("[ ] Section", 1);
  glutAddMenuEntry("Modify section", 2);
  glutAddMenuEntry("Modify section (across Gamma)", 3);
  /*
  Shift Fermi energy
  */
  imenu_shift = glutCreateMenu(menu_shift);
  sprintf(menu_str, "Fermi energy :%9.5f", EF);
  glutAddMenuEntry(menu_str, 1);
  /*
   Stereogram
  */
  imenu_stereo = glutCreateMenu(menu_stereo);
  if (lstereo == 1) glutAddMenuEntry("[x] None", 1);
  else glutAddMenuEntry("[ ] None", 1);
  if (lstereo == 2) glutAddMenuEntry("[x] Parallel", 2);
  else glutAddMenuEntry("[ ] Parallel", 2);
  if (lstereo == 3) glutAddMenuEntry("[x] Cross", 3);
  else glutAddMenuEntry("[ ] Cross", 3);
  /*
   Tetrahedron 
  */
  imenu_tetra = glutCreateMenu(menu_tetra);
  for (ib = 0; ib < 16; ib++) {
    if (itet == ib) sprintf(menu_str, "[x] tetra # %d", ib + 1);
    else sprintf(menu_str, "[ ] tetra # %d", ib + 1);
    glutAddMenuEntry(menu_str, ib);
  }
  /*
  Set view
  */
  imenu_view = glutCreateMenu(menu_view);
  sprintf(menu_str, "Scale");// :%6.2f", scl);
  glutAddMenuEntry(menu_str, 1);
  sprintf(menu_str, "Position");// :%6.2f %6.2f", trans[0], trans[1]);
  glutAddMenuEntry(menu_str, 2);
  sprintf(menu_str, "Rotation");// : %4.0f %4.0f %4.0f", thetax, thetay, thetaz);
  glutAddMenuEntry(menu_str, 3);
  /*
   Main menu
  */
  imenu = glutCreateMenu(main_menu);
  glutAddSubMenu("Background color", imenu_background);
  glutAddSubMenu("Band", imenu_band);
  glutAddSubMenu("Brillouin zone", imenu_bzmode);
  glutAddSubMenu("Color bar", imenu_colorbar);
  glutAddSubMenu("Color scale mode", imenu_colorscale);
  glutAddSubMenu("Equator", imenu_equator);
  glutAddSubMenu("Interpolation", imenu_interpol);
  glutAddSubMenu("Lighting", imenu_light);
  glutAddSubMenu("Line width", imenu_line);
  glutAddSubMenu("Mouse Drag", imenu_mouse);
  glutAddSubMenu("Nodal line", imenu_nodeline);
  glutAddSubMenu("Section", imenu_section);
  glutAddSubMenu("Shift Fermi energy", imenu_shift);
  glutAddSubMenu("Stereogram", imenu_stereo);
  glutAddSubMenu("Tetrahedron", imenu_tetra);
  glutAddSubMenu("View point", imenu_view);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
