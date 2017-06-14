プログラムの再配布
==================

自分のプログラムにFermiSurferを含める
-------------------------------------

FermiSurferは下記の :ref:`mitlicense` に基づいて配布されている.
これはかいつまんで言うと,
個人的(研究室や共同研究者等のグループ)なプログラムであろうとも,
公開したり売ったりするプログラムであろうとも
自由にコピペしたり改変して良いし,
どのようなライセンスで配布しても構わない, と言うことである.

Autoconfを使わずにFermiSurferをビルドする
-----------------------------------------

このパッケージではAutotools (Autoconf, Aitomake, Libtool)を使ってfermisurferをビルドしている.
もし再配布するソースコードにfermisurferを含めるときに,
Autoconfの使用に支障がある場合には, 以下の簡易版のMakefileを使うと良い (タブに注意).

.. code-block:: makefile

   CC = gcc
   CFLAGS=-g -O0 -lglut -lGLU -lGL -lm -fopenmp -g -DHAVE_GL_GLUT_H
   
   OBJS= \
   basic_math.o \
   bz_lines.o \
   calc_nodeline.o \
   draw.o \
   fermisurfer.o \
   fermi_patch.o \
   free_patch.o \
   initialize.o \
   kumo.o \
   menu.o \
   operation.o \
   read_file.o \
   section.o
   
   HEADERS= \
   basic_math.h \
   bz_lines.h \
   calc_nodeline.h \
   draw.h \
   fermi_patch.h \
   free_patch.h \
   initialize.h \
   kumo.h \
   menu.h \
   operation.h \
   read_file.h \
   section.h \
   variable.h
   
   all:fermisurfer bxsf2frmsf
   
   SUFFIXES: .o .c
   
   .c.o:
       $(CC) $(CFLAGS) -c $<
   
   fermisurfer:$(OBJS)
       $(CC) $(OBJS) $(CFLAGS) -o $@
   
   bxsf2frmsf:bxsf2frmsf.o
       $(CC) $< $(CFLAGS) -o $@
   
   clean:
       rm -rf *.o fermisurfer bxsf2frmsf
   
   basic_math.o:$(HEADERS)
   bz_lines.o:$(HEADERS)
   calc_nodeline.o:$(HEADERS)
   draw.o:$(HEADERS)
   fermisurfer.o:$(HEADERS)
   fermi_patch.o:$(HEADERS)
   free_patch.o:$(HEADERS)
   initialize.o:$(HEADERS)
   kumo.o:$(HEADERS)
   menu.o:$(HEADERS)
   operation.o:$(HEADERS)
   read_file.o:$(HEADERS)
   section.o:$(HEADERS)

.. _mitlicense:
   
MIT ライセンス
--------------

| Copyright (c) 2014 Mitsuaki Kawamura
| 
| 以下に定める条件に従い,
| 本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）
| の複製を取得するすべての人に対し,
| ソフトウェアを無制限に扱うことを無償で許可します. これには,
| ソフトウェアの複製を使用, 複写, 変更, 結合, 掲載, 頒布, サブライセンス,
| および/または販売する権利,
| およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます.
| 
| 上記の著作権表示および本許諾表示を,
| ソフトウェアのすべての複製または重要な部分に記載するものとします.
| 
| ソフトウェアは「現状のまま」で, 明示であるか暗黙であるかを問わず,
| 何らの保証もなく提供されます. ここでいう保証とは, 商品性,
| 特定の目的への適合性, および権利非侵害についての保証も含みますが,
| それに限定されるものではありません. 作者または著作権者は, 契約行為,
| 不法行為, またはそれ以外であろうと, ソフトウェアに起因または関連し,
| あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求,
| 損害, その他の義務について何らの責任も負わないものとします.
