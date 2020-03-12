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
#include "fermisurfer.xpm"
#include <wx/splitter.h>
#include <wx/gbsizer.h>
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
#include <cmath>

void compute_patch_segment() {
  if (refresh_interpol == 1){
    interpol_energy();
    refresh_patch = 1;
    refresh_interpol = 0;
  }
  if (refresh_patch == 1) {
    fermi_patch();
    refresh_color = 1;
    refresh_section = 1;
    refresh_equator = 1;
    refresh_nodeline = 1;
    refresh_patch = 0;
  }
  if (refresh_color == 1) {
    if (skip_minmax == 1) skip_minmax = 0;
    else max_and_min();
    paint();
    refresh_section = 1;
    refresh_color = 0;
  }
  if (refresh_nodeline == 1) {
    calc_nodeline();
    refresh_nodeline = 0;
  }
  if (refresh_section == 1) {
    calc_2dbz();
    calc_section();
    refresh_section = 0;
  }
  if (refresh_equator == 1) {
    equator();
    refresh_equator = 0;
  }
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
  ibutton_rotate,
  icheck_band,
  itext_BackGroundR,
  itext_BackGroundG,
  itext_BackGroundB,
  itext_LineColorR,
  itext_LineColorG,
  itext_LineColorB,
  iradio_BarColor,
  ibutton_section
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
void MyFrame::button_section(
  wxCommandEvent& event//!<[in] Selected menu
) {
  int ib, i2d, i, j;
  FILE *fp;
  fp = fopen("fermi_line.dat", "w");
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      for (i2d = 0; i2d < n2d[ib]; i2d++) {
        for (i = 0; i < 2; i++) {
          fprintf(fp, "%15.5e %15.5e\n",
            kv2d[ib][i * 3 + 6 * i2d], 
            kv2d[ib][1 + i * 3 + 6 * i2d]);
        }
        fprintf(fp, "\n\n");
      }
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  fclose(fp);
  *terminal << wxT("  fermi_line.dat was written.\n");

  fp = fopen("bz_line.dat", "w");
  for (i2d = 0; i2d < nbzl2d; i2d++) {
    fprintf(fp, "%15.5e %15.5e\n",
      bzl2d_proj[i2d][0], bzl2d_proj[i2d][1]);
  }
  fprintf(fp, "%15.5e %15.5e\n",
    bzl2d_proj[0][0], bzl2d_proj[0][1]);
  fclose(fp);
  *terminal << wxT("  bz_line.dat was written.\n");
}
/**
@brief Change Line color color (::blackback)
*/
void MyFrame::textctrl_LineColor(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  double dvalue;

  if (event.GetId() == itext_LineColorR) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[0] = (GLfloat)dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_LineColorG) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[1] = (GLfloat)dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_LineColorB) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[2] = (GLfloat)dvalue;
      Refresh(false);
    }
  }
}
/**
@brief Change background color (::blackback)
*/
void MyFrame::textctrl_BackGround(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ierr;
  double dvalue;

  if (event.GetId() == itext_BackGroundR) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[0] = (GLfloat)dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1],
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BackGroundG) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[1] = (GLfloat)dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1], 
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BackGroundB) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[2] = (GLfloat)dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1],
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
}
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
  }
  refresh_patch = 1;
} /* menu_brillouinzone */
/**
 @brief Change Brillouin zone (::fbz)
*/
void MyFrame::radio_BarColor(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  int ii;
  if (event.GetString().Cmp(wxT("BGR")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = blue[ii];
      BarColor[1][ii] = cyan[ii];
      BarColor[2][ii] = green[ii];
      BarColor[3][ii] = yellow[ii];
      BarColor[4][ii] = red[ii];
    }
  }
  else if (event.GetString().Cmp(wxT("CMY")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = cyan[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = magenta[ii];
      BarColor[3][ii] = red[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  else if (event.GetString().Cmp(wxT("MCY")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = magenta[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = cyan[ii];
      BarColor[3][ii] = green[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  paint();
  Refresh(false);
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
    if (event.GetString().ToDouble(&dminmax))
      patch_min = (GLfloat)dminmax;
    skip_minmax = 1;
  }
  else if (event.GetId() == itext_colorscalemax) {
    if (event.GetString().ToDouble(&dminmax))
      patch_max = (GLfloat)dminmax;
    skip_minmax = 1;
  }
  else if (event.GetString().Cmp(wxT("Input (1D)")) == 0) 
    color_scale = 1;
  else if (event.GetString().Cmp(wxT("Input (2D)")) == 0)
    color_scale = 2;
  else if (event.GetString().Cmp(wxT("Input (3D)")) == 0)
    color_scale = 3;
  else if (event.GetString().Cmp(wxT("Fermi Velocity")) == 0)
    color_scale = 4;
  else if (event.GetString().Cmp(wxT("Band Index")) == 0)
    color_scale = 5;
  else if (event.GetString().Cmp(wxT("Input (1D, Gray Scale)")) == 0)
    color_scale = 6;
  else if (event.GetString().Cmp(wxT("Fermi Velocity (Gray Scale)")) == 0)
    color_scale = 7;
  refresh_color = 1;
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
    refresh_equator = 1;
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
 @brief Toggle Lighting
*/
void MyFrame::radio_lighting(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Both")) == 0) {
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  }
  if (event.GetString().Cmp(wxT("Unoccupy")) == 0) {
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  if (event.GetString().Cmp(wxT("Occupy")) == 0) {
    side = -1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  Refresh(false);
} /* menu_lighting */
/**
 @brief Line width
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
    if (lsection == 0)lsection = 1;
    else lsection = 0;
    Refresh(false);
  }
  else if(event.GetId() == icheck_gamma) {
    if(secscale > 0.5) secscale = 0.0;
    else secscale = 1.0;
    refresh_section = 1;
  }
  else {
    if (event.GetId() == itext_sectionx) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[0] = (GLfloat)dsecvec;
    }
    else if (event.GetId() == itext_sectiony) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[1] = (GLfloat)dsecvec;
    }
    else if (event.GetId() == itext_sectionz) {
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
    refresh_section = 1;
  }/*else if (event.GetId() > 1)*/
} /*void menu_section*/
/**
 @brief Shift Fermi energy
*/
void MyFrame::textctrl_shift(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  double dEF;
  if (event.GetString().ToDouble(&dEF)) {
    EF = (GLfloat)dEF;
    refresh_patch = 1;
  }
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
  itet = wxAtoi(event.GetString()) - 1;
  init_corner();
  refresh_patch = 1;
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
    if (event.GetString().ToDouble(&dvalue)) {
      linewidth *= (GLfloat)dvalue/scl;
      scl = (GLfloat)dvalue;
      textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_positionx) {
    if (event.GetString().ToDouble(&dvalue)) {
      trans[0] = (GLfloat)dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_positiony) {
    if (event.GetString().ToDouble(&dvalue)) {
      trans[1] = (GLfloat)dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_rotx ||
    event.GetId() == itext_roty ||
    event.GetId() == itext_rotz) {
    /*
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
    */
    if (event.GetId() == itext_rotx) {
      if (event.GetString().ToDouble(&dvalue))
        thetax = (GLfloat)(dvalue) / 180.0f * 3.14159265f;
    }
    else if (event.GetId() == itext_roty) {
      if (event.GetString().ToDouble(&dvalue))
        thetay = (GLfloat)(dvalue) / 180.0f * 3.14159265f;
    }
    else if (event.GetId() == itext_rotz) {
      if (event.GetString().ToDouble(&dvalue))
        thetaz = (GLfloat)(dvalue) / 180.0f * 3.14159265f;
    }
  }
  else if(event.GetId() == ibutton_rotate){
    rot[0][0] = cosf(thetay)* cosf(thetaz);
    rot[0][1] = -cosf(thetay)* sinf(thetaz);
    rot[0][2] = sinf(thetay);
    rot[1][0] = cosf(thetaz)* sinf(thetax)* sinf(thetay) + cosf(thetax)* sinf(thetaz);
    rot[1][1] = cosf(thetax) * cosf(thetaz) - sinf(thetax)* sinf(thetay)* sinf(thetaz);
    rot[1][2] = -cosf(thetay)* sinf(thetax);
    rot[2][0] = -cosf(thetax)* cosf(thetaz)* sinf(thetay) + sinf(thetax)* sinf(thetaz);
    rot[2][1] = cosf(thetaz)* sinf(thetax) + cosf(thetax)* sinf(thetay)* sinf(thetaz);
    rot[2][2] = cosf(thetax)* cosf(thetay);
    Refresh(false);
  }
}

MyFrame::MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
  const wxSize& size, long style)
  : wxFrame(frame, wxID_ANY, title, pos, size, style),
  m_canvas(NULL)
{
  int ib, itet;
  char menuname[8];

  SetIcon(wxICON(fermisurfer));

  // Make a menubar
  //wxMenu* fileMenu = new wxMenu;
  //wxMenuBar* menuBar = new wxMenuBar;
  //menuBar->Append(fileMenu, wxT("File"));
  //SetMenuBar(menuBar);
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

  wxBoxSizer* sizermain = new wxBoxSizer(wxVERTICAL);

  splitterV = new wxSplitterWindow(this, wxID_ANY);
  splitterV->SetSashGravity(1.0);
  splitterV->SetMinimumPaneSize(0); 
  sizermain->Add(splitterV, 1, wxEXPAND, 0);

  panel = new wxPanel(splitterV);

  gbsizer = new wxGridBagSizer();

  splitterH = new wxSplitterWindow(splitterV, wxID_ANY);
  splitterH->SetSashGravity(1.0);
  splitterH->SetMinimumPaneSize(0);

  m_canvas = new TestGLCanvas(splitterH, wxID_ANY, gl_attrib);

  terminal = new wxTextCtrl(splitterH, wxID_ANY, wxT(""),
    wxPoint(0, 250), wxSize(100, 0), wxTE_MULTILINE);
  if (lbatch == 1) splitterH->SplitHorizontally(m_canvas, terminal, -1);
  else splitterH->SplitHorizontally(m_canvas, terminal, -200);
  panel->SetSizer(gbsizer);

  if (lbatch == 1)splitterV->SplitVertically(splitterH, panel, -1);
  else splitterV->SplitVertically(splitterH, panel, 0);

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::button_compute, this, ibutton_compute);
  gbsizer->Add(new wxButton(panel, ibutton_compute, wxT("Update")), wxGBPosition(0,0), wxGBSpan(1,1));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Line width : ")),
    wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_line, this, itext_line);
  textbox_linewidth = new wxTextCtrl(panel, itext_line, wxT(""));
  gbsizer->Add(textbox_linewidth, wxGBPosition(0, 2), wxGBSpan(1, 1));
  textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_gamma);
  wxCheckBox *check_ongamma = new wxCheckBox(panel, icheck_gamma, wxT("On Gamma"));
  gbsizer->Add(check_ongamma, wxGBPosition(0, 3), wxGBSpan(1, 1));
  check_ongamma->SetValue(true);

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Section-v : ")), 
    wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionx);
  wxTextCtrl* textbox_sectionx = new wxTextCtrl(panel, itext_sectionx, wxT(""));
  gbsizer->Add(textbox_sectionx, wxGBPosition(1, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectiony);
  wxTextCtrl* textbox_sectiony = new wxTextCtrl(panel, itext_sectiony, wxT(""));
  gbsizer->Add(textbox_sectiony, wxGBPosition(1, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionz);
  wxTextCtrl* textbox_sectionz = new wxTextCtrl(panel, itext_sectionz, wxT(""));
  gbsizer->Add(textbox_sectionz, wxGBPosition(1, 3), wxGBSpan(1, 1));
  textbox_sectionx->ChangeValue(wxT("0"));
  textbox_sectiony->ChangeValue(wxT("0"));
  textbox_sectionz->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Equator-v : ")), 
    wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorx);
  wxTextCtrl* textbox_equatorx = new wxTextCtrl(panel, itext_equatorx, wxT(""));
  gbsizer->Add(textbox_equatorx, wxGBPosition(2, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatory);
  wxTextCtrl* textbox_equatory = new wxTextCtrl(panel, itext_equatory, wxT(""));
  gbsizer->Add(textbox_equatory, wxGBPosition(2, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorz);
  wxTextCtrl* textbox_equatorz = new wxTextCtrl(panel, itext_equatorz, wxT(""));
  gbsizer->Add(textbox_equatorz, wxGBPosition(2, 3), wxGBSpan(1, 1));
  textbox_equatorx->ChangeValue(wxT("0"));
  textbox_equatory->ChangeValue(wxT("0"));
  textbox_equatorz->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Interpol ratio : ")),
    wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_interpol, this, itext_interpol);
  wxTextCtrl* textbox_interpol = new wxTextCtrl(panel, itext_interpol, wxT(""));
  gbsizer->Add(textbox_interpol, wxGBPosition(3, 1), wxGBSpan(1, 1));
  textbox_interpol->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Fermi energy : ")), 
    wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_shift, this, itext_shift);
  wxTextCtrl* textbox_shift = new wxTextCtrl(panel, itext_shift, wxT(""));
  gbsizer->Add(textbox_shift, wxGBPosition(4, 1), wxGBSpan(1, 1));
  textbox_shift->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Min of Scale : ")), 
    wxGBPosition(5, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemin);
  textbox_min = new wxTextCtrl(panel, itext_colorscalemin, wxT(""));
  gbsizer->Add(textbox_min, wxGBPosition(5, 1), wxGBSpan(1, 1));
  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Max of Scale : ")),
    wxGBPosition(6, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemax);
  textbox_max = new wxTextCtrl(panel, itext_colorscalemax, wxT(""));
  gbsizer->Add(textbox_max, wxGBPosition(6, 1), wxGBSpan(1, 1));

  wxString choices_tetra[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7") ,
wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"),
 wxT("15"), wxT("16") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_tetra, this, iradio_tetra);
  gbsizer->Add(new wxRadioBox(panel, iradio_tetra, wxT("Tetrahedron"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_tetra), choices_tetra,
    4, wxRA_SPECIFY_COLS), wxGBPosition(3,2), wxGBSpan(5, 2));

  wxString choices_colorscale[] = { wxT("Input (1D)"), wxT("Input (2D)"),
    wxT("Input (3D)"), wxT("Fermi Velocity"), wxT("Band Index"),
    wxT("Input (1D, Gray Scale)"), wxT("Fermi Velocity (Gray Scale)") };
  radiobox_color = new wxRadioBox(panel, iradio_colorscale,
    wxT("Color scale mode"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_colorscale), choices_colorscale,
    1, wxRA_SPECIFY_COLS);
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radiovalue_colorscale, this, iradio_colorscale);
  gbsizer->Add(radiobox_color, wxGBPosition(7, 0), wxGBSpan(3, 2));

  wxString choices_bz[] = { wxT("First Brillouin zone"), wxT("Primitive Brillouin zone") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_brillouinzone, this, iradio_brillouinzone);
  gbsizer->Add(new wxRadioBox(panel, iradio_brillouinzone, wxT("Brillouin zone"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_bz), choices_bz,
    1, wxRA_SPECIFY_COLS), wxGBPosition(8, 2), wxGBSpan(1, 2));

  wxString choices_stereo[] = { wxT("None"), wxT("Parallel"), wxT("Cross") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_stereo, this, iradio_stereo);
  gbsizer->Add(new wxRadioBox(panel, iradio_stereo, wxT("Stereogram"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_stereo), choices_stereo,
    1, wxRA_SPECIFY_COLS), wxGBPosition(9, 2), wxGBSpan(1, 1));

  wxString choices_mouse[] = { wxT("Rotate"), wxT("Scale"), wxT("Translate") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_mouse, this, iradio_mouse);
  gbsizer->Add(new wxRadioBox(panel, iradio_mouse, wxT("Mouse Drag"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_mouse), choices_mouse,
    1, wxRA_SPECIFY_COLS), wxGBPosition(9, 3), wxGBSpan(1, 1));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Background (RGB) : ")),
    wxGBPosition(10, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundR);
  textbox_BackGroundR = new wxTextCtrl(panel, itext_BackGroundR, wxT(""));
  gbsizer->Add(textbox_BackGroundR, wxGBPosition(10, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundG);
  textbox_BackGroundG = new wxTextCtrl(panel, itext_BackGroundG, wxT(""));
  gbsizer->Add(textbox_BackGroundG, wxGBPosition(10, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundB);
  textbox_BackGroundB = new wxTextCtrl(panel, itext_BackGroundB, wxT(""));
  gbsizer->Add(textbox_BackGroundB, wxGBPosition(10, 3), wxGBSpan(1, 1));
  textbox_BackGroundR->ChangeValue(wxT("0"));
  textbox_BackGroundG->ChangeValue(wxT("0"));
  textbox_BackGroundB->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Line color (RGB) : ")),
    wxGBPosition(11, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorR);
  textbox_LineColorR = new wxTextCtrl(panel, itext_LineColorR, wxT(""));
  gbsizer->Add(textbox_LineColorR, wxGBPosition(11, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorG);
  textbox_LineColorG = new wxTextCtrl(panel, itext_LineColorG, wxT(""));
  gbsizer->Add(textbox_LineColorG, wxGBPosition(11, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorB);
  textbox_LineColorB = new wxTextCtrl(panel, itext_LineColorB, wxT(""));
  gbsizer->Add(textbox_LineColorB, wxGBPosition(11, 3), wxGBSpan(1, 1));
  textbox_LineColorR->ChangeValue(wxT("1"));
  textbox_LineColorG->ChangeValue(wxT("1"));
  textbox_LineColorB->ChangeValue(wxT("1"));

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::textctrl_view, this, ibutton_rotate);
  gbsizer->Add(new wxButton(panel, ibutton_rotate, wxT("Rotate")), 
    wxGBPosition(12, 0), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotx);
  textbox_rotatex = new wxTextCtrl(panel, itext_rotx, wxT(""));
  gbsizer->Add(textbox_rotatex, wxGBPosition(12, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_roty);
  textbox_rotatey = new wxTextCtrl(panel, itext_roty, wxT(""));
  gbsizer->Add(textbox_rotatey, wxGBPosition(12, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotz);
  textbox_rotatez = new wxTextCtrl(panel, itext_rotz, wxT(""));
  gbsizer->Add(textbox_rotatez, wxGBPosition(12, 3), wxGBSpan(1, 1));
  textbox_rotatex->ChangeValue(wxT("0"));
  textbox_rotatey->ChangeValue(wxT("0"));
  textbox_rotatez->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Position : ")), 
    wxGBPosition(13, 1), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positionx);
  textbox_positionx = new wxTextCtrl(panel, itext_positionx, wxT(""));
  gbsizer->Add(textbox_positionx, wxGBPosition(13, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positiony);
  textbox_positiony = new wxTextCtrl(panel, itext_positiony, wxT(""));
  gbsizer->Add(textbox_positiony, wxGBPosition(13, 3), wxGBSpan(1, 1));
  textbox_positionx->ChangeValue(wxT("0"));
  textbox_positiony->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Scale : ")), 
    wxGBPosition(14, 2), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_scale);
  textbox_scale = new wxTextCtrl(panel, itext_scale, wxT(""));
  gbsizer->Add(textbox_scale, wxGBPosition(14, 3), wxGBSpan(1, 1));
  textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_colorbar, this, icheck_colorbar);
  wxCheckBox* check = new wxCheckBox(panel, icheck_colorbar, wxT("Color bar"));
  gbsizer->Add(check, wxGBPosition(14, 1), wxGBSpan(1, 1));
  check->SetValue(true);
  // debug fileMenu->Check(menu_colorbar_check, true);

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::checkvalue_equator, this, icheck_equator);
  gbsizer->Add(new wxCheckBox(panel, icheck_equator, wxT("Equator")), wxGBPosition(15, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_nodeline, this, icheck_nodeline);
  gbsizer->Add(new wxCheckBox(panel, icheck_nodeline, wxT("Nodal line")), wxGBPosition(16, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_section);
  gbsizer->Add(new wxCheckBox(panel, icheck_section, wxT("Section")), wxGBPosition(17, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::button_section, this, ibutton_section);
  gbsizer->Add(new wxButton(panel, ibutton_section, wxT("Section file")),
    wxGBPosition(18, 1), wxGBSpan(1, 1));

  wxString choices_light[] = { wxT("Both"), wxT("Unoccupy"), wxT("Occupy") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_lighting, this, iradio_lighting);
  gbsizer->Add(new wxRadioBox(panel, iradio_lighting, wxT("Lighting"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_light), choices_light,
    1, wxRA_SPECIFY_COLS), wxGBPosition(15, 2), wxGBSpan(4, 1));

  wxString choices_BarColor[] = { wxT("BGR"), wxT("CMY"), wxT("MCY")};
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_BarColor, this, iradio_BarColor);
  gbsizer->Add(new wxRadioBox(panel, iradio_BarColor, wxT("Bar Color"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_BarColor), choices_BarColor,
    1, wxRA_SPECIFY_COLS), wxGBPosition(15, 3), wxGBSpan(4, 1));

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
  int ib, width, height;
  wxCheckBox** check;

  radiobox_color->SetSelection(color_scale - 1);

  check = new wxCheckBox * [nb];

  for (ib = 0; ib < nb; ib++) {
    Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_band, this, icheck_band + ib);
    check[ib] = new wxCheckBox(panel, icheck_band + ib, 
      wxString::Format(wxT("Band %d"), ib));
    gbsizer->Add(check[ib], wxGBPosition(13 + ib, 0), wxGBSpan(1, 1));
    check[ib]->SetValue(true);
  }
  gbsizer->Layout();
  if (lbatch == 1) {
    splitterH->Unsplit();
    splitterV->Unsplit();
  }
  else {
    splitterV->SetSashPosition(-gbsizer->CalcMin().GetX());
  }
  splitterV->Layout();
  splitterH->Layout();
  Refresh(false);
  Raise();
}
