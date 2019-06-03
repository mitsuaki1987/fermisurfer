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
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "free_patch.hpp"
#include "fermi_patch.hpp"
#include "calc_nodeline.hpp"
#include "kumo.hpp"
#include "initialize.hpp"
#include "section.hpp"
#include "equator.hpp"
#include "draw.hpp"
#include "variable.hpp"
#include "menu.hpp"
#include "operation.hpp"

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

enum
{
  menu_bz_radio1,
  menu_bz_radio2,
  menu_background_radio1,
  menu_background_radio2,
  menu_lighting_radio1,
  menu_lighting_radio2,
  menu_lighting_radio3,
  menu_mouse_radio1,
  menu_mouse_radio2,
  menu_mouse_radio3,
  menu_stereo_radio1,
  menu_stereo_radio2,
  menu_stereo_radio3,
  menu_tetra_radio1,
  menu_tetra_radio2,
  menu_tetra_radio3,
  menu_tetra_radio4,
  menu_tetra_radio5,
  menu_tetra_radio6,
  menu_tetra_radio7,
  menu_tetra_radio8,
  menu_tetra_radio9,
  menu_tetra_radio10,
  menu_tetra_radio11,
  menu_tetra_radio12,
  menu_tetra_radio13,
  menu_tetra_radio14,
  menu_tetra_radio15,
  menu_tetra_radio16,
  menu_view_radio1,
  menu_view_radio2,
  menu_view_radio3,
  menu_nodeline_check,
  menu_colorbar_check,
  imenu_line,
  imenu_shift,
  colorscale_maxmin,
  colorscale_radio1,
  colorscale_radio2,
  colorscale_radio3,
  colorscale_radio4,
  colorscale_radio5,
  colorscale_radio6,
  colorscale_radio7,
  equator_check,
  equator_modify,
  menu_section_check,
  menu_section_modify,
  menu_section_gamma,
  menu_view_scale,
  menu_view_position,
  menu_view_rotation,
  imenu_interpol,
  menu_band_check
};

/**
@brief Change background color (::blackback)
*/
void MyFrame::menu_background(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetId() == menu_background_radio1 && blackback != 1) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    blackback = 1;
    if (color_scale == 2 || color_scale == 3) paint();
    Refresh(false);
  }
  else if (event.GetId() == menu_background_radio2 && blackback != 0) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    blackback = 0;
    if (color_scale == 2 || color_scale == 3) paint();
    Refresh(false);
  }
}/* bgcolor change*/
 /**
 @brief Toggle the appearance of each band (::draw_band)
*/
void MyFrame::menu_band(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ib = event.GetId() - menu_band_check;
  if (draw_band[ib] == 0) {
    draw_band[ib] = 1;
  }
  else {
    draw_band[ib] = 0;
  }
  Refresh(false);
} /* menu_band */
/**
 @brief Change Brillouin zone (::fbz)
*/
void MyFrame::menu_brillouinzone(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetId() == menu_bz_radio1 && fbz != 1) {
    fbz = 1;
    refresh_patch_segment();
    Refresh(false);
  }
  else if (event.GetId() == menu_bz_radio2 && fbz != -1) {
    fbz = -1;
    lsection = 0;
    refresh_patch_segment();
    Refresh(false);
  }
} /* menu_brillouinzone */
/**
 @brief Toggle Colorbar (::lcolorbar)
*/
void MyFrame::menu_colorbar(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (lcolorbar != 1)  lcolorbar = 1;
  else lcolorbar = 0;
  Refresh(false);
} /* menu_colorbar */
/**
 @brief Change color scale mode (::color_scale)
*/
void MyFrame::menu_colorscale(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii;

  if (event.GetId() == colorscale_maxmin) {
    max_and_min();
    if (color_scale == 1 || color_scale == 4
      || color_scale == 6 || color_scale == 7) {
      printf("    Set min. and max. value for scale : ");
      ierr = scanf("%f%f", &patch_min[0], &patch_max[0]);
      if (ierr == 0) printf("error ! reading min or max");
    }
    else if (color_scale == 2) {
      printf("    Set max. value for scale: ");
      ierr = scanf("%f", &patch_max[0]);
      if (ierr == 0) printf("error ! reading max");
    }
    else if (color_scale == 3) {
      for (ii = 0; ii < 3; ii++) {
        printf("    Set min. and max. value for scale %d : ", ii);
        ierr = scanf("%f%f", &patch_min[ii], &patch_max[ii]);
        if (ierr == 0) printf("error ! reading min or max");
      }
    }
    else {
      printf("  No color scale in this case.\n");
    }
    paint();
    Refresh(false);
  }
  else if (event.GetId() - colorscale_radio1 + 1 != color_scale) {
    color_scale = event.GetId() - colorscale_radio1 + 1;
    max_and_min();
    paint();
    Refresh(false);
  }
} /* menu_colorscale */
/**
 @brief Modify and toggle appearance of equator (::lequator)
*/
void MyFrame::menu_equator(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  GLfloat vec[3];

  if (event.GetId() == equator_check) {
    if (lequator != 1) lequator = 1;
    else lequator = 0;
    Refresh(false);
  }/*if (event.GetId() == 1)*/
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
          delete[] kveq[ib][ii][jj];
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        delete[] kveq[ib][ii];
      }/*for (i0 = 0; i0 < nequator[ib]; ++i0)*/
      delete[] kveq[ib];
      delete[] kveq_rot[ib];
      delete[] nmleq[ib];
      delete[] clreq[ib];
    }/*for (ib = 0; ib < nb; ++ib)*/
    delete[] kveq;
    delete[] kveq_rot;
    delete[] nmleq;
    delete[] clreq;

    query = 1; equator();
    query = 0; equator();
    Refresh(false);
  }/*else if (event.GetId() > 1)*/
} /*void menu_equator*/
/**
 @brief Modify interpolation ratio

 This routine modify interpolation ratio (::interpol) 
 then compute Fermi surfaces, etc.
*/
void MyFrame::menu_interpol(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;

  if (event.GetId() == imenu_interpol) {
    printf("    Old interpolation ratio : %d\n", interpol);
    printf("    New interpolation ratio : ");
    //
    ierr = scanf("%d", &interpol);
    if (ierr != 1) printf("error ! reading interpol");
    /**/
    interpol_energy();
    refresh_patch_segment();
    /**/
    Refresh(false);
  }
}/*static void menu_interpol*/
/**
 @brief Toggle Lighting (::lside)
*/
void MyFrame::menu_lighting(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetId() == menu_lighting_radio1 && lside != 1) {
    lside = 1;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    Refresh(false);
  }
  if (event.GetId() == menu_lighting_radio2 && lside != 2) {
    lside = 2;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    Refresh(false);
  }
  if (event.GetId() == menu_lighting_radio3 && lside != 3) {
    lside = 3;
    side = -1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    Refresh(false);
  }
} /* menu_lighting */
/**
 @brief Line width (::lside)
*/
void MyFrame::menu_line(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  printf("    Current line width : %f\n", linewidth);
  printf("        New line width : ");
  ierr = scanf("%f", &linewidth);

  Refresh(false);
} /* menu_line */
/**
 @brief Change the function associated to the mouse movement(::lmouse)
*/
void MyFrame::menu_mouse(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetId() == menu_mouse_radio1 && lmouse != 1) {
    lmouse = 1;
    Refresh(false);
  }
  if (event.GetId() == menu_mouse_radio2 && lmouse != 2) {
    lmouse = 2;
    Refresh(false);
  }
  if (event.GetId() == menu_mouse_radio3 && lmouse != 3) {
    lmouse = 3;
    Refresh(false);
  }
} /* menu_mouse */
/**
 @brief Toggle apearance of nodale-line
*/
void MyFrame::menu_nodeline(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (nodeline != 1) nodeline = 1;
  else nodeline = 0;
  Refresh(false);
}/*menu_nodeline*/
/**
 @brief Modify and toggle appearance of 2D Fermi lines (::lsection)
*/
void MyFrame::menu_section(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  GLfloat vec[3];

  if (event.GetId() == menu_section_check) {
    if (lsection != 1) {
      lsection = 1;
      Refresh(false);
    }
    else {
      lsection = 0;
      Refresh(false);
    }
  }/*if (event.GetId() == 1)*/
  else {
    if (event.GetId() == menu_section_modify) secscale = 1.0;
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
      delete[] kv2d[ib];
      delete[] clr2d[ib];
      delete[] nml2d[ib];
    }/*for (ib = 0; ib < nb; ++ib)*/
    delete[] kv2d;
    delete[] clr2d;
    delete[] nml2d;

    calc_2dbz();
    query = 1; calc_section();
    query = 0; calc_section();
    Refresh(false);
  }/*else if (event.GetId() > 1)*/
} /*void menu_section*/
/**
 @brief Shift Fermi energy
*/
void MyFrame::menu_shift(
  wxCommandEvent& event //!<[in] Selected menu
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
  Refresh(false);
} /* menu_shift */
/**
 @brief Tern stereogram (::lstereo)
*/
void MyFrame::menu_stereo(
  wxCommandEvent& event //!<[in] Selected menu
) {
  if (event.GetId() == menu_stereo_radio1 && lstereo != 1) {
    lstereo = 1;
    Refresh(false);
  }
  if (event.GetId() == menu_stereo_radio2 && lstereo != 2) {
    lstereo = 2;
    Refresh(false);
  }
  if (event.GetId() == menu_stereo_radio3 && lstereo != 3) {
    lstereo = 3;
    Refresh(false);
  }
} /* menu_stereo */
/**
 @brief Change tetrahedron (::itet)
*/
void MyFrame::menu_tetra(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetId() - menu_tetra_radio1 != itet) {
    printf("    Tetra patern %d \n", event.GetId()- menu_tetra_radio1 + 1);
    itet = event.GetId() - menu_tetra_radio1;
    init_corner();
    refresh_patch_segment();
    Refresh(false);
  }
}/*menu_tetra*/
 /**
 @brief Setting of view

 This modify scale (::scl) & tarnslation (::trans) &
 rotation (::thetax, ::thetay, ::thetaz, ::rot),
 */
void MyFrame::menu_view(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;

  if (event.GetId() == menu_view_scale) {

    printf("    Current Scale : %f\n", scl);
    printf("        New Scale : ");
    ierr = scanf("%f", &scl);

  }
  else  if (event.GetId() == menu_view_position) {

    printf("    Current Position(x y) : %f %f\n", trans[0], trans[1]);
    printf("        New Position(x y) : ");
    ierr = scanf("%f %f", &trans[0], &trans[1]);

  }
  else  if (event.GetId() == menu_view_rotation) {

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

  Refresh(false);
}

MyFrame::MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
  const wxSize& size, long style)
  : wxFrame(frame, wxID_ANY, title, pos, size, style),
  m_canvas(NULL)
{
  int ib, itet;
  char menuname[8];

  // debug SetIcon(wxICON(sample));

  wxMenu* imenu_bzmode = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_brillouinzone, this, menu_bz_radio1);
  imenu_bzmode->AppendRadioItem(menu_bz_radio1, wxT("First Brillouin zone"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_brillouinzone, this, menu_bz_radio2);
  imenu_bzmode->AppendRadioItem(menu_bz_radio2, wxT("Primitive Brillouin zone"));

  wxMenu* imenu_background = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_background, this, menu_background_radio1);
  imenu_background->AppendRadioItem(menu_background_radio1, wxT("Black"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_background, this, menu_background_radio2);
  imenu_background->AppendRadioItem(menu_background_radio2, wxT("White"));

  wxMenu* imenu_band = new wxMenu;
  for (ib = 0; ib < nb; ib++) {
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_band, this, menu_band_check + ib);
    imenu_band->AppendCheckItem(menu_band_check + ib, wxString::Format(wxT("Band %d"), ib));
    imenu_band->Check(menu_band_check + ib, true);
  }

  wxMenu* imenu_colorscale = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_maxmin);
  imenu_colorscale->Append(colorscale_maxmin, wxT("Max/Min of Scale"));
  imenu_colorscale->AppendSeparator();
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio1);
  imenu_colorscale->AppendRadioItem(colorscale_radio1, wxT("Input (Real)"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio2);
  imenu_colorscale->AppendRadioItem(colorscale_radio2, wxT("Input (Complex)"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio3);
  imenu_colorscale->AppendRadioItem(colorscale_radio3, wxT("Input (Tri-number)"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio4);
  imenu_colorscale->AppendRadioItem(colorscale_radio4, wxT("Fermi Velocity"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio5);
  imenu_colorscale->AppendRadioItem(colorscale_radio5, wxT("Band Index"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio6);
  imenu_colorscale->AppendRadioItem(colorscale_radio6, wxT("Input (Real, Gray Scale)"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorscale, this, colorscale_radio7);
  imenu_colorscale->AppendRadioItem(colorscale_radio7, wxT("Fermi Velocity (Gray Scale)"));

  wxMenu* imenu_equator = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_equator, this, equator_check);
  imenu_equator->AppendCheckItem(equator_check, wxT("Equator"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_equator, this, equator_modify);
  imenu_equator->Append(equator_modify, wxT("Modify euqtor"));

  wxMenu* imenu_light = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_lighting, this, menu_lighting_radio1);
  imenu_light->AppendRadioItem(menu_lighting_radio1, wxT("Both side"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_lighting, this, menu_lighting_radio2);
  imenu_light->AppendRadioItem(menu_lighting_radio2, wxT("Unoccupied side"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_lighting, this, menu_lighting_radio3);
  imenu_light->AppendRadioItem(menu_lighting_radio3, wxT("Occupied side"));

  wxMenu* imenu_mouse = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_mouse, this, menu_mouse_radio1);
  imenu_mouse->AppendRadioItem(menu_mouse_radio1, wxT("Rotate"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_mouse, this, menu_mouse_radio2);
  imenu_mouse->AppendRadioItem(menu_mouse_radio2, wxT("Scale"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_mouse, this, menu_mouse_radio3);
  imenu_mouse->AppendRadioItem(menu_mouse_radio3, wxT("Translate"));

  wxMenu* imenu_section = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_section, this, menu_section_check);
  imenu_section->AppendCheckItem(menu_section_check, wxT("Section"));
  imenu_section->AppendSeparator();
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_section, this, menu_section_modify);
  imenu_section->Append(menu_section_modify, wxT("Modify section"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_section, this, menu_section_gamma);
  imenu_section->Append(menu_section_gamma, wxT("Modify section (across Gamma)"));

  wxMenu* imenu_stereo = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_stereo, this, menu_stereo_radio1);
  imenu_stereo->AppendRadioItem(menu_stereo_radio1, wxT("None"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_stereo, this, menu_stereo_radio2);
  imenu_stereo->AppendRadioItem(menu_stereo_radio2, wxT("Parallel"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_stereo, this, menu_stereo_radio3);
  imenu_stereo->AppendRadioItem(menu_stereo_radio3, wxT("Cross"));

  wxMenu* imenu_tetra = new wxMenu;
  for (itet = 0; itet < 16; itet++) {
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_tetra, this, menu_tetra_radio1 + itet);
    imenu_tetra->AppendRadioItem(menu_tetra_radio1 + itet,
      wxString::Format(wxT("tetra # %d"), itet));
  }

  wxMenu* imenu_view = new wxMenu;
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_view, this, menu_view_scale);
  imenu_view->Append(menu_view_scale, wxT("Scale"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_view, this, menu_view_position);
  imenu_view->Append(menu_view_position, wxT("Position"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_view, this, menu_view_rotation);
  imenu_view->Append(menu_view_rotation, wxT("Rotation"));

  // Make a menubar
  wxMenu* fileMenu = new wxMenu;

  fileMenu->AppendSubMenu(imenu_background, wxT("Background color"));
  fileMenu->AppendSubMenu(imenu_band, wxT("Band"));
  fileMenu->AppendSubMenu(imenu_bzmode, wxT("Brillouin zone"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_colorbar, this, menu_colorbar_check);
  fileMenu->AppendCheckItem(menu_colorbar_check, wxT("Color bar"));
  fileMenu->Check(menu_colorbar_check, true);
  fileMenu->AppendSubMenu(imenu_colorscale, wxT("Color scale mode"));
  fileMenu->AppendSubMenu(imenu_equator, wxT("Equator"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_interpol, this, imenu_interpol);
  fileMenu->Append(imenu_interpol, wxT("Interpol ratio"));
  fileMenu->AppendSubMenu(imenu_light, wxT("Lighting"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_line, this, imenu_line);
  fileMenu->Append(imenu_line, wxT("Line width"));
  fileMenu->AppendSubMenu(imenu_mouse, wxT("Mouse Drag"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_nodeline, this, menu_nodeline_check);
  fileMenu->AppendCheckItem(menu_nodeline_check, wxT("Nodal line"));
  fileMenu->AppendSubMenu(imenu_section, wxT("Section"));
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::menu_shift, this, imenu_shift);
  fileMenu->Append(imenu_shift, wxT("Shift Fermi energy"));
  fileMenu->AppendSubMenu(imenu_stereo, wxT("Stereogram"));
  fileMenu->AppendSubMenu(imenu_tetra, wxT("Tetrahedron"));
  fileMenu->AppendSubMenu(imenu_view, wxT("View point"));

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("File"));
  SetMenuBar(menuBar);

  // Make a TestGLCanvas

  // JACS
#ifdef __WXMSW__
  int* gl_attrib = NULL;
#else
  int gl_attrib[20] =
  { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
  WX_GL_DOUBLEBUFFER,
#  if defined(__WXMAC__) || defined(__WXCOCOA__)
        GL_NONE };
#  else
    None
};
#  endif
#endif

  m_canvas = new TestGLCanvas(this, wxID_ANY, gl_attrib);

  // Show the frame
  Show(true);
  Raise();

  m_canvas->InitGL();
}

MyFrame::~MyFrame()
{
  delete m_canvas;
}

// Intercept menu commands
void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  // true is to force the frame to close
  Close(true);
}
