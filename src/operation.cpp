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
#include "wx/glcanvas.h"

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GLU_H)
#include <GL/glu.h>
#elif defined(HAVE_OPENGL_GLU_H)
#include <OpenGL/glu.h>
#endif

#include <math.h>
#include "draw.hpp"
#include "variable.hpp"
#include "operation.hpp"

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
  /**/
  glMatrixMode(GL_MODELVIEW);
  Refresh(false);
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
        }
      }
      else if (lmouse == 2) {
        scl = scl * expf(-dy);
      }
      else {
        trans[0] = trans[0] + dx;
        trans[1] = trans[1] - dy;
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
    Refresh(false);
  }
  else if (wheel < 0) {
    scl = scl * 0.9f;
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
  case WXK_LEFT:
    trans[0] = trans[0] - 0.1f;
    Refresh(false);
    break;

  case WXK_RIGHT:
    trans[0] = trans[0] + 0.1f;
    Refresh(false);
    break;

  case WXK_UP:
    trans[1] = trans[1] + 0.1f;
    Refresh(false);
    break;

  case WXK_DOWN:
    trans[1] = trans[1] - 0.1f;
    Refresh(false);
    break;

  default:
    event.Skip();
    return;
  }

  Refresh(false);
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
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
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
