Install
=======

Installation in Linux and macOS
-------------------------------

#. Install the required package

   -  For Debian/Ubuntu

      .. code-block:: bash

          $ sudo aptitude install libwxgtk3.0-dev
                    

   -  For Red Hat Enterprise Linux/CentOS

      .. code-block:: bash

          $ sudo yum install wxGTK3-devel.x86_64
                    
   -  macOS (Homebrew)

      .. code-block:: bash

          $ brew install wxmac
                      
   -  macOS (Mac Ports)

      .. code-block:: bash

          $ port install wxWidgets-3.0
                      
#. Install

   .. code-block:: bash

      $ ./configure
      $ make
      $ sudo make install
               
Then a binary file ``src/fermisurfer`` is generated and
copied into ``/usr/local/bin/``.

Installation in Windows
-----------------------

Download the binary file.

Alternatively, we can build FermiSurfer by ourselves
after we install wxWidgets library.
We can use ``fermisurfer.vcxproj`` together with VisualStudio.

