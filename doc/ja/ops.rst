操作方法
========

起動
----

Linux・Unix・Macの場合
~~~~~~~~~~~~~~~~~~~~~~

作成した実行可能ファイル ``fermisurfer`` にパスが通っている状態で

.. code-block:: bash

    $ fermisurfer mgb2_vfz.frmsf
        

とコマンド, スペース, 入力ファイル名とタイプします.
(サンプルファイルの中身はMgB\ :sub:`2`\ のFermi速度の :math:`z` 方向成分です. )

Windowsの場合
~~~~~~~~~~~~~

入力ファイル(この場合は ``mgb2_vfz.frmsf``)を右クリックし,
メニューから「プログラムから開く」を選択し,
実行ファイルを ``fermisurfer.exe`` に設定してください.

次に操作方法が出力され, Fermi面が描画されます(図 :num:`startpng` ).

.. _startpng:
     
.. figure:: ../figs/start.png
   :width: 500

   Fermisurferを起動した直後の画面.

-  マウスのドラッグによる回転が出来る.

-  マウスのホイールを使っての拡大・縮小が出来る.

-  ウィンドウの大きさを変えることもできる.

-  カーソルキー(Windowsではwasd)を使ってウィンドウ内で上下左右に図を動かせる.

-  右側のパネルを用いて様々な操作ができる.

次から右側のパネルでの操作をを説明する.

.. note::
   
   いくつかの操作はすぐに反映されず「Update」ボタンを押すことによって反映される.
   そのような操作については「要Update」と明記してある.

背景色
------

背景色を黒または白に切り替えます. Brillouin
Zoneの枠線も白/黒と切り替わります.

.. _backgroundpng:
     
.. figure:: ../figs/background.png
   :width: 500

バンド毎の表示・非表示の切り替え
--------------------------------

バンド毎の表示on/offを切り替えます(図 :num:`bandpng`).

.. _bandpng:
     
.. figure:: ../figs/band.png
   :width: 600

   "Band" メニューで3番目のバンドの表示/非表示を切り替える.

Brillouin zone (要Update)
-------------------------

描画範囲をFirst Brillouin Zone/Primitive Brillouin Zoneと
切り替える事が出来ます (図 :num:`brillouinzonepng`).

Fisrst Brillouin Zone
    :math:`{\rm \Gamma}` 点から一番近いBragg面で囲まれた領域

Primitive Brillouine Zone
    逆格子ベクトルを辺とする平行6面体領域

.. _brillouinzonepng:
     
.. figure:: ../figs/brillouinzone.png
   :width: 600

   "Brillouin zone" メニューでBrillouin領域のとり方を変更する.

Color bar
---------

カラーバーの表示/非表示を切り替えます(図 :num:`colorbarpng`).

.. _colorbarpng:
     
.. figure:: ../figs/colorbar.png
   :width: 600

   "Color bar On/Off" メニューでカラーバーの表示/非表示を切り替える.

カラープロットの種類と範囲 (要Update)
-------------------------------------

Fermi面の色表示のさせ方を変更します(図 :num:`colorscalepng`).

Input (1D) (入力ファイルのデータが1個のときのデフォルト) :
    青→緑→赤の範囲でカラープロットする.

Input (2D) (入力ファイルのデータが2個のときのデフォルト) :
    カラーサークル(図参照)上の色でカラープロットする.

Input (3D) (入力ファイルのデータが3個のときのデフォルト) :
    フェルミ面上の線としてプロットする.
    フェルミ面の色は「Band Index」の場合と同様.

Fermi Velocity (入力ファイルのデータがエネルギーだけのときのデフォルト) :
    エネルギーの差分からFermi速度 :math:`{\bf v}_{\rm F} = \nabla_k \varepsilon_k`
    を計算し, その絶対値をカラープロットする.
    
Band Index :
    物理量に関係なく, 各バンド毎に単色でFermi面を塗る.

Input (1D, Gray), Fermi Velocity (Gray)
    黒色の濃淡でプロットする.

また, カラープロットの色の範囲や3D線プロットの線の長さは
「Min of Scale」および 「Max of Scale」のテキストボックスに入力することで変更できる.
    
.. _colorscalepng:
     
.. figure:: ../figs/colorscale.png
   :width: 700

   "Color scale mode" メニュー.

Equator (要Update)
------------------

ある :math:`{\bf k}` に対して,
:math:`{\bf v}_{\rm F} \cdot {\bf k} = 0` となる線
(Equator:極軌道, もしくはExtremal orbit)を表示, 変更する.
(図 :num:`equatorpng`).
「Equator」チェックボックスで表示・非表示を切り替える.
この時「Update」を押す必要はない.
「Equator-v :」のテキストボックスで :math:`{\bf k}` を指定する. 
:math:`{\bf k}` ベクトルはフラクショナル座標で入力する.
       
.. _equatorpng:
     
.. figure:: ../figs/equator.png
   :width: 700

   "Equator"メニューで Fermi面の極軌道(Equator)を表示する.

補間の細かさ (要Update)
-----------------------

補間により図の曲面を滑らかにする (図 :num:`interpolatepng`).
ただし分点数を増やすと描画にかかる時間も増えるので注意.

.. _interpolatepng:
     
.. figure:: ../figs/interpolate.png
   :width: 700

   "Interpolate"メニューで 分点数を1から4に変える.

Fermi面のどちら側に光を当てるか
-------------------------------

光を当てる面を変更します (図 :num:`lightpng`).

Both :
    Fermi面の表裏両面に光を当てます.

Unoccupy :
    非占有領域側のみに光を当てます.
   
Occupy :
    占有領域側のみに光を当てます.

.. _lightpng:
     
.. figure:: ../figs/light.png
   :width: 500

   "Lighting"メニューで光を当てるFermi面を変更する.

Line width
----------

ブリルアンゾーンの境界やノーダルライン等の線幅を変更します.

マウスドラッグをしたときの振る舞い
----------------------------------

マウスの左ボタンドラッグを行った時の動作を変更します.

Rotate(デフォルト)
    ドラッグをした方向に図形を回転させます.

Scale
    上方にドラッグすると図形を拡大,
    下方にドラッグすると図形を縮小します.

Translate
    ドラッグした方向に図形を動かします.

.. figure:: ../figs/mouce.png
   :width: 200
           
ノーダルライン
--------------

物理量が0となるところに引く線(ノーダルライン)のOn/Offを切り替えます(図 :num:`nodelinepng`).

.. _nodelinepng:
     
.. figure:: ../figs/nodeline.png
   :width: 500

   "Nodal line" メニューでnodal lineの表示/非表示を切り替える.

ブリルアンゾーンの断面 (要Update)
---------------------------------

Brillouin領域を任意の断面で切り取り,
2次元のFermi面(線)を描画する (図 :num:`sectionpng`).

「Section」チェックボックスで断面の表示・非表示を切り替える
(この操作は「Update」の必要はない).
断面の法線の指定は「Section-v :」のテキストボックスで行う.
法線ベクトルはフラクショナル座標で指定する。

また, 「On Gamma」のチェックボックスがオンになっているときには
断面は :math:`\Gamma` 点を通る.
   
.. _sectionpng:
     
.. figure:: ../figs/section.png
   :width: 700

Fermiエネルギーの変更 (要Update)
--------------------------------

Fermiエネルギー(デフォルトでは0)を任意の値にずらします.
(図 :num:`shiftpng`).

.. _shiftpng:
     
.. figure:: ../figs/shift.png
   :width: 500

   "Shift Fermi energy"メニューで Fermi エネルギーを0.0 Ryから0.1 Ryに変える.

立体視
------

裸眼立体視用の図の表示/非表示を切り替えます(図 :num:`stereogrampng`).

None (デフォルト) :
    立体視を無効にします.

Parallel :
    平行法用の図を表示します.

Cross :
    交差法用の図を表示します.

.. _stereogrampng:
     
.. figure:: ../figs/stereogram.png
   :width: 700

四面体の切り方 (要Update)
-------------------------

四面体の切り方を変えます.
図が綺麗になる可能性がありますが,
多くの場合は逆に図がギザギザして汚くなるようです.

.. figure:: ../figs/tetrahedron.png
   :width: 200
           
サイズ・角度・位置の数値での調整
--------------------------------

視点を変更します(図 :num:`setviewpng`).

Scale :
    図形のサイズを指定します.

Position :
    図形の上下位置を指定します.

Rotate :
    x,y,z軸周りの回転角を指定し, "Roate" ボタンを押すと回転する.
    回転操作はz軸-y軸-x軸の順で行われます.

.. _setviewpng:
     
.. figure:: ../figs/setview.png
   :width: 300

   "View point"メニューで 視点を変更する.

画像の保存方法
--------------

``fermisurfer`` には画像をファイル出力する機能はありません.
お使いのPCにあった方法でスクリーンショットを取得して
(``Printscreen`` キーを押すなど)
ペイントブラシやgimpで編集して画像を作成してください.

