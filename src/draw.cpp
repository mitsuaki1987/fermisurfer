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
 @brief Functions for actual displaying
*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#include "wx/wx.h"
#include "wx/glcanvas.h"
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "variable.hpp"
#include "operation.hpp"
/**
Transform line to rectngular
*/
static void line2rect(
  GLfloat* line, 
  GLfloat* rect) 
{
  int ii, jj;
  GLfloat view[] = { 0.0, 0.0, -5.0 };
  GLfloat diff[3], dline[6], cross[6], thr = 0.00001, lcross;

  for (ii = 0; ii < 3; ii++) {
    diff[ii] = line[ii + 3] - line[ii];
    for (jj = 0; jj < 2; jj++) dline[3 * jj + ii] = line[3 * jj + ii] - view[ii];
  }
  //
  // Orthogonal to two vectors
  //
  for (jj = 0; jj < 2; jj++){
    cross[jj * 3 + 0] = dline[jj * 3 + 1] * diff[2] - dline[jj * 3 + 2] * diff[1];
    cross[jj * 3 + 1] = dline[jj * 3 + 2] * diff[0] - dline[jj * 3 + 0] * diff[2];
    cross[jj * 3 + 2] = dline[jj * 3 + 0] * diff[1] - dline[jj * 3 + 1] * diff[0];
  }
  //
  // Normalize
  //
  for (jj = 0; jj < 2; jj++) {
    lcross = 0.0;
    for (ii = 0; ii < 3; ii++) {
      lcross += cross[3 * jj + ii] * cross[3 * jj + ii];
    }
    lcross= sqrtf(lcross);
    if (lcross < thr) lcross = thr;
    for (ii = 0; ii < 3; ii++) cross[3 * jj + ii] *= linewidth * 0.01 / lcross;
  }
  //
  // Set thin rectangular
  //
  for (ii = 0; ii < 3; ii++) {
    rect[0 * 3 + ii] = line[0 * 3 + ii] + cross[0 * 3 + ii];
    rect[1 * 3 + ii] = line[0 * 3 + ii] - cross[0 * 3 + ii];
    rect[2 * 3 + ii] = line[1 * 3 + ii] - cross[1 * 3 + ii];
    rect[3 * 3 + ii] = line[1 * 3 + ii] + cross[1 * 3 + ii];
  }
}/**
Transform line to rectngular
*/
static void line2tri(
  GLfloat* line,
  GLfloat* tri)
{
  int ii, jj;
  GLfloat view[] = { 0.0, 0.0, -5.0 };
  GLfloat diff[3], dline[6], cross[6], thr = 0.00001, lcross;

  for (ii = 0; ii < 3; ii++) {
    diff[ii] = line[ii + 3] - line[ii];
    for (jj = 0; jj < 2; jj++) dline[3 * jj + ii] = line[3 * jj + ii] - view[ii];
  }
  //
  // Orthogonal to two vectors
  //
  for (jj = 0; jj < 2; jj++) {
    cross[jj * 3 + 0] = dline[jj * 3 + 1] * diff[2] - dline[jj * 3 + 2] * diff[1];
    cross[jj * 3 + 1] = dline[jj * 3 + 2] * diff[0] - dline[jj * 3 + 0] * diff[2];
    cross[jj * 3 + 2] = dline[jj * 3 + 0] * diff[1] - dline[jj * 3 + 1] * diff[0];
  }
  //
  // Normalize
  //
  for (jj = 0; jj < 2; jj++) {
    lcross = 0.0;
    for (ii = 0; ii < 3; ii++) {
      lcross += cross[3 * jj + ii] * cross[3 * jj + ii];
    }
    lcross = sqrtf(lcross);
    if (lcross < thr) lcross = thr;
    for (ii = 0; ii < 3; ii++) cross[3 * jj + ii] *= linewidth * 0.01 / lcross;
  }
  //
  // Set thin rectangular
  //
  for (ii = 0; ii < 3; ii++) {
    tri[0 * 3 + ii] = line[0 * 3 + ii] + cross[0 * 3 + ii];
    tri[1 * 3 + ii] = line[0 * 3 + ii] - cross[0 * 3 + ii];
    tri[2 * 3 + ii] = line[1 * 3 + ii] - cross[1 * 3 + ii];
  }
}
/**
 @brief Draw Fermi surfaces

 Modify : ::nmlp_rot, ::kvp_rot, ::kvnl_rot

 First, rotate ::nmlp and ::kvp with OpenMP then draw them.
 Also draw nodeline in the same way.
*/
static void draw_fermi() {
  int ib, a0, a1, a2, ia;
  GLfloat kshift[3];
  /*
   First, rotate k-vector and normal vector
  */
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;
#pragma omp parallel default(none) \
shared(nb,draw_band,ntri,rot,nmlp,nmlp_rot,kvp,kvp_rot,trans,side,kshift) \
private(ib)
        {
          int i, j, l, itri;

          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
#pragma omp for nowait
              for (itri = 0; itri < ntri[ib]; ++itri) {
                for (i = 0; i < 3; ++i) {
                  for (j = 0; j < 3; ++j) {
                    kvp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * (kvp[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvp[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvp[ib][itri][i][2] + kshift[2])
                      + trans[j];
                    nmlp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * nmlp[ib][itri][i][0]
                      + rot[j][1] * nmlp[ib][itri][i][1]
                      + rot[j][2] * nmlp[ib][itri][i][2];
                    nmlp_rot[ib][j + 3 * i + 9 * itri] *= side;
                  }
                }/*for (i = 0; i < 3; ++i)*/
              }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
            }/*if (draw_band[ib] == 1)*/
          }/*for (ib = 0; ib < nb; ib++)*/
        }/*End of parallel region*/
        /*
          Second, draw each triangle
        */
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        for (ib = 0; ib < nb; ib++) {
          if (draw_band[ib] == 1) {
            glVertexPointer(3, GL_FLOAT, 0, kvp_rot[ib]);
            glNormalPointer(GL_FLOAT, 0, nmlp_rot[ib]);
            glColorPointer(4, GL_FLOAT, 0, clr[ib]);
            glDrawArrays(GL_TRIANGLES, 0, ntri[ib] * 3);
          }/*if (draw_band[ib] == 1)*/
        }/*for (ib = 0; ib < nb; ib++)*/
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        /*
        Arrow for 3D value
        */
        glNormal3f(0.0f, 0.0f, 1.0f);
        if (color_scale == 3) {
#pragma omp parallel default(none) \
shared(nb,draw_band,ntri,rot,arw,arw_rot,trans,kshift) \
private(ib)
          {
            int i, j, l, itri;
            GLfloat line[6];

            for (ib = 0; ib < nb; ib++) {
              if (draw_band[ib] == 1) {
#pragma omp for nowait
                for (itri = 0; itri < ntri[ib]; ++itri) {
                  for (i = 0; i < 3; ++i) {
                    for (j = 0; j < 3; ++j) {
                      for (l = 0; l < 2; ++l) {
                        line[j + 3 * l]
                          = rot[j][0] * (arw[ib][itri][i][l][0] + kshift[0])
                          + rot[j][1] * (arw[ib][itri][i][l][1] + kshift[1])
                          + rot[j][2] * (arw[ib][itri][i][l][2] + kshift[2])
                          + trans[j];
                      }
                    }
                    for (l = 0; l < 2; ++l) line[2 + 3 * l] += 0.001f;
                    line2tri(line, &arw_rot[ib][9 * i + 27 * itri]);
                  }/*for (i = 0; i < 3; ++i)*/
                }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
              }
            }
          }/*End of parallel region*/
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glColor3f(1.0 - clr[ib][0], 1.0 - clr[ib][1], 1.0 - clr[ib][2]);
              glVertexPointer(3, GL_FLOAT, 0, arw_rot[ib]);
              glDrawArrays(GL_TRIANGLES, 0, ntri[ib] * 3 * 3);
            }
          }
        }
        /*
         Nodeline
        */
        if (nodeline == 1) {
          /*
           First, rotate k-vector
          */
#pragma omp parallel default(none) \
shared(nb,draw_band,nnl,rot,trans,kvnl,kvnl_rot,kshift) \
private(ib)
          {
            int i, j, itri;
            GLfloat line[6];

            for (ib = 0; ib < nb; ib++) {
              /**/
              if (draw_band[ib] == 1) {
#pragma omp for nowait
                for (itri = 0; itri < nnl[ib]; ++itri) {
                  for (i = 0; i < 2; ++i) {
                    /**/
                    for (j = 0; j < 3; ++j)
                      line[j + 3 * i]
                      = rot[j][0] * (kvnl[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvnl[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvnl[ib][itri][i][2] + kshift[2])
                      + trans[j];
                    line[2 + 3 * i] += 0.001f;
                  }/*for (i = 0; i < 2; ++i)*/
                  line2rect(line, &kvnl_rot[ib][12 * itri]);
                }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
              }/*if (draw_band[ib] == 1)*/
            }/*for (ib = 0; ib < nb; ib++)*/
          }/*End of parallel region*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0f, 0.0f, 1.0f);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kvnl_rot[ib]);
              glDrawArrays(GL_QUADS, 0, 4 * nnl[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
        /*
        Equator
        */
        if (lequator == 1) {
          /*
          First, rotate k-vector
          */
#pragma omp parallel default(none) \
shared(nb,draw_band,nequator,rot,trans,kveq,kveq_rot,kshift) \
private(ib)
          {
            int i, j, itri;
            GLfloat line[6];

            for (ib = 0; ib < nb; ib++) {
              /**/
              if (draw_band[ib] == 1) {
#pragma omp for nowait
                for (itri = 0; itri < nequator[ib]; ++itri) {
                  for (i = 0; i < 2; ++i) {
                    /**/
                    for (j = 0; j < 3; ++j)
                      line[j + 3 * i]
                      = rot[j][0] * (kveq[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kveq[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kveq[ib][itri][i][2] + kshift[2])
                      + trans[j];
                    line[2 + 3 * i] += 0.001f;
                  }/*for (i = 0; i < 2; ++i)*/
                  line2rect(line, &kveq_rot[ib][12 * itri]);
                }/*for (itri = 0; itri < nequator[ib]; ++itri)*/
              }/*if (draw_band[ib] == 1)*/
            }/*for (ib = 0; ib < nb; ib++)*/
          }/*End of parallel region*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0f, 0.0f, 1.0f);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kveq_rot[ib]);
              glDrawArrays(GL_QUADS, 0, 4 * nequator[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
      }
    }
  }
}/*void draw_fermi*/
/**
 @brief Draw lines of BZ boundaries
*/
static void draw_bz_lines() {
  int ibzl, i, j, a0, a1, a2, ia, icount;
  GLfloat bzl2[3], bvec2[3][3], linecolor[4], secvec2[3], kshift[3], arrow_c[3];
  GLfloat vertices[300], sphere_v2[1140]/*190*2*3*/, rect[600];
  //
  // Line color is oposit of BG color
  //
  for (i = 0; i < 4; i++) linecolor[i] = LineColor[i];
  //
  glLineWidth(linewidth);
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;
        //
        // First Brillouin zone mode
        //
        if (fbz == 1) {
          for (ibzl = 0; ibzl < nbzl; ++ibzl) {
            for (i = 0; i < 2; ++i) {
              for (j = 0; j < 3; ++j)
                bzl2[j] = rot[j][0] * (bzl[ibzl][i][0] + kshift[0])
                        + rot[j][1] * (bzl[ibzl][i][1] + kshift[1])
                        + rot[j][2] * (bzl[ibzl][i][2] + kshift[2])
                        + trans[j];
              for (j = 0; j < 3; j++) vertices[j + 3 * i] = bzl2[j];
            }/*for (i = 0; i< 2; ++i)*/
            line2rect(vertices, rect);
            glColor3fv(linecolor);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertexPointer(3, GL_FLOAT, 0, rect);
            glDrawArrays(GL_QUADS, 0, 4);
          }/*for (ibzl = 0; ibzl < nbzl; ++ibzl)*/
        }/*if (fbz == 1)*/
        else {
          //
          //  Premitive BZ mode
          //
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
              bvec2[i][j] = rot[j][0] * bvec[i][0]
                          + rot[j][1] * bvec[i][1]
                          + rot[j][2] * bvec[i][2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 3; ++i) vertices[i + 3 * 0 ] = trans[i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 1 ] = trans[i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 2 ] = trans[i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 3 ] = trans[i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 4 ] = trans[i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 5 ] = trans[i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 6 ] = trans[i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 7 ] = trans[i] + bvec2[0][i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 8 ] = trans[i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 9 ] = trans[i] + bvec2[0][i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 10] = trans[i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 11] = trans[i] + bvec2[1][i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 12] = trans[i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 13] = trans[i] + bvec2[1][i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 14] = trans[i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 15] = trans[i] + bvec2[2][i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 16] = trans[i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 17] = trans[i] + bvec2[2][i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 18] = trans[i] + bvec2[1][i] + bvec2[2][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 19] = trans[i] + bvec2[1][i] + bvec2[2][i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 20] = trans[i] + bvec2[2][i] + bvec2[0][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 21] = trans[i] + bvec2[2][i] + bvec2[0][i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 22] = trans[i] + bvec2[0][i] + bvec2[1][i];
          for (i = 0; i < 3; ++i) vertices[i + 3 * 23] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i];
          for (i = 0; i < 24; ++i) {
            for (j = 0; j < 3; ++j) {
              vertices[j + 3 * i] = vertices[j + 3 * i]
                + rot[j][0] * kshift[0]
                + rot[j][1] * kshift[1]
                + rot[j][2] * kshift[2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 12; ++i) {
            line2rect(&vertices[6*i], &rect[12*i]);
          }
          glColor3fv(linecolor);
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertexPointer(3, GL_FLOAT, 0, rect);
          glDrawArrays(GL_QUADS, 0, 48);
        }/*if (fbz != 1)*/
      }
    }
  }
  //
  // Arrow
  //
  for (i = 0; i < 2; ++i) {
    for (j = 0; j < 3; ++j)
      arrow_c[j] = bvec[0][j] * arrow[i][0]
                 + bvec[1][j] * arrow[i][1]
                 + bvec[2][j] * arrow[i][2];
    for (j = 0; j < 3; ++j)
      vertices[j + 3 * i] = rot[j][0] * arrow_c[0]
                          + rot[j][1] * arrow_c[1]
                          + rot[j][2] * arrow_c[2]
                          + trans[j];
  }/*for (i = 0; i< 2; ++i)*/
  line2tri(vertices, rect);
  glColor3fv(linecolor);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertexPointer(3, GL_FLOAT, 0, rect);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  //
  // Section for the 2D Fermi line
  //
  if (lsection == 1 && fbz == 1) {
    for (j = 0; j < 3; ++j)
      secvec2[j] = rot[j][0] * secvec[0]
                 + rot[j][1] * secvec[1]
                 + rot[j][2] * secvec[2];
    for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
      for (j = 0; j < 3; ++j)
        bzl2[j] = rot[j][0] * bzl2d[ibzl][0]
                + rot[j][1] * bzl2d[ibzl][1]
                + rot[j][2] * bzl2d[ibzl][2]
                + trans[j];
      for (j = 0; j < 3; j++)vertices[j + 3 * ibzl] = bzl2[j];
    }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
    glColor3fv(SectionColor);
    glNormal3fv(secvec2);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nbzl2d);
  }/*if (lsection == 1)*/
  //
  // Wireflame Sphere 
  //
  for (ibzl = 0; ibzl < 190; ibzl++) {
    for (i = 0; i < 2; ++i) {
      for (j = 0; j < 3; ++j)
        sphere_v2[j + 3 * i + 6 * ibzl] = rot[j][0] * sphere_v[ibzl][i][0]
                                        + rot[j][1] * sphere_v[ibzl][i][1]
                                        + rot[j][2] * sphere_v[ibzl][i][2]
                                        + trans[j];
    }/*for (i = 0; i< 2; ++i)*/
  }
  glColor3fv(linecolor);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertexPointer(3, GL_FLOAT, 0, sphere_v2);
  glDrawArrays(GL_LINES, 0, 380);
}/*draw bz_lines */
/**
 @brief Draw color-bar or colr-circle (periodic) 
        as a color scale
*/
static void draw_colorbar()
{
  int i, j, k;
  GLfloat mat2, vertices[366], colors[488], vector[6], rect[27], rect_color[36],
    norm;

  glEnableClientState(GL_COLOR_ARRAY);
  /*
  Reciplocal lattice vectors
  */
  for (i = 0; i < 3; i++) {
    vector[0] = -1.2f;
    vector[1] = -1.05f;
    vector[2] = 0.0f;

    norm = sqrtf(bvec[i][0] * bvec[i][0]+ bvec[i][1] * bvec[i][1]+ bvec[i][2] * bvec[i][2]);
    for (j = 0; j < 3; j++) {
      vector[3+j] = vector[j]
        + (rot[j][0] * bvec[i][0]
         + rot[j][1] * bvec[i][1]
         + rot[j][2] * bvec[i][2]) * 0.2f / norm;
    }
    line2tri(vector, &rect[9 * i]);
    for (k = 0; k < 3; k++) {
      for (j = 0; j < 4; j++) {
        rect_color[12 * i + 4 * k + j] = BarColor[i * 2][j];
      }
    }
  }
  glLineWidth(linewidth);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertexPointer(3, GL_FLOAT, 0, rect);
  glColorPointer(4, GL_FLOAT, 0, rect_color);
  glDrawArrays(GL_TRIANGLES, 0, 9);
  /*
  Cartesian (XYZ) flame
  */
  for (i = 0; i < 3; i++) {
    vector[0] = 1.2f;
    vector[1] = -1.05f;
    vector[2] = 0.0f;
  
    for (j = 0; j < 3; j++) {
      vector[3 + j] = vector[j] + rot[j][i] * 0.2f;
    }
    line2tri(vector, &rect[9 * i]);
    for (k = 0; k < 3; k++) {
      for (j = 0; j < 4; j++) {
        rect_color[12 * i + 4 * k + j] = BarColor[i * 2][j];
      }
    }
  }
  glLineWidth(linewidth);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertexPointer(3, GL_FLOAT, 0, rect);
  glColorPointer(4, GL_FLOAT, 0, rect_color);
  glDrawArrays(GL_TRIANGLES, 0, 9);
  /*
   Color bar/circle/cube
  */
  if (color_scale == 1 || color_scale == 4) {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0f + 0.5f*(GLfloat)i;
        vertices[1 + j * 3 + i * 6] = -1.0f - 0.1f*(GLfloat)j;
        vertices[2 + j * 3 + i * 6] = 0.0f;
        for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = BarColor[i][k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
  }/*if (color_scale == 1 || color_scale == 4)*/
  else if (color_scale == 2) {
    /*
     Periodic color scale
    */
    vertices[0] = 0.0f;
    vertices[1] = -1.0f;
    vertices[2] = 0.0f;
    for (j = 0; j < 4; j++) colors[j] = 1.0f - BackGroundColor[j];
    /**/
    for (i = 0; i <= 60; i++) {
      /**/
      mat2 = (GLfloat)i / 60.0f * 6.0f;
      /**/
      if (mat2 <= 1.0) {
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = yellow[j] * mat2 + red[j] * (1.0f - mat2);
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0f;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = green[j] * mat2 + yellow[j] * (1.0f - mat2);
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0f;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = cyan[j] * mat2 + green[j] * (1.0f - mat2);
      }
      else if (mat2 <= 4.0) {
        mat2 = mat2 - 3.0f;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = blue[j] * mat2 + cyan[j] * (1.0f - mat2);
      }
      else if (mat2 <= 5.0) {
        mat2 = mat2 - 4.0f;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = magenta[j] * mat2 + blue[j] * (1.0f - mat2);
      }
      else {
        mat2 = mat2 - 5.0f;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = red[j] * mat2 + magenta[j] * (1.0f - mat2);
      }
      /**/
      vertices[0 + 3 * (i + 1)] = 0.2f * cosf((GLfloat)i / 60.0f * 6.283185307f);
      vertices[1 + 3 * (i + 1)] = 0.2f * sinf((GLfloat)i / 60.0f * 6.283185307f) - 1.0f;
      vertices[2 + 3 * (i + 1)] = 0.0f;
    }/*for (i = 0; i <= 60; i++)*/
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }/*else if (color_scale == 2)*/
  else  if (color_scale == 6 || color_scale == 7) {
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0f + 2.0f*(GLfloat)i;
        vertices[1 + j * 3 + i * 6] = -1.0f - 0.1f*(GLfloat)j;
        vertices[2 + j * 3 + i * 6] = 0.0f;
        if (i == 0) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = bgray[k];
        else if (i == 1) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = wgray[k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }/*if (color_scale == 6 || color_scale == 7)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*void draw_colorbar*/
/**
 @brief Draw eye-points for the stereogram
*/
static void draw_circles(
  GLfloat dx2d //!< [in] Translation used for the section-mode
) {
  int i, j;
  GLfloat r, vertices[66];
  /**/
  r = 0.05f;
  /**/
  vertices[0] = 0.7f - dx2d;
  vertices[1] = scl;
  vertices[2] = 0.0f;
  for (i = 0; i <= 20; i++) {
    vertices[0 + (i + 1) * 3] = r * cosf((GLfloat)i / 20.0f * 6.283185307f) + 0.7f - dx2d;
    vertices[1 + (i + 1) * 3] = r * sinf((GLfloat)i / 20.0f * 6.283185307f) + scl;
    vertices[2 + (i + 1) * 3] = 0.0f;
  }/*for (i = 0; i <= 20; i++)*/
  glNormal3f(0.0f, 0.0f, 1.0f);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
  /**/
  for (i = 0; i < 22; i++) vertices[3 * i] += -1.4f;
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}/*void draw_circles*/
/**
 @brief Draw 2D Fermi lines
*/
static void draw_fermi_line() {
  int i, ib, ibzl, i2d;
  GLfloat vertices[6], rect[120];
  /*
   Draw 2D BZ lines
  */
  glLineWidth(linewidth);
  for (i = 0; i < 3; i++) vertices[i] = bzl2d_proj[nbzl2d - 1][i];
  for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
    for (i = 0; i < 3; i++)vertices[3 + i] = bzl2d_proj[ibzl][i];
    line2rect(vertices, &rect[12 * ibzl]);
    for (i = 0; i < 3; i++)vertices[i] = vertices[3 + i];
  }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
  glNormal3f(0.0f, 0.0f, 1.0f);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, rect);
  glDrawArrays(GL_QUADS, 0, nbzl2d*4);
  /*
   Draw Fermi lines
  */
  glLineWidth(linewidth);
  glEnableClientState(GL_COLOR_ARRAY);
  glNormal3f(0.0f, 0.0f, 1.0f);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      for (i2d = 0; i2d < n2d[ib]; i2d++) {
        line2rect(&kv2d[ib][6 * i2d], &kv2d_fat[ib][12 * i2d]);
      }
      glVertexPointer(3, GL_FLOAT, 0, kv2d_fat[ib]);
      glColorPointer(4, GL_FLOAT, 0, clr2d[ib]);
      glDrawArrays(GL_QUADS, 0, 4 * n2d[ib]);
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*void draw_fermi_line*/
/**
 @brief Glut Display function
 called by glutDisplayFunc
*/
void TestGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  GLfloat pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  GLfloat amb[] = { 0.2f, 0.2f, 0.2f, 0.0f };
  GLfloat dx, dx2d, theta, posz, phi;
  GLfloat pos1[4], pos2[4];
  int ierr;
  char command_name[256];

  if (draw_band == NULL) return;

  // This is a dummy, to avoid an endless succession of paint messages.
  // OnPaint handlers must always create a wxPaintDC.
  wxPaintDC dc(this);

  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);

  if (lstereo == 2) {
    /*
     Parallel eyes
    */
    theta = 3.1416f / 180.0f * 2.5f;
    posz = 5.0f;
    dx = 0.7f;
    phi = atanf(posz / dx) - theta;
    theta = (3.1416f * 0.5f - phi) / 3.1416f * 180.0f;
    /**/
    pos1[0] = posz * cosf(phi) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * sinf(phi);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * cosf(phi) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * sinf(phi);
    pos2[3] = 1.0;
  }/*if (lstereo == 2)*/
  else if (lstereo == 3) {
    /*
     Cross eyes
    */
    theta = -3.1416f / 180.0f * 2.0f;
    posz = 5.0;
    dx = -0.7f;
    /**/
    pos1[0] = posz * sinf(theta) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * cosf(theta);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * sinf(theta) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * cosf(theta);
    pos2[3] = 1.0;
  }/*if (lstereo == 3)*/
  else {
    theta = 0.0;
    dx = 0.0;
  }/*lstero == 1*/
  if (lsection == 1 && fbz == 1) dx2d = 0.7f;
  else dx2d = 0.0;
  /*
   Initialize
  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /*
   Set view point & view line
  */
  glTranslatef(0.0, 0.0, -5.0);
  //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /*
   Set position of light
  */
  if (lstereo == 1) {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glTranslatef(-dx2d, 0.0, 0.0);
    /*
     Draw color scale
    */
    if (lcolorbar == 1) draw_colorbar();
  }
  else {
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    draw_circles(dx2d);
    glTranslatef(-dx-dx2d, 0.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);
  }
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  /*
   Rotation & Zoom
  */
  glScalef(scl, scl, scl);
  /*
   Draw Brillouin zone boundaries
  */
  draw_bz_lines();
  /*
   Draw Fermi surfaces
  */
  draw_fermi();
  /*
   Draw the second object for stereogram
  */
  if (lstereo != 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos2);
    /**/
    glTranslatef(dx-dx2d, 0.0, 0.0);
    glRotatef(-theta, 0.0, 1.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_bz_lines();
    draw_fermi();
    /**/
    glPopMatrix();
  }
  /*
   Draw 2D Fermi line
  */
  if (lsection == 1 && fbz == 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    /**/
    if (lstereo == 1) glTranslatef(dx2d, 0.0, 0.0);
    else glTranslatef(2.0f * dx2d, 0.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_fermi_line();
    /**/
    glPopMatrix();
  }/*if (lsection == 1)*/
  glFlush(); // Not really necessary: buffer swapping below implies glFlush()
  SwapBuffers();
  if (lbatch == 1) {
    char file2[256], batch2[256];
    glFlush();
    strncpy(file2, (const char*)frmsf_file_name.mb_str(wxConvUTF8), 255);
    strncpy(batch2, (const char*)batch_name.mb_str(wxConvUTF8), 255);
    sprintf(command_name, "import -window \"%s\" %s.png", file2, batch2);
    ierr = system(command_name);
    exit(0);
  }
}/*void display*/
