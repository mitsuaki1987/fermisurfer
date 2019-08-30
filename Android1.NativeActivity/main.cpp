/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (「本ライセンス」) に基づいてライセンスされます。;
* 本ライセンスに準拠しない場合はこのファイルを使用できません。
* 本ライセンスのコピーは、以下の場所から入手できます。
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* 適用される法令または書面での同意によって命じられない限り、本ライセンスに基づいて頒布されるソフトウェアは、
* 明示黙示を問わず、いかなる保証も条件もなしに現状のまま
* 頒布されます。
* 本ライセンスでの権利と制限を規定した文言ついては、
* 本ライセンスを参照してください。
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
* 保存状態のデータです。
*/
struct saved_state {
  int32_t x;
  int32_t y;
};
/**
* アプリの保存状態です。
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
* 現在の表示の EGL コンテキストを初期化します。
*/
static int engine_init_display(struct engine* engine) {
  // OpenGL ES と EGL の初期化

  /*
  * 目的の構成の属性をここで指定します。
  * 以下で、オンスクリーン ウィンドウと
  * 互換性のある、各色最低 8 ビットのコンポーネントの EGLConfig を選択します
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

  /* ここで、アプリケーションは目的の構成を選択します。このサンプルでは、
  * 抽出条件と一致する最初の EGLConfig を
  * 選択する単純な選択プロセスがあります */
  eglChooseConfig(engine->display, attribs, &config, 1, &numConfigs);

  /* EGL_NATIVE_VISUAL_ID は、ANativeWindow_setBuffersGeometry() に
  * よって受け取られることが保証されている EGLConfig の属性です。
  * EGLConfig を選択したらすぐに、ANativeWindow バッファーを一致させるために
  * EGL_NATIVE_VISUAL_ID を使用して安全に再構成できます。*/
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

  // GL の状態を初期化します。
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
* ディスプレイ内の現在のフレームのみ。
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
    // ディスプレイがありません。
    return;
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  //glScalef(1.0, 1.0, 1.0);

  // 色で画面を塗りつぶします。
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
* 現在ディスプレイに関連付けられている EGL コンテキストを削除します。
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
* 次の入力イベントを処理します。
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
* 次のメイン コマンドを処理します。
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    // 現在の状態を保存するようシステムによって要求されました。保存してください。
    engine->app->savedState = malloc(sizeof(struct saved_state));
    *((struct saved_state*)engine->app->savedState) = engine->state;
    engine->app->savedStateSize = sizeof(struct saved_state);
    break;
  case APP_CMD_INIT_WINDOW:
    // ウィンドウが表示されています。準備してください。
    if (engine->app->window != NULL) {
      engine_init_display(engine);
      engine_draw_frame(engine);
    }
    break;
  case APP_CMD_TERM_WINDOW:
    // ウィンドウが非表示または閉じています。クリーン アップしてください。
    engine_term_display(engine);
    break;
  case APP_CMD_GAINED_FOCUS:
    break;
  case APP_CMD_LOST_FOCUS:
    // また、アニメーションを停止します。
    engine_draw_frame(engine);
    break;
  }
}
/**
* これは、android_native_app_glue を使用しているネイティブ アプリケーション
* のメイン エントリ ポイントです。それ自体のスレッドでイベント ループによって実行され、
* 入力イベントを受け取ったり他の操作を実行したりします。
*/
void android_main(struct android_app* state) {
  struct engine engine;

  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  if (state->savedState != NULL) {
    // 以前の保存状態で開始します。復元してください。
    engine.state = *(struct saved_state*)state->savedState;
  }

  // ループはスタッフによる開始を待っています。

  while (1) {
    // 保留中のすべてのイベントを読み取ります。
    int ident;
    int events;
    struct android_poll_source* source;

    // 無期限にブロックしてイベントが発生するのを待ちます。
    while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)& source)) >= 0) {

      // このイベントを処理します。
      if (source != NULL) {
        source->process(state, source);
      }

      // 終了するかどうか確認します。
      if (state->destroyRequested != 0) {
        engine_term_display(&engine);
        return;
      }
    }
  }
}
