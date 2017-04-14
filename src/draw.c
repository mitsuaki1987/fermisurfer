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

#include <math.h>
#include "variable.h"

#if defined(MAC)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
* Draw Fermi surfaces
*/
void draw_fermi() {
  int i, j, ib, itri;
  GLfloat vect[3];
  /**
   * First, rotate k-vector and normal vector
   */
#pragma omp parallel default(none) \
  shared(nb,draw_band,ntri,rot,nmlp,nmlp_rot,kvp,kvp_rot,trans) \
  private(ib,itri,i,j)
  {
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] == 1) {
#pragma omp for nowait
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (j = 0; j < 3; ++j)
            nmlp_rot[ib][itri][j] = rot[j][0] * nmlp[ib][itri][0]
                                  + rot[j][1] * nmlp[ib][itri][1]
                                  + rot[j][2] * nmlp[ib][itri][2];
          /**/
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j)
              kvp_rot[ib][itri][i][j] = rot[j][0] * kvp[ib][itri][i][0]
                                      + rot[j][1] * kvp[ib][itri][i][1]
                                      + rot[j][2] * kvp[ib][itri][i][2]
                                      + trans[j];
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*if (draw_band[ib] == 1)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
  /**
   * Second, draw each triangle
   */
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glBegin(GL_TRIANGLES);
      for (itri = 0; itri < ntri[ib]; ++itri) {
        glNormal3fv(nmlp_rot[ib][itri]);
        for (i = 0; i < 3; ++i) {
          glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clr[ib][itri][i]);
          glVertex3fv(kvp_rot[ib][itri][i]);
        }/**/
      }/**/
      glEnd();
    }/**/
  }/**/
  /**
   * Nodeline
   */
  if (nodeline == 1) {
    /**
     * First, rotate k-vector
     */
#pragma omp parallel default(none) \
    shared(nb,draw_band,nnl,rot,trans,kvnl,kvnl_rot) \
    private(ib,itri,i,j)
    {
      for (ib = 0; ib < nb; ib++) {
        /**/
        if (draw_band[ib] == 1) {
#pragma omp for nowait
          for (itri = 0; itri < nnl[ib]; ++itri) {
            for (i = 0; i < 2; ++i) {
              /**/
              for (j = 0; j < 3; ++j)
                kvnl_rot[ib][itri][i][j] = rot[j][0] * kvnl[ib][itri][i][0]
                                         + rot[j][1] * kvnl[ib][itri][i][1]
                                         + rot[j][2] * kvnl[ib][itri][i][2]
                                         + trans[j] + 0.001;
            }/*for (i = 0; i < 2; ++i)*/
          }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
        }/*if (draw_band[ib] == 1)*/
      }/*for (ib = 0; ib < nb; ib++)*/
    }/*End of parallel region*/
    /**
     * Second, draw each triangle
     */
  glLineWidth(10.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    glBegin(GL_LINES);
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] == 1) {
        for (itri = 0; itri < nnl[ib]; ++itri) {
          for (i = 0; i < 2; ++i) glVertex3fv(kvnl_rot[ib][itri][i]);
        }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
      }/*if (draw_band[ib] == 1)*/
    }/* for (ib = 0; ib < nb; ib++)*/
    glEnd();
  }/*if (nodeline == 1)*/
}/*void draw_fermi*/
/**
 * Draw lines of BZ boundaries
 */
void draw_bz_lines() {
  int ibzl, i, j;
  GLfloat bzl2[3], bvec2[3][3], linecolor[4];
  /**
   * Line color is oposit of BG color
   */
  if (blackback == 1) 
    for (i = 0; i<4; i++) linecolor[i] = white[i];
  else
    for (i = 0; i<4; i++) linecolor[i] = black[i];
  /**/
  glLineWidth(2.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, linecolor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, linecolor);
  /**
   * First Brillouin zone mode
   */
  if (fbz == 1) {
    glBegin(GL_LINES);
    for (ibzl = 0; ibzl < nbzl; ++ibzl) {
      for (i = 0; i< 2; ++i) {
        for (j = 0; j < 3; ++j) 
          bzl2[j] = rot[j][0] * bzl[ibzl][i][0]
                  + rot[j][1] * bzl[ibzl][i][1]
                  + rot[j][2] * bzl[ibzl][i][2]
                  + trans[j];
        glVertex3fv(bzl2);
      }
    }
    glEnd();
  }
  else {
    /**
     * Premitive BZ mode
     */
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        bvec2[i][j] = rot[j][0] * bvec[i][0]
                    + rot[j][1] * bvec[i][1]
                    + rot[j][2] * bvec[i][2];
      }
    }
    glBegin(GL_LINE_STRIP);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[1][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[1][i] + bvec2[2][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[1][i]; glVertex3fv(bzl2);
    for (i = 0; i<3; ++i) bzl2[i] = trans[i] + bvec2[0][i] + bvec2[1][i]; glVertex3fv(bzl2);
    glEnd();
  }
  /**/
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
  /**/
} /* draw bz_lines */
/**
 * Draw color scale
 */
void draw_colorbar()
{
  int i, j;
  GLfloat mat2, barcolor[4];
  GLfloat colorbar[13][3] = {
    { 0.0, 0.0, 1.0 },
    { -1.0, -1.0,       0.0 },
    { -1.0, -1.0 - 0.1, 0.0 },
    { -0.5, -1.0,       0.0 },
    { -0.5, -1.0 - 0.1, 0.0 },
    { 0.0, -1.0,       0.0 },
    { 0.0, -1.0 - 0.1, 0.0 },
    { 0.5, -1.0,       0.0 },
    { 0.5, -1.0 - 0.1, 0.0 },
    { 1.0, -1.0,       0.0 },
    { 1.0, -1.0 - 0.1, 0.0 },
    { 0.0, -1.0,       0.00001 },
    { 0.0, -1.0 - 0.1, 0.00001 }
  };
  /**/
  if (fcscl == 1 || fcscl == 2) {
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3fv(colorbar[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    glVertex3fv(colorbar[1]);
    glVertex3fv(colorbar[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
    glVertex3fv(colorbar[3]);
    glVertex3fv(colorbar[4]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glVertex3fv(colorbar[5]);
    glVertex3fv(colorbar[6]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
    glVertex3fv(colorbar[7]);
    glVertex3fv(colorbar[8]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    glVertex3fv(colorbar[9]);
    glVertex3fv(colorbar[10]);
    glEnd();
    /**/
    /* if(nodeline == 1){ */
    /*   glLineWidth(2.0); */
    /*   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black); */
    /*   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black); */
    /*   glBegin(GL_LINES); */
    /*   glVertex3fv(colorbar[11]); */
    /*   glVertex3fv(colorbar[12]); */
    /*   glEnd(); */
    /* } */
  }
  else if (fcscl == 4) {
    /**
     * Periodic color scale
     */
    for (i = 0; i <= 60; i++) {
      /**/
      mat2 = (GLfloat)i / 60.0 * 6.0;
      /**/
      if (mat2 <= 1.0) {
        for (j = 0; j<4; ++j) barcolor[j] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0;
        for (j = 0; j<4; ++j) barcolor[j] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0;
        for (j = 0; j<4; ++j) barcolor[j] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
      }
      else if (mat2 <= 4.0) {
        mat2 = mat2 - 3.0;
        for (j = 0; j<4; ++j) barcolor[j] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
      }
      else if (mat2 <= 5.0) {
        mat2 = mat2 - 4.0;
        for (j = 0; j<4; ++j) barcolor[j] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
      }
      else {
        mat2 = mat2 - 5.0;
        for (j = 0; j<4; ++j) barcolor[j] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
      }
      /**/
      glBegin(GL_TRIANGLES);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, barcolor);
      glNormal3fv(colorbar[0]);
      glVertex3f(0.15 * cos((GLfloat)(i + 1) / 60.0 * 6.283185307),
                 0.15 * sin((GLfloat)(i + 1) / 60.0 * 6.283185307) - 1.0, 0.0);
      glVertex3f(0.15 * cos((GLfloat)i / 60.0 * 6.283185307),
                 0.15 * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
      glVertex3f(0.2  * cos((GLfloat)i / 60.0 * 6.283185307),
                 0.2  * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
      glVertex3f(0.2  * cos((GLfloat)i / 60.0 * 6.283185307),
                 0.2  * sin((GLfloat)i / 60.0 * 6.283185307) - 1.0, 0.0);
      glVertex3f(0.2  * cos((GLfloat)(i + 1) / 60.0 * 6.283185307),
                 0.2  * sin((GLfloat)(i + 1) / 60.0 * 6.283185307) - 1.0, 0.0);
      glVertex3f(0.15 * cos((GLfloat)(i + 1) / 60.0 * 6.283185307),
                 0.15 * sin((GLfloat)(i + 1) / 60.0 * 6.283185307) - 1.0, 0.0);
      glEnd();
    }
  }
}/*void draw_colorbar*/
/**
 * Draw points for the stereogram
 */
void draw_circles() {
  int i;
  GLfloat r;
  /**/
  r = 0.05;
  /**/
  if (blackback == 1) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
  }
  else {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
  }
  /**/
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(0.7, scl, 0.0);
  for (i = 0; i <= 20; i++) {
    glVertex3f(r * cos((GLfloat)i / 20.0 * 6.283185307) + 0.7,
               r * sin((GLfloat)i / 20.0 * 6.283185307) + scl, 0.0);
  }
  glEnd();
  /**/
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(-0.7, scl, 0.0);
  for (i = 0; i <= 20; i++) {
    glVertex3f(r * cos((GLfloat)i / 20.0 * 6.283185307) - 0.7,
               r * sin((GLfloat)i / 20.0 * 6.283185307) + scl, 0.0);
  }
  glEnd();
}/*void draw_circles*/
/**
 * Glut Display function
 */
void display()
{
  GLfloat pos[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat amb[] = { 0.2, 0.2, 0.2, 0.0 };
  double dx, theta, posz, phi;
  GLfloat pos1[4], pos2[4];
  /**/
  if (lstereo == 2) {
    theta = 3.1416 / 180.0 * 2.50;
    posz = 5.0;
    dx = 0.7;
    phi = atan(posz / dx) - theta;
    theta = (3.1416 * 0.50 - phi) / 3.1416 * 180.0;
    /**/
    pos1[0] = posz * cos(phi) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * sin(phi);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * cos(phi) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * sin(phi);
    pos2[3] = 1.0;
  }
  else if (lstereo == 3) {
    theta = -3.1416 / 180.0 * 2.0;
    posz = 5.0;
    dx = -0.7;
    /**/
    pos1[0] = posz * sin(theta) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * cos(theta);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * sin(theta) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * cos(theta);
    pos2[3] = 1.0;
  }
  else {
    theta = 0.0;
    dx = 0.0;
  }
  /**
   * Initialize
   */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /**
   * Set view point & view line
   */
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /**
   * Set position of light
   */
  if (lstereo == 1) {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    /**
     * Draw color scale
     */
    if (lcolorbar == 1) draw_colorbar();
  }
  else {
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    draw_circles();
    glTranslated(-dx, 0.0, 0.0);
    glRotated(theta, 0.0, 1.0, 0.0);
  }
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  /**
   * Rotation & Zoom
   */
  glScaled(scl, scl, scl);
  /**
   * Draw Brillouin zone boundaries
   */
  draw_bz_lines();
  /**
   * Draw Fermi surfaces
   */
  draw_fermi();
  /**
   * Draw the second object for stereogram
   */
  if (lstereo != 1) {
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos2);
    /**/
    glTranslated(dx, 0.0, 0.0);
    glRotated(-theta, 0.0, 1.0, 0.0);
    /**/
    glScaled(scl, scl, scl);
    draw_bz_lines();
    draw_fermi();
    /**/
    glPopMatrix();
  }
  /**/
  glutSwapBuffers();
}/*void display*/
