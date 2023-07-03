/*
The MIT License (MIT)

Copyright (c) 2014 Mitsuaki KAWAMURA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**@file
@brief Compute nodal lines
*/
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <wx/wx.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "basic_math.hpp"
#include "variable.hpp"
/**
@brief Compute node-line where \f$\Delta_{n k} = 0\f$

 Modify : ::nnl, ::kvnl, ::kvnl_rot

*/
void calc_nodeline()
{
  int ib, itri, i, j, ithread, nnl0;
  std::vector<std::vector<std::vector<std::vector<GLfloat> > > > kvnl_v;

  kvnl_v.resize(nthreads);

  *terminal << wxT("    band   # of nodeline\n");
  for (ib = 0; ib < nb; ib++) {

#pragma omp parallel default(none) \
shared(nb,ib,matp,kvp,ntri,kvnl_v) \
private(itri,i,j,ithread)
    {
      int sw[3];
      GLfloat a[3][3], matps[3];
      std::vector<std::vector<GLfloat> > kvnl0;

      kvnl0.resize(2);
      for (i = 0; i < 2; i++)kvnl0.at(i).resize(3);

      ithread = get_thread();
      kvnl_v.at(ithread).resize(0);

#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {

        for (i = 0; i < 3; ++i) matps[i] = matp[ib][itri][i][0];
        eigsort(3, matps, sw);

        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.f - matp[ib][itri][sw[j]][0]) 
              / (matp[ib][itri][sw[i]][0] - matp[ib][itri][sw[j]][0]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if (( matp[ib][itri][sw[0]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[1]][0])
          || (matp[ib][itri][sw[0]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[1]][0])) {
          for (i = 0; i < 3; ++i) {
            kvnl0.at(0).at(i) = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
            kvnl0.at(1).at(i) = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          }/*for (i = 0; i < 3; ++i)*/
          kvnl_v.at(ithread).push_back(kvnl0);
        }/*else if (matp[ib][itri][sw[0]] < 0.0 && 0.0 <= matp[ib][itri][sw[1]])*/
        else if ((matp[ib][itri][sw[1]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[2]][0])
          ||     (matp[ib][itri][sw[1]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[2]][0])) {
          for (i = 0; i < 3; ++i) {
            kvnl0.at(0).at(i) = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
            kvnl0.at(1).at(i) = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
          }/*for (i = 0; i < 3; ++i)*/
          kvnl_v.at(ithread).push_back(kvnl0);
        }/*else if (matp[ib][itri][sw[1]] < 0.0 && 0.0 <= matp[ib][itri][sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/* End of parallel region */
    /*
     Allocation of nodeline
     */
    nnl[ib] = 0;
    for (ithread = 0; ithread < nthreads; ithread++)
      nnl[ib] += kvnl_v.at(ithread).size();
    *terminal << wxString::Format(wxT("    %d       %d\n"), ib + 1, nnl[ib]);

    kvnl[ib] = new GLfloat * *[nnl[ib]];
    kvnl_rot[ib] = new GLfloat[12 * nnl[ib]];
    for (itri = 0; itri < nnl[ib]; ++itri) {
      kvnl[ib][itri] = new GLfloat *[2];
      for (i = 0; i < 2; ++i) {
        kvnl[ib][itri][i] = new GLfloat[3];
      }/*for (j = 0; j < 2; ++j)*/
    }
    /*
     Input
    */
    nnl0 = 0;
    for (ithread = 0; ithread < nthreads; ithread++){
      for (itri = 0; itri < kvnl_v.at(ithread).size(); ++itri) {
        for (i = 0; i < 2; ++i) {
          for (j = 0; j < 3; j++) {
            kvnl[ib][nnl0][i][j] = kvnl_v.at(ithread).at(itri).at(i).at(j);
          }
        }/*for (j = 0; j < 2; ++j)*/
        nnl0 += 1;
      }
    }
  }/*for (ib = 0; ib < nb; ib++)*/
}/*void calc_nodeline()*/
