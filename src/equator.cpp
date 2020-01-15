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
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "basic_math.hpp"
/**
@brief Compute equator \f$\{\bf v}_{n k} \cdot {\bf k} = 0\f$

 Modify : ::nequator, ::kveq, ::kveq_rot
*/
void equator(
int nb,
int nthreads,
GLfloat eqvec[3],
int *ntri,
GLfloat ****kvp,
GLfloat ****nmlp,
wxTextCtrl* terminal,
int *nequator,
GLfloat ****kveq,
GLfloat **kveq_rot
) {
  int ib, itri, i, j, ithread, nequator0;
  std::vector<std::vector<std::vector<std::vector<GLfloat>>>> kveq_v;
 
  kveq_v.resize(nthreads);

  *terminal << wxT("    band   # of equator\n");
  for (ib = 0; ib < nb; ib++) {

#pragma omp parallel default(none) \
  shared(nb,ib,nmlp,kvp,ntri,eqvec,kveq_v) \
  private(itri,i,j,ithread)
    {
      int sw[3];
      GLfloat a[3][3], prod[3];
      std::vector<std::vector<GLfloat>> kveq_0;

      kveq_0.resize(2);
      for (i = 0; i < 2; i++) kveq_0.at(i).resize(3);

      ithread = get_thread();
      kveq_v.at(ithread).resize(0);

#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {

        for (i = 0; i < 3; ++i) {
          prod[i] = 0.0f;
          for (j = 0; j < 3; ++j) prod[i] += eqvec[j] * nmlp[ib][itri][i][j];
        }/*for (i = 0; i < 3; ++i)*/

        eigsort(3, prod, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.f - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if ((prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])
          || (prod[sw[0]] <= 0.0 && 0.0 < prod[sw[1]])) {
          for (i = 0; i < 3; ++i) {
            kveq_0.at(0).at(i) = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
            kveq_0.at(1).at(i) = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          }/*for (i = 0; i < 3; ++i)*/
          kveq_v.at(ithread).push_back(kveq_0);
        }/*else if (prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])*/
        else if ((prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])
          || (prod[sw[1]] <= 0.0 && 0.0 < prod[sw[2]])) {
           for (i = 0; i < 3; ++i) {
             kveq_0.at(0).at(i) = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
             kveq_0.at(1).at(i) = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
           }/*for (i = 0; i < 3; ++i)*/
           kveq_v.at(ithread).push_back(kveq_0);
        }/*else if (prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/* End of parallel region */
    /*
     Sum node-lines in all threads
    */
    nequator[ib] = 0;
    for (ithread = 0; ithread < nthreads; ithread++)
      nequator[ib] += kveq_v.at(ithread).size();
    *terminal << wxString::Format(wxT("    %d       %d\n"), ib + 1, nequator[ib]);
    /*
    Allocate
    */
    kveq[ib] = new GLfloat * *[nequator[ib]];
    kveq_rot[ib] = new GLfloat[6 * nequator[ib]];
    for (itri = 0; itri < nequator[ib]; ++itri) {
      kveq[ib][itri] = new GLfloat * [2];
      for (i = 0; i < 2; ++i)
        kveq[ib][itri][i] = new GLfloat[3];
    }
    /*
    Input
    */
    nequator0 = 0;
    for (ithread = 0; ithread < nthreads; ithread++) {
      for (itri = 0; itri < kveq_v.at(ithread).size(); ++itri) {
        for (i = 0; i < 2; ++i) {
          for (j = 0; j < 3; ++j) {
            kveq[ib][nequator0][i][j] = kveq_v.at(ithread).at(itri).at(i).at(j);
          }
        }
        nequator0 += 1;
      }
    }
  }/*for (ib = 0; ib < nb; ib++)*/
}/*void equator()*/
