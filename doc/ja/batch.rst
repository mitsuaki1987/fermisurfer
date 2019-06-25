バッチ・モード
==============

コマンドライン操作のみでFermiSurferで描画した図を画像(PNG)ファイルに保存する, 
「バッチ・モード」について説明する.
例として、 ``example/`` ディレクトリ内で

.. code-block:: bash

    $ fermisurfer mgb2_vfz.frmsf frmsf.in 500 500

を実行すると, 画像ファイル ``frmsf.in.png`` が出力される.
末尾の二つの数字はウィンドウの幅および高さである.
``frmsf.in`` はバッチ・モード用の描画設定ファイルで,
次のように書かれている.

.. literalinclude:: ../../examples/frmsf.in

これは前節のパネル操作に対応しており,
指定可能なキーワードは次の通りである。
また、指定しなかったキーワードについてはデフォルト値が使われる.

.. csv-table::
   :widths: 10, 30, 30, 30
   :header: "キーワード", "指定可能なパラメーター", "デフォルト値", "説明"

   "background", "black, white", "black", "背景色"
   "band", "1または0をバンド本数分", "1 1 1 1 ...", "各バンドの表示(1)または非表示(0)"
   "brillouinzone", "first, primitive", "first", "ブリルアンゾーンの種類"
   "colorbar", "0, 1", "1", "カラーバーの表示(1)または非表示(0)"
   "colorscale", "input1d, input2d, input3d, fermivelocity, bandindex, inputgray, fermivelocitygray", "input1d", "カラープロットの種類"
   "minmax", "実数 実数", "フェルミ面全体の最小値, 最大値", "カラースケールの範囲"
   "equator", "実数 実数 実数", "指定しない場合は極軌道を表示しない", "極軌道の接ベクトル(フラクショナル座標)"
   "interpol", "自然数", "1", "補間の細かさ"
   "linewidth", "実数", "3.0", "線幅"
   "lighting", "both, unoccupied, occupied", "both", "どちらの面に照光するか"
   "nodalline", "0, 1", "0", "ノーダルラインの表示(1)または非表示(0)"
   "section", "実数 実数 実数", "指定しない場合は断面を表示しない", "断面の法線ベクトル(フラクショナル座標)"
   "acrossgamma", "0, 1", "1", "断面がΓ点を通るか(1)否か(0)"
   "position", "実数 実数", "0.0, 0.0", "図形の描画位置"
   "scale", "実数", "1.0", "図形の拡大率"
   "rotation", "実数 実数 実数", "0.0, 0.0, 0.0", "図形のx-, y-, z-軸周りの回転角"
   "fermienergy", "実数", "0.0", "Fermiエネルギー"
   "stereogram", "none, parallel, cross", "none", "立体視"
   "tetrahedron", "0から15の整数", "0", "四面体の切り方"

.. note::

   この機能は ImageMagic のスクリーンショットを取るコマンド「import」を用いている.
   そのためこの機能を使うには ImageMagic がインストールされていなければならない.
