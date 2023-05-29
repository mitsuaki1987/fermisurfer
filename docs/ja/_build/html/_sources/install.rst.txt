インストール手順
================

Linux, macOSの場合
------------------

#. 必要なパッケージをインストール(既にパッケージが入っている場合は何も起こりません.)

   -  Debian/Ubuntu 等

      .. code-block:: bash

          $ sudo aptitude install libwxgtk3.0-dev
                      

   -  Red Hat Enterprise Linux/CentOS 等

      .. code-block:: bash

          $ sudo yum install wxGTK3-devel.x86_64
                      
   -  macOS (Homebrew)

      .. code-block:: bash

          $ brew install wxmac
                      
   -  macOS (Mac Ports)

      .. code-block:: bash

          $ port install wxWidgets-3.0
                      

#. インストール

   .. code-block:: bash
         
       $ ./configure
       $ make
       $ sudo make install

以上で実行可能ファイル ``src/fermisurfer`` が作られ,
``/usr/local/bin/`` にコピーされます.

Windowsの場合
-------------

バイナリ実行ファイルをダウンロードする.

またwxWidgetsライブラリをインストールしたのちにFermiSurferを自前でビルドすることも可能.
VisualStudio用ファイル ``fermisurfer.vcxproj`` が利用できる.
