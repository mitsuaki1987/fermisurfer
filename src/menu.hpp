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
#if ! defined(FERMISURFER_MENU_H)
#define FERMISURFER_MENU_H

#include "wx/wx.h"
#include "operation.hpp"
#include "wx/gbsizer.h"
#include "wx/splitter.h"

void compute_patch_segment();
void refresh_patch_segment();

// The frame containing the GL canvas
class MyFrame : public wxFrame
{
public:
  MyFrame(wxFrame* frame,
    const wxString& title,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = wxDEFAULT_FRAME_STYLE);

  virtual ~MyFrame();
  void modify_band();
  TestGLCanvas* m_canvas;
  wxGridBagSizer* gbsizer;
  wxScrolledWindow* panel;
  wxRadioBox* radiobox_color;
  wxRadioBox* radiobox_tetra;
  wxRadioBox* radiobox_stereo;
  wxSplitterWindow* splitterV;
  wxSplitterWindow* splitterH;
  wxCheckBox* checkbox_section;
  /*
 Text Box
*/
  wxTextCtrl* textbox_scale;
  wxTextCtrl* textbox_positionx;
  wxTextCtrl* textbox_positiony;
  wxTextCtrl* textbox_rotatex;
  wxTextCtrl* textbox_rotatey;
  wxTextCtrl* textbox_rotatez;
  wxTextCtrl* textbox_min;
  wxTextCtrl* textbox_max;
  wxTextCtrl* textbox_linewidth;
  wxTextCtrl* textbox_BackGroundR;
  wxTextCtrl* textbox_BackGroundG;
  wxTextCtrl* textbox_BackGroundB;
  wxTextCtrl* textbox_LineColorR;
  wxTextCtrl* textbox_LineColorG;
  wxTextCtrl* textbox_LineColorB;
  wxTextCtrl* textbox_BZ_number0;
  wxTextCtrl* textbox_BZ_number1;
  wxTextCtrl* textbox_BZ_number2;
  wxTextCtrl* textbox_SectionR;
  wxTextCtrl* textbox_SectionG;
  wxTextCtrl* textbox_SectionB;
  wxTextCtrl* textbox_ArrowStart0;
  wxTextCtrl* textbox_ArrowStart1;
  wxTextCtrl* textbox_ArrowStart2;
  wxTextCtrl* textbox_ArrowEnd0;
  wxTextCtrl* textbox_ArrowEnd1;
  wxTextCtrl* textbox_ArrowEnd2;
  wxTextCtrl* textbox_ArrowDiff0;
  wxTextCtrl* textbox_ArrowDiff1;
  wxTextCtrl* textbox_ArrowDiff2;
  wxTextCtrl* textbox_sphereX;
  wxTextCtrl* textbox_sphereY;
  wxTextCtrl* textbox_sphereZ;
  wxTextCtrl* textbox_sphereR;
  wxTextCtrl* textbox_ArrowStep;
  wxTextCtrl* textbox_ArrowWidth;

private:
  void OnExit(wxCommandEvent& event);
  void check_band(wxCommandEvent& event);
  void radio_brillouinzone(wxCommandEvent& event);
  void textctrl_BackGround(wxCommandEvent& event);
  void textctrl_LineColor(wxCommandEvent& event);
  void check_colorbar(wxCommandEvent& event);
  void radiovalue_colorscale(wxCommandEvent& event);
  void checkvalue_equator(wxCommandEvent& event);
  void textctrl_interpol(wxCommandEvent& event);
  void radio_lighting(wxCommandEvent& event);
  void textctrl_line(wxCommandEvent& event);
  void radio_mouse(wxCommandEvent& event);
  void check_nodeline(wxCommandEvent& event);
  void radiovalue_section(wxCommandEvent& event);
  void textctrl_shift(wxCommandEvent& event);
  void radio_stereo(wxCommandEvent& event);
  void radio_tetra(wxCommandEvent& event);
  void textctrl_view(wxCommandEvent& event);
  void button_refresh(wxCommandEvent& event);
  void button_compute(wxCommandEvent& event);
  void radio_BarColor(wxCommandEvent& event);
  void button_section(wxCommandEvent& event);
  void textctrl_BZ_number(wxCommandEvent& event);
  void textctrl_Section(wxCommandEvent& event);
  void textctrl_Band(wxCommandEvent& event);
  void textctrl_Arrow(wxCommandEvent& event);
  void textctrl_sphere(wxCommandEvent& event);
  void check_perspective(wxCommandEvent& event);
  void button_nesting(wxCommandEvent& event);
  void textctrl_VectorField(wxCommandEvent& event);
  //wxDECLARE_EVENT_TABLE();
};

#endif // FERMISURFER_MENU_H
