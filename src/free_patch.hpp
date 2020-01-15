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
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include "menu.hpp"
void free_patch(int nb,  int refresh_patch,
  int* ntri, GLfloat**** kvp, GLfloat** kvp_rot, GLfloat**** matp,
  GLfloat** clr, GLfloat**** nmlp, GLfloat** nmlp_rot, GLfloat***** arw,
  GLfloat** arw_rot, int refresh_nodeline, int* nnl, GLfloat**** kvnl,
  GLfloat** kvnl_rot, int refresh_section, GLfloat** kv2d, GLfloat** clr2d,
  int refresh_equator, int* nequator, GLfloat**** kveq, GLfloat** kveq_rot);
void max_and_min(int nb, int nthreads, wxTextCtrl* terminal,
  MyFrame* myf, int color_scale, int* ntri, GLfloat**** matp,
  GLfloat *patch_min, GLfloat *patch_max, GLfloat**** nmlp);
void paint(int nb,  int color_scale,  int blackback,
  int* ntri,  GLfloat patch_min,  GLfloat patch_max,
  GLfloat**** kvp,  GLfloat**** matp,  GLfloat** clr,
  GLfloat**** nmlp,  GLfloat***** arw,
  GLfloat red[4], GLfloat green[4], GLfloat blue[4],
  GLfloat cyan[4], GLfloat magenta[4], GLfloat yellow[4],
  GLfloat wgray[4], GLfloat bgray[4]);

