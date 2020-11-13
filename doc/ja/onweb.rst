FermiSurfer on Web
==================

下記のページにアクセスするとWebブラウザー上でFermiSurferを使うことができます。
https://fermisurfer.osdn.jp/js/index.php

操作方法
--------

:ref:`アプリ版FermiSurferの機能 <ops>` と同等を目指していますが、一部の機能はまだサポートされていません。

ファイル入力
------------

画面左上のファイル選択ボタンでローカルのFRMSFファイル(BXSFファイルはサポートしていません)
を選択するとフェルミ面が表示されます。
表示されるまでややタイムラグが生じる場合があります。

Web上のファイルを開く
---------------------

以下のようにURLに引数をつけることにより、Web上で公開されているフェルミ面の入力ファイルを、リンクをクリックするだけで開くことができます。

https://fermisurfer.osdn.jp/js/index.php?frmsf=https://fermisurfer.osdn.jp/js/Pb.js

``https://fermisurfer.osdn.jp/js/index.php?frmsf=`` に続けてファイルの保存先のURLを付け足します。
これは研究紹介やプレスリリースのページで使うとよいでしょう。

このときの入力ファイル ``Pb.js`` は、

.. code-block:: javascript

   frmsf="16 16 16 1 2 -0.67303315756516724 0.67303315756516724 ... ";

のように、 :ref:`FRMSF形式 <input>` のファイルの改行をスペースに置き換えて1行の文字列にしたものを
``frmsf`` という変数に代入するjavascriptソースになっています。
BXSF形式には対応していません。

FRMSF形式のファイルを上記の形式に変換するにはコマンドラインで

.. code-block:: bash

   sed -e '1i frmsf="' -e '$a ";' ANY.frmsf | perl -pe 's/\n/ /g' | sed -E -e 's/ +/ /g' -e 's/" /"/g' -e 's/ "/"/g' > ANY.js

とします。
これと同じ処理を行うシェルスクリプトファイルがこちらにあります。

https://fermisurfer.osdn.jp/js/frmsf2js.sh

使い方は下記のとおりです。 ``ANY.js`` (ANYの部分は任意) というファイルが生成されるので、それをWebサーバーに配置します。

.. code-block:: bash

   $ bash frmsf2js.sh ANY.frmsf

