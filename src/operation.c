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
#if defined(HAVE_GL_GLUT_H)
#include <GL/glut.h>
#elif defined(HAVE_GLUT_GLUT_H)
#include <GLUT/glut.h>
#endif

#include <math.h>
#include "variable.h"
/**
 @brief Window resize

 Modify : ::sx, ::sy
*/
void resize(
  int w, //!<[in] Window width
  int h //!<[in] Window height
)
{
  /*
   Scale of translation of mousepointer
  */
  sx = 1.0f / (GLfloat)w;
  sy = 1.0f / (GLfloat)h;
  /**/
  glViewport(0, 0, w, h);
  /**/
  glMatrixMode(GL_PROJECTION);
  /**/
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
  /**/
  glMatrixMode(GL_MODELVIEW);
} /* end resize */
/**
 @brief Idling
*/
void idle(void)
{
  glutPostRedisplay();
} /* idle */
/**
 @brief Glut mouse function

 Modify : ::cx, ::cy, ::scl
*/
void mouse(
  int button, //!< [in] pushed button
  int state, //!< [in] down or up or ?
  int x, //!< [in] position of mouse cursor
  int y //!< [in] position of mouse cursor
)
{
  switch (button) {
  /*
   Drag
  */
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* Start of animation */
      glutIdleFunc(idle);
      /* Record drag start point */
      cx = x;
      cy = y;
      break;
    case GLUT_UP:
      /* End of animation */
      glutIdleFunc(0);
      break;
    default:
      break;
    }
    break;
    /*
     Zoom up
    */
  case MOUSE_SCROLL_UP:
    switch (state) {
    case GLUT_DOWN:
      scl = scl * 1.1f;
      glutPostRedisplay();
      break;
    case GLUT_UP:
      break;
    default:
      break;
    }
    break;
    /*
     Zoom down
    */
  case MOUSE_SCROLL_DOWN:
    switch (state) {
    case GLUT_DOWN:
      scl = scl * 0.9f;
      glutPostRedisplay();
      break;
    case GLUT_UP:
      break;
    default:
      break;
    }
    break;
    /*
     No pushing
    */
  default:
    break;
  }
} /* end mouse */
/**
 @brief Glut motion function

 Modify : ::scl, ::trans
*/
void motion(
  int x, //!< [in] position of cursor
  int y //!< [in] position of cursor
)
{
  int i, j;
  GLfloat dx, dy, a, rot0[3][3], rot1[3][3], ax, ay;
  /*
   Translation of mousepointer from starting point
  */
  dx = (x - cx) * sx;
  dy = (y - cy) * sy;
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
      rot0[1][2] =  ax * sinf(a) / sqrtf(ax * ax + ay * ay);
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
  cx = x;
  cy = y;
} /* motion */
/**
 @brief Glut keyboard function
*/
void keyboard(
  unsigned char key, //!< [in] Typed key
  int x, //!< [in]
  int y //!< [in]
)
{
  switch (key) {
  }
} /* keyboard */
/**
 @brief Glut special key function

 Modify : ::trans
*/
void special_key(
  int key, //!< [in] typed special key
  int x, //!< [in]
  int y //!< [in]
)
{
  switch (key) {
  case GLUT_KEY_UP:
    trans[1] = trans[1] + 0.1f;
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    trans[1] = trans[1] - 0.1f;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    /**/
    trans[0] = trans[0] + 0.1f;
    glutPostRedisplay();
    break;
    /**/
  case GLUT_KEY_LEFT:
    /**/
    trans[0] = trans[0] - 0.1f;
    glutPostRedisplay();
    break;
    /**/
  }
} /* special_key */
