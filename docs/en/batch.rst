Batch mode
==========

By using "Batch mode", we can generate an image (PNG) file drawn by FermiSurfer
only with the command-line operation.
By using this batch mode, we can easily make 
`this kind of many figures. <http://fermisurfer.osdn.jp/table.html>`_

For example, in ``example/`` directory, when we execute

.. code-block:: bash

    $ fermisurfer mgb2_vfz.frmsf frmsf.in 500 500

we will obtain an image file ``frmsf.in.png``.
The last two numbers are the width and the height of the window.
``frmsf.in`` is a configuration file for the batch mode;
its contents are as follows:

.. literalinclude:: ../../examples/frmsf.in

They are corresponding to the operations in the panel
written in the previos section,
and the available keywords are as follows
(for the ignored keyword, each default value is used) :

.. csv-table::
   :widths: 10, 30, 30, 30
   :header: "Keyword", "Available parameter", "Default value", "Description"

   "background", "float float float", "0 0 0", "Background color"
   "band", "1 or 0 for each band", "1 1 1 1 ...", "Show(1) or hide(0) each band"
   "brillouinzone", "first, primitive", "first", "Kind of the Brillouin zone"
   "colorbar", "0, 1", "1", "Show(1) or hide (0) the color bar"
   "colorscale", "input1d, input2d, input3d, fermivelocity, bandindex, inputgray, fermivelocitygray", "input1d", "Kind of the color plot"
   "minmax", "float float", "The min. and max. through Fermi surfaces", "the range of the color scale"
   "equator", "float float float", "If it is not specified, equator is not shown", "Tangent vector for equator (fractional coordinate)"
   "interpol", "int", "1", "Degree of the interpolation"
   "linewidth", "float", "3.0", "Line width"
   "lighting", "both, unoccupied, occupied", "both", "Which side is illuminated"
   "nodalline", "0, 1", "0", "Show(1) or hide(0) the nodal line"
   "section", "float float float", "Section is not shown", "Normal vector for the section (fractional coordinate)"
   "acrossgamma", "0, 1", "1", "Whether math:`{\rm \Gamma}` is included (1) or not (0) in the section."
   "position", "float float", "0.0, 0.0", "The position of the figure"
   "scale", "float", "1.0", "The scale of the figure"
   "rotation", "float float float", "0.0, 0.0, 0.0", "Rotation arround x-, y-, and z-axis"
   "fermienergy", "float", "0.0", "Fermi energy"
   "stereogram", "none, parallel, cross", "none", "Stereogram"
   "tetrahedron", "int from 0 to 15", "0", "Direction to cut tetrahedra"

.. note::

   This function uses "import" command to get the screen-shot in ImageMagic.
   Therefore ImageMagic have to be installed to use this function.
