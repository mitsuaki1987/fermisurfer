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
                    

#. Install

   .. code-block:: bash

      $ ./configure
      $ make
      $ make install
               
Then binary files ``src/fermisurfer`` and ``src/bxsf2frmsf`` are generated and
copied into ``/usr/local/bin/``.

For Mac OSX
-----------

#. Install Xcode.

#. Install

   .. code-block:: bash

      $ ./configure
      $ make
      $ make install
               
Then binary files ``src/fermisurfer`` and ``src/bxsf2frmsf`` are generated and
copied into ``/usr/local/bin/``.

Installation in Windows
-----------------------

Use an executable ``bin/fermisurfer.exe`` . You can also compile
``fermisurfer`` manually by using freeglut library.

