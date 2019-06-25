Install
=======

Installation in Linux
---------------------

#. Install the required package

   -  For Debian/Ubuntu

      .. code-block:: bash

          $ sudo aptitude install libwxgtk3.0-dev
                    

   -  For Red Hat Enterprise Linux/CentOS

      .. code-block:: bash

          $ sudo yum install wxGTK3-devel.x86_64
                    

#. Install

   .. code-block:: bash

      $ ./configure
      $ make
      $ sudo make install
               
Then a binary file ``src/fermisurfer`` is generated and
copied into ``/usr/local/bin/``.

Installation in Mac and Windows
-------------------------------

Download the binary file for each OS.

Alternatively, we can build FermiSurfer by ourselves
after we install wxWidgets library.
For Mac, we can beuild by using configuare script as in Linux.
For Windows, we can use ``fermisurfer.vcxproj`` together with VisualStudio.

