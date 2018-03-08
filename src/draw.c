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
#if defined(HAVE_GL_GLUT_H)
#include <GL/glut.h>
#elif defined(HAVE_GLUT_GLUT_H)
#include <GLUT/glut.h>
#endif

#include <math.h>
#include "variable.h"
#include <stdio.h>
/**
 @brief Draw Fermi surfaces

 Modify : ::nmlp_rot, ::kvp_rot, ::kvnl_rot

 First, rotate ::nmlp and ::kvp with OpenMP then draw them.
 Also draw nodeline in the same way.
*/
static void draw_fermi() {
  int ib;
  /*
   First, rotate k-vector and normal vector
  */
#pragma omp parallel default(none) \
  shared(nb,draw_band,ntri,rot,nmlp,nmlp_rot,kvp,kvp_rot,trans,side) \
  private(ib)
  {
    int i, j, itri;

    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] == 1) {
#pragma omp for nowait
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
              kvp_rot[ib][j + 3 * i + 9 * itri]
                = rot[j][0] * kvp[ib][itri][i][0]
                + rot[j][1] * kvp[ib][itri][i][1]
                + rot[j][2] * kvp[ib][itri][i][2]
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
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glVertexPointer(3, GL_FLOAT, 0, kvp_rot[ib]);
      glNormalPointer(GL_FLOAT, 0, nmlp_rot[ib]);
      glColorPointer(4, GL_FLOAT, 0, clr[ib]);
      glDrawArrays(GL_TRIANGLES, 0, ntri[ib] * 3);
    }/*if (draw_band[ib] == 1)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  /*
   Nodeline
  */
  if (nodeline == 1) {
    /*
     First, rotate k-vector
    */
#pragma omp parallel default(none) \
    shared(nb,draw_band,nnl,rot,trans,kvnl,kvnl_rot) \
    private(ib)
    {
    int i, j, itri;

      for (ib = 0; ib < nb; ib++) {
        /**/
        if (draw_band[ib] == 1) {
#pragma omp for nowait
          for (itri = 0; itri < nnl[ib]; ++itri) {
            for (i = 0; i < 2; ++i) {
              /**/
              for (j = 0; j < 3; ++j)
                kvnl_rot[ib][j+3*i+6*itri]
                = rot[j][0] * kvnl[ib][itri][i][0]
                + rot[j][1] * kvnl[ib][itri][i][1]
                + rot[j][2] * kvnl[ib][itri][i][2]
                + trans[j];
              kvnl_rot[ib][2 + 3 * i + 6 * itri] += 0.001f;
            }/*for (i = 0; i < 2; ++i)*/
          }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
        }/*if (draw_band[ib] == 1)*/
      }/*for (ib = 0; ib < nb; ib++)*/
    }/*End of parallel region*/
    /*
     Second, draw each lines
    */
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] == 1) {
        glVertexPointer(3, GL_FLOAT, 0, kvnl_rot[ib]);
        glNormalPointer(GL_FLOAT, 0, nmlnl[ib]);
        glColorPointer(4, GL_FLOAT, 0, clrnl[ib]);
        glDrawArrays(GL_LINES, 0, 2 * nnl[ib]);
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
    shared(nb,draw_band,nequator,rot,trans,kveq,kveq_rot) \
    private(ib)
  {
    int i, j, itri;

    for (ib = 0; ib < nb; ib++) {
      /**/
      if (draw_band[ib] == 1) {
#pragma omp for nowait
        for (itri = 0; itri < nequator[ib]; ++itri) {
          for (i = 0; i < 2; ++i) {
            /**/
            for (j = 0; j < 3; ++j)
              kveq_rot[ib][j + 3 * i + 6 * itri]
              = rot[j][0] * kveq[ib][itri][i][0]
              + rot[j][1] * kveq[ib][itri][i][1]
              + rot[j][2] * kveq[ib][itri][i][2]
              + trans[j];
            kveq_rot[ib][2 + 3 * i + 6 * itri] += 0.001f;
          }/*for (i = 0; i < 2; ++i)*/
        }/*for (itri = 0; itri < nequator[ib]; ++itri)*/
      }/*if (draw_band[ib] == 1)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
   /*
   Second, draw each lines
   */
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glVertexPointer(3, GL_FLOAT, 0, kveq_rot[ib]);
      glNormalPointer(GL_FLOAT, 0, nmleq[ib]);
      glColorPointer(4, GL_FLOAT, 0, clreq[ib]);
      glDrawArrays(GL_LINES, 0, 2 * nequator[ib]);
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  }/*if (nodeline == 1)*/
}/*void draw_fermi*/
/**
 @brief Draw lines of BZ boundaries
*/
static void draw_bz_lines() {
  int ibzl, i, j;
  GLfloat bzl2[3], bvec2[3][3], linecolor[4], secvec2[3];
  GLfloat vertices[300], colors[400], normals[300];
  /*
   Line color is oposit of BG color
  */
  if (blackback == 1) 
    for (i = 0; i<4; i++) linecolor[i] = white[i];
  else
    for (i = 0; i<4; i++) linecolor[i] = black[i];
  /**/
  glLineWidth(linewidth*scl);
  for (i = 0; i < 18; ++i) {
    for (j = 0; j < 4; j++) colors[j + i * 4] = linecolor[j];
    for (j = 0; j < 2; j++) normals[j + i * 3] = 0.0f;
    normals[2 + i * 3] = 1.0f;
  }/*for (i = 0; i < 2; ++i)*/
  /*
   First Brillouin zone mode
  */
  if (fbz == 1) {
    for (ibzl = 0; ibzl < nbzl; ++ibzl) {
      for (i = 0; i< 2; ++i) {
        for (j = 0; j < 3; ++j) 
          bzl2[j] = rot[j][0] * bzl[ibzl][i][0]
                  + rot[j][1] * bzl[ibzl][i][1]
                  + rot[j][2] * bzl[ibzl][i][2]
                  + trans[j];
        for (j = 0; j < 3; j++) vertices[j + 3 * i] = bzl2[j];
      }/*for (i = 0; i< 2; ++i)*/
      glVertexPointer(3, GL_FLOAT, 0, vertices);
      glNormalPointer(GL_FLOAT, 0, normals);
      glColorPointer(4, GL_FLOAT, 0, colors);
      glDrawArrays(GL_LINES, 0, 2);
    }/*for (ibzl = 0; ibzl < nbzl; ++ibzl)*/
  }/*if (fbz == 1)*/
  else {
    /*
     Premitive BZ mode
    */
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        bvec2[i][j] = rot[j][0] * bvec[i][0]
                    + rot[j][1] * bvec[i][1]
                    + rot[j][2] * bvec[i][2];
      }/*for (j = 0; j < 3; ++j)*/
    }/*for (i = 0; i < 3; ++i)*/
    for (i = 0; i<3; ++i) vertices[i] = trans[i];
    for (i = 0; i<3; ++i) vertices[i+3] = trans[i] + bvec2[0][i];
    for (i = 0; i<3; ++i) vertices[i+3*2] = trans[i] + bvec2[0][i] + bvec2[1][i];
    for (i = 0; i<3; ++i) vertices[i+3*3] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*4] = trans[i] + bvec2[1][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*5] = trans[i] + bvec2[1][i];
    for (i = 0; i<3; ++i) vertices[i+3*6] = trans[i];
    for (i = 0; i<3; ++i) vertices[i+3*7] = trans[i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*8] = trans[i] + bvec2[0][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*9] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*10] = trans[i] + bvec2[0][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*11] = trans[i] + bvec2[0][i];
    for (i = 0; i<3; ++i) vertices[i+3*12] = trans[i];
    for (i = 0; i<3; ++i) vertices[i+3*13] = trans[i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*14] = trans[i] + bvec2[1][i] + bvec2[2][i];
    for (i = 0; i<3; ++i) vertices[i+3*15] = trans[i] + bvec2[1][i];
    for (i = 0; i<3; ++i) vertices[i+3*16] = trans[i] + bvec2[0][i] + bvec2[1][i];
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_LINE_STRIP, 0, 17);
  }/*if (fbz != 1)*/
  /*
   Section for the 2D Fermi line
  */
  if (lsection == 1) {
    for (j = 0; j < 3; ++j)
      secvec2[j] = rot[j][0] * secvec[0]
                 + rot[j][1] * secvec[1]
                 + rot[j][2] * secvec[2];
    for (i = 0; i < nbzl2d; ++i) {
      for (j = 0; j < 4; j++) colors[j + i * 4] = gray[j];
      for (j = 0; j < 3; j++) normals[j + i * 3] = secvec2[j];
    }
    for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
      for (j = 0; j < 3; ++j)
        bzl2[j] = rot[j][0] * bzl2d[ibzl][0]
                + rot[j][1] * bzl2d[ibzl][1]
                + rot[j][2] * bzl2d[ibzl][2]
                + trans[j];
      for (j = 0; j < 3; j++)vertices[j + 3 * ibzl] = bzl2[j];
    }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nbzl2d);
  }/*if (lsection == 1)*/
}/*draw bz_lines */
/**
 @brief Draw color-bar or colr-circle (periodic) 
        as a color scale
*/
static void draw_colorbar()
{
  int i, j, k;
  GLfloat mat2, vertices[366], normals[366], colors[488];
  /**/
  if (color_scale == 1 || color_scale == 4) {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 2; j++) {
        for (k = 0; k < 2; k++) normals[k + j*3 + i * 6] = 0.0f;
        normals[2 + j * 3 + i * 6] = 1.0f;
        vertices[0 + j * 3 + i * 6] = -1.0f + 0.5f*(GLfloat)i;
        vertices[1 + j * 3 + i * 6] = -1.0f - 0.1f*(GLfloat)j;
        vertices[2 + j * 3 + i * 6] = 0.0f;
        if (     i == 0) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = blue[k];
        else if (i == 1) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = cyan[k];
        else if (i == 2) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = green[k];
        else if (i == 3) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = yellow[k];
        else if (i == 4) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = red[k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
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
    normals[0] = 0.0f;
    normals[1] = 0.0f;
    normals[2] = 1.0f;
    if (blackback == 1)
      for (j = 0; j < 4; j++) colors[j] = wgray[j];
    else
      for (j = 0; j < 4; j++) colors[j] = bgray[j];
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
      for (j = 0; j < 2; j++) normals[j + 3 * (i + 1)] = 0.0f;
      normals[2 + 3 * (i + 1)] = 1.0f;
    }/*for (i = 0; i <= 60; i++)*/
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }/*else if (color_scale == 2)*/
  else if (color_scale == 3) {
    /*
    Periodic color scale
    */
    vertices[0] = 0.0f;
    vertices[1] = -1.0f;
    vertices[2] = 0.0f;
    normals[0] = 0.0f;
    normals[1] = 0.0f;
    normals[2] = 1.0f;
    for (j = 0; j < 4; j++) colors[j] = bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 1] = red[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 2] = yellow[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 3] = green[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 4] = cyan[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 5] = blue[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 6] = magenta[j] * 0.8f + bgray[j];
    for (j = 0; j < 4; j++) colors[j + 4 * 7] = red[j] * 0.8f + bgray[j];
    /**/
    for (i = 0; i <= 6; i++) {
      vertices[0 + 3 * (i + 1)] = 0.2f * cosf((GLfloat)i / 6.0f * 6.283185307f);
      vertices[1 + 3 * (i + 1)] = 0.2f * sinf((GLfloat)i / 6.0f * 6.283185307f) - 1.0f;
      vertices[2 + 3 * (i + 1)] = 0.0f;
      for (j = 0; j < 2; j++) normals[j + 3 * (i + 1)] = 0.0f;
      normals[2 + 3 * (i + 1)] = 1.0f;
    }/*for (i = 0; i <= 60; i++)*/
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
  }/*else if (color_scale == 3)*/
  else  if (color_scale == 6 || color_scale == 7) {
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 2; j++) {
        for (k = 0; k < 2; k++) normals[k + j * 3 + i * 6] = 0.0f;
        normals[2 + j * 3 + i * 6] = 1.0f;
        vertices[0 + j * 3 + i * 6] = -1.0f + 2.0f*(GLfloat)i;
        vertices[1 + j * 3 + i * 6] = -1.0f - 0.1f*(GLfloat)j;
        vertices[2 + j * 3 + i * 6] = 0.0f;
        if (i == 0) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = bgray[k];
        else if (i == 1) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = wgray[k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }/*if (color_scale == 6 || color_scale == 7)*/

}/*void draw_colorbar*/
/**
 @brief Draw eye-points for the stereogram
*/
static void draw_circles(
  GLfloat dx2d //!< [in] Translation used for the section-mode
) {
  int i, j;
  GLfloat r, vertices[66], colors[88], normals[66];
  /**/
  r = 0.05f;
  /**/
  for (i = 0; i < 22; i++) {
    for (j = 0; j < 2; j++)normals[j + i * 3] = 0.0f;
    normals[2 + i * 3] = 1.0f;
    vertices[2 + i * 3] = 0.0f;
    if (blackback == 1)for (j = 0; j < 4; j++) colors[j + i * 4] = white[j];
    else for (j = 0; j < 4; j++) colors[j + i * 4] = black[j];
  }/*for (i = 0; i < 22; i++)*/
  /**/
  vertices[0] = 0.7f - dx2d;
  vertices[1] = scl;
  for (i = 0; i <= 20; i++) {
    vertices[0 + (i + 1) * 3] = r * cosf((GLfloat)i / 20.0f * 6.283185307f) + 0.7f - dx2d;
    vertices[1 + (i + 1) * 3] = r * sinf((GLfloat)i / 20.0f * 6.283185307f) + scl;
  }/*for (i = 0; i <= 20; i++)*/
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glNormalPointer(GL_FLOAT, 0, normals);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
  /**/
  for (i = 0; i < 22; i++) vertices[3 * i] += -1.4f;
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glNormalPointer(GL_FLOAT, 0, normals);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}/*void draw_circles*/
/**
 @brief Draw 2D Fermi lines
*/
static void draw_fermi_line() {
  int i, ib, ibzl;
  GLfloat linecolor[4], vertices[60], normals[60], colors[80];
  /*
   Draw 2D BZ lines
  */
  if (blackback == 1)
    for (i = 0; i<4; i++) linecolor[i] = white[i];
  else
    for (i = 0; i<4; i++) linecolor[i] = black[i];
  /**/
  glLineWidth(linewidth*scl);
  for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
    for (i = 0; i < 3; i++) vertices[i + 3 * ibzl] = bzl2d_proj[ibzl][i];
    for (i = 0; i < 4; i++) colors[i + 4 * ibzl] = linecolor[i];
    for (i = 0; i < 2; i++) normals[i + 3 * ibzl] = 0.0f;
    normals[2 + 3 * ibzl] = 1.0f;
  }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glNormalPointer(GL_FLOAT, 0, normals);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawArrays(GL_LINE_LOOP, 0, nbzl2d);
  /*
   Draw Fermi lines
  */
  glLineWidth(linewidth*scl);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glVertexPointer(3, GL_FLOAT, 0, kv2d[ib]);
      glNormalPointer(GL_FLOAT, 0, nml2d[ib]);
      glColorPointer(4, GL_FLOAT, 0, clr2d[ib]);
      glDrawArrays(GL_LINES, 0, 2 * n2d[ib]);
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
}/*void draw_fermi_line*/
/**
 @brief Glut Display function
 called by glutDisplayFunc
*/
void display(void)
{
  GLfloat pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  GLfloat amb[] = { 0.2f, 0.2f, 0.2f, 0.0f };
  GLfloat dx, dx2d, theta, posz, phi;
  GLfloat pos1[4], pos2[4];
  int ierr;
  char command_name[256];

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
  if (lsection == 1) dx2d = 0.7f;
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
  if (lsection == 1) {
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
  glutSwapBuffers();
  if (lbatch == 1) {
    glFlush();
    sprintf(command_name, "import -window \"%s\" %s.png", window_name, batch_name);
    ierr = system(command_name);
    exit(0);
  }
}/*void display*/
