FermiSurfer

Web page : http://fermisurfer.osdn.jp/

FermiSurfer is an visualization tool for Fermi surfaces.
This application displays Fermi surfaces colored as a function of an arbitrary
scalar quantities such as magnitude of Fermi velocities and superconducting gap.

# Contents of this package

 * bin/ : Executable (binary) files for Windows
 * configure : Script for the installation
 * src/ : Source code directory
 * docs/ : Directory for User's manual
   * docs/index.html : Index of documentation
 * examples/ : Sample input files (Pb and MgB2)

# Installation

## Windows

We do not need compile programs;
Binary file bin/fermisurfer.exe can be used immediately.

## Linux/UNIX/macOS

Before we install FermiSurfer, we need to install glut or freeglut package
with a package installer such as apt, yum, fink, homebrew, macports.

Next, we install FermiSurfer into a directory which is included in $PATH as

$ ./configure --prefix=**where we install**
$ make
$ make install

# Usage

We try to display Fermi surfaces of MgB2 with an input file examples/mgb2_vfz.frmsf

## Windows

We click the mouse-right-button on the input file. Choose "Open With ..." menu,
then choose fermisurfer.exe from the directory tree.

## Linux/UNIX/macOS

From the terminal

$ fermisurfer examples/mgb2_vfz.frmsf

# Contact

Mitsuaki Kawamura

mkawamura__at__issp.u-tokyo.ac.jp
