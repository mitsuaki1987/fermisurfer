Input file
==========

input fileの書式
----------------

用意するデータは,

-  Brillouin領域分割数(3方向)

-  逆格子ベクトル

-  バンド本数

-  軌道固有値(以下エネルギーと呼びます)の各バンド,  :math:`k` グリッド点での値

-  カラープロットしたい物理量(以下物理量と呼びます)の各バンド,
   :math:`k` グリッド点での値

です.

上記データを次のとおりの書式で並べます(サンプルファイル
``mgb2_vfz.frmsf`` の中身).

::

    40          40          36        (1)
    0                                 (2)
    3                                 (3)
    1.0000000      0.57735026      -0.0000000 (4)
    0.0000000       1.1547005       0.0000000 (5)
    0.0000000      -0.0000000      0.87206507 (6)
    2.91340202E-02                            (7)
    2.93242838E-02
    2.98905596E-02
    3.08193434E-02
    :
    :
    0.14393796
    0.12800488
    0.0000000                                 (8)
    0.36269817
    0.71675694
    1.0535113
    1.3644149
    :
    :
    -26.409407
    -19.318560
    -10.315671
        

#. :math:`k` グリッド数

#. :math:`k` グリッドの指定方法( ``0, 1, 2`` のいずれか)

   :math:`k` グリッドを次のように表します.

   .. math::

      \begin{align}
      {\boldsymbol k}_{i,j,k} =
      x_i {\boldsymbol b}_1 + y_j {\boldsymbol b}_2 + z_k {\boldsymbol b}_3
      \end{align}

   ただし:math:`i, j, k` は :math:`1` から各逆格子ベクトル方向の分割数
   :math:`N_1, N_2, N_3` (上で読み取ったもの)とする

   :math:`x_i, y_j,z_k` は次の3つのとり方が可能です.

   -  ``0`` の場合(Monkhorst-Packグリッド) :
      :math:`x_i = \frac{2 i - 1 - N_1}{2 N_1}`

   -  ``1`` の場合 : :math:`x_i = \frac{i - 1}{N_1}`

   -  ``2`` の場合 : :math:`x_i = \frac{2 i - 1}{2 N_1}`

#. バンド本数

#. 逆格子ベクトル1 (任意単位)

#. 逆格子ベクトル2

#. 逆格子ベクトル3

#. エネルギー (並び順は5.3節参照)

   ``fermisurfer`` はデフォルトではFermiエネルギーを
   ``0.0`` としています. ただし,
   後述の ``Shift Fermi Energy`` メニューを用いて
   Fermiエネルギーを ``0.0`` 以外の値に変更することも可能です.

#. 物理量 (並び順は5.3節参照)

   特にカラープロットをしたい量が無く,
   Fermi面の描画のみを行いたい場合には,
   後述の ``Color scale mode`` メニューから
   ``Unicolor`` を選択してください. その際,
   この"物理量"は利用されることはありませんが,
   ファイルの読み込みを完了させるために何らかの数字で埋めてください
   (すべて ``1.0`` とするなど).

BXSF形式からの変換
------------------

付属のユーティリティー ``bxsf2frmsf`` を使うと
XCrysDen用の入力ファイルから ``fermisurfer`` で読み取り可能な入力ファイルが
作られます. また, そのときにFermi面上にカラープロットされるFermi速度が,
エネルギーバンドから計算されます.

使い方は次のとおりです. ``examples/`` ディレクトリにある ``pb.bxsf``
ファイルを例として使います.

Linuxの場合
~~~~~~~~~~~

作成した実行可能ファイル ``bxsf2frmsf`` にパスが通っている状態で

.. code-block:: bash

    $ bxsf2frmsf pb.bxsf
        

とコマンド, スペース, 入力ファイル名とタイプします. その後,
次のファイルが生成されます.

-  ``pb_vf.frmsf``: Fermi速度の絶対値
-  ``pb_vfx.frmsf``: Fermi速度の :math:`x` 成分
-  ``pb_vfy.frmsf``: Fermi速度の :math:`y` 成分
-  ``pb_vfz.frmsf``: Fermi速度の :math:`z` 成分
-  ``pb_vfa1.frmsf``: Fermi速度の, 格子ベクトル :math:`{\bf a}_1` に平行な成分
-  ``pb_vfa2.frmsf``: Fermi速度の, 格子ベクトル :math:`{\bf a}_2` に平行な成分
-  ``pb_vfa3.frmsf``: Fermi速度の, 格子ベクトル :math:`{\bf a}_3` に平行な成分

Windowsの場合
~~~~~~~~~~~~~

入力ファイル(この場合は ``pb.bxsf``)を右クリックし,
メニューから「プログラムから開く」を選択し,
実行ファイルを ``bxsf2frmsf.exe`` に設定してください.

C/fortranでの入力ファイルの書き出し方
-------------------------------------

fortran

.. code-block:: fortran

      real(4) :: bvec1(3), bvec2(3), bvec3(3) !逆格子ベクトル
      INTEGER :: nk1, nk2, nk3 !各逆格子ベクトルの方向の分割数
      integer :: ishift !グリットをシフトさせるか(=1)否か(=0)
      integer :: nbnd !バンド数
      real(4) :: eig(nk3,nk2,nk1,nbnd) !エネルギー
      real(4) :: x(nk3,nk2,nk1,nbnd) !物理量

      integer :: ik1, ik2, ik3, ibnd, fo

      open(fo, file = "sample.frmsf")
      write(fo,*) nk1, nk2, nk3
      write(fo,*) ishift
      write(fo,*) nbnd
      write(fo,*) real(bvec1(1:3))
      write(fo,*) real(bvec2(1:3))
      write(fo,*) real(bvec3(1:3))
      do ibnd = 1, nbnd
         do ik1 = 1, nk1
            do ik2 = 1, nk2
               do ik3 = 1, nk3
                  write(fo,*) real(eig(ik3,ik2,ik1,ibnd)) 
               end do
            end do
         end do
      end do
      do ibnd = 1, nbnd
         do ik1 = 1, nk1
            do ik2 = 1, nk2
               do ik3 = 1, nk3
                  write(fo,*) real(x(ik3,ik2,ik1,ibnd)) 
               end do
            end do
         end do
      end do
      close(fo)

C言語

.. code-block:: c

      float bvec1[3], bvec2[3], bvec3[3]; /*逆格子ベクトル*/
      int nk1, nk2, nk3; /*各逆格子ベクトルの方向の分割数*/
      int ishift; /*グリットをシフトさせるか(=1)否か(=0)*/
      int nbnd; /*バンド数*/
      float eig[nbnd][nk1][nk2][nk3]; /*エネルギー*/
      float x[nbnd][nk1][nk2][nk3]; /*物理量*/
      
      FILE* fo;
      int ibnd, ik1, ik2, ik3;
      
      fo = fopen("sample.frmsf", "w");
      ierr = fprintf(fo, "%d %d %d\n", nk1, nk2, nk3);
      ierr = fprintf(fo, "%d\n", iswitch);
      ierr = fprintf(fo, "%d\n", nbnd);
      ierr = fprintf(fo, "%e %e %e\n", bvec1[0], bvec1[1], bvec1[2]); 
      ierr = fprintf(fo, "%e %e %e\n", bvec2[0], bvec2[1], bvec2[2]);
      ierr = fprintf(fo, "%e %e %e\n", bvec3[0], bvec3[1], bvec3[2]);
      for (ibnd = 0; ibnd < nbnd; ++ibnd) {  
         for (ik1 = 0; ik1 < nk1; ++ik1) { 
            for (ik2 = 0; ik2 < nk2; ++ik2) { 
               for (ik3 = 0; ik3 < nk3; ++ik3) { 
                  ierr = fprintf(fo, "%e\n", eig[ibnd][ik1][ik2][ik3]); 
               } 
            } 
         } 
      } 
      for (ibnd = 0; ibnd < nbnd; ++ibnd) {  
         for (ik1 = 0; ik1 < nk1; ++ik1) { 
            for (ik2 = 0; ik2 < nk2; ++ik2) { 
               for (ik3 = 0; ik3 < nk3; ++ik3) { 
                  ierr = fprintf(fo, "%e\n", x[ibnd][ik1][ik2][ik3]); 
               } 
            } 
         } 
      } 
      fclose(fo); 
        

