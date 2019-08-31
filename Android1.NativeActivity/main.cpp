/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (�u�{���C�Z���X�v) �Ɋ�Â��ă��C�Z���X����܂��B;
* �{���C�Z���X�ɏ������Ȃ��ꍇ�͂��̃t�@�C�����g�p�ł��܂���B
* �{���C�Z���X�̃R�s�[�́A�ȉ��̏ꏊ�������ł��܂��B
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* �K�p�����@�߂܂��͏��ʂł̓��ӂɂ���Ė������Ȃ�����A�{���C�Z���X�Ɋ�Â��ĔЕz�����\�t�g�E�F�A�́A
* �����َ����킸�A�����Ȃ�ۏ؂��������Ȃ��Ɍ���̂܂�
* �Еz����܂��B
* �{���C�Z���X�ł̌����Ɛ������K�肵���������ẮA
* �{���C�Z���X���Q�Ƃ��Ă��������B
*
*/
#include "read_file.hpp"
#include "kumo.hpp"
#include "initialize.hpp"
#include "bz_lines.hpp"
#include "section.hpp"
#include "menu.hpp"
#include "draw.hpp"
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/log.h>
#include "android_native_app_glue.h"

#include <malloc.h>
#include <stdio.h>
#include <math.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))
/*
 Input variables
*/
int ng0[3];         //!< @f$k@f$-point grid in the input file
int shiftk[3];      //!< Wherether @f$k@f$-grid is shifted or not
int nb;             //!< The number of Bands
GLfloat avec[3][3]; //!< Direct lattice vector
GLfloat bvec[3][3]; //!< Reciprocal lattice vector
GLfloat**** eig0;   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
GLfloat***** mat0;   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]][3]
/*
 Interpolation
*/
int ng[3];        //!< @b Interpolated @f$k@f$-grids
GLfloat**** eig;  //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
GLfloat***** mat; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
GLfloat***** vf;  //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
int interpol = 1;     //!< Ratio of interpolation
/*
 Switch for some modes
*/
int blackback = 1;   //!< Switch for black background
int color_scale = 1; //!< Switch for full color scale mode
int fbz = 1;         //!< Switch for 1st Brillouin zone mode
int nodeline = 0;    //!< Switch for node lines
int lcolorbar = 1;   //!< Switch for colorbar
int lstereo = 1;     //!< Switch for the stereogram
int lmouse = 1;      //!< Switch for the mouse function
int lsection = 0;    //!< Switch for the 2D Fermi lines
int lequator = 0;    //!< Switch for equator
/*
 Variables for Brillouin zone boundaries
*/
int nbzl;               //!< The number of Lines of 1st Brillouin zone
GLfloat bzl[676][2][3]; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
GLfloat bragg[26][3];   //!< Bragg plane vectors
GLfloat brnrm[26];      //!< Norms of Bragg plane vectors
GLfloat brnrm_min;     //!< Minimum scale of the reciplocal space
int nbragg;             //!< Number of Bragg plane og 1st BZ
/*
 Variables for patchs
*/
int* ntri;          //!< The number of triangle patch [::nb]
int** ntri_th;      //!< The number of triangle patch in each thread [::nb]
int* draw_band;     //!< Switch for drawn bands [::nb]
GLfloat**** nmlp;    //!< Normal vector of patchs [::nb][::ntri][3][3]
GLfloat**** kvp;    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
GLfloat***** arw;
GLfloat** nmlp_rot; //!< Normal vector of patchs [::nb][::ntri*3*3]
GLfloat** kvp_rot;  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
GLfloat** arw_rot;
GLfloat**** matp;    //!< Matrix elements of points [::nb][::ntri][3][3]
GLfloat** clr;      //!< Colors of points [::nb][::ntri*3*4]
int itet = 0;           //!< Counter for tetrahedron
GLfloat side = 1.0;       //!< Which side is lighted
GLfloat patch_max;  //!< Max value across patch
GLfloat patch_min;  //!< Max value across patch
/*
  Variables for nodeline
*/
int* nnl;             //!< The number of nodeline
GLfloat**** kvnl;     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
GLfloat** kvnl_rot; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
/*
 2D Fermi line
*/
GLfloat secvec[3];         //!< @f$k@f$-vector to define section
GLfloat secvec_fr[3];         //!< @f$k@f$-vector to define section
GLfloat secscale;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
GLfloat axis2d[2][3];      //!< @f$k@f$-vector to define section
int* n2d;                  //!< Number of line segment
GLfloat** kv2d;          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
GLfloat** clr2d;         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
int nbzl2d;                //!< The number of Lines of 1st Brillouin zone
GLfloat bzl2d[26][3];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
GLfloat bzl2d_proj[26][3]; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
GLfloat eqvec[3]; //!<  @f$k@f$-vector for equator
GLfloat eqvec_fr[3]; //!<  @f$k@f$-vector for equator
int* nequator;             //!< The number of equator
GLfloat**** kveq;     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
GLfloat** kveq_rot; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
/*
  Variables for mouse  & cursorkey
*/
GLfloat sx;        //!< Scale of mouse movement
GLfloat sy;        //!< Scale of mouse movement
int cx;            //!< Starting point of drug
int cy;            //!< Starting point of drug
GLfloat scl = 1.0;       //!< Initial scale
GLfloat trans[3] = { 0.0, 0.0, 0.0 };  //!< Translation
GLfloat rot[3][3] = { { 1.0, 0.0, 0.0 },
                      { 0.0, 1.0, 0.0 },
                      { 0.0, 0.0, 1.0 } }; //!< Rotation matrix
GLfloat thetax = 0.0;    //!< Rotation angle
GLfloat thetay = 0.0;    //!< Rotation angle
GLfloat thetaz = 0.0;    //!< Rotation angle
GLfloat linewidth = 3.0; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
GLfloat   black[4] = { 0.0, 0.0, 0.0, 1.0 }; //!< Black color code
GLfloat    gray[4] = { 0.5f, 0.5f, 0.5f, 1.0 }; //!< Gray color code
GLfloat   wgray[4] = { 0.9f, 0.9f, 0.9f, 1.0 }; //!< Gray color code
GLfloat   bgray[4] = { 0.1f, 0.1f, 0.1f, 1.0 }; //!< Gray color code
GLfloat   white[4] = { 1.0, 1.0, 1.0, 1.0 }; //!< White color code
GLfloat    cyan[4] = { 0.0, 1.0, 1.0, 1.0 }; //!< Cyan color code
GLfloat magenta[4] = { 1.0, 0.0, 1.0, 1.0 }; //!< Magenta color code
GLfloat  yellow[4] = { 1.0, 1.0, 0.0, 1.0 }; //!< Yellow color code
GLfloat     red[4] = { 1.0, 0.0, 0.0, 1.0 }; //!< Red color code
GLfloat   green[4] = { 0.0, 1.0, 0.0, 1.0 }; //!< Green color code
GLfloat    blue[4] = { 0.0, 0.0, 1.0, 1.0 }; //!< Blue color code
/*
 Others
*/
int query;        //!< Query switch
int corner[6][4]; //!< Corners of tetrahedron
GLfloat EF = 0.0;       //!< Fermi energy
enum
{
  MOUSE_SCROLL_UP = 3,  //!< Mouse wheel up
  MOUSE_SCROLL_DOWN = 4 //!< Mouse wheel down
};
int nthreads;//!< Number of OpenMP threads
int refresh_interpol = 0;
int refresh_patch = 1;
int refresh_color = 1;
int refresh_nodeline = 1;
int refresh_equator = 1;
int refresh_section = 1;
int skip_minmax = 0;
int windowx = 1100;
int windowy = 850;
/**
* �ۑ���Ԃ̃f�[�^�ł��B
*/
struct saved_state {
  int32_t x;
  int32_t y;
  int32_t x0;
  int32_t y0;
  int32_t zoom;
  int32_t zoom0;
};
/**
* �A�v���̕ۑ���Ԃł��B
*/
struct engine {
  struct android_app* app;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  EGLint width;
  EGLint height;
  struct saved_state state;
};
/**
* ���݂̕\���� EGL �R���e�L�X�g�����������܂��B
*/
static int engine_init_display(struct engine* engine) {
  // OpenGL ES �� EGL �̏�����

  /*
  * �ړI�̍\���̑����������Ŏw�肵�܂��B
  * �ȉ��ŁA�I���X�N���[�� �E�B���h�E��
  * �݊����̂���A�e�F�Œ� 8 �r�b�g�̃R���|�[�l���g�� EGLConfig ��I�����܂�
  */
  const EGLint attribs[] = {
          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
          EGL_BLUE_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_RED_SIZE, 8,
          EGL_DEPTH_SIZE, 16,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
          EGL_NONE
  };
  EGLint format;
  EGLint numConfigs;
  EGLConfig config;

  engine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  eglInitialize(engine->display, 0, 0);

  /* �����ŁA�A�v���P�[�V�����͖ړI�̍\����I�����܂��B���̃T���v���ł́A
  * ���o�����ƈ�v����ŏ��� EGLConfig ��
  * �I������P���ȑI���v���Z�X������܂� */
  eglChooseConfig(engine->display, attribs, &config, 1, &numConfigs);

  /* EGL_NATIVE_VISUAL_ID �́AANativeWindow_setBuffersGeometry() ��
  * ����Ď󂯎���邱�Ƃ��ۏ؂���Ă��� EGLConfig �̑����ł��B
  * EGLConfig ��I�������炷���ɁAANativeWindow �o�b�t�@�[����v�����邽�߂�
  * EGL_NATIVE_VISUAL_ID ���g�p���Ĉ��S�ɍč\���ł��܂��B*/
  eglGetConfigAttrib(engine->display, config, EGL_NATIVE_VISUAL_ID, &format);

  ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

  engine->surface = eglCreateWindowSurface(engine->display, config, engine->app->window, NULL);
  engine->context = eglCreateContext(engine->display, config, NULL, NULL);

  if (eglMakeCurrent(engine->display, engine->surface, engine->surface, engine->context) == EGL_FALSE) {
    LOGW("Unable to eglMakeCurrent");
    return -1;
  }

  eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &engine->width);
  eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &engine->height);

  FILE* fp = fopen("/sdcard/Download/test.txt", "w");
  fprintf(fp, "test %d %d\n", engine->width, engine->height);
  fclose(fp);

  // GL �̏�Ԃ����������܂��B
  glDisable(GL_DITHER);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnableClientState(GL_VERTEX_ARRAY);
  glViewport(0, 0, engine->width, engine->height);
  GLfloat ratio = (GLfloat)engine->width/ (GLfloat)engine->height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustumf(-ratio, ratio, -1, 1, 1, 10);

  glMatrixMode(GL_MODELVIEW);
  return 0;
}
/**
* �f�B�X�v���C���̌��݂̃t���[���̂݁B
*/
static void engine_draw_frame(struct engine* engine, int flag) {
  int i, j;
  GLfloat dx, dy, a, rot0[3][3], rot1[3][3], ax, ay, sx = 1.0/engine->width, sy = 1.0/engine->height;
  if (flag == 0) {
    scl *= expf((GLfloat)(engine->state.zoom0 - engine->state.zoom) * sy);
  }
  else if (flag == 1) {
    /*
     Translation of mousepointer from starting point
    */
    dx = (engine->state.x - engine->state.x0) * sx;
    dy = (engine->state.y - engine->state.y0) * sy;
    /*
     Distanse from starting point
    */
    a = sqrtf(dx * dx + dy * dy);
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
  if (engine->display == NULL) {
    // �f�B�X�v���C������܂���B
    return;
  }
  draw();
  eglSwapBuffers(engine->display, engine->surface);
}
/**
* ���݃f�B�X�v���C�Ɋ֘A�t�����Ă��� EGL �R���e�L�X�g���폜���܂��B
*/
static void engine_term_display(struct engine* engine) {
  if (engine->display != EGL_NO_DISPLAY) {
    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (engine->context != EGL_NO_CONTEXT) {
      eglDestroyContext(engine->display, engine->context);
    }
    if (engine->surface != EGL_NO_SURFACE) {
      eglDestroySurface(engine->display, engine->surface);
    }
    eglTerminate(engine->display);
  }
  engine->display = EGL_NO_DISPLAY;
  engine->context = EGL_NO_CONTEXT;
  engine->surface = EGL_NO_SURFACE;
}
/**
* ���̓��̓C�x���g���������܂��B
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
  struct engine* engine = (struct engine*)app->userData;
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    if (AMotionEvent_getX(event, 0) > 0.95* engine->width ||
        AMotionEvent_getX(event, 0) < 0.05 * engine->width) {
      if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN) {
        engine->state.zoom = AMotionEvent_getY(event, 0);
      }
      else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE) {
        engine->state.zoom0 = engine->state.zoom;
        engine->state.zoom = AMotionEvent_getY(event, 0);
        engine_draw_frame(engine, 0);
      }
    }
    else {
      if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN) {
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
      }
      else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE) {
        engine->state.x0 = engine->state.x;
        engine->state.y0 = engine->state.y;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        engine_draw_frame(engine, 1);
      }
    }
    return 1;
  }
  return 0;
}
/**
* ���̃��C�� �R�}���h���������܂��B
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    // ���݂̏�Ԃ�ۑ�����悤�V�X�e���ɂ���ėv������܂����B�ۑ����Ă��������B
    engine->app->savedState = malloc(sizeof(struct saved_state));
    *((struct saved_state*)engine->app->savedState) = engine->state;
    engine->app->savedStateSize = sizeof(struct saved_state);
    break;
  case APP_CMD_INIT_WINDOW:
    // �E�B���h�E���\������Ă��܂��B�������Ă��������B
    if (engine->app->window != NULL) {
      engine_init_display(engine);
      engine->state.zoom = 0;
      engine->state.zoom0 = 0;
      engine_draw_frame(engine, 0);
    }
    break;
  case APP_CMD_TERM_WINDOW:
    // �E�B���h�E����\���܂��͕��Ă��܂��B�N���[�� �A�b�v���Ă��������B
    engine_term_display(engine);
    break;
  case APP_CMD_GAINED_FOCUS:
    break;
  case APP_CMD_LOST_FOCUS:
    // �܂��A�A�j���[�V�������~���܂��B
    engine_draw_frame(engine, 0);
    break;
  }
}
/**
* ����́Aandroid_native_app_glue ���g�p���Ă���l�C�e�B�u �A�v���P�[�V����
* �̃��C�� �G���g�� �|�C���g�ł��B���ꎩ�̂̃X���b�h�ŃC�x���g ���[�v�ɂ���Ď��s����A
* ���̓C�x���g���󂯎�����葼�̑�������s�����肵�܂��B
*/
void android_main(struct android_app* state) {
  struct engine engine;

  nthreads = 1;
  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  if (state->savedState != NULL) {
    // �ȑO�̕ۑ���ԂŊJ�n���܂��B�������Ă��������B
    engine.state = *(struct saved_state*)state->savedState;
  }

  color_scale = read_file();
  lcolorbar = 0;
  interpol_energy();
  init_corner();
  bragg_vector();
  /*
     Brillouin zone
    */
  bz_lines();
  calc_2dbz();
  /**/
  max_and_min_bz();
  /**/
  compute_patch_segment();
  while (1) {
    // �ۗ����̂��ׂẴC�x���g��ǂݎ��܂��B
    int ident;
    int events;
    struct android_poll_source* source;

    // �������Ƀu���b�N���ăC�x���g����������̂�҂��܂��B
    while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)& source)) >= 0) {

      // ���̃C�x���g���������܂��B
      if (source != NULL) {
        source->process(state, source);
      }

      // �I�����邩�ǂ����m�F���܂��B
      if (state->destroyRequested != 0) {
        engine_term_display(&engine);
        return;
      }
    }
  }
}
