Quantum ESPRESSOを用いたチュートリアル
======================================

Quantum ESPRESSO version 6.2 から, FermiSurferで読み込む事が出来るデータ形式での
ファイルを出力出来るようになった.
FermiSurferでプロット出来るのは次の量である.

* Fermi速度の絶対値 :math:`|{\bf v}_{\rm F}|` (``fermi_velocity.x``)
* 各原子軌道への射影 :math:`|\langle \phi_{n l m} | \psi_{n k} \rangle|^2` (``fermi_proj.x``)

``PostProcess`` ツールのビルド
------------------------------

上記の量をFermiSurferで描画するためには, 次のようにしてQuantumESPRESSO内の ``PostProcess`` ツール
(バンド図や電子密度をプロットするためのツール群)をビルドする必要がある.

.. code-block:: sh

   $ make pp

SCF計算
-------

ここからチュートリアルに入る.
まず初めに ``pw.x`` プログラムを用いて電子状態計算をおこなう.
MgB\ :sub:`2`\ を題材として扱う.
入力ファイルは次の通りである.

:download:`scf.in <../qe/scf.in>`

.. literalinclude:: ../qe/scf.in

この計算で使われている擬ポテンシャルファイルは
`PS Library <http://theossrv1.epfl.ch/Main/Pseudopotentials>`_ に含まれているものであり,
以下のアドレスからダウンロードできる.

* http://theossrv1.epfl.ch/uploads/Main/NoBackup/Mg.pbe-n-kjpaw_psl.0.3.0.upf
* http://theossrv1.epfl.ch/uploads/Main/NoBackup/B.pbe-n-kjpaw_psl.0.1.upf
    
入力ファイルと擬ポテンシャルファイルを同じディレクトリに置き,
そのディレクトリで ``pw.x`` を実行する.

.. code-block:: sh

   $ mpiexec -np 4 pw.x -npool 4 -in scf.in

プロセス数, *k* 点並列数(``npool``)は自由.
お好みで, 異なる *k* グリッドでNon-scf計算を行っても良い.

Fermi速度の計算と描画
---------------------

``fermi_velocity.x`` プログラムを実行する.
入力ファイルは ``pw.x`` のものと同一である.

.. code-block:: sh

   $ mpiexec -np 1 fermi_velocity.x -npool 1 -in scf.in

このとき, *k* 点並列数(``npool``)は1(もしくは指定しない)でなければならない.
これにより, Fermi速度のファイル ``vfermi.frmsf`` が作られるので,
それをFermiSurferで読み込む.

.. code-block:: sh

   $ fermisurfer vfermi.frmsf

.. figure:: ../figs/qe_vf.png
   :width: 30%

なお, コリニアスピン計算では各スピンについてそれぞれ ``vfermi1.frmsf``, ``vfermi2.frmsf``
の2つのファイルが出力される.

原子軌道射影の計算と描画
------------------------

原子軌道射影の描画では, まず部分状態密度等を計算するプログラム
``projwfc.x`` を実行する.
入力ファイルは次の通りである.

:download:`proj.in <../qe/proj.in>`

.. literalinclude:: ../qe/proj.in

``PROJWFC`` ネームリストの終わり(``/``)以降は ``projwfc.x`` では使われず,
後で ``fermi_proj.x`` を実行するときのみ使われる.
``projwfc.x`` を実行するときのプロセス数, *k* 点並列数(``npool``)は
直前の ``pw.x`` の実行時と同じ値にしなければならない.

.. code-block:: sh

   $ mpiexec -np 4 projwfc.x -npool 4 -in proj.in

ただし, ``wf_collect=.true.`` としていたときは除く.

``projwfc.x`` の標準出力のはじめの方に次のような箇所がある.

::
   
     Atomic states used for projection
     (read from pseudopotential files):

     state #   1: atom   1 (Mg ), wfc  1 (l=0 m= 1)
     state #   2: atom   1 (Mg ), wfc  2 (l=1 m= 1)
     state #   3: atom   1 (Mg ), wfc  2 (l=1 m= 2)
     state #   4: atom   1 (Mg ), wfc  2 (l=1 m= 3)
     state #   5: atom   2 (B  ), wfc  1 (l=0 m= 1)
     state #   6: atom   2 (B  ), wfc  2 (l=1 m= 1)
     state #   7: atom   2 (B  ), wfc  2 (l=1 m= 2)
     state #   8: atom   2 (B  ), wfc  2 (l=1 m= 3)
     state #   9: atom   3 (B  ), wfc  1 (l=0 m= 1)
     state #  10: atom   3 (B  ), wfc  2 (l=1 m= 1)
     state #  11: atom   3 (B  ), wfc  2 (l=1 m= 2)
     state #  12: atom   3 (B  ), wfc  2 (l=1 m= 3)
          
これは各原子軌道の番号(``state #``)とその中身
(詳しくはQEに付属の ``INPUT_PROJWFC.html`` 等を参照)を表している.
この後でFermi面上で描画する原子軌道射影を選ぶ時にはこの番号を用いる.
具体的な例を示す. 上記の ``proj.in`` を用いて

.. code-block:: sh
                
   $ mpiexec -np 1 fermi_proj.x -npool 1 -in proj.in

のように実行してFermiSurfer用ファイル ``proj.frmsf`` を得るのだが,
このとき ``proj.in`` の ``/`` 以降

.. literalinclude:: ../qe/proj.in
   :lines: 8,9

では, 最初の数字 (``2``) が足し合わされる原子軌道射影の総数を表し,
その後に足し合わされる原子軌道射影に対応する番号が列挙される.
この場合は1番目のB原子の2pz軌道(``6``)と2番目のB原子の2pz軌道(``10``)
を足したもの

.. math::

   |\langle \phi_{B_1 2pz} | \psi_{nk} \rangle|^2
   + |\langle \phi_{B_2 2pz} | \psi_{nk} \rangle|^2

が出力される.
     
.. code-block:: sh

   $ fermisurfer proj.frmsf

.. figure:: ../figs/qe_proj_pz.png
   :width: 30%

また例えば, すべてのB原子の2px, 2py軌道からの寄与を合わせたものをプロットしたい場合には,
::

   &PROJWFC
   outdir = './'
   prefix='mgb2'
   Emin=-0.3422, 
   Emax=10.0578, 
   DeltaE=0.1
   /
   4
   7 8 11 12

のように ``proj.in`` を書き換えて, ``fermi_proj.x`` をもう一度実行すれば良い.
``projwfc.x`` を再度実行する必要は無い.

.. figure:: ../figs/qe_proj_pxy.png
   :width: 30%
