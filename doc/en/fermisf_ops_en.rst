Control FermiSurfer
===================

Launch
------

For Linux
~~~~~~~~~

You can launch generated executable as follows:

.. code-block:: bash

    $ ./fermisurfer mgb2_vfz.fs
        
You need a space between the command and input-file name.
(The sample input file ``mgb2_vfz.fs`` contains :math:`z` element of
the Fermi velocity in MgB\ :math:`_2`.)

For Windows
~~~~~~~~~~~

Click mouse right button on the input file. Choose "Open With ..." menu,
then choose ``fermisurfer.exe``.

After that, ``fermisurfer`` runs as the same whether you use Linux or
Windows. The information from the input file is printed.

::

   ########################################
   ##                                    ##
   ##  Welocome to FermiSurfer ver. 1.8  ##
   ##                                    ##
   ########################################
   
     Number of threads : 4

     Initialize variables ...
   
   
     ##  Brillouin zone informations  ###########
   
       k point grid : 40 40 36
       k point grid starts from Gamma.
       # of bands : 3
       bvec 1 : 1.000000 0.577350 -0.000000
       bvec 2 : 0.000000 1.154701 0.000000
       bvec 3 : 0.000000 -0.000000 0.872065
   
     ##  Max. and Min. of each bands  #######################
   
       Band   Eig_Min.      Eig_Max      Mat_Min      Mat_Max
       1     -0.428153     0.056262     -24.048639     24.048639 (1)
       2     -0.289572     0.121181     -23.320309     23.320309 (1)
       3     -0.133566     0.497620     -43.651634     43.651634 (1)
   
     ##  First Brillouin zone mode  #######
   
       band   # of patchs
       1       8824  (2)
       2       29354 (2)
       3       28293 (2)
   
   
     ##  Full color scale mode #############
   
       Max. value : 22.283419  (3)
       Min. value : -22.251053 (3)
   
       band   # of nodeline
       1       632  (4)
       2       1524 (4)
       3       2268 (4)
       band   # of Fermi-line
       1       100
       2       736
       3       0
   
     ##  How to handle  ###################
   
                 mouse drag : Rotate objects
                 mousewheel : Resize objects
                  cursorkey : Move objects
         mouse right button : Menu

#. The maximum/minimum value of energies and matrix elements in each
   bands.

#. The number of patches (planes that makes Fermi surfaces) in each
   bands.

#. The maximum and the minimum of matrix elements on Fermi surface.
   These correspond to the red and the blue; in this case, the matrix
   element is -22.283419 in the blue region, and that is 22.283419 in
   the red region. [(1) is Max./Min. in whole Brillouin zone.]

#. The number of node lines in each band.

Then, Operations are printed, and Fermi surfaces are drawn (Fig. :num:`startpng`).

.. _startpng:
     
.. figure:: ../figs/start.png

            Main view.

The following operations are available:

-  Rotation of objects with mouse drag

-  Expand and shrink with mouse wheel

-  Window re-sizing

-  Moving objects with cursor keys

-  Opening the menu with mouse right button

Here, I will explain all menus.

Band
----

It makes each band enable/disable (Fig. :num:`bandpng`).

.. _bandpng:
     
.. figure:: ../figs/band.png

            You make each band enable/disable with "Band" menu.

Background color
----------------

The background color is toggled between black and white; the edge of the
Brillouin Zone is also toggled between white and black (Fig. :num:`backgroundpng`).

.. _backgroundpng:
     
.. figure:: ../figs/background.png

            The background color is toggled with
            "Background color" menu.

Brillouin zone
--------------

You choose Brillouin-zone type as follows (Fig. :num:`brillouinzonepng`):

First Brillouin Zone
   The region surrounded by Bragg's planes the
   nearest to :math:`{\rm \Gamma}` point.

Primitive Brillouin Zone
   A hexahedron whose corner is the reciprocal
   lattice point.

.. _brillouinzonepng:
     
.. figure:: ../figs/brillouinzone.png

            You can change the type of the Brillouin zone with
            "Brillouin zone" menu.

Color bar
---------

The color bar becomes enable/disable (Fig. :num:`colorbarpng`).

.. _colorbarpng:
     
.. figure:: ../figs/colorbar.png

            Toggling the color bar with "Color bar On/Off" menu.

Color scale mode
----------------

It turns color pattern on Fermi surfaces (Fig. :num:`colorscalepng`).

Auto(default)
   It makes blue as the minimum on Fermi surfaces and red
   as the maximum on them.

Manual
   You can set manually (from standard input) values corresponding
   to blue and red.

Unicolor
   Fermi surfaces of each band are depicted with uni-color
   without relation to the matrix element.

Periodic
   It makes periodic color plot enable. When the matrix element
   varies as :math:`0` :math:`\rightarrow` :math:`\pi/ 3` :math:`\rightarrow` :math:`2\pi/ 3`
   :math:`\rightarrow` :math:`\pi` :math:`\rightarrow` :math:`4\pi/ 3` :math:`\rightarrow`
   :math:`5\pi/3` :math:`\rightarrow` :math:`2\pi`,
   the color varies as red :math:`\rightarrow` yellow
   :math:`\rightarrow` green :math:`\rightarrow` cyan :math:`\rightarrow` blue
   :math:`\rightarrow` magenta :math:`\rightarrow` red.

Fermi velocity (Auto)
   Compute the Fermi velocity :math:`{\bf v}_{\rm F} = \nabla_k \varepsilon_k`
   with the numerical differentiation of the energy,
   and plot the absolute value of that.
   The color scale spans from the minimum and the maximum of :math:`|{\bf v}_{\rm F}|`
   on the Fermi surface.
    
Fermi velocity (Manual)
   Compute the Fermi velocity :math:`{\bf v}_{\rm F} = \nabla_k \varepsilon_k`
   with the numerical differentiation of the energy,
   and plot the absolute value of that.
   The range of color scale can be specified manually from the terminal.

Gray scale (Manual), Gray scale (Auto)
   Plot with gray scale.

.. _colorscalepng:
     
.. figure:: ../figs/colorscale.png

            "Color scale mode" menu.

Equator
-------

We can draw the line where 
:math:`{\bf v}_{\rm F} \cdot {\bf k} = 0` for a vector :math:`{\bf k}`
(equator). See fig. :num:`equatorpng`
   
Equator
   Toggle the equator.

Modify equator
   We specify the :math:`{\bf k}` vector.
   Please type a vector (**fractional coordinate**) at the prompt

   ::

       New Miller index : 

.. _equatorpng:
     
.. figure:: ../figs/equator.png

   Display the equator with the "Equator" menu.

Interpolation
-------------

Smooth the Fermi surface with the interpolation (Fig. :num:`interpolatepng`).
Please type the number of interpolation points at the prompt

::

   New interpolation ratio :

The time for the plot increases with the interpolation ratio.

.. _interpolatepng:
     
.. figure:: ../figs/interpolate.png
   :width: 50%

   Modify the number of interpolation points from 1 to 4 with "Interpolate" menu.
            
Lighting
--------

We can choose the lighted side of the Fermi surface (Fig. :num:`lightpng`).

Both side
    Light both sides.

Unoccupied side
    Light unoccupied side.
   
Occupied side
    Light the occupied side.

.. _lightpng:
     
.. figure:: ../figs/light.png
   :width: 50%

   Change the lighted side by using the "Lighting" menu.

Line width
----------

Modify the width of the Brillouin-zone boundary, the nodal line, etc.
   
Mouse Drag
----------

It turns the event of the mouse-left-drag.

Rotate(default)
   Rotate the figure along the mouse drag.

Scale
   Expand/shrink the figure in upward/downward drag.

Translate
   Translate the figure along the mouse drag.

Nodal line
----------

The line on which the matrix element becomes 0 (we call it nodal line)
becomes enable/disable (Fig. :num:`nodelinepng`).

.. _nodelinepng:
     
.. figure:: ../figs/nodeline.png

            Toggling the node line with "Nodal line" menu.

Section
-------

Display a 2D plot of the Fermi surface (line)
on an arbitrary section of the Brillouin zone (Fig. :num:`sectionpng`).
   
Section
   Toggle the 2D plot of the Fermi surface (line).

Modify Section
   Specify the section. Please type the normal vector (**fractional coordinate**) at the prompt

   ::

       New Miller index : 

   The section crosses the head of the normal vector.
       
Modify Section (across Gamma)
   Specify the section. Please type the normal vector (**fractional coordinate**) at the prompt

   ::

       New Miller index : 

   The section crosses :math:`\Gamma` point.

.. _sectionpng:
     
.. figure:: ../figs/section.png

   Display 2D plot of the Fermi surface (line) with "Section" menu.

Shift Fermi energy
------------------

It shifts the Fermi energy (= 0 in default) to arbitrary value. When you
use this menu, first, it displays minimum and maximum energy in the
input file and the current Fermi energy;

::

    Min  Max  E_F 
    -0.428153 0.497620 0.000000 
    Fermi energy shift : 
        

Then, you should type the new Fermi energy; finally, the new Fermi
surfaces are depicted (Fig. :num:`shiftpng`).

.. _shiftpng:
     
.. figure:: ../figs/shift.png

            The Fermi energy is set from 0 Ry to 0.1 Ry with
            "Shift Fermi energy" menu

Stereogram
----------

The stereogram (parallel eyes and cross eyes) becomes enabled/disabled
(Fig. :num:`stereogrampng`).

None (Default)

Parallel
   Parallel-eyes stereogram

Cross
   Cross-eyes stereogram

.. _stereogrampng:
     
.. figure:: ../figs/stereogram.png

            The stereogram becomes enabled/disabled with
            "Stereogram" menu.

Tetrahedron
-----------

You change the scheme to divide into tetrahedra (``tetra # 1`` as default).
It is experimental.

View point
----------

Changing the view point.

Scale
    Change the size of the figure.

Position
    Change the xy position of the figure.

Rotation
    Change angles at x-, y-, z- axis. Rotations are performed as z-y-x
    axis.

In each menu, first the current value is printed. then a prompt to input
the new value appears (Fig. :num:`setviewpng`).

.. _setviewpng:
     
.. figure:: ../figs/setview.png
   :width: 70%

            Modify the view point by using "View point" menu

Saving images
-------------

``fermisurfer`` does not have any functions to save images to a file.
Please use the screenshot on your PC.

