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
#include <wx/wx.h>

void fermi_patch(int nb,  int nthreads,  int ng[3],  int ng0[3],  int shiftk[3],
  int fbz, GLfloat**** eig, GLfloat EF, GLfloat***** mat, GLfloat***** vf, int corner[6][4], GLfloat bvec[3][3], int nbragg,
  GLfloat bragg[26][3], GLfloat brnrm[26], GLfloat brnrm_min, wxTextCtrl* terminal, int* ntri, GLfloat**** kvp, GLfloat**** matp,
  GLfloat** clr, GLfloat***** arw, GLfloat**** nmlp, GLfloat** kvp_rot, GLfloat** nmlp_rot, GLfloat** arw_rot);
