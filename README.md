# Finite elements library

This repository is archived. The library has been merged into [`eslib` library](https://github.com/eugnsp/eslib).

## Synopsis

This is a finite elements library that is being developed mostly for educational purposes.

## Main features

* 1D meshes
* 2D meshed based on half-edge datastructure
* Vertex, edge and face iterators and views
* 1D and 2D Lagrange elements of variable degree that is fixed at compile-time
* 2D&otimes;1D Lagrange elements
* Guass and Dunavant rules for itegration
* Matrix-based linear, non-linear and eigensolvers
* Mesh faces colouring and multi-threaded matrix assembly
* Import from Gmsh mesh files, export into VTK `.vtk` and Matlab `.mat` files

## Dependencies

* [`esl` library](https://github.com/eugnsp/esl)
* [`esu` library](https://github.com/eugnsp/esu)

Requires C++17 compiler. Tested with GCC 8.3.0 and Clang 7.0.0.
