インストール手順
================

Linuxの場合
-----------

#. 必要なパッケージをインストール(既にパッケージが入っている場合は何も起こりません.)

   -  Debian/Ubuntu 等

      .. code-block:: bash

          $ sudo aptitude install freeglut3-dev
                      

   -  Red Hat Enterprise Linux/CentOS 等

      .. code-block:: bash

          $ sudo yum install freeglut-devel.x86_64
                      

#. メイク

   .. code-block:: bash
         
       $ cd ./src
       $ make all
       $ make install
               

以上で実行可能ファイル ``fermisurfer`` および ``bxsf2frmsf`` が作られ,
``/usr/local/bin/`` にコピーされます.

Mac OSX の場合
--------------

#. Xcodeをあらかじめインストールしておく

#. Makefileを適当なエディタで開いて#macの下の行の#を消す.

   .. code-block:: bash

       $ cd ./src
       $ emacs Makefile 
               

#. メイク

   .. code-block:: bash

       $ make fermisurfer
       $ make install
               

以上で実行可能ファイル ``fermisurfer`` および ``bxsf2frmsf`` が作られ,
``/usr/local/bin/`` にコピーされます.

Windowsの場合
-------------

実行可能ファイル ``bin/fermisurfer.exe`` を直接実行します.
freeglutライブラリをダウンロードして自分でコンパイルすることも可能です.

