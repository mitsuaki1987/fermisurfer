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
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/log.h>
#include "android_native_app_glue.h"

#include <malloc.h>
#include <stdio.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

/**
* �ۑ���Ԃ̃f�[�^�ł��B
*/
struct saved_state {
  int32_t x;
  int32_t y;
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
  glClearColor(1.0f, 0.41f, 0.71f, 1.0f);
  //glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  //glShadeModel(GL_SMOOTH);

  //glClearColor(0.0, 0.0, 0.0, 0.0);
  //glEnable(GL_DEPTH_TEST);
  //glDisable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glViewport(0, 0, engine->width, engine->height);
  GLfloat ratio = (GLfloat)engine->width/ (GLfloat)engine->height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustumf(-ratio, ratio, -1, 1, 1, 10);
  glEnableClientState(GL_VERTEX_ARRAY);

 // glMatrixMode(GL_MODELVIEW);
  return 0;
}
/**
* �f�B�X�v���C���̌��݂̃t���[���̂݁B
*/
static void engine_draw_frame(struct engine* engine) {
  int ntri = 8;
  GLfloat kvp[][3] = { 
    { 1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0,  1.0},
    { 1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0, -1.0},
    { 1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0,  1.0},
    { 1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0},
    {-1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0,  1.0},
    {-1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0, -1.0},
    {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0,  1.0},
    {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}
  };
  GLfloat nmlp[][3] = {
    { 1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0,  1.0},
    { 1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0, -1.0},
    { 1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0,  1.0},
    { 1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0},
    {-1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0,  1.0},
    {-1.0, 0.0, 0.0}, {0.0,  1.0, 0.0}, {0.0, 0.0, -1.0},
    {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0,  1.0},
    {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}
  };
  GLfloat clr[][4] = { 
    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 0.0, 1.0}
  };
  GLfloat pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  GLfloat amb[] = { 0.2f, 0.2f, 0.2f, 0.0f };

  if (engine->display == NULL) {
    // �f�B�X�v���C������܂���B
    return;
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  //glScalef(1.0, 1.0, 1.0);

  // �F�ŉ�ʂ�h��Ԃ��܂��B
  //glClearColor(((float)engine->state.x) / engine->width, 0.5, ((float)engine->state.y) / engine->height, 1);
  glClearColor(0.7, 0.7, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glLineWidth(5.0);
  //glColor4f(0.0, 1.0, 0.0, 1.0);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  glVertexPointer(3, GL_FLOAT, 0, kvp);
  glNormalPointer(GL_FLOAT, 0, nmlp);
  glColorPointer(4, GL_FLOAT, 0, clr);
  glDrawArrays(GL_TRIANGLES, 0, ntri * 3);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

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
    engine->state.x = AMotionEvent_getX(event, 0);
    engine->state.y = AMotionEvent_getY(event, 0);
    engine_draw_frame(engine);
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
      engine_draw_frame(engine);
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
    engine_draw_frame(engine);
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

  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  if (state->savedState != NULL) {
    // �ȑO�̕ۑ���ԂŊJ�n���܂��B�������Ă��������B
    engine.state = *(struct saved_state*)state->savedState;
  }

  // ���[�v�̓X�^�b�t�ɂ��J�n��҂��Ă��܂��B

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
