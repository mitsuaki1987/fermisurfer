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
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
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

void compute_patch_segment() {
  query = 1; fermi_patch();
  query = 0; fermi_patch();
  max_and_min();
  query = 1; calc_nodeline();
  query = 0; calc_nodeline();
  query = 1; calc_section();
  query = 0; calc_section();
}

static void refresh_patch_segment() {
  free_patch();
  compute_patch_segment();
}

/**
 @brief handle main menu. Currently, only Exit.
 Othere menus are in submenu.
*/
static void main_menu(
  int value //!< [in] Selected menu
) {
  int i0, i1, ib;
  /**/
  if (value == 9) {
    /*
     Exit
    */
    printf("\nExit. \n\n");
    free_patch();
    for (ib = 0; ib < nb; ib++) {
      for (i0 = 0; i0 < ng[0]; i0++) {
        for (i1 = 0; i1 < ng[1]; i1++) {
          free(eig[ib][i0][i1]);
          free(mat[ib][i0][i1]);
        }
        free(eig[ib][i0]);
        free(mat[ib][i0]);
      }
      free(eig[ib]);
      free(mat[ib]);
    }
    free(eig);
    free(mat);
    free(ntri);
    free(draw_band);
    free(nnl);
    exit(0);
  }
}
/**
 @brief Shift Fermi energy

 Modify : ::EF

 This menu modify Fermi energy and compute Fermi surfaces, etc.
*/
static void menu_shiftEF(int value /**< [in] Selected menu*/)
{
  int ib, i0, i1, i2, ierr;
  GLfloat emin, emax;

  if (value == 1) {
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
  }
}
/**
 @brief Shift Fermi energy

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
} /* menu_band */
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
 @brief Change background color (::blackback)
*/
static void menu_bgcolor(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && blackback != 1) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    blackback = 1;
    glutPostRedisplay();
  }
  else if (value == 2 && blackback != 0) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    blackback = 0;
    glutPostRedisplay();
  }
}/* bgcolor change*/
/**
 @brief Change color scale mode (::fcscl)
*/
static void menu_colorscale(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && fcscl != 1) {
    fcscl = 1;
    max_and_min();
    glutPostRedisplay();
  }
  else if (value == 2 && fcscl != 2) {
    fcscl = 2;
    max_and_min();
    glutPostRedisplay();
  }
  else if (value == 3 && fcscl != 3) {
    fcscl = 3;
    max_and_min();
    glutPostRedisplay();
  }
  else if (value == 4 && fcscl != 4) {
    fcscl = 4;
    max_and_min();
    glutPostRedisplay();
  }
} /* menu_colorscale */
/**
 @brief Change Brillouin zone (::fbz)
*/
static void menu_bzmode(
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
} /* menu_bzmode */
/**
 @brief Toggle apearance of nodale-line
*/
static void menu_nodeline(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && nodeline != 1) {
    nodeline = 1;
    glutPostRedisplay();
  }
  else if (value == 2 && nodeline != 0) {
    nodeline = 0;
    glutPostRedisplay();
  }
}/*menu_nodeline*/
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
 @brief Toggle Colorbar (::lcolorbar)
*/
static void menu_colorbar(
  int value //!<[in] Selected menu
)
{
  if (value == 1 && lcolorbar != 1) {
    lcolorbar = 1;
    glutPostRedisplay();
  }
  if (value == 2 && lcolorbar != 0) {
    lcolorbar = 0;
    glutPostRedisplay();
  }
} /* menu_colorbar */
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
 @brief Modify text in the right-click munu
*/
void FS_ModifyMenu(
  int status//!<[in]
)
{
  int ib;
  char menu_str[20] = { 0 };

  if (status == GLUT_MENU_IN_USE) {
    glutPostRedisplay();
  }
  else {
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
     Operation with mouse drag
    */
    glutSetMenu(imenu_interpol);
    glutRemoveMenuItem(1);
    sprintf(menu_str, "Ratio : %d", interpol);
    glutAddMenuEntry(menu_str, 1);
    /*
     Background color
    */
    glutSetMenu(imenu_bgcolor);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (blackback == 1) glutAddMenuEntry("[x] Black", 1);
    else glutAddMenuEntry("[ ] Black", 1);
    if (blackback == 0) glutAddMenuEntry("[x] White", 2);
    else glutAddMenuEntry("[ ] White", 2);
    /*
     Color scale mode
    */
    glutSetMenu(imenu_colorscale);
    for (ib = 0; ib < 4; ib++) glutRemoveMenuItem(1);
    if (fcscl == 1) glutAddMenuEntry("[x] Auto", 1);
    else glutAddMenuEntry("[ ] Auto", 1);
    if (fcscl == 2) glutAddMenuEntry("[x] Manual", 2);
    else glutAddMenuEntry("[ ] Manual", 2);
    if (fcscl == 3) glutAddMenuEntry("[x] Unicolor", 3);
    else glutAddMenuEntry("[ ] Unicolor", 3);
    if (fcscl == 4) glutAddMenuEntry("[x] Periodic", 4);
    else glutAddMenuEntry("[ ] Periodic", 4);
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
     Nodeline on/off
    */
    glutSetMenu(imenu_nodeline);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (nodeline == 1) glutAddMenuEntry("[x] On", 1);
    else glutAddMenuEntry("[ ] On", 1);
    if (nodeline == 0) glutAddMenuEntry("[x] Off", 2);
    else glutAddMenuEntry("[ ] Off", 2);
    /*
     Colorbar on/off
    */
    glutSetMenu(imenu_colorbar);
    for (ib = 0; ib < 2; ib++) glutRemoveMenuItem(1);
    if (lcolorbar == 1) glutAddMenuEntry("[x] On", 1);
    else glutAddMenuEntry("[ ] On", 1);
    if (lcolorbar == 0) glutAddMenuEntry("[x] Off", 2);
    else glutAddMenuEntry("[ ] Off", 2);
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
     Tetrahedron
    */
    glutSetMenu(imenu_tetra);
    for (ib = 0; ib < 16; ib++) glutRemoveMenuItem(1);
    for (ib = 0; ib < 16; ib++) {
      if (itet == ib) sprintf(menu_str, "[x] tetra # %d", ib + 1);
      else sprintf(menu_str, "[ ] tetra # %d", ib + 1);
      glutAddMenuEntry(menu_str, ib);
    }
    glutPostRedisplay();
  }
}/*void FS_ModifyMenu*/
/**
 @brief Create Right-click Menu
*/
void FS_CreateMenu()
{
  int ib, imenu_shiftEF, imenu_view;
  char menu_str[20] = { 0 };
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
   On/Off  each band 
  */
  imenu_band = glutCreateMenu(menu_band);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) sprintf(menu_str, "[x] band # %d", ib + 1);
    else sprintf(menu_str, "[ ] band # %d", ib + 1);
    glutAddMenuEntry(menu_str, ib);
  }
  /*
   Shift Fermi energy
  */
  imenu_shiftEF = glutCreateMenu(menu_shiftEF);
  glutAddMenuEntry("Shift Fermi energy", 1);
  /*
   Modify interpolation ratio
  */
  sprintf(menu_str, "Ratio : %d", interpol);
  imenu_interpol = glutCreateMenu(menu_interpol);
  glutAddMenuEntry(menu_str, 1);
  /*
   Set view
  */
  imenu_view = glutCreateMenu(menu_view);
  glutAddMenuEntry("Scale", 1);
  glutAddMenuEntry("Position", 2);
  glutAddMenuEntry("Rotation", 3);
  /*
   Background color
  */
  imenu_bgcolor = glutCreateMenu(menu_bgcolor);
  if (blackback == 1) glutAddMenuEntry("[x] Black", 1);
  else glutAddMenuEntry("[ ] Black", 1);
  if (blackback == 0) glutAddMenuEntry("[x] White", 2);
  else glutAddMenuEntry("[ ] White", 2);
  /*
   Color scale mode
  */
  imenu_colorscale = glutCreateMenu(menu_colorscale);
  if (fcscl == 1) glutAddMenuEntry("[x] Auto", 1);
  else glutAddMenuEntry("[ ] Auto", 1);
  if (fcscl == 2) glutAddMenuEntry("[x] Manual", 2);
  else glutAddMenuEntry("[ ] Manual", 2);
  if (fcscl == 3) glutAddMenuEntry("[x] Unicolor", 3);
  else glutAddMenuEntry("[ ] Unicolor", 3);
  if (fcscl == 4) glutAddMenuEntry("[x] Periodic", 4);
  else glutAddMenuEntry("[ ] Periodic", 4);
  /*
   Brillouin zone
  */
  imenu_bzmode = glutCreateMenu(menu_bzmode);
  if (fbz == 1) glutAddMenuEntry("[x] First Brillouin zone", 1);
  else glutAddMenuEntry("[ ] First Brillouin zone", 1);
  if (fbz == -1) glutAddMenuEntry("[x] Primitive Brillouin zone", 2);
  else glutAddMenuEntry("[ ] Primitive Brillouin zone", 2);
  /*
   Nodeline on/off
  */
  imenu_nodeline = glutCreateMenu(menu_nodeline);
  if (nodeline == 1) glutAddMenuEntry("[x] On", 1);
  else glutAddMenuEntry("[ ] On", 1);
  if (nodeline == 0) glutAddMenuEntry("[x] Off", 2);
  else glutAddMenuEntry("[ ] Off", 2);
  /*
   Colorbar on/off
  */
  imenu_colorbar = glutCreateMenu(menu_colorbar);
  if (lcolorbar == 1) glutAddMenuEntry("[x] On", 1);
  else glutAddMenuEntry("[ ] On", 1);
  if (lcolorbar == 0) glutAddMenuEntry("[x] Off", 2);
  else glutAddMenuEntry("[ ] Off", 2);
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
   2D Fermi lines
  */
  imenu_section = glutCreateMenu(menu_section);
  if (fbz == -1) glutAddMenuEntry("[Cannot] Section", 0); 
  else if (lsection == 1) glutAddMenuEntry("[x] Section", 1);
  else glutAddMenuEntry("[ ] Section", 1);
  glutAddMenuEntry("Modify section", 2);
  glutAddMenuEntry("Modify section (across Gamma)", 3);
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
   Main menu
  */
  imenu = glutCreateMenu(main_menu);
  glutAddSubMenu("Band", imenu_band);
  glutAddSubMenu("Mouse Drag", imenu_mouse);
  glutAddSubMenu("Shift Fermi energy", imenu_shiftEF);
  glutAddSubMenu("Interpolation", imenu_interpol);
  glutAddSubMenu("Set view", imenu_view);
  glutAddSubMenu("Background color", imenu_bgcolor);
  glutAddSubMenu("Color scale mode", imenu_colorscale);
  glutAddSubMenu("Brillouin zone", imenu_bzmode);
  glutAddSubMenu("Node lines", imenu_nodeline);
  glutAddSubMenu("Color bar On/Off", imenu_colorbar);
  glutAddSubMenu("Stereogram", imenu_stereo);
  glutAddSubMenu("Section", imenu_section);
  glutAddSubMenu("Tetrahedron", imenu_tetra);
  glutAddMenuEntry("Exit", 9);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
