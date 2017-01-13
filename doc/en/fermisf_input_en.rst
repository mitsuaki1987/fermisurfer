Input file
==========

input-file format
-----------------

You have to prepare following data:

-  The number of :math:`k` grid (three direction)

-  Reciprocal lattice vectors

-  The number of bands

-  The orbital energy at each band and :math:`k` (We call it "energy") .

-  Variables that you want to plot with color (We call it "matrix
   elements").

The input file is as follows (``mgb2_vfz.fs``):

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
        

#. The number of :math:`k` in each direction

#. Switch to specify type of :math:`k` grid (Choose from ``0, 1, 2``)

   :math:`k` grid is represented as follows:

   .. math::

   \begin{align} {\boldsymbol k}_{i,j,k} =
   x_i {\boldsymbol b}_1 + y_j {\boldsymbol b}_2 + z_k {\boldsymbol b}_3,
   \end{align}

   where :math:`i, j, k = 1 \cdots N_1, 1 \cdots N_2, 1 \cdots N_3` , and
   :math:`N_1, N_2, N_3` are the number of :math:`k` in each direction.

   :math:`x_i, y_j,z_k` can be chosen from below:

   -  ``0`` (Monkhorst-Pack grid) : :math:`x_i = \frac{2 i - 1 - N_1}{2 N_1}`

   -  ``1`` : :math:`x_i = \frac{i - 1}{N_1}`

   -  ``2`` : :math:`x_i = \frac{2 i - 1}{2 N_1}`

#. The number of bands

#. Reciprocal lattice vector 1 (arbitrary unit)

#. Reciprocal lattice vector 2

#. Reciprocal lattice vector 3

#. Energy (The order of component is written in  :ref:`format` )

   ``fermisurfer`` assume that the Fermi energy is ``0.0`` in the
   default. You can shift the Fermi energy by using
   ``Shift Fermi Energy`` menu described at the section 6.5.

#. Matrix elements (The order of component is written in  :ref:`format` )

   If you have no quantity to plot on Fermi surfaces, please use
   ``Unicolor`` switch in ``Color scale mode`` menu described at section
   6.7. In that case, although this "Matrix elements" is not used by
   ``fermisurfer``, please fill them with arbitrary numbers.

--------------

Converting bxsf file
--------------------

You can generate input files for ``fermisurfer`` from an input file for
XCrysDen (the bxsf format) by using the utility program ``bxsf2frmsf``.

The usage is as follow (we use ``examples/pb.bxsf`` as an example.):

For Linux
~~~~~~~~~

You can launch generated executable as follows:

.. code-block:: bash

    $ bxsf2frmsf pb.bxsf
        

You need a space between the command and input-file name. After it
finishes, the following files are generated:

-  ``pb_vf.frmsf``: Absolute value of the Fermi velocity
-  ``pb_vfx.frmsf``: :math:`x` component of the Fermi velocity
-  ``pb_vfy.frmsf``: :math:`y` component of the Fermi velocity
-  ``pb_vfz.frmsf``: :math:`z` component of the Fermi velocity
-  ``pb_vfa1.frmsf``: Component of the Fermi velocity (along :math:`{\bf a}_1`)
-  ``pb_vfa2.frmsf``: Component of the Fermi velocity (along :math:`{\bf a}_2`)
-  ``pb_vfa3.frmsf``: Component of the Fermi velocity (along :math:`{\bf a}_3`)

For Windows
~~~~~~~~~~~

Click mouse right button on the input file. Choose "Open With ..." menu,
then choose ``bxsf2frmsf.exe``.

.. _format:

How to produce the input file in C and fortran programs
-------------------------------------------------------

fortran

.. code-block:: fortran

      real(4) :: bvec1(3), bvec2(3), bvec3(3) ! Resiplocal lattice vector
      integer :: nk1, nk2, nk3 ! k-grid of each direction
      integer :: ishift ! 1 for shifted grid, 0 for unshifted grid.
      integer :: nbnd ! The number of bands
      real(4) :: eig(nk3,nk2,nk1,nbnd) ! energy
      real(4) :: x(nk3,nk2,nk1,nbnd) ! matrix element

      integer :: ik1, ik2, ik3, ibnd, fo

      open(fo, file = "sample.fs")
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
        
C

.. code-block:: c

      float bvec1[3], bvec2[3], bvec3[3]; /*Resiplocal lattice vector*/
      int nk1, nk2, nk3; /*k-grid of each direction*/
      int ishift; /*1 for shifted grid, 0 for unshifted grid.*/
      int nbnd; /*The number of bands*/
      float eig[nbnd][nk1][nk2][nk3]; /*Energy*/
      float x[nbnd][nk1][nk2][nk3]; /*Matrix element*/

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
        
