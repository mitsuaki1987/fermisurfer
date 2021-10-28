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
@brief Handle operations associated to mouse drag and window resize
*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#include "wx/glcanvas.h"
#if defined(HAVE_GL_GLU_H)
#include <GL/glu.h>
#elif defined(HAVE_OPENGL_GLU_H)
#include <OpenGL/glu.h>
#endif

#include <cmath>
#include "draw.hpp"
#include "variable.hpp"
#include "operation.hpp"
#include "menu.hpp"

wxBEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
EVT_SIZE(TestGLCanvas::OnSize)
EVT_PAINT(TestGLCanvas::OnPaint)
EVT_CHAR(TestGLCanvas::OnChar)
EVT_MOUSE_EVENTS(TestGLCanvas::OnMouseEvent)
wxEND_EVENT_TABLE()

/**
 @brief Window resize

 Modify : ::sx, ::sy
*/
void TestGLCanvas::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen())
    return;
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);

  /*
   Scale of translation of mousepointer
  */
  sx = 1.0f / (GLfloat)event.GetSize().x;
  sy = 1.0f / (GLfloat)event.GetSize().y;
  // It's up to the application code to update the OpenGL viewport settings.
  // This is OK here only because there is only one canvas that uses the
  // context. See the cube sample for that case that multiple canvases are
  // made current with one context.
  glViewport(0, 0, event.GetSize().x, event.GetSize().y);
  /**/
  glMatrixMode(GL_PROJECTION);
  /**/
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)event.GetSize().x / (GLfloat)event.GetSize().y, 1.0, 100.0);
  //glOrtho(-1, 1, -(GLfloat)event.GetSize().y / (GLfloat)event.GetSize().x, (GLfloat)event.GetSize().y / (GLfloat)event.GetSize().x, 1.0, 100.0);
  /**/
  glMatrixMode(GL_MODELVIEW);
  Refresh(false);
  //myf->Show(true);
}
/**
 @brief Glut mouse function

 Modify : ::cx, ::cy, ::scl
*/
void TestGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
  static int dragging = 0;
  static float last_x, last_y;
  int i, j, wheel;
  GLfloat dx, dy, a, rot0[3][3], rot1[3][3], ax, ay;

  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  if (event.LeftIsDown())
  {
    if (!dragging)
    {
      dragging = 1;
    }
    else
    {
      /*
       Translation of mousepointer from starting point
      */
      dx = (event.GetX() - last_x) * sx;
      dy = (event.GetY() - last_y) * sy;
      /*
       Distanse from starting point
      */
      a = sqrtf(dx * dx + dy * dy);
      /**/
      if (lmouse == 1) {
        /**/
        if (a != 0.0) {
          /*
           Compute rotational matrix from translation of mousepointer
          */
          ax = -dy;
          ay = dx;
          /**/
          a = a * 10.0f;
          /**/
          rot0[0][0] = (ax * ax + ay * ay * cosf(a)) / (ax * ax + ay * ay);
          rot0[0][1] = ax * ay * (cosf(a) - 1.0f) / (ax * ax + ay * ay);
          rot0[0][2] = ay * sinf(a) / sqrtf(ax * ax + ay * ay);
          rot0[1][0] = ax * ay * (cosf(a) - 1.0f) / (ax * ax + ay * ay);
          rot0[1][1] = (ax * ax * cosf(a) + ay * ay) / (ax * ax + ay * ay);
          rot0[1][2] = ax * sinf(a) / sqrtf(ax * ax + ay * ay);
          rot0[2][0] = -ay * sinf(a) / sqrtf(ax * ax + ay * ay);
          rot0[2][1] = -ax * sinf(a) / sqrtf(ax * ax + ay * ay);
          rot0[2][2] = cosf(a);
          /**/
          for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) rot1[i][j] = rot[i][j];
          /**/
          for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
              rot[i][j] = rot0[i][0] * rot1[0][j]
                        + rot0[i][1] * rot1[1][j]
                        + rot0[i][2] * rot1[2][j];
            }
          }
          /*
          Print angle to text Box
          */
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
          thetax *= 180.0f / 3.14159265f;
          thetay *= 180.0f / 3.14159265f;
          thetaz *= 180.0f / 3.14159265f;
          myf->textbox_rotatex->ChangeValue(wxString::Format(wxT("%f"), thetax));
          myf->textbox_rotatey->ChangeValue(wxString::Format(wxT("%f"), thetay));
          myf->textbox_rotatez->ChangeValue(wxString::Format(wxT("%f"), thetaz));
          myf->Show(true);
        }
      }
      else if (lmouse == 2) {
        scl = scl * expf(-dy);
        myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
        linewidth *= expf(-dy);
        myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
        myf->Show(true);
      }
      else {
        trans[0] = trans[0] + dx;
        trans[1] = trans[1] - dy;
        myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
        myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
        myf->Show(true);
      }
      Refresh(false);
    }
    last_x = event.GetX();
    last_y = event.GetY();
  }
  else
  {
    dragging = 0;
  }

  wheel = event.GetWheelRotation();
  if (wheel > 0) {
    scl = scl * 1.1f; 
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 1.1f;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  else if (wheel < 0) {
    scl = scl * 0.9f;
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 0.9f;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  if (event.LeftDClick()) {
    trans[0] = (event.GetX() * sx * 2.0 - 1.0) / scl;
    trans[1] = -(event.GetY() * sy * 2.0 - 1.0) / scl;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    Refresh(false);
  }
}
/**
 @brief Glut special key function

 Modify : ::trans
*/
void TestGLCanvas::OnChar(wxKeyEvent& event)
{
  switch (event.GetKeyCode())
  {
  case 'a':
  case WXK_LEFT:
    trans[0] += - 0.1f;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->Show(true);
    Refresh(false);
    break;

  case 'd':
  case WXK_RIGHT:
    trans[0] += 0.1f;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->Show(true);
    Refresh(false);
    break;

  case 'w':
  case WXK_UP:
    trans[1] += 0.1f;
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    myf->Show(true);
    Refresh(false);
    break;

  case 's':
  case WXK_DOWN:
    trans[1] += - 0.1f;
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    myf->Show(true);
    Refresh(false);
    break;

  default:
    event.Skip();
    return;
  }
}

void TestGLCanvas::InitGL()
{
  // Make the new context current (activate it for use) with this canvas.
  SetCurrent(*m_glRC);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_COLOR_MATERIAL);
  PostSizeEventToParent();
}

TestGLCanvas::TestGLCanvas(wxWindow* parent,
  wxWindowID id,
  int* gl_attrib)
  : wxGLCanvas(parent, id, gl_attrib)
{
  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);
}

TestGLCanvas::~TestGLCanvas()
{
  delete m_glRC;
}
