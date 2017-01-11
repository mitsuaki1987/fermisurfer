Install
=======

Installation in Linux
---------------------

#. Install the required package

   -  For Debian/Ubuntu

      .. code-block:: bash

              $ sudo aptitude install freeglut3-dev
                    

   -  For Red Hat Enterprise Linux/CentOS

      .. code-block:: bash

              $ sudo yum install freeglut-devel.x86_64
                    

#. Make

   .. code-block:: bash

       $ cd ./src
       $ make fermisurfer
       $ make install
               

Then a binary file ``fermisurfer`` is generated and put in
``/usr/local/bin/``.

For Mac OSX
-----------

#. Install Xcode.

#. Open ``Makefile`` with an editor, and delete ``#`` below ``#mac``.

   .. code-block:: bash

       $ cd ./src
       $ emacs Makefile
               

#. Make.

   .. code-block:: bash

       $ make fermisurfer
       $ make install
               

   Then a binary file ``fermisurfer`` is generated and put in
   ``/usr/local/bin/``.

Installation in Windows
-----------------------

Use an executable ``bin/fermisurfer.exe`` . You can also compile
``fermisurfer`` manually by using freeglut library.

