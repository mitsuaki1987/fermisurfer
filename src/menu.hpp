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
  wxPanel* panel;
  wxRadioBox* radiobox_color;
  wxSplitterWindow* splitterV;
  wxSplitterWindow* splitterH;
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

private:
  void OnExit(wxCommandEvent& event);
  void check_band(wxCommandEvent& event);
  void radio_brillouinzone(wxCommandEvent& event);
  void radio_background(wxCommandEvent& event);
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
  //wxDECLARE_EVENT_TABLE();
};

#endif // FERMISURFER_MENU_H
