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
#include "wx/wx.h"
#include "operation.hpp"

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

  TestGLCanvas* m_canvas;

private:
  void OnExit(wxCommandEvent& event);
  void menu_brillouinzone(wxCommandEvent& event);
  void menu_background(wxCommandEvent& event);
  void menu_band(wxCommandEvent& event);
  void menu_colorbar(wxCommandEvent& event);
  void menu_colorscale(wxCommandEvent& event);
  void menu_equator(wxCommandEvent& event);
  void menu_interpol(wxCommandEvent& event);
  void menu_lighting(wxCommandEvent& event);
  void menu_line(wxCommandEvent& event);
  void menu_mouse(wxCommandEvent& event);
  void menu_nodeline(wxCommandEvent& event);
  void menu_section(wxCommandEvent& event);
  void menu_shift(wxCommandEvent& event);
  void menu_stereo(wxCommandEvent& event);
  void menu_tetra(wxCommandEvent& event);
  void menu_view(wxCommandEvent& event);
  wxDECLARE_EVENT_TABLE();
};
