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
#include "wx/splitter.h"

void compute_patch_segment() {
  if (refresh_interpol == 1){
    interpol_energy();
    refresh_interpol = 0;
  }
  query = 1; fermi_patch();
  query = 0; fermi_patch();
  max_and_min();
  paint();
  query = 1; calc_nodeline();
  query = 0; calc_nodeline();
  calc_2dbz();
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
  ibutton_reflesh,
  ibutton_compute,
  iradio_background,
  iradio_brillouinzone,
  iradio_lighting,
  iradio_mouse,
  iradio_stereo,
  iradio_tetra,
  iradio_colorscale,
  itext_colorscalemin,
  itext_colorscalemax,
  icheck_section,
  icheck_gamma,
  itext_sectionx,
  itext_sectiony,
  itext_sectionz,
  icheck_nodeline,
  icheck_colorbar,
  icheck_equator,
  itext_equatorx,
  itext_equatory,
  itext_equatorz,
  itext_line,
  itext_shift,
  itext_interpol,
  itext_scale,
  itext_positionx,
  itext_positiony,
  itext_rotx,
  itext_roty,
  itext_rotz,
  icheck_band
};

void MyFrame::button_refresh(
  wxCommandEvent& event//!<[in] Selected menu
) {
  Refresh(false);
}

void MyFrame::button_compute(
  wxCommandEvent& event//!<[in] Selected menu
) {
  free_patch();
  compute_patch_segment();
  Refresh(false);
}
/**
@brief Change background color (::blackback)
*/
void MyFrame::radio_background(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Black")) == 0 && blackback != 1) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    blackback = 1;
    // debug if (color_scale == 2 || color_scale == 3) paint();
  }
  else if (event.GetString().Cmp(wxT("White")) == 0 && blackback != 0) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    blackback = 0;
    // debug if (color_scale == 2 || color_scale == 3) paint();
  }
  Refresh(false);
}/* bgcolor change*/
 /**
 @brief Toggle the appearance of each band (::draw_band)
*/
void MyFrame::check_band(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ib = event.GetId() - icheck_band;
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
void MyFrame::radio_brillouinzone(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("First Brillouin zone")) == 0) {
    fbz = 1;
  }
  else if (event.GetString().Cmp(wxT("Primitive Brillouin zone")) == 0) {
    fbz = -1;
    lsection = 0;
  }
} /* menu_brillouinzone */
/**
 @brief Toggle Colorbar (::lcolorbar)
*/
void MyFrame::check_colorbar(
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
void MyFrame::radiovalue_colorscale(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii;
  double dminmax;

  if (event.GetId() == itext_colorscalemin) {
    max_and_min();
    if (event.GetString().ToDouble(&dminmax))
      patch_min[0] = (GLfloat)dminmax;
    paint();
    Refresh(false);
  }
  else if (event.GetId() == itext_colorscalemax) {
    max_and_min();
    if (event.GetString().ToDouble(&dminmax))
      patch_max[0] = (GLfloat)dminmax;
    paint();
    Refresh(false);
  }
  else if (event.GetString().Cmp(wxT("Input (Real)")) == 0) 
    color_scale = 1;
  else if (event.GetString().Cmp(wxT("Input (Complex)")) == 0)
    color_scale = 2;
  else if (event.GetString().Cmp(wxT("Input (Tri-number)")) == 0)
    color_scale = 3;
  else if (event.GetString().Cmp(wxT("Fermi Velocity")) == 0)
    color_scale = 4;
  else if (event.GetString().Cmp(wxT("Band Index")) == 0)
    color_scale = 5;
  else if (event.GetString().Cmp(wxT("Input (Real, Gray Scale)")) == 0)
    color_scale = 6;
  else if (event.GetString().Cmp(wxT("Fermi Velocity (Gray Scale)")) == 0)
    color_scale = 7;
} /* menu_colorscale */
/**
 @brief Modify and toggle appearance of equator (::lequator)
*/
void MyFrame::checkvalue_equator(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  double deqvec;

  if (event.GetId() == icheck_equator) {
    if (lequator != 1) lequator = 1;
    else lequator = 0;
    Refresh(false);
  }/*if (event.GetId() == 1)*/
  else {
    if (event.GetId() == itext_equatorx) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[0] = (GLfloat)deqvec;
    }
    else if (event.GetId() == itext_equatory) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[1] = (GLfloat)deqvec;
    }
    else if (event.GetId() == itext_equatorz) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[2] = (GLfloat)deqvec;
    }
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      eqvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        eqvec[ii] += eqvec_fr[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
  }/*else if (event.GetId() > 1)*/
} /*void menu_equator*/
/**
 @brief Modify interpolation ratio

 This routine modify interpolation ratio (::interpol) 
 then compute Fermi surfaces, etc.
*/
void MyFrame::textctrl_interpol(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  long int long_interpol;

  if (event.GetString().ToLong(&long_interpol)) {
    interpol = (int)long_interpol;
    refresh_interpol = 1;
  }
}/*static void menu_interpol*/
/**
 @brief Toggle Lighting (::lside)
*/
void MyFrame::radio_lighting(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Both side")) == 0) {
    lside = 1;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  }
  if (event.GetString().Cmp(wxT("Unoccupied side")) == 0) {
    lside = 2;
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  if (event.GetString().Cmp(wxT("Occupied side")) == 0) {
    lside = 3;
    side = -1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  Refresh(false);
} /* menu_lighting */
/**
 @brief Line width (::lside)
*/
void MyFrame::textctrl_line(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  double dlinewidth;

  if (event.GetString().ToDouble(&dlinewidth)) linewidth = (GLfloat)dlinewidth;
  Refresh(false);
} /* menu_line */
/**
 @brief Change the function associated to the mouse movement(::lmouse)
*/
void MyFrame::radio_mouse(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Rotate")) == 0) lmouse = 1;
  else if (event.GetString().Cmp(wxT("Scale")) == 0) lmouse = 2;
  else if (event.GetString().Cmp(wxT("Translate")) == 0) lmouse = 3;
  Refresh(false);
} /* menu_mouse */
/**
 @brief Toggle apearance of nodale-line
*/
void MyFrame::check_nodeline(
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
void MyFrame::radiovalue_section(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr, ii, jj, ib;
  double dsecvec;

  if (event.GetId() == icheck_section) {
    lsection = 0;
  }
  else if(event.GetId() == icheck_gamma) {
    if(secscale > 0.5) secscale = 0.0;
    else secscale = 1.0;
  }
  else {
    if (event.GetId() == itext_equatorx) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[0] = (GLfloat)dsecvec;
    }
    else if (event.GetId() == itext_equatory) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[1] = (GLfloat)dsecvec;
    }
    else if (event.GetId() == itext_equatorz) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[2] = (GLfloat)dsecvec;
    }
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      secvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        secvec[ii] += secvec_fr[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
  }/*else if (event.GetId() > 1)*/
} /*void menu_section*/
/**
 @brief Shift Fermi energy
*/
void MyFrame::textctrl_shift(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ib, i0, i1, i2, ierr;
  GLfloat emin, emax;
  double dEF;

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
  if (event.GetString().ToDouble(&dEF)) EF = (GLfloat)dEF;
} /* menu_shift */
/**
 @brief Tern stereogram (::lstereo)
*/
void MyFrame::radio_stereo(
  wxCommandEvent& event //!<[in] Selected menu
) {
  if (event.GetString().Cmp(wxT("None")) == 0) lstereo = 1;
  else if (event.GetString().Cmp(wxT("Parallel")) == 0) lstereo = 2;
  else if (event.GetString().Cmp(wxT("Cross")) == 0) lstereo = 3;
  Refresh(false);
} /* menu_stereo */
/**
 @brief Change tetrahedron (::itet)
*/
void MyFrame::radio_tetra(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  itet = wxAtoi(event.GetString());
  init_corner();
}/*menu_tetra*/
 /**
 @brief Setting of view

 This modify scale (::scl) & tarnslation (::trans) &
 rotation (::thetax, ::thetay, ::thetaz, ::rot),
 */
void MyFrame::textctrl_view(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  double dvalue;

  if (event.GetId() == itext_scale) {
    if (event.GetString().ToDouble(&dvalue)) scl = (GLfloat)dvalue;

  }
  else  if (event.GetId() == itext_positionx) {
    if (event.GetString().ToDouble(&dvalue)) trans[0] = (GLfloat)dvalue;

  }
  else  if (event.GetId() == itext_positiony) {
    if (event.GetString().ToDouble(&dvalue)) trans[1] = (GLfloat)dvalue;
  }
  else  if (event.GetId() == itext_rotx|| 
    event.GetId() == itext_roty|| 
    event.GetId() == itext_rotz) {
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
    if (event.GetId() == itext_rotx) {
      if (event.GetString().ToDouble(&dvalue)) thetax = (GLfloat)dvalue;
    }
    else if (event.GetId() == itext_roty) {
      if (event.GetString().ToDouble(&dvalue)) thetay = (GLfloat)dvalue;
    }
    else if (event.GetId() == itext_rotz) {
      if (event.GetString().ToDouble(&dvalue)) thetaz = (GLfloat)dvalue;
    }
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

  // Make a menubar
  wxMenu* fileMenu = new wxMenu;
  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("File"));
  SetMenuBar(menuBar);

  wxBoxSizer* sizermain = new wxBoxSizer(wxVERTICAL);

  wxSplitterWindow* splitterV = new wxSplitterWindow(this, wxID_ANY);
  splitterV->SetSashGravity(0.5);
  splitterV->SetMinimumPaneSize(20); // Smalest size the
  sizermain->Add(splitterV, 1, wxEXPAND, 0);

  panel = new wxPanel(splitterV);

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::button_compute, this, ibutton_compute);
  new wxButton(panel, ibutton_compute, wxT("Update"), wxPoint(10, 10));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_gamma);
  new wxCheckBox(panel, icheck_gamma, wxT("On Gamma"), wxPoint(210, 25));
  new wxStaticText(panel, wxID_ANY, wxT("Section-v :"),
    wxPoint(30, 52), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionx);
  new wxTextCtrl(panel, itext_sectionx, wxT(""), wxPoint(110, 50), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectiony);
  new wxTextCtrl(panel, itext_sectiony, wxT(""), wxPoint(210, 50), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionz);
  new wxTextCtrl(panel, itext_sectionz, wxT(""), wxPoint(310, 50), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Equator-v :"),
    wxPoint(30, 77), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorx);
  new wxTextCtrl(panel, itext_equatorx, wxT(""), wxPoint(110, 75), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatory);
  new wxTextCtrl(panel, itext_equatory, wxT(""), wxPoint(210, 75), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorz);
  new wxTextCtrl(panel, itext_equatorz, wxT(""), wxPoint(310, 75), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Interpol ratio :"),
    wxPoint(10, 102), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_interpol, this, itext_interpol);
  new wxTextCtrl(panel, itext_interpol, wxT(""), wxPoint(110, 100), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Fermi energy :"),
    wxPoint(10, 127), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_shift, this, itext_shift);
  new wxTextCtrl(panel, itext_shift, wxT(""), wxPoint(110, 125), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Min of Scale :"),
    wxPoint(15, 152), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemin);
  new wxTextCtrl(panel, itext_colorscalemin, wxT(""), wxPoint(110, 150), wxDefaultSize);
  new wxStaticText(panel, wxID_ANY, wxT("Max of Scale :"),
    wxPoint(15, 177), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemax);
  new wxTextCtrl(panel, itext_colorscalemax, wxT(""), wxPoint(110, 175), wxDefaultSize);

  wxString choices_colorscale[] = { wxT("Input (Real)"), wxT("Input (Complex)"),
    wxT("Input (Tri-number)"), wxT("Fermi Velocity"), wxT("Band Index"),
    wxT("Input (Real, Gray Scale)"), wxT("Fermi Velocity (Gray Scale)") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radiovalue_colorscale, this, iradio_colorscale);
  new wxRadioBox(panel, iradio_colorscale, wxT("Color scale mode"),
    wxPoint(10, 200), wxDefaultSize,
    WXSIZEOF(choices_colorscale), choices_colorscale,
    1, wxRA_SPECIFY_COLS);

  wxString choices_bz[] = { wxT("First Brillouin zone"), wxT("Primitive Brillouin zone") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_brillouinzone, this, iradio_brillouinzone);
  new wxRadioBox(panel, iradio_brillouinzone, wxT("Brillouin zone"),
    wxPoint(10, 400), wxDefaultSize,
    WXSIZEOF(choices_bz), choices_bz,
    1, wxRA_SPECIFY_COLS);

  wxString choices_tetra[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7") ,
wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"),
 wxT("15"), wxT("16") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_tetra, this, iradio_tetra);
  new wxRadioBox(panel, iradio_tetra, wxT("Tetrahedron"),
    wxPoint(220, 100), wxDefaultSize,
    WXSIZEOF(choices_tetra), choices_tetra,
    1, wxRA_SPECIFY_COLS);

  new wxStaticText(panel, wxID_ANY, wxT("Line width :"),
    wxPoint(30, 477), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_line, this, itext_line);
  new wxTextCtrl(panel, itext_line, wxT(""), wxPoint(110, 475), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Rotation :"),
    wxPoint(40, 502), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotx);
  new wxTextCtrl(panel, itext_rotx, wxT(""), wxPoint(110, 500), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_roty);
  new wxTextCtrl(panel, itext_roty, wxT(""), wxPoint(210, 500), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotz);
  new wxTextCtrl(panel, itext_rotz, wxT(""), wxPoint(310, 500), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Position :"),
    wxPoint(140, 527), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positionx);
  new wxTextCtrl(panel, itext_positionx, wxT(""), wxPoint(210, 525), wxDefaultSize);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positiony);
  new wxTextCtrl(panel, itext_positiony, wxT(""), wxPoint(310, 525), wxDefaultSize);

  new wxStaticText(panel, wxID_ANY, wxT("Scale :"),
    wxPoint(260, 552), wxDefaultSize, wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_scale);
  new wxTextCtrl(panel, itext_scale, wxT(""), wxPoint(310, 550), wxDefaultSize);

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_colorbar, this, icheck_colorbar);
  new wxCheckBox(panel, icheck_colorbar, wxT("Color bar"), wxPoint(110, 550));
  // debug fileMenu->Check(menu_colorbar_check, true);

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::checkvalue_equator, this, icheck_equator);
  new wxCheckBox(panel, icheck_equator, wxT("Equator"), wxPoint(110, 575));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_nodeline, this, icheck_nodeline);
  new wxCheckBox(panel, icheck_nodeline, wxT("Nodal line"), wxPoint(110, 600));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_section);
  new wxCheckBox(panel, icheck_section, wxT("Section"), wxPoint(110, 625));

  wxString choices_stereo[] = { wxT("None"), wxT("Parallel"), wxT("Cross") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_stereo, this, iradio_stereo);
  new wxRadioBox(panel, iradio_stereo, wxT("Stereogram"),
    wxPoint(210,575), wxDefaultSize,
    WXSIZEOF(choices_stereo), choices_stereo,
    1, wxRA_SPECIFY_COLS);

  wxString choices_bg[] = { wxT("Black"), wxT("White") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_background, this, iradio_background);
  new wxRadioBox(panel, iradio_background, wxT("Background color"),
    wxPoint(260, 670), wxDefaultSize,
    WXSIZEOF(choices_bg), choices_bg,
    1, wxRA_SPECIFY_COLS);

  wxString choices_mouse[] = { wxT("Rotate"), wxT("Scale"), wxT("Translate") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_mouse, this, iradio_mouse);
  new wxRadioBox(panel, iradio_mouse, wxT("Mouse Drag"),
    wxPoint(310, 575), wxDefaultSize,
    WXSIZEOF(choices_mouse), choices_mouse,
    1, wxRA_SPECIFY_COLS);

  wxString choices_light[] = { wxT("Both side"), wxT("Unoccupied side"), wxT("Occupied side") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_lighting, this, iradio_lighting);
  new wxRadioBox(panel, iradio_lighting, wxT("Lighting"),
    wxPoint(110, 670), wxDefaultSize,
    WXSIZEOF(choices_light), choices_light,
    1, wxRA_SPECIFY_COLS);

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

  wxSplitterWindow* splitterH = new wxSplitterWindow(splitterV, wxID_ANY);
  splitterH->SetSashGravity(0.5);
  splitterH->SetMinimumPaneSize(20); // Smalest size the

  m_canvas = new TestGLCanvas(splitterH, wxID_ANY, gl_attrib);

  terminal = new wxTextCtrl(splitterH, wxID_ANY, wxT(""),
    wxPoint(0, 250), wxSize(100, 50), wxTE_MULTILINE);
  splitterH->SplitHorizontally(m_canvas, terminal);

  splitterV->SplitVertically(splitterH, panel);

  SetSizer(sizermain);
  SetAutoLayout(true);

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

void MyFrame::modify_band() {
  int ib;

  for (ib = 0; ib < nb; ib++) {
    Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_band, this, icheck_band + ib);
    new wxCheckBox(panel, icheck_band + ib, wxString::Format(wxT("Band %d"), ib),
      wxPoint(10, 525 + 25 * ib));
    // debug imenu_band->Check(icheck_band + ib, true);
  }
  Show(true);

}
