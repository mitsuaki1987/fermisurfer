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
@brief Mathematical operations used in various step
*/
/**
 @brief Work as Modulo function of fortran
 @return Modulated value
*/
function modulo(
  i = 0, //!< [in]
  n = 0 //!< [in]
) {
  let j = 0;
  j = (i + 100 * n) % n;
  return j;
};/*modulo(let i, let n)*/
/**
 @brief Solve linear system
 @return Determinant
*/
function solve3(
  a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!< [in] Matix
  b = [0.0, 0.0, 0.0] //!< [in,out] Right hand side vector
)
{
  let i = 0;
  let det = 0.0, c = [0.0, 0.0, 0.0];
  /**/
  det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
      + a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
      + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
  /**/
  c[0] = b[0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
       + b[1] * (a[0][2] * a[2][1] - a[0][1] * a[2][2])
       + b[2] * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);
  /**/
  c[1] = b[0] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
       + b[1] * (a[0][0] * a[2][2] - a[0][2] * a[2][0])
       + b[2] * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);
  /**/
  c[2] = b[0] * (a[1][0] * a[2][1] - a[1][1] * a[2][0])
       + b[1] * (a[0][1] * a[2][0] - a[0][0] * a[2][1])
       + b[2] * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
  /**/
  for (i = 0; i<3; ++i) b[i] = c[i] / det;
  return det;
}/*let solve3*/
/**
 @brief Simple sort
*/
function eigsort(
  n = 0, //!< [in] the number of components
  key = [0.0], //!< [in] Variables to be sorted [n].
  swap = [0] //!< [out] Order of index (sorted)
)
{
  let i = 0, j = 0, k = 0;

  for (i = 0; i < n; ++i) swap[i] = i;

  for (i = 0; i < n - 1; ++i) {
    for (j = i + 1; j < n; ++j) {
      if (key[swap[j]] < key[swap[i]]) {
        /*
         Swap
        */
        k = swap[j];
        swap[j] = swap[i];
        swap[i] = k;
      }/*if (sortee[j][0] < sortee[i][0])*/
    }/*for (j = i + 1; j < n; ++j)*/
  }/*for (i = 0; i < n - 1; ++i)*/
}/*eigsort*/
/**
 @brief Calculate normal vector from corners of triangle
*/
function normal_vec(
  in1 = [0.0, 0.0, 0.0], //!< [in] Corner 1
  in2 = [0.0, 0.0, 0.0], //!< [in] Corner 2
  in3 = [0.0, 0.0, 0.0], //!< [in] Corner 3
  out = [0.0, 0.0, 0.0] //!< [out] The normal vector
)
{
  let i = 0;
  let norm = 0.0;
  out[0] = in1[1] * in2[2] - in1[2] * in2[1]
         + in2[1] * in3[2] - in2[2] * in3[1]
         + in3[1] * in1[2] - in3[2] * in1[1];
  out[1] = in1[2] * in2[0] - in1[0] * in2[2]
         + in2[2] * in3[0] - in2[0] * in3[2]
         + in3[2] * in1[0] - in3[0] * in1[2];
  out[2] = in1[0] * in2[1] - in1[1] * in2[0]
         + in2[0] * in3[1] - in2[1] * in3[0]
         + in3[0] * in1[1] - in3[1] * in1[0];
  norm = Math.sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
  for (i = 0; i<3; i++) out[i] = out[i] / norm;
} /* normal_vec */
/**
 @brief Judge wheser this line is the edge of 1st BZ
*/
function bragg_vert(
  bragg = [[0.0]],
  brnrm = [0.0],
  ibr = 0, //!< [in] Index of a Bragg plane
  jbr = 0, //!< [in] Index of a Bragg plane
  nbr = 0, //!< [in]
  vert = [0.0], //!< [in] start point of line
  vert2 = [0.0]//!< [in] end point of line
)
{
  let kbr = 0, i = 0, lbr = 0, nbr0 = 0;
  let bmat = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], rhs = [0.0, 0.0, 0.0], prod, thr, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < 26; ++kbr) {
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = bragg[ibr][i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = brnrm[ibr];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = Math.sqrt(rhs[0] * rhs[1] * rhs[2]) * 0.001;
    /*
     if Bragg planes do not cross, roop next kbr
    */
    det = solve3(bmat, rhs);
    if (Math.abs(det) < thr) continue;
    /*
     if vert0 = vert1, roop next kbr
    */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
         + (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
         + (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if (prod < thr) continue;
    /*
     is corner really in 1st BZ ?
    */
    i = 0;
    for (lbr = 0; lbr < 26; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }
  /*
   this line is not a BZ boundary
  */
  return 0;
}/* bragg_vert */
/**
 @brief Compute real number of Bragg plane at 1st BZ
*/
function check_bragg()
{
  let ibr = 0, ibzl = 0, ibzc = 0;
  let ii = 0, kk = 0, bzflag = 0, nbzcorner = 0, nn = 0;
  let thr = 0.0001, prod = 0.0, bzc = []; //[676][3];
  /*
  First, compute real number of corners of 1st BZ
  */
  nbzcorner = 0;
  for (ibzl = 0; ibzl < nbzl; ibzl++) {
    for (ii = 0; ii < 2; ii++) {
      bzflag = 0;
      
      for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
        prod = 0.0;
        for (kk = 0; kk < 3; kk++) prod += (bzl[ibzl][ii][kk] - bzc[ibzc][kk]) * (bzl[ibzl][ii][kk] - bzc[ibzc][kk]);
        if (prod < thr) bzflag = 1;
      }

      if (bzflag == 0) {
        bzc.push([0.0, 0.0, 0.0]);
        for (kk = 0; kk < 3; kk++) bzc[nbzcorner][kk] = bzl[ibzl][ii][kk];
        nbzcorner += 1;
      }

    }/*for (ii = 0; ii < 2; ii++)*/
  }/*for (ibzl = 0; ibzl < nbzl; ibzl++)*/
  terminal("    Number of corners of 1st BZ : " + toString(nbzcorner) + "\n");
  /**@brief
  Then, compute real number Bragg plane of 1st BZ (::nbragg), 
  Re-order ::bragg and ::brnrm
  */
  nbragg = 0;
  for (ibr = 0; ibr < 26; ibr++) {
    nn = 0;

    for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
      prod = bragg[ibr][0] * bzc[ibzc][0] + bragg[ibr][1] * bzc[ibzc][1] + bragg[ibr][2] * bzc[ibzc][2];
      if (Math.abs(prod - brnrm[ibr]) < thr) nn += 1;
    }
    if (nn >= 3) {
      for (kk = 0; kk < 3; kk++) bragg[nbragg][kk] = bragg[ibr][kk];
      brnrm[nbragg] = brnrm[ibr];
      nbragg += 1;
    }
  }
  terminal("    Number of plane of 1st BZ : " + toString(nbragg) + "\n");
}/*function check_bragg*/
/**
 @brief Compute Brillouin zone boundariy lines

 Modify : ::nbzl, ::bzl
*/
function bz_lines()
{
  /**/
  let ibr = 0, jbr = 0, nbr = 0, i = 0, j = 0, lvert = 0;
  let vert = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];
  /**/
  nbzl = 0;

  /**/
  for (ibr = 0; ibr < 26; ++ibr) {
    for (jbr = 0; jbr < 26; ++jbr) {
      /**/
      for (i = 0; i<3; ++i) vert[1][i] = 0.0;
      nbr = 0;
      lvert = bragg_vert(bragg, brnrm, ibr, jbr, nbr, vert[0], vert[1]);
      if (lvert == 0) continue;
      nbr = lvert;
      /**/
      lvert = bragg_vert(bragg, brnrm, ibr, jbr, nbr, vert[1], vert[0]);
      if (lvert == 0) continue;
      /**/
      for (i = 0; i < 2; ++i) for (j = 0; j < 3; ++j) bzl[nbzl][i][j] = vert[i][j];
      nbzl = nbzl + 1;

    }/*for (jbr = 0; jbr < 26; ++jbr)*/
  }/*for (ibr = 0; ibr < 26; ++ibr)*/

  check_bragg();

}/*bz_lines*/
/**
@brief Compute node-line where \f$\Delta_{n k} = 0\f$

 Modify : ::nnl, ::kvnl, ::kvnl_rot

*/
function calc_nodeline()
{
  let ib = 0, itri = 0, i = 0, j = 0;
  let sw = [0, 0, 0];
  let a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], matps = [0.0, 0.0, 0.0];
  let kvnl0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; // std::vector<std::vector<let> > 

  terminal("    band   # of nodeline\n");
  for (ib = 0; ib < nb; ib++) {

    kvnl.push([]);

    for (itri = 0; itri < ntri[ib]; ++itri) {


      for (i = 0; i < 3; ++i) matps[i] = matp[ib][itri][i][0];
      eigsort(3, matps, sw);

      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (0.0 - matp[ib][itri][sw[j]][0]) 
            / (matp[ib][itri][sw[i]][0] - matp[ib][itri][sw[j]][0]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/

      if ((matp[ib][itri][sw[0]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[1]][0])
        || (matp[ib][itri][sw[0]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[1]][0])) {
        for (i = 0; i < 3; ++i) {
          kvnl0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
          kvnl0[1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        kvnl[ib].push(kvnl0);
      }/*else if (matp[ib][itri][sw[0]] < 0.0 && 0.0 <= matp[ib][itri][sw[1]])*/
      else if ((matp[ib][itri][sw[1]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[2]][0])
        || (matp[ib][itri][sw[1]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[2]][0])) {
        for (i = 0; i < 3; ++i) {
          kvnl0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          kvnl0[1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
        }/*for (i = 0; i < 3; ++i)*/
        kvnl[ib].push(kvnl0);
      }/*else if (matp[ib][itri][sw[1]] < 0.0 && 0.0 <= matp[ib][itri][sw[2]])*/
    }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    /*
     Allocation of nodeline
     */
    nnl[ib] = kvnl[ib].size;
    terminal("    " + toString(ib + 1) + "       " + toString(nnl[ib]) + "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function calc_nodeline()*/
/**
 @brief Draw Fermi surfaces

 Modify : ::nmlp_rot, ::kvp_rot, ::kvnl_rot

 First, rotate ::nmlp and ::kvp with OpenMP then draw them.
 Also draw nodeline in the same way.
*/
function draw_fermi() {
  let ib = 0, a0 = 0, a1 = 0, a2 = 0, ia = 0;
  let kshift = [0.0, 0.0, 0.0];
  let i = 0, j = 0, l = 0, itri = 0;
  /*
   First, rotate k-vector and normal vector
  */
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;

        for (ib = 0; ib < nb; ib++) {
          if (draw_band[ib] == 1) {
            for (itri = 0; itri < ntri[ib]; ++itri) {
              for (i = 0; i < 3; ++i) {
                for (j = 0; j < 3; ++j) {
                  kvp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * (kvp[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvp[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvp[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  nmlp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * nmlp[ib][itri][i][0]
                      + rot[j][1] * nmlp[ib][itri][i][1]
                      + rot[j][2] * nmlp[ib][itri][i][2];
                  nmlp_rot[ib][j + 3 * i + 9 * itri] *= side;
                  for (l = 0; l < 2; ++l) {
                      arw_rot[ib][j + 3 * l + 6 * i + 18 * itri]
                        = rot[j][0] * (arw[ib][itri][i][l][0] + kshift[0])
                        + rot[j][1] * (arw[ib][itri][i][l][1] + kshift[1])
                        + rot[j][2] * (arw[ib][itri][i][l][2] + kshift[2])
                        + trans[j];
                  }
                }
              }/*for (i = 0; i < 3; ++i)*/
            }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
          }/*if (draw_band[ib] == 1)*/
        }/*for (ib = 0; ib < nb; ib++)*/
        /*
          Second, draw each triangle
        */
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        for (ib = 0; ib < nb; ib++) {
          if (draw_band[ib] == 1) {
            glVertexPointer(3, GL_FLOAT, 0, kvp_rot[ib]);
            glNormalPointer(GL_FLOAT, 0, nmlp_rot[ib]);
            glColorPointer(4, GL_FLOAT, 0, clr[ib]);
            glDrawArrays(GL_TRIANGLES, 0, ntri[ib] * 3);
          }/*if (draw_band[ib] == 1)*/
        }/*for (ib = 0; ib < nb; ib++)*/
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        /*
        Arrow for 3D value
        */
        glNormal3f(0.0, 0.0, 1.0);
        if (color_scale == 3) {
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glColor3f(1.0 - clr[ib][0], 1.0 - clr[ib][1], 1.0 - clr[ib][2]);
              glVertexPointer(3, GL_FLOAT, 0, arw_rot[ib]);
              glDrawArrays(GL_LINES, 0, ntri[ib] * 3 * 2);
            }
          }
        }
        /*
         Nodeline
        */
        if (nodeline == 1) {
          /*
           First, rotate k-vector
          */
          let i, j, itri;
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              for (itri = 0; itri < nnl[ib]; ++itri) {
                for (i = 0; i < 2; ++i) {
                  for (j = 0; j < 3; ++j)
                    kvnl_rot[ib][j + 3 * i + 6 * itri]
                      = rot[j][0] * (kvnl[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvnl[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvnl[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  kvnl_rot[ib][2 + 3 * i + 6 * itri] += 0.001;
                }/*for (i = 0; i < 2; ++i)*/
              }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
            }/*if (draw_band[ib] == 1)*/
          }/*for (ib = 0; ib < nb; ib++)*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0, 0.0, 1.0);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kvnl_rot[ib]);
              glDrawArrays(GL_LINES, 0, 2 * nnl[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
        /*
        Equator
        */
        if (lequator == 1) {
          /*
          First, rotate k-vector
          */
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              for (itri = 0; itri < nequator[ib]; ++itri) {
                for (i = 0; i < 2; ++i) {
                  for (j = 0; j < 3; ++j)
                    kveq_rot[ib][j + 3 * i + 6 * itri]
                      = rot[j][0] * (kveq[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kveq[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kveq[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  kveq_rot[ib][2 + 3 * i + 6 * itri] += 0.001;
                }/*for (i = 0; i < 2; ++i)*/
              }/*for (itri = 0; itri < nequator[ib]; ++itri)*/
            }/*if (draw_band[ib] == 1)*/
          }/*for (ib = 0; ib < nb; ib++)*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0, 0.0, 1.0);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kveq_rot[ib]);
              glDrawArrays(GL_LINES, 0, 2 * nequator[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
      }
    }
  }
}/*function draw_fermi*/
/**
 @brief Draw lines of BZ boundaries
*/
function draw_bz_lines() {
  let ibzl = 0, i = 0, j = 0, a0 = 0, a1 = 0, a2 = 0, ia = 0;
  let bzl2 = [0.0, 0.0, 0.0], bvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    linecolor = [0.0, 0.0, 0.0, 0.0], secvec2 = [0.0, 0.0, 0.0], kshift = [0.0, 0.0, 0.0];
  let vertices = [];
  /*
   Line color is oposit of BG color
  */
  for (i = 0; i < 4; i++) linecolor[i] = LineColor[i];
  /**/
  glLineWidth(linewidth);
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;
        /*
        First Brillouin zone mode
        */
        if (fbz == 1) {
          for (ibzl = 0; ibzl < nbzl; ++ibzl) {
            for (i = 0; i < 2; ++i) {
              for (j = 0; j < 3; ++j)
                bzl2[j] = rot[j][0] * (bzl[ibzl][i][0] + kshift[0])
                        + rot[j][1] * (bzl[ibzl][i][1] + kshift[1])
                        + rot[j][2] * (bzl[ibzl][i][2] + kshift[2])
                        + trans[j];
              for (j = 0; j < 3; j++) vertices.push(bzl2[j]);
            }/*for (i = 0; i< 2; ++i)*/
            glColor3fv(linecolor);
            glNormal3f(0.0, 0.0, 1.0);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_LINES, 0, 2);
          }/*for (ibzl = 0; ibzl < nbzl; ++ibzl)*/
        }/*if (fbz == 1)*/
        else {
          /*
           Premitive BZ mode
          */
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
              bvec2[i][j] = rot[j][0] * bvec[i][0]
                          + rot[j][1] * bvec[i][1]
                          + rot[j][2] * bvec[i][2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i]);
          for (i = 0; i < 17; ++i) {
            for (j = 0; j < 3; ++j) {
              vertices[j + 3 * i] = vertices[j + 3 * i]
                + rot[j][0] * kshift[0]
                + rot[j][1] * kshift[1]
                + rot[j][2] * kshift[2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          glColor3fv(linecolor);
          glNormal3f(0.0, 0.0, 1.0);
          glVertexPointer(3, GL_FLOAT, 0, vertices);
          glDrawArrays(GL_LINE_STRIP, 0, 17);
        }/*if (fbz != 1)*/
      }
    }
  }
  /*
  Section for the 2D Fermi line
  */
  if (lsection == 1 && fbz == 1) {
    for (j = 0; j < 3; ++j)
      secvec2[j] = rot[j][0] * secvec[0]
                 + rot[j][1] * secvec[1]
                 + rot[j][2] * secvec[2];
    for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
      for (j = 0; j < 3; ++j)
        bzl2[j] = rot[j][0] * bzl2d[ibzl][0]
                + rot[j][1] * bzl2d[ibzl][1]
                + rot[j][2] * bzl2d[ibzl][2]
                + trans[j];
      for (j = 0; j < 3; j++)vertices[j + 3 * ibzl] = bzl2[j];
    }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
    glColor3fv(gray);
    glNormal3fv(secvec2);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nbzl2d);
  }/*if (lsection == 1)*/
}/*draw bz_lines */
/**
 @brief Draw color-bar or colr-circle (periodic) 
        as a color scale
*/
function draw_colorbar()
{
  let i = 0, j = 0, k = 0;
  let mat2 = 0.0, vertices = [], colors = [], vector = [], vector_color = [],
    norm = 0.0;

  glEnableClientState(GL_COLOR_ARRAY);
  /*
  Reciplocal lattice vectors
  */
  for (i = 0; i < 6; i++) {
    vector[3 * i] = -1.2;
    vector[3 * i + 1] = -1.05;
    vector[3 * i + 2] = 0.0;
  }
  for (i = 0; i < 3; i++) {
    norm = Math.sqrt(bvec[i][0] * bvec[i][0]+ bvec[i][1] * bvec[i][1]+ bvec[i][2] * bvec[i][2]);
    for (j = 0; j < 3; j++) {
      vector[j + 6 * i] 
        += (rot[j][0] * bvec[i][0]
        + rot[j][1] * bvec[i][1]
        + rot[j][2] * bvec[i][2]) * 0.2 / norm;
    }
    for (j = 0; j < 4; j++) {
      vector_color[j + 8 * i] = BarColor[i * 2][j];
      vector_color[j + 8 * i + 4] = BarColor[i * 2][j];
    }
  }
  glNormal3f(0.0, 0.0, 1.0);
  glVertexPointer(3, GL_FLOAT, 0, vector);
  glColorPointer(4, GL_FLOAT, 0, vector_color);
  glDrawArrays(GL_LINES, 0, 6);
  /*
   Color bar/circle/cube
  */
  if (color_scale == 1 || color_scale == 4) {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0 + 0.5*i;
        vertices[1 + j * 3 + i * 6] = -1.0 - 0.1*j;
        vertices[2 + j * 3 + i * 6] = 0.0;
        for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = BarColor[i][k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
  }/*if (color_scale == 1 || color_scale == 4)*/
  else if (color_scale == 2) {
    /*
     Periodic color scale
    */
    vertices[0] = 0.0;
    vertices[1] = -1.0;
    vertices[2] = 0.0;
    for (j = 0; j < 4; j++) colors[j] = 1.0 - BackGroundColor[j];
    /**/
    for (i = 0; i <= 60; i++) {
      /**/
      mat2 = i / 60.0 * 6.0;
      /**/
      if (mat2 <= 1.0) {
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
      }
      else if (mat2 <= 4.0) {
        mat2 = mat2 - 3.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
      }
      else if (mat2 <= 5.0) {
        mat2 = mat2 - 4.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
      }
      else {
        mat2 = mat2 - 5.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
      }
      /**/
      vertices[0 + 3 * (i + 1)] = 0.2 * Math.cos(i / 60.0 * 6.283185307);
      vertices[1 + 3 * (i + 1)] = 0.2 * Math.sin(i / 60.0 * 6.283185307) - 1.0;
      vertices[2 + 3 * (i + 1)] = 0.0;
    }/*for (i = 0; i <= 60; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }/*else if (color_scale == 2)*/
  else  if (color_scale == 6 || color_scale == 7) {
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0 + 2.0*i;
        vertices[1 + j * 3 + i * 6] = -1.0 - 0.1*j;
        vertices[2 + j * 3 + i * 6] = 0.0;
        if (i == 0) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = bgray[k];
        else if (i == 1) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = wgray[k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }/*if (color_scale == 6 || color_scale == 7)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*function draw_colorbar*/
/**
 @brief Draw eye-points for the stereogram
*/
function draw_circles(
  dx2d = 0.0 //!< [in] Translation used for the section-mode
) {
  let i = 0, j = 0;
  let r = 0.0, vertices = [];
  /**/
  r = 0.05;
  /**/
  vertices[0] = 0.7 - dx2d;
  vertices[1] = scl;
  vertices[2] = 0.0;
  for (i = 0; i <= 20; i++) {
    vertices[0 + (i + 1) * 3] = r * Math.cos(i / 20.0 * 6.283185307) + 0.7 - dx2d;
    vertices[1 + (i + 1) * 3] = r * Math.sin(i / 20.0 * 6.283185307) + scl;
    vertices[2 + (i + 1) * 3] = 0.0;
  }/*for (i = 0; i <= 20; i++)*/
  glNormal3f(0.0, 0.0, 1.0);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
  /**/
  for (i = 0; i < 22; i++) vertices[3 * i] += -1.4;
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}/*function draw_circles*/
/**
 @brief Draw 2D Fermi lines
*/
function draw_fermi_line() {
  let i = 0, ib = 0, ibzl = 0;
  let vertices = [];
  /*
   Draw 2D BZ lines
  */
  glLineWidth(linewidth);
  for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
    for (i = 0; i < 3; i++) vertices[i + 3 * ibzl] = bzl2d_proj[ibzl][i];
  }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
  glNormal3f(0.0, 0.0, 1.0);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_LINE_LOOP, 0, nbzl2d);
  /*
   Draw Fermi lines
  */
  glLineWidth(linewidth);
  glEnableClientState(GL_COLOR_ARRAY);
  glNormal3f(0.0, 0.0, 1.0);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glVertexPointer(3, GL_FLOAT, 0, kv2d[ib]);
      glColorPointer(4, GL_FLOAT, 0, clr2d[ib]);
      glDrawArrays(GL_LINES, 0, 2 * n2d[ib]);
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*function draw_fermi_line*/
/**
 @brief Glut Display function
 called by glutDisplayFunc
*/
function draw()
{
  let pos = [1.0, 1.0, 1.0, 0.0];
  let amb = [0.2, 0.2, 0.2, 0.0];
  let dx = 0.0, dx2d = 0.0, theta = 0.0, posz = 0.0, phi = 0.0;
  let pos1 = [0.0, 0.0, 0.0, 0.0], pos2 = [0.0, 0.0, 0.0, 0.0];

  if (draw_band == NULL) return;

  if (lstereo == 2) {
    /*
     Parallel eyes
    */
    theta = 3.14159265359 / 180.0 * 2.5;
    posz = 5.0;
    dx = 0.7;
    phi = Math.atan(posz / dx) - theta;
    theta = (3.1416 * 0.5 - phi) / 3.1416 * 180.0;
    /**/
    pos1[0] = posz * Math.cos(phi) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * Math.sin(phi);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * Math.cos(phi) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * Math.sin(phi);
    pos2[3] = 1.0;
  }/*if (lstereo == 2)*/
  else if (lstereo == 3) {
    /*
     Cross eyes
    */
    theta = -3.1416 / 180.0 * 2.0;
    posz = 5.0;
    dx = -0.7;
    /**/
    pos1[0] = posz * Math.sin(theta) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * Math.cos(theta);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * Math.sin(theta) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * Math.cos(theta);
    pos2[3] = 1.0;
  }/*if (lstereo == 3)*/
  else {
    theta = 0.0;
    dx = 0.0;
  }/*lstero == 1*/
  if (lsection == 1 && fbz == 1) dx2d = 0.7;
  else dx2d = 0.0;
  /*
   Initialize
  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /*
   Set view point & view line
  */
  glTranslatef(0.0, 0.0, -5.0);
  //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /*
   Set position of light
  */
  if (lstereo == 1) {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glTranslatef(-dx2d, 0.0, 0.0);
    /*
     Draw color scale
    */
    if (lcolorbar == 1) draw_colorbar();
  }
  else {
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    draw_circles(dx2d);
    glTranslatef(-dx-dx2d, 0.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);
  }
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  /*
   Rotation & Zoom
  */
  glScalef(scl, scl, scl);
  /*
   Draw Brillouin zone boundaries
  */
  draw_bz_lines();
  /*
   Draw Fermi surfaces
  */
  draw_fermi();
  /*
   Draw the second object for stereogram
  */
  if (lstereo != 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos2);
    /**/
    glTranslatef(dx-dx2d, 0.0, 0.0);
    glRotatef(-theta, 0.0, 1.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_bz_lines();
    draw_fermi();
    /**/
    glPopMatrix();
  }
  /*
   Draw 2D Fermi line
  */
  if (lsection == 1 && fbz == 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    /**/
    if (lstereo == 1) glTranslatef(dx2d, 0.0, 0.0);
    else glTranslatef(2.0 * dx2d, 0.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_fermi_line();
    /**/
    glPopMatrix();
  }/*if (lsection == 1)*/
  glFlush(); // Not really necessary: buffer swapping below implies glFlush()
  SwapBuffers();
}/*function display*/
/**
@brief Compute equator \f$\{\bf v}_{n k} \cdot {\bf k} = 0\f$

 Modify : ::nequator, ::kveq, ::kveq_rot
*/
function equator() {
  let ib = 0, itri = 0, i = 0, j = 0, nequator0 = 0;
  let sw = [0,0,0];
  let a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], prod = [0.0, 0.0, 0.0];
  let kveq_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  terminal("    band   # of equator\n");
  for (ib = 0; ib < nb; ib++) {
    kveq.push([]);

    for (itri = 0; itri < ntri[ib]; ++itri) {
      for (i = 0; i < 3; ++i) {
        prod[i] = 0.0;
        for (j = 0; j < 3; ++j) prod[i] += eqvec[j] * nmlp[ib][itri][i][j];
      }/*for (i = 0; i < 3; ++i)*/

      eigsort(3, prod, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.0 - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if ((prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])
          || (prod[sw[0]] <= 0.0 && 0.0 < prod[sw[1]])) {
          for (i = 0; i < 3; ++i) {
            kveq_0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
            kveq_0[1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          }/*for (i = 0; i < 3; ++i)*/
          kveq[ib].push(kveq_0);
        }/*else if (prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])*/
        else if ((prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])
          || (prod[sw[1]] <= 0.0 && 0.0 < prod[sw[2]])) {
           for (i = 0; i < 3; ++i) {
             kveq_0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
             kveq_0[1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
           }/*for (i = 0; i < 3; ++i)*/
          kveq[ib].push(kveq_0);
        }/*else if (prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    /*
     Sum node-lines in all threads
    */
    nequator.push(kveq[ib].size);
    terminal("    " + toString(ib + 1) + "       " + toString(nequator[ib])+ "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function equator()*/
/**
 @brief Store triangle patch

 Modify : ::matp, ::kvp

 For the 1st BZ  mode, this routine cuts triangle recursivly at the
 BZ boundary (Bragg plane).

 - DO @f${\bf l}@f$ in Bragg vector
   - @f${p_i = {\bf l}\cdot{\bf k}}@f$
   - Sort : @f$p_0<p_1<p_2@f$
   - @f[
       a_{i j} \equiv \frac{-p_j}{p_i - p_j}
     @f]
   - if (@f$|{\bf l}| < p_0@f$)
     - This patch is not in the 1st BZ
   - if (@f$p_0 < |{\bf l}| < p_1@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_1 < |{\bf l}| < p_2@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
     - and
     - @f${\bf k}'_0 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_2 < |{\bf l}| < p_3@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_2@f$
 - END DO
*/
function triangle(
  nbr = 1, //!<[in] Bragg plane
  mat1 = [[0.0,0.0,0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] The matrix element
  kvec1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] @f$k@f$-vector of corners
  vf1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] @f$v_f@f$-vector of corners
  kvp_v =[],
  matp_v = [],
  nmlp_v = []
  )
{
  let ibr = 0, i = 0, j = 0, sw = [0, 0, 0];
  let prod = [0.0, 0.0, 0.0], thr = 0.0, thr2 = 0.001,
    mat2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    kvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    bshift = 0.0,
    vfave = [0.0, 0.0, 0.0], norm = [0.0, 0.0, 0.0],
    kvp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], 
    matp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], 
    nmlp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  /*
   If the area is nearly 0, it is ignored.
  */
  for (i = 0; i < 3; i++)norm[i] = 0.0;
  for (i = 0; i < 3; i++) {
    norm[0] += (kvec1[1][i] - kvec1[2][i])*(kvec1[1][i] - kvec1[2][i]);
    norm[1] += (kvec1[2][i] - kvec1[0][i])*(kvec1[2][i] - kvec1[0][i]);
    norm[2] += (kvec1[0][i] - kvec1[1][i])*(kvec1[0][i] - kvec1[1][i]);
  }
  for (i = 0; i < 3; i++) {
    if (norm[i] < 1.0e-10*brnrm_min) return;
  }
  /*
   For 1st BZ, it is cut at the BZ boundary.
  */
  if (fbz == 1) {
    /**/
    for (ibr = 0; ibr < nbragg; ++ibr) {

      thr = brnrm[ibr] * 0.001;
      /**/
      for (i = 0; i < 3; ++i) 
        prod[i] = bragg[ibr][0] * kvec1[i][0]
                + bragg[ibr][1] * kvec1[i][1]
                + bragg[ibr][2] * kvec1[i][2];
      eigsort(3, prod, sw);
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (brnrm[ibr] - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/
      i = (0.5 * ((prod[sw[2]] / brnrm[ibr]) + 1.0));
      bshift = -2.0 *i;

      if (brnrm[ibr] + thr > prod[sw[2]]) continue;

      if (brnrm[ibr] < prod[sw[0]]) {
        /*
         All corners are outside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            kvec2[i][j] = kvec1[sw[i]][j] + bshift * bragg[ibr][j];
            mat2[i][j] = mat1[sw[i]][j];
            vf2[i][j] = vf1[sw[i]][j];
          }
        }
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else if (brnrm[ibr] < prod[sw[1]]) {
        /*
         Single corner (#0) is inside of the Bragg plane
        */
       for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          
          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[2]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[2]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[2]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else if (brnrm[ibr] < prod[sw[2]]) {
        /*
        Two corners (#0, #1) are inside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
          kvec2[1][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[2][i] = kvec1[sw[2]][i];

          mat2[0][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          mat2[1][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[2][i] = mat1[sw[2]][i];

          vf2[0][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
          vf2[1][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[2][i] = vf1[sw[2]][i];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else {
        /*
        All corners are inside of the Bragg plane
        */
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < nbragg*/
  } /* if fbz == 1 */
  /*
   Store patch
  */
  normal_vec(kvec1[0], kvec1[1], kvec1[2], norm);
  for (i = 0; i < 3; ++i) {
    vfave[i] = 0.0;
    for (j = 0; j < 3; ++j) vfave[i] += vf1[j][i];
  }
  prod[0] = 0.0;
  for (i = 0; i < 3; ++i) prod[0] += vfave[i] * norm[i];

  if (prod[0] < 0.0) {
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        kvp_0[i][j] = kvec1[2 - i][j];
        matp_0[i][j] = mat1[2 - i][j];
        nmlp_0[i][j] = vf1[2 - i][j];
      }
    }/*for (i = 0; i < 3; ++i)*/
  }
  else {
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        kvp_0[i][j] = kvec1[i][j];
        matp_0[i][j] = mat1[i][j];
        nmlp_0[i][j] = vf1[i][j];
      }
    }/*for (i = 0; i < 3; ++i)*/
  }
  kvp_v.push(kvp_0);
  matp_v.push(matp_0);
  nmlp_v.push(nmlp_0);
}/* triangle */
/**
@brief Cut triangle patch with the tetrahedron method.

 - Sort : @f$\varepsilon_0<\varepsilon_1<\varepsilon_2<\varepsilon_3@f$
 - @f[
     a_{i j} \equiv \frac{-\varepsilon_j}{\varepsilon_i - \varepsilon_j}
   @f]
 - if (@f$\varepsilon_0 < 0 < \varepsilon_1@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_2 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
 - if (@f$\varepsilon_1 < 0 < \varepsilon_2@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
   - and
   - @f${\bf k}'_0 = {\bf k}_1 a_{1 3} + {\bf k}_3 a_{3 1}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
 - if (@f$\varepsilon_2 < 0 < \varepsilon_3@f$)
   - @f${\bf k}'_0 = {\bf k}_3 a_{3 0} + {\bf k}_0 a_{0 3}@f$
   - @f${\bf k}'_1 = {\bf k}_3 a_{3 1} + {\bf k}_1 a_{1 3}@f$
   - @f${\bf k}'_2 = {\bf k}_3 a_{3 2} + {\bf k}_2 a_{2 3}@f$
*/
function tetrahedron(
  eig1 = [], //!< [in] Orbital energies @f$\varepsilon_{n k}@f$
  mat1 = [], //!< [in] Matrix elements @f$\Delta_{n k}@f$
  kvec1 = [], //!< [in] @f$k@f$-vectors
  vf1 = [], //!< [in] @f$v_f@f$-vectors
  kvp_v = [],
  matp_v = [],
  nmlp_v = []
)
{
  let it = 0, i = 0, j = 0, sw = [0, 0, 0, 0];
  let eig2 = [0.0, 0.0, 0.0, 0.0],
    mat2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    kvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    a = [[0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0]],
    kvec3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    mat3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vol = 0.0, thr = 0.000;

  for (it = 0; it < 6; ++it) {
    /*
     Define corners of the tetrahedron
    */
    for (i = 0; i < 4; ++i) {
      eig2[i] = eig1[corner[it][i]];
      for (j = 0; j < 3; ++j) {
        mat2[i][j] = mat1[corner[it][i]][j];
        vf2[i][j] = vf1[corner[it][i]][j];
      }
      /*
       Fractional -> Cartecian
      */
      for (j = 0; j < 3; ++j) 
        kvec2[i][j] = bvec[0][j] * kvec1[corner[it][i]][0]
                    + bvec[1][j] * kvec1[corner[it][i]][1]
                    + bvec[2][j] * kvec1[corner[it][i]][2];
    }/*for (i = 0; i < 4; ++i)*/
    eigsort(4, eig2, sw);

    for (i = 0; i < 4; ++i) {
      for (j = 0; j < 4; ++j) {
        a[i][j] = (0.0 - eig2[sw[j]]) / (eig2[sw[i]] - eig2[sw[j]]);
      }/*for (j = 0; j < 4; ++j)*/
    }/*for (i = 0; i < 4; ++i)*/
    /*
     Draw triangle in each cases
    */
    if (eig2[sw[0]] <= 0.0 && 0.0 < eig2[sw[1]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][1] + kvec2[sw[1]][i] * a[1][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[2][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];

        mat3[0][i] = mat2[sw[0]][i] * a[0][1] + mat2[sw[1]][i] * a[1][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[2][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];

        vf3[0][i] = vf2[sw[0]][i] * a[0][1] + vf2[sw[1]][i] * a[1][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[2][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
      }
      
      vol = a[1][0] * a[2][0] * a[3][0];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else if (eig2[sw[1]] <= 0.0 && 0.0 < eig2[sw[2]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }
      
      vol = a[1][2] * a[2][0] * a[3][0];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
      /**/
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[1]][i] * a[1][3] + kvec2[sw[3]][i] * a[3][1];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[1]][i] * a[1][3] + mat2[sw[3]][i] * a[3][1];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[1]][i] * a[1][3] + vf2[sw[3]][i] * a[3][1];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }

      vol = a[1][3] * a[3][0] * a[2][1];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else if (eig2[sw[2]] <= 0.0 && 0.0 < eig2[sw[3]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[3]][i] * a[3][0] + kvec2[sw[0]][i] * a[0][3];
        kvec3[1][i] = kvec2[sw[3]][i] * a[3][1] + kvec2[sw[1]][i] * a[1][3];
        kvec3[2][i] = kvec2[sw[3]][i] * a[3][2] + kvec2[sw[2]][i] * a[2][3];

        mat3[0][i] = mat2[sw[3]][i] * a[3][0] + mat2[sw[0]][i] * a[0][3];
        mat3[1][i] = mat2[sw[3]][i] * a[3][1] + mat2[sw[1]][i] * a[1][3];
        mat3[2][i] = mat2[sw[3]][i] * a[3][2] + mat2[sw[2]][i] * a[2][3];

        vf3[0][i] = vf2[sw[3]][i] * a[3][0] + vf2[sw[0]][i] * a[0][3];
        vf3[1][i] = vf2[sw[3]][i] * a[3][1] + vf2[sw[1]][i] * a[1][3];
        vf3[2][i] = vf2[sw[3]][i] * a[3][2] + vf2[sw[2]][i] * a[2][3];
      }

      vol = a[0][3] * a[1][3] * a[2][3];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else {
    }
  }/*for (it = 0; it < 6; ++it)*/
}/* tetrahedron */
/**
 @brief Compute patches for Fermi surfaces

 Modify : ::ntri, nmlp, ::matp, ::kvp, ::clr, ::nmlp_rot, ::kvp_rot
*/
function fermi_patch()
{
  let ntri0 = 0, ib = 0, i0 = 0, i1 = 0, j0 = 0, start = [0, 0, 0], last = [0, 0, 0];
  let i = 0, j = 0, i2 = 0, j1 = 0, j2 = 0, ii0 = 0, ii1 = 0, ii2 = 0;
  let kvec1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    mat1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    eig1 = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
    vf1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  if (fbz == 1) {
    terminal("\n");
    terminal("  ##  First Brillouin zone mode  #######\n");
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = ng[i0] / 2 - ng[i0];
      last[i0] = ng[i0] / 2;
    }
  }
  else {
    terminal("\n");
    terminal("  ##  Premitive Brillouin zone mode  #######\n");
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = 0;
      last[i0] = ng[i0];
    }
  }
  terminal("    Computing patch ...\n");
  terminal("      band   # of patchs\n");
  /**/
  for (ib = 0; ib < nb; ++ib) {
    for (j0 = start[0]; j0 < last[0]; ++j0) {
      for (j1 = start[1]; j1 < last[1]; ++j1) {
        for (j2 = start[2]; j2 < last[2]; ++j2) {

          i0 = j0;
          i1 = j1;
          i2 = j2;
          ii0 = j0 + 1;
          ii1 = j1 + 1;
          ii2 = j2 + 1;
          /**/
          kvec1[0][0] = i0 / ng[0];
          kvec1[1][0] = i0 / ng[0];
          kvec1[2][0] = i0 / ng[0];
          kvec1[3][0] = i0 / ng[0];
          kvec1[4][0] = ii0 / ng[0];
          kvec1[5][0] = ii0 / ng[0];
          kvec1[6][0] = ii0 / ng[0];
          kvec1[7][0] = ii0 / ng[0];
          /**/
          kvec1[0][1] = i1 / ng[1];
          kvec1[1][1] = i1 / ng[1];
          kvec1[2][1] = ii1 / ng[1];
          kvec1[3][1] = ii1 / ng[1];
          kvec1[4][1] = i1 / ng[1];
          kvec1[5][1] = i1 / ng[1];
          kvec1[6][1] = ii1 / ng[1];
          kvec1[7][1] = ii1 / ng[1];
          /**/
          kvec1[0][2] = i2 / ng[2];
          kvec1[1][2] = ii2 / ng[2];
          kvec1[2][2] = i2 / ng[2];
          kvec1[3][2] = ii2 / ng[2];
          kvec1[4][2] = i2 / ng[2];
          kvec1[5][2] = ii2 / ng[2];
          kvec1[6][2] = i2 / ng[2];
          kvec1[7][2] = ii2 / ng[2];
          /**/
          for (i = 0; i < 8; i++)
            for (j = 0; j < 3; j++)
              kvec1[i][j] = kvec1[i][j] + shiftk[j] / (2.0 * ng0[j]);
          /**/
          i0 = modulo(i0, ng[0]);
          i1 = modulo(i1, ng[1]);
          i2 = modulo(i2, ng[2]);
          ii0 = modulo(ii0, ng[0]);
          ii1 = modulo(ii1, ng[1]);
          ii2 = modulo(ii2, ng[2]);
          /**/
          eig1[0] = eig[ib][i0][i1][i2] - EF;
          eig1[1] = eig[ib][i0][i1][ii2] - EF;
          eig1[2] = eig[ib][i0][ii1][i2] - EF;
          eig1[3] = eig[ib][i0][ii1][ii2] - EF;
          eig1[4] = eig[ib][ii0][i1][i2] - EF;
          eig1[5] = eig[ib][ii0][i1][ii2] - EF;
          eig1[6] = eig[ib][ii0][ii1][i2] - EF;
          eig1[7] = eig[ib][ii0][ii1][ii2] - EF;
          /**/
          for (j = 0; j < 3; j++) {
            mat1[0][j] = mat[ib][i0][i1][i2][j];
            mat1[1][j] = mat[ib][i0][i1][ii2][j];
            mat1[2][j] = mat[ib][i0][ii1][i2][j];
            mat1[3][j] = mat[ib][i0][ii1][ii2][j];
            mat1[4][j] = mat[ib][ii0][i1][i2][j];
            mat1[5][j] = mat[ib][ii0][i1][ii2][j];
            mat1[6][j] = mat[ib][ii0][ii1][i2][j];
            mat1[7][j] = mat[ib][ii0][ii1][ii2][j];
            /**/
            vf1[0][j] = vf[ib][i0][i1][i2][j];
            vf1[1][j] = vf[ib][i0][i1][ii2][j];
            vf1[2][j] = vf[ib][i0][ii1][i2][j];
            vf1[3][j] = vf[ib][i0][ii1][ii2][j];
            vf1[4][j] = vf[ib][ii0][i1][i2][j];
            vf1[5][j] = vf[ib][ii0][i1][ii2][j];
            vf1[6][j] = vf[ib][ii0][ii1][i2][j];
            vf1[7][j] = vf[ib][ii0][ii1][ii2][j];
          }/*for (j = 0; j < 3; j++)*/
          /**/
          tetrahedron(eig1, mat1, kvec1, vf1, 
            kvp[ib], matp[ib], nmlp[ib]);
        }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
      }/*for (j1 = start[1]; j1 < ng[1]; ++j1)*/
    }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
  }/*for (ib = 0; ib < nb; ++ib)*/
  terminal("    ... Done\n");
} /* fermi_patch */
/**@file
 @brief Main routine
*/
/**@mainpage FermiSurfer Main Page

Fermisurfer displays Fermi surfaces 
with a color-plot of the arbitraly matrix element

@section Notation
 
- @f$\varepsilon_{n k}@f$ : Energy
- @f$\Delta_{n k}@f$ : Any @f$(n, k)@f$-dependent value for the color-plot.
- @f$N_b@f$ : Number of bands
 
@section sec_routine Important routines
- Main routine: main()
- Right click menu : FS_CreateMenu(), FS_ModifyMenu(let status)

@section sec_file Important files
- Main routine : fermisurfer.cpp
- Global valiable : variable.hpp

@section sec_flow Flow

- main() :
  - read_file() : Read .frmsf file
  - compute_patch_segment() : Compute patch and segment
    - fermi_patch() : Compute patches constructing Fermi surface
  - display() : Display figures with OpenGL

*/
/*
 Input variables
*/
let ng0 = [0, 0, 0];         //!< @f$k@f$-point grid in the input file
let shiftk = [0, 0, 0];      //!< Wherether @f$k@f$-grid is shifted or not
let nb = 0;             //!< The number of Bands
let avec = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; //!< Direct lattice vector
let bvec = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; //!< Reciprocal lattice vector
let eig0 = [];   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
let mat0 = [];   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]][3]
/*
 Interpolation
*/
let ng = [0, 0, 0];        //!< @b Interpolated @f$k@f$-grids
let eig = [];  //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
let mat =[]; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
let vf =[];  //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
let interpol = 1;     //!< Ratio of interpolation
/*
 Switch for some modes
*/
let color_scale = 1; //!< Switch for full color scale mode
let fbz = 1;         //!< Switch for 1st Brillouin zone mode
let nodeline = 0;    //!< Switch for node lines
let lcolorbar = 1;   //!< Switch for colorbar
let lstereo = 1;     //!< Switch for the stereogram
let lmouse = 1;      //!< Switch for the mouse function
let lsection = 0;    //!< Switch for the 2D Fermi lines
let lequator = 0;    //!< Switch for equator
let BZ_number = [1, 1, 1];
/*
 Variables for Brillouin zone boundaries
*/
let nbzl = 1;      //!< The number of Lines of 1st Brillouin zone
let bzl = []; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
let bragg = [];   //!< Bragg plane vectors
let brnrm = [];      //!< Norms of Bragg plane vectors
let brnrm_min = 0.0;     //!< Minimum scale of the reciplocal space
let nbragg = 0;             //!< Number of Bragg plane og 1st BZ
/*
 Variables for patchs
*/
let ntri = [];          //!< The number of triangle patch [::nb]
let draw_band = [];     //!< Switch for drawn bands [::nb]
let nmlp = [];    //!< Normal vector of patchs [::nb][::ntri][3][3]
let kvp = [];    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
let arw = [];
let nmlp_rot = []; //!< Normal vector of patchs [::nb][::ntri*3*3]
let kvp_rot = [];  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
let arw_rot = [];
let matp = [];    //!< Matrix elements of points [::nb][::ntri][3][3]
let clr = [];      //!< Colors of points [::nb][::ntri*3*4]
let itet = 0;           //!< Counter for tetrahedron
let side = 1.0;       //!< Which side is lighted
let patch_max = 0.0;  //!< Max value across patch
let patch_min = 0.0;  //!< Max value across patch
/*
  Variables for nodeline
*/
let nnl = [];             //!< The number of nodeline
let kvnl = [];     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
let kvnl_rot = []; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
/*
 2D Fermi line
*/
let secvec = [0.0, 0.0, 0.0];         //!< @f$k@f$-vector to define section
let secvec_fr = [0.0, 0.0, 0.0];         //!< @f$k@f$-vector to define section
let secscale;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
let axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];      //!< @f$k@f$-vector to define section
let n2d = [];                  //!< Number of line segment
let kv2d = [];          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
let clr2d = [];         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
let nbzl2d = 0;                //!< The number of Lines of 1st Brillouin zone
let bzl2d = [];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
let bzl2d_proj = []; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
let eqvec = [0.0, 0.0, 0.0]; //!<  @f$k@f$-vector for equator
let eqvec_fr = [0.0, 0.0, 0.0]; //!<  @f$k@f$-vector for equator
let nequator = [];             //!< The number of equator
let kveq = [];     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
let kveq_rot = []; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
/*
  Variables for mouse  & cursorkey
*/
let sx = 0.0;        //!< Scale of mouse movement
let sy = 0.0;        //!< Scale of mouse movement
let cx = 0.0;            //!< Starting point of drug
let cy = 0.0;            //!< Starting point of drug
let scl = 1.0;       //!< Initial scale
let trans = [0.0, 0.0, 0.0];  //!< Translation
let rot = [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]; //!< Rotation matrix
let thetax = 0.0;    //!< Rotation angle
let thetay = 0.0;    //!< Rotation angle
let thetaz = 0.0;    //!< Rotation angle
let linewidth = 3.0; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
let black = [0.0, 0.0, 0.0, 1.0]; //!< Black color code
let gray = [0.5, 0.5, 0.5, 1.0]; //!< Gray color code
let wgray = [0.9, 0.9, 0.9, 1.0]; //!< Gray color code
let bgray = [0.1, 0.1, 0.1, 1.0]; //!< Gray color code
let white = [1.0, 1.0, 1.0, 1.0]; //!< White color code
let cyan = [0.0, 1.0, 1.0, 1.0]; //!< Cyan color code
let magenta = [1.0, 0.0, 1.0, 1.0]; //!< Magenta color code
let yellow = [1.0, 1.0, 0.0, 1.0]; //!< Yellow color code
let red = [1.0, 0.0, 0.0, 1.0]; //!< Red color code
let green = [0.0, 1.0, 0.0, 1.0]; //!< Green color code
let blue = [0.0, 0.0, 1.0, 1.0]; //!< Blue color code
let BackGroundColor = [0.0, 0.0, 0.0, 1.0];//!< BackGround color code
let LineColor = [1.0, 1.0, 1.0, 1.0];//!< Line color code
let BarColor = [[0.0, 0.0, 1.0, 1.0], [0.0, 1.0, 1.0, 1.0],
[0.0, 1.0, 0.0, 1.0], [1.0, 1.0, 0.0, 1.0], [1.0, 0.0, 0.0, 1.0]];
/*
 Others
*/
let corner = [[0.0, 0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0, 0.0]]; //!< Corners of tetrahedron
let EF = 0.0;       //!< Fermi energy
/*
Batch mode
*/
let batch_name;
let frmsf_file_name;
let lbatch = 0;

wxTextCtrl* terminal;
let refresh_interpol = 0;
let refresh_patch = 1;
let refresh_color = 1;
let refresh_nodeline = 1;
let refresh_equator = 1;
let refresh_section = 1;
let skip_minmax = 0;
let windowx = 1100;
let windowy = 850;
/**
  @brief Glut Display function
  called by glutDisplayFunc
*/
function batch_draw()
{
  let iminmax = 0;
  let minmax = [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0]];

  printf("\n  Batch mode.\n");

  read_batch();
  refresh_patch_segment();
}
/**
 @brief Main routine of FermiSurfer

*/
function OnInit()
{
  let ierr;

  myf = new MyFrame(NULL, argv[1], wxDefaultPosition, wxSize(windowx, windowy));

  terminal("\n");
  terminal("#####  Welocome to FermiSurfer ver. ") << 
    wxT(VERSION) << wxT("  #####\n");
  terminal("\n");
  if (argc < 2) {
    printf("\n");
    printf("  Input file is not specified !\n");
    printf("    Press any key to exit.\n");
    ierr = getchar();
    exit(-1);
  }
  /**/
  terminal("  Initialize variables ...\n");
  terminal("\n");
  /*
  Input from BXSF or FRMSF file
  */
  if (frmsf_file_name.AfterLast(wxUniChar('.')).CmpNoCase(wxT("bxsf")) == 0) {
    read_bxsf();
  }
  else {
    color_scale = read_file();
    if (color_scale == 0)color_scale = 4;
  }
  /**/
  interpol_energy();
  init_corner();
  bragg_vector();
  modify_band();
  /*
   Brillouin zone
  */
  bz_lines();
  calc_2dbz();
  /**/
  max_and_min_bz();
  /**/
  compute_patch_segment();
  /*
    Description
  */
  terminal("\n");
  terminal("  ##  How to handle  ###################\n");
  terminal("\n");
  terminal("              mouse drag : Rotate objects\n");
  terminal("              mousewheel : Resize objects\n");
  terminal("    cursorkey or w,a,s,d : Move objects\n");
  terminal("\n");
  /**/
  if (lbatch == 1) {
    batch_draw();
  }
  return true;
} /* main */

function OnInitCmdLine(parser)
{
  OnInitCmdLine(parser);

  parser.AddParam("FRMSF file to plot.",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
  parser.AddParam("Batch file",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddParam("Window Size x",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
  parser.AddParam("Window Size y",
    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

function OnCmdLineParsed(parser)
{
  if (parser.GetParamCount() > 0) {
    frmsf_file_name = parser.GetParam(0);
    if (parser.GetParamCount() > 1) {
      batch_name = parser.GetParam(1);
      lbatch = 1;
      if (parser.GetParamCount() > 2) {
        windowx = wxAtoi(parser.GetParam(2));
        if (parser.GetParamCount() > 3) {
          windowy = wxAtoi(parser.GetParam(3));
        }
      }
    }
  }

  return OnCmdLineParsed(parser);
}
/**
 @brief Free variables for patch before new patch is computed

 Free : ::nmlp, ::matp, ::clr, ::kvp, ::nmlp_rot, ::kvp_rot,
        ::kvnl, ::kvnl_rot, ::kv2d, ::clr2d
*/
function free_patch()
{
  let ib, i0, i1, i2;
  /*
   Fermi patch
  */
  if (refresh_patch == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < ntri[ib]; ++i0) {
        for (i1 = 0; i1 < 3; ++i1) {
          for (i2 = 0; i2 < 2; ++i2)
            arw[ib][i0][i1][i2] = [];
          nmlp[ib][i0][i1] = [];
          matp[ib][i0][i1] = [];
          kvp[ib][i0][i1] = [];
          arw[ib][i0][i1] = [];
        }
        nmlp[ib][i0] = [];
        matp[ib][i0] = [];
        kvp[ib][i0] = [];
        arw[ib][i0] = [];
      }
      nmlp[ib] = [];
      matp[ib] = [];
      clr[ib] = [];
      kvp[ib] = [];
      arw[ib] = [];
      nmlp_rot[ib] = [];
      kvp_rot[ib] = [];
      arw_rot[ib] = [];
    }
    nmlp = [];
    matp = [];
    clr = [];
    kvp = [];
    arw = [];
    nmlp_rot = [];
    kvp_rot = [];
    arw_rot = [];
  }/*if (refresh_patch == 1)*/
  /*
   Nodal line
  */
  if (refresh_nodeline == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < nnl[ib]; ++i0) {
        for (i1 = 0; i1 < 2; ++i1) {
          kvnl[ib][i0][i1] = [];
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        kvnl[ib][i0] = [];
      }/*for (i0 = 0; i0 < nnl[ib]; ++i0)*/
      kvnl[ib] = [];
      kvnl_rot[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kvnl = [];
    kvnl_rot = [];
  }/*if (refresh_nodeline == 1)*/
  /*
   2D Fermi line
  */
  if (refresh_section == 1) {
    for (ib = 0; ib < nb; ++ib) {
      kv2d[ib] = [];
      clr2d[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kv2d = [];
    clr2d = [];
  }/*if (refresh_section == 1)*/
  /*
  equator
  */
  if (refresh_equator == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < nequator[ib]; ++i0) {
        for (i1 = 0; i1 < 2; ++i1) {
          kveq[ib][i0][i1] = [];
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        kveq[ib][i0] = [];
      }/*for (i0 = 0; i0 < nequator[ib]; ++i0)*/
      kveq[ib] = [];
      kveq_rot[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kveq = [];
    kveq_rot = [];
  }/*if (refresh_equator == 1)*/
}/*function free_patch()*/
/**
 @brief Compute Max. & Min. of matrix elements.
 Compute color of each patch 

 Modify : ::clr
*/
function max_and_min() 
{
  let itri = 0;
  let i = 0, ib = 0;
  let abs = 0.0;

  terminal("\n");
  if (color_scale == 1) terminal("  ##  Color Scale as Input Quantity (Real) #############\n");
  else if (color_scale == 2) terminal("  ##  Color Scale as Input Quantity (Complex) #############\n");
  else if (color_scale == 3) terminal("  ##  Color Scale as Fermi Velocity #############\n");
  else if (color_scale == 4) terminal("  ##  Color Scale as Band Index #############\n");
  else if (color_scale == 5) terminal("  ##  Gray Scale as Input Quantity (Real) #############\n");
  else if (color_scale == 6) terminal("  ##  Gray Scale as Fermi Velocity #############\n");
  terminal("\n");
  /*
   Search max and min.
  */
  if (color_scale == 1 || color_scale == 6) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          if (matp[ib][itri][i][0] > patch_max) patch_max = matp[ib][itri][i][0];
          if (matp[ib][itri][i][0] < patch_min) patch_min = matp[ib][itri][i][0];
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 0 || color_scale == 4)*/
  else if (color_scale == 2) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                    + matp[ib][itri][i][1] * matp[ib][itri][i][1]);
          if (abs > patch_max) patch_max = abs;
          if (abs < patch_min) patch_min = abs;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
    }/*if (color_scale == 2)*/
  else   if (color_scale == 3) {

      patch_min = 1.0e10;
      patch_max = -1.0e10;

      for (ib = 0; ib < nb; ib++) {
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                      + matp[ib][itri][i][1] * matp[ib][itri][i][1]
                      + matp[ib][itri][i][2] * matp[ib][itri][i][2]);
            if (abs > patch_max) patch_max = abs;
            if (abs < patch_min) patch_min = abs;
          }
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 3)*/
  else if (color_scale == 4 || color_scale == 7) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          norm = 0.0;
          for (j = 0; j < 3; ++j) norm += nmlp[ib][itri][i][j]*nmlp[ib][itri][i][j];
          norm = Math.sqrt(norm);

          if (norm > patch_max) patch_max = norm;
          if (norm < patch_min) patch_min = norm;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 5 || color_scale == 6)*/

  myf->textbox_min->ChangeValue(wxString::Format(wxT("%f"), patch_min));
  myf->textbox_max->ChangeValue(wxString::Format(wxT("%f"), patch_max));
}/* max_and_min */
 /**
 @brief Compute Max. & Min. of matrix elements.
 Compute color of each patch

 Modify : ::clr
 */
function paint()
{
  let itri, j;
  let origin = [0.0, 0.0, 0.0, 0.0];
  let i, ib;
  let mat2;

  if (color_scale == 1) {

    for (ib = 0; ib < nb; ib++) {

      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = (matp[ib][itri][i][0] - patch_min) / (patch_max - patch_min);
          mat2 = mat2 * 4.0;
          /**/
          if (mat2 <= 1.0) {
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
              = BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2);
          }
          else if (mat2 <= 2.0) {
            mat2 = mat2 - 1.0;
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri]
              = BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2);
          }
          else if (mat2 <= 3.0) {
            mat2 = mat2 - 2.0;
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri]
              = BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2);
          }
          else {
            mat2 = mat2 - 3.0;
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
              = BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2);
          }
        }/*for (i = 0; i < 3; ++i)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 1 || color_scale == 2)*/
  else if (color_scale == 2) {

    for (j = 0; j < 4; ++j) origin[j] = 1.0 - BackGroundColor[j];

      let i, ib;
      let theta, abs, theta2;

      for (ib = 0; ib < nb; ib++) {
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                      + matp[ib][itri][i][1] * matp[ib][itri][i][1]);
            if (abs / patch_max < 0.00001) theta = 0.0;
            else if (matp[ib][itri][i][1] > 0.0) theta = Math.acos(matp[ib][itri][i][0] / abs);
            else theta = -Math.acos(matp[ib][itri][i][0] / abs);
            abs /= patch_max;
            theta = 3.0 * theta / Math.acos(-1.0);
            /**/
            if (-3.0 <= theta && theta < -2.0) {
              theta2 = theta + 3.0;
              for (j = 0; j < 4; ++j)
                clr[ib][j + 4 * i + 12 * itri] = blue[j] * theta2 + cyan[j] * (1.0 - theta2);
            }
            else if (-2.0 <= theta && theta < -1.0) {
              theta2 = theta + 2.0;
              for (j = 0; j < 4; ++j) 
                clr[ib][j + 4 * i + 12 * itri] = magenta[j] * theta2 + blue[j] * (1.0 - theta2);
            }
            else if (-1.0 <= theta && theta < 0.0) {
              theta2 = theta + 1.0;
              for (j = 0; j < 4; ++j) 
                clr[ib][j + 4 * i + 12 * itri] = red[j] * theta2 + magenta[j] * (1.0 - theta2);
            }
            else if (0.0 <= theta && theta < 1.0) {
              theta2 = theta;
              for (j = 0; j < 4; ++j)
                clr[ib][j + 4 * i + 12 * itri] = yellow[j] * theta2 + red[j] * (1.0 - theta2);
            }
            else if (1.0 <= theta && theta < 2.0) {
              theta2 = theta - 1.0;
              for (j = 0; j < 4; ++j)
                clr[ib][j + 4 * i + 12 * itri] = green[j] * theta2 + yellow[j] * (1.0 - theta2);
            }
            else {
              theta2 = theta - 2.0;
              for (j = 0; j < 4; ++j) 
                clr[ib][j + 4 * i + 12 * itri] = cyan[j] * theta2 + green[j] * (1.0 - theta2);
            }
            clr[ib][j + 4 * i + 12 * itri] = clr[ib][j + 4 * i + 12 * itri] * abs + origin[j] * (1.0 - abs);

          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 2)*/
  else if (color_scale == 4) {
      let i, ib;
      let mat2;

      for (ib = 0; ib < nb; ib++) {
  
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            mat2 = 0.0;
            for (j = 0; j < 3; ++j) mat2 += nmlp[ib][itri][i][j] * nmlp[ib][itri][i][j];
            mat2 = Math.sqrt(mat2);
            mat2 = (mat2 - patch_min) / (patch_max - patch_min);
            mat2 = mat2 * 4.0;
            /**/
            if (mat2 <= 1.0) {
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri]
                = BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2);
            }
            else if (mat2 <= 2.0) {
              mat2 = mat2 - 1.0;
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
                = BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2);
            }
            else if (mat2 <= 3.0) {
              mat2 = mat2 - 2.0;
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri]
                = BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2);
            }
            else {
              mat2 = mat2 - 3.0;
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
                = BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2);
            }
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 4)*/
  else if (color_scale == 3 || color_scale == 5) {
      let i, ib;
      let mat2;

      for (ib = 0; ib < nb; ib++) {
        /**/
        if (nb == 1) mat2 = 0.5;
        else mat2 = 1.0 / (nb - 1) * ib;
        mat2 *= 4.0;
        /**/
        if (mat2 <= 1.0) {

          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri]
                = BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2);
            }
          }
        }
        else if (mat2 <= 2.0) {
          mat2 = mat2 - 1.0;

          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
                = BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2);
            }
          }
        }
        else if (mat2 <= 3.0) {
          mat2 = mat2 - 2.0;

          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
                = BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2);
            }
          }
        }
        else {
          mat2 = mat2 - 3.0;

          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] 
                = BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2);
            }
          }
        }
      }/*for (ib = 0; ib < nb; ib++*/

      if (color_scale == 3) {
        for (ib = 0; ib < nb; ib++) {

          for (itri = 0; itri < ntri[ib]; ++itri) {
            for (i = 0; i < 3; ++i) {
              for (j = 0; j < 3; ++j) {
                arw[ib][itri][i][0][j] = kvp[ib][itri][i][j]
                               - 0.5 * matp[ib][itri][i][j] / patch_max;
                arw[ib][itri][i][1][j] = kvp[ib][itri][i][j]
                               + 0.5 * matp[ib][itri][i][j] / patch_max;
              }/*for (j = 0; j < 3; ++j)*/
            }/*for (i = 0; i < 3; ++i)*/
          }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
        }/*for (ib = 0; ib < nb; ib++)*/
      }/*if (color_scale == 3)*/
  }/*if (color_scale == 5)*/
  else if (color_scale == 6) {
      let i, ib;
      let mat2;

      for (ib = 0; ib < nb; ib++) {

        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            mat2 = (matp[ib][itri][i][0] - patch_min) / (patch_max - patch_min);
            /**/
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] = wgray[j] * mat2 + bgray[j] * (1.0 - mat2);
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 6)*/
  else if (color_scale == 7) {
      let i, ib;
      let mat2;

      for (ib = 0; ib < nb; ib++) {

        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
           mat2 = 0.0;
            for (j = 0; j < 3; ++j) mat2 += nmlp[ib][itri][i][j] * nmlp[ib][itri][i][j];
            mat2 = Math.sqrt(mat2);
            mat2 = (mat2 - patch_min) / (patch_max - patch_min);
            /**/
            for (j = 0; j < 4; ++j) clr[ib][j + 4 * i + 12 * itri] = wgray[j] * mat2 + bgray[j] * (1.0 - mat2);
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 7)*/
}/* paint */
/**
 @brief Specify corners of tetrahedron

 Modify : ::corner
*/
function init_corner()
{
  let i = 0, j = 0;
  let corner1 = [
    /*
    [0] min = 0-7
    */
    [ [ 0, 1, 3, 7 ],
    [ 0, 1, 5, 7 ],
    [ 0, 2, 3, 7 ],
    [ 0, 2, 6, 7 ],
    [ 0, 4, 5, 7 ],
    [ 0, 4, 6, 7 ] ],
    /*
    [1] min = 1-6
    */
    [ [ 1, 0, 2, 6 ],
    [ 1, 0, 4, 6 ],
    [ 1, 3, 2, 6 ],
    [ 1, 3, 7, 6 ],
    [ 1, 5, 4, 6 ],
    [ 1, 5, 7, 6 ] ],
    /*
    [2] min = 2-5
    */
    [ [ 2, 0, 1, 5 ],
    [ 2, 0, 4, 5 ],
    [ 2, 3, 1, 5 ],
    [ 2, 3, 7, 5 ],
    [ 2, 6, 4, 5 ],
    [ 2, 6, 7, 5 ] ],
    /*
    [3] min = 3-4
    */
    [ [ 4, 0, 1, 3 ],
    [ 4, 0, 2, 3 ],
    [ 4, 5, 1, 3 ],
    [ 4, 5, 7, 3 ],
    [ 4, 6, 2, 3 ],
    [ 4, 6, 7, 3 ] ],
    /*
    [4] min = 0-7, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 0, 7, 2, 6 ],
    [ 0, 7, 1, 5 ],
    [ 0, 7, 1, 2 ],
    [ 0, 7, 6, 5 ] ],
    /*
    [5] min = 1-6, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 1, 6, 5, 7 ],
    [ 1, 6, 7, 2 ],
    [ 1, 6, 2, 0 ],
    [ 1, 6, 0, 5 ] ],
    /*
    [6] min = 2-5, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 2, 5, 7, 6 ],
    [ 2, 5, 6, 0 ],
    [ 2, 5, 0, 1 ],
    [ 2, 5, 1, 7 ] ],
    /*
    [7] min = 3-4, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 3, 4, 1, 5 ],
    [ 3, 4, 5, 6 ],
    [ 3, 4, 6, 2 ],
    [ 3, 4, 2, 1 ] ],
    /*
    [8] min = 2-5, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 2, 5, 1, 3 ],
    [ 2, 5, 3, 6 ],
    [ 2, 5, 6, 4 ],
    [ 2, 5, 4, 1 ] ],
    /*
    [9] min = 1-6, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 1, 6, 2, 3 ],
    [ 1, 6, 3, 5 ],
    [ 1, 6, 5, 4 ],
    [ 1, 6, 4, 2 ] ],
    /*
    [10] min = 0-7, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 0, 7, 2, 3 ],
    [ 0, 7, 3, 5 ],
    [ 0, 7, 5, 4 ],
    [ 0, 7, 4, 2 ] ],
    /*
    [11] min = 3-4, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 3, 4, 0, 2 ],
    [ 3, 4, 2, 7 ],
    [ 3, 4, 7, 5 ],
    [ 3, 4, 5, 0 ] ],
    /*
    [12] min = 2-5, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 2, 5, 0, 3 ],
    [ 2, 5, 3, 7 ],
    [ 2, 5, 7, 4 ],
    [ 2, 5, 4, 0 ] ],
    /*
    [13] min = 0-7, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 0, 7, 1, 3 ],
    [ 0, 7, 3, 6 ],
    [ 0, 7, 6, 4 ],
    [ 0, 7, 4, 1 ] ],
    /*
    [14] min = 1-6, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 1, 6, 0, 3 ],
    [ 1, 6, 3, 7 ],
    [ 1, 6, 7, 4 ],
    [ 1, 6, 4, 0 ] ],
    /*
    [15] min = 3-4, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 3, 4, 0, 1 ],
    [ 3, 4, 1, 7 ],
    [ 3, 4, 7, 6 ],
    [ 3, 4, 6, 0 ] ],
  ];
  /**/
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 4; ++j) {
      corner[i][j] = corner1[itet][i][j];
    }
  }
}
/**
 @brief Compute Bragg vetor

 Modify : ::bragg, ::brnrm
*/
function bragg_vector()
{
  let i0, i1, i2, i, ibr;
  /**/
  ibr = 0;
  /**/
  for (i0 = -1; i0 <= 1; ++i0) {
    for (i1 = -1; i1 <= 1; ++i1) {
      for (i2 = -1; i2 <= 1; ++i2) {
        /*
         Excepte Gamma points
        */
        if (i0 == 0 && i1 == 0 && i2 == 0) continue;
        /*
         Fractional -> Cartecian
        */
        for (i = 0; i < 3; ++i)
          bragg[ibr][i] = (i0 * bvec[0][i]
                        +  i1 * bvec[1][i]
                        +  i2 * bvec[2][i]) * 0.5;
        /*
         And its norm
        */
        brnrm[ibr] = bragg[ibr][0] * bragg[ibr][0]
                   + bragg[ibr][1] * bragg[ibr][1]
                   + bragg[ibr][2] * bragg[ibr][2];
        /**/
        ibr = ibr + 1;
      }/*for (i2 = -1; i2 <= 1; ++i2)*/
    }/*for (i1 = -1; i1 <= 1; ++i1)*/
  }/*for (i0 = -1; i0 <= 1; ++i0)*/
  /*
   Search min. of brnrm
  */
  brnrm_min = brnrm[0];
  for (ibr = 1; ibr < 26; ibr++) {
    if (brnrm_min > brnrm[ibr]) brnrm_min = brnrm[ibr];
  }
  terminal("    Minimum Bragg norm : " + toString(brnrm_min) + "\n");
}/* bragg_vector */
/**
 @brief Print max and minimum @f$\varepsilon_{n k}, \Delta_{n k}@f$
        in the whole Brillouine zone
*/
function max_and_min_bz()
{
  let ib, i0, i1, i2;
  let eigmin, eigmax, matmin, matmax;
  /**/
  terminal("\n");
  terminal("  ##  Max. and Min. of each bands  #######################\n");
  terminal("\n");
  terminal("    Band   Eig_Min.      Eig_Max      Mat_Min      Mat_Max\n");
  for (ib = 0; ib < nb; ib++) {
    eigmax = eig0[0][0][0][0];
    eigmin = eig0[0][0][0][0];
    matmax = mat0[0][0][0][0][0];
    matmin = mat0[0][0][0][0][0];
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (eig0[ib][i0][i1][i2] > eigmax) eigmax = eig0[ib][i0][i1][i2];
          if (eig0[ib][i0][i1][i2] < eigmin) eigmin = eig0[ib][i0][i1][i2];
          if (mat0[ib][i0][i1][i2][0] > matmax) matmax = mat0[ib][i0][i1][i2][0];
          if (mat0[ib][i0][i1][i2][0] < matmin) matmin = mat0[ib][i0][i1][i2][0];
        }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
      }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
    }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    terminal("    " + toString(ib + 1) + "     " + toString(eigmin) + "     " +
      toString(eigmax) + "     " + toString(matmin) + "     " + toString(matmax) + "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/* max_and_min_bz */
/**
 @brief Compute coefficient for the French-curve (Kumo) interpolation
 @f[
  A^{\rm intp} = \sum_{i = 1}^4 C_i A_i^{\rm orig}
 @f]
*/
function kumo_coef(
  j = 0, //!< [in] Interpolated grid index
  coef = [0.0, 0.0, 0.0, 0.0],//!< [out] Coefficient of interpolation @f$C_i@f$
  interpol = 0
) {
  let x = 0.0, mx = 0.0;
  x = j / interpol;
  mx = 1.0 - x;
  coef[0] = -0.5 * x * mx * mx;
  coef[1] = mx * (mx*mx + 3.0* x*mx + 0.5* x* x);
  coef[2] =  x * ( x* x + 3.0*mx* x + 0.5*mx*mx);
  coef[3] = -0.5 * x *  x * mx;
}
/**
 @brief Interpolation of energy and matrix 
        with the French-curve (Kumo) interpolation.

 Modify : ::eig, ::mat
*/
function interpol_energy() 
{
  let ib, i0, i1, i2, ii;

  terminal("    Interpolating ... ");
  /*
   Reallocate
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng[0]; i0++) {
      for (i1 = 0; i1 < ng[1]; i1++) {
        for (i2 = 0; i2 < ng[2]; i2++) {
          vf[ib][i0][i1][i2] = [];
          mat[ib][i0][i1][i2] = [];
        }
        eig[ib][i0][i1] = [];
        mat[ib][i0][i1] = [];
        vf[ib][i0][i1] = [];
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
      eig[ib][i0] = [];
      mat[ib][i0] = [];
      vf[ib][i0] = [];
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
    eig[ib] = [];
    mat[ib] = [];
    vf[ib] = [];
  }/*for (ib = 0; ib < nb; ib++)*/
  eig = [];
  mat = [];
  vf = [];
  for (ii = 0; ii < 3; ii++)ng[ii] = ng0[ii] * interpol;
  /**/
  for (ib = 0; ib < nb; ib++) {
    eig[ib] = new let**[ng[0]];
    mat[ib] = new let***[ng[0]];
    vf[ib] = new let***[ng[0]];
    for (i0 = 0; i0 < ng[0]; i0++) {
      eig[ib][i0] = new let*[ng[1]];
      mat[ib][i0] = new let**[ng[1]];
      vf[ib][i0] = new let**[ng[1]];
      for (i1 = 0; i1 < ng[1]; i1++) {
        eig[ib][i0][i1] = new let[ng[2]];
        mat[ib][i0][i1] = new let*[ng[2]];
        vf[ib][i0][i1] = new let*[ng[2]];
        for (i2 = 0; i2 < ng[2]; i2++) {
          mat[ib][i0][i1][i2] = new let[3];
          vf[ib][i0][i1][i2] = new let[3];
        }
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  /*
   3rd order - three dimensional Kumo interpolation
  */
    let j0, j1, j2, jj;
    let coef[4], 
      mat1[4][4][4][3], mat2[4][4][3], mat3[4][3],
      eig1[4][4][4], eig2[4][4], eig3[4];

    for (ib = 0; ib < nb; ib++) {
      for (i0 = 0; i0 < ng0[0]; i0++) {
        //if (ith == 1) continue;
        for (i1 = 0; i1 < ng0[1]; i1++) {
          for (i2 = 0; i2 < ng0[2]; i2++) {
            for (j0 = 0; j0 < 4; j0++) {
              for (j1 = 0; j1 < 4; j1++) {
                for (j2 = 0; j2 < 4; j2++) {
                  eig1[j0][j1][j2] = eig0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                             [modulo(i1 + j1 - 1, ng0[1])]
                                             [modulo(i2 + j2 - 1, ng0[2])];
                  for (jj = 0; jj < 3; jj++) {
                    mat1[j0][j1][j2][jj] = mat0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                                   [modulo(i1 + j1 - 1, ng0[1])]
                                                   [modulo(i2 + j2 - 1, ng0[2])][jj];
                  }
                }/*for (j2 = 0; j2 < 4; j2++)*/
              }/*for (j1 = 0; j1 < 4; j1++)*/
            }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
            for (j0 = 0; j0 < interpol; j0++) {
              kumo_coef(j0, &coef[0], interpol);
              for (j1 = 0; j1 < 4; j1++) {
                for (j2 = 0; j2 < 4; j2++) {
                  eig2[j1][j2] = 0.0;
                  for (jj = 0; jj < 3; jj++) mat2[j1][j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig2[j1][j2] += coef[ii] * eig1[ii][j1][j2];
                    for (jj = 0; jj < 3; jj++) 
                      mat2[j1][j2][jj] += coef[ii] * mat1[ii][j1][j2][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < 4; j2++)*/
              }/*for (j1 = 0; j1 < 4; j1++)*/
              for (j1 = 0; j1 < interpol; j1++) {
                kumo_coef(j1, &coef[0], interpol);
                for (j2 = 0; j2 < 4; j2++) {
                  eig3[j2] = 0.0;
                  for (jj = 0; jj < 3; jj++) mat3[j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig3[j2] += coef[ii] * eig2[ii][j2];
                    for (jj = 0; jj < 3; jj++)
                      mat3[j2][jj] += coef[ii] * mat2[ii][j2][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < 4; j2++)*/
                for (j2 = 0; j2 < interpol; j2++) {
                  kumo_coef(j2, &coef[0], interpol);
                  eig[ib][i0*interpol + j0]
                         [i1*interpol + j1]
                         [i2*interpol + j2] = 0.0;
                  for (jj = 0; jj < 3; jj++)
                    mat[ib][i0*interpol + j0]
                           [i1*interpol + j1]
                           [i2*interpol + j2][jj] = 0.0;
                  for (ii = 0; ii < 4; ii++) {
                    eig[ib][i0*interpol + j0]
                           [i1*interpol + j1]
                           [i2*interpol + j2] += coef[ii] * eig3[ii];
                    for (jj = 0; jj < 3; jj++)
                      mat[ib][i0*interpol + j0]
                             [i1*interpol + j1]
                             [i2*interpol + j2][jj] += coef[ii] * mat3[ii][jj];
                  }/*for (ii = 0; ii < 4; ii++)*/
                }/*for (j2 = 0; j2 < interpol; j2++)*/
              }/*for (j1 = 0; j1 < interpol; j1++)*/
            }/*for (j0 = 0; j0 < interpol; j0++)*/
          }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
        }/*for (i1 = 0; i1 < ng0[1]; i1++)*/
      }/*for (i0 = 0; i0 < ng0[0]; i0++)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  /*
   Fermi velocity
  */
#pragma omp parallel default(none) \
  shared(nb,ng,eig,vf,avec) \
  private (ib,i0,i1,i2,ii)
  {
    let i0p, i0m, i1p, i1m, i2p, i2m;
    let de[3];

    for (ib = 0; ib < nb; ib++) {
      for (i0 = 0; i0 < ng[0]; i0++) {
        i0p = modulo(i0 + 1, ng[0]);
        i0m = modulo(i0 - 1, ng[0]);
        for (i1 = 0; i1 < ng[1]; i1++) {
          i1p= modulo(i1 + 1, ng[1]);
          i1m = modulo(i1 - 1, ng[1]);
          for (i2 = 0; i2 < ng[2]; i2++) {
            i2p = modulo(i2 + 1, ng[2]);
            i2m = modulo(i2 - 1, ng[2]);

            de[0] = eig[ib][i0p][i1][i2] - eig[ib][i0m][i1][i2];
            de[1] = eig[ib][i0][i1p][i2] - eig[ib][i0][i1m][i2];
            de[2] = eig[ib][i0][i1][i2p] - eig[ib][i0][i1][i2m];
            for (ii = 0; ii < 3; ii++)de[ii] *= 0.5f * ng[ii];
            for (ii = 0; ii < 3; ii++) vf[ib][i0][i1][i2][ii] =
              avec[0][ii] * de[0] + avec[1][ii] * de[1] + avec[2][ii] * de[2];

          }/*for (i2 = 0; i2 < ng[2]; i2++)*/
        }/*for (i1 = 0; i1 < ng[1]; i1++)*/
      }/*for (i0 = 0; i0 < ng[0]; i0++)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*End of parallel region*/
  terminal("Done\n\n");
}/*function interpol_energy() */

function compute_patch_segment() {
  if (refresh_interpol == 1){
    interpol_energy();
    refresh_patch = 1;
    refresh_interpol = 0;
  }
  if (refresh_patch == 1) {
    fermi_patch();
    refresh_color = 1;
    refresh_section = 1;
    refresh_equator = 1;
    refresh_nodeline = 1;
    refresh_patch = 0;
  }
  if (refresh_color == 1) {
    if (skip_minmax == 1) skip_minmax = 0;
    else max_and_min();
    paint();
    refresh_section = 1;
    refresh_color = 0;
  }
  if (refresh_nodeline == 1) {
    calc_nodeline();
    refresh_nodeline = 0;
  }
  if (refresh_section == 1) {
    calc_2dbz();
    calc_section();
    refresh_section = 0;
  }
  if (refresh_equator == 1) {
    equator();
    refresh_equator = 0;
  }
}

function refresh_patch_segment() {
  free_patch();
  compute_patch_segment();
}

enum
{
  ibutton_reflesh,
  ibutton_compute,
  iradio_background,
  iradio_brillouinzone,
  iradio_lighting,
  iradio_mouse,
  iradio_stereo,
  iradio_tetra,
  iradio_colorscale,
  itext_colorscalemin,
  itext_colorscalemax,
  icheck_section,
  icheck_gamma,
  itext_sectionx,
  itext_sectiony,
  itext_sectionz,
  icheck_nodeline,
  icheck_colorbar,
  icheck_equator,
  itext_equatorx,
  itext_equatory,
  itext_equatorz,
  itext_line,
  itext_shift,
  itext_interpol,
  itext_scale,
  itext_positionx,
  itext_positiony,
  itext_rotx,
  itext_roty,
  itext_rotz,
  ibutton_rotate,
  icheck_band,
  itext_BackGroundR,
  itext_BackGroundG,
  itext_BackGroundB,
  itext_LineColorR,
  itext_LineColorG,
  itext_LineColorB,
  iradio_BarColor,
  ibutton_section,
  itext_BZ_number0,
  itext_BZ_number1,
  itext_BZ_number2
};

function MyFrame::button_refresh(
  wxCommandEvent& event//!<[in] Selected menu
) {
  Refresh(false);
}

function MyFrame::button_compute(
  wxCommandEvent& event//!<[in] Selected menu
) {
  free_patch();
  compute_patch_segment();
  Refresh(false);
}
function MyFrame::button_section(
  wxCommandEvent& event//!<[in] Selected menu
) {
  let ib, i2d, i, j;
  FILE *fp;
  fp = fopen("fermi_line.dat", "w");
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      for (i2d = 0; i2d < n2d[ib]; i2d++) {
        for (i = 0; i < 2; i++) {
          fprintf(fp, "%15.5e %15.5e\n",
            kv2d[ib][i * 3 + 6 * i2d], 
            kv2d[ib][1 + i * 3 + 6 * i2d]);
        }
        fprintf(fp, "\n\n");
      }
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  fclose(fp);
  terminal("  fermi_line.dat was written.\n");

  fp = fopen("bz_line.dat", "w");
  for (i2d = 0; i2d < nbzl2d; i2d++) {
    fprintf(fp, "%15.5e %15.5e\n",
      bzl2d_proj[i2d][0], bzl2d_proj[i2d][1]);
  }
  fprintf(fp, "%15.5e %15.5e\n",
    bzl2d_proj[0][0], bzl2d_proj[0][1]);
  fclose(fp);
  terminal("  bz_line.dat was written.\n");
}
/**
@brief Change Line color color (::blackback)
*/
function MyFrame::textctrl_LineColor(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  double dvalue;

  if (event.GetId() == itext_LineColorR) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[0] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_LineColorG) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[1] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_LineColorB) {
    if (event.GetString().ToDouble(&dvalue)) {
      LineColor[2] = dvalue;
      Refresh(false);
    }
  }
}
/**
@brief Change background color (::blackback)
*/
function MyFrame::textctrl_BackGround(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  double dvalue;

  if (event.GetId() == itext_BackGroundR) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[0] = dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1],
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BackGroundG) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[1] = dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1], 
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BackGroundB) {
    if (event.GetString().ToDouble(&dvalue)) {
      BackGroundColor[2] = dvalue;
      glClearColor(BackGroundColor[0], BackGroundColor[1],
        BackGroundColor[2], BackGroundColor[3]);
      Refresh(false);
    }
  }
}
/**
@brief Change Number of Brillouin zone
*/
function MyFrame::textctrl_BZ_number(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  double dvalue;

  if (event.GetId() == itext_BZ_number0) {
    if (event.GetString().ToDouble(&dvalue)) {
      BZ_number[0] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BZ_number1) {
    if (event.GetString().ToDouble(&dvalue)) {
      BZ_number[1] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_BZ_number2) {
    if (event.GetString().ToDouble(&dvalue)) {
      BZ_number[2] = dvalue;
      Refresh(false);
    }
  }
}
 /**
 @brief Toggle the appearance of each band (::draw_band)
*/
function MyFrame::check_band(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ib = event.GetId() - icheck_band;
  if (draw_band[ib] == 0) {
    draw_band[ib] = 1;
  }
  else {
    draw_band[ib] = 0;
  }
  Refresh(false);
} /* menu_band */
/**
 @brief Change Brillouin zone (::fbz)
*/
function MyFrame::radio_brillouinzone(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("First Brillouin zone")) == 0) {
    fbz = 1;
  }
  else if (event.GetString().Cmp(wxT("Primitive Brillouin zone")) == 0) {
    fbz = -1;
  }
  refresh_patch = 1;
} /* menu_brillouinzone */
/**
 @brief Change Brillouin zone (::fbz)
*/
function MyFrame::radio_BarColor(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ii;
  if (event.GetString().Cmp(wxT("BGR")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = blue[ii];
      BarColor[1][ii] = cyan[ii];
      BarColor[2][ii] = green[ii];
      BarColor[3][ii] = yellow[ii];
      BarColor[4][ii] = red[ii];
    }
  }
  else if (event.GetString().Cmp(wxT("CMY")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = cyan[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = magenta[ii];
      BarColor[3][ii] = red[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  else if (event.GetString().Cmp(wxT("MCY")) == 0) {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = magenta[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = cyan[ii];
      BarColor[3][ii] = green[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  paint();
  Refresh(false);
} /* menu_brillouinzone */
/**
 @brief Toggle Colorbar (::lcolorbar)
*/
function MyFrame::check_colorbar(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (lcolorbar != 1)  lcolorbar = 1;
  else lcolorbar = 0;
  Refresh(false);
} /* menu_colorbar */
/**
 @brief Change color scale mode (::color_scale)
*/
function MyFrame::radiovalue_colorscale(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr, ii;
  double dminmax;

  if (event.GetId() == itext_colorscalemin) {
    if (event.GetString().ToDouble(&dminmax))
      patch_min = dminmax;
    skip_minmax = 1;
  }
  else if (event.GetId() == itext_colorscalemax) {
    if (event.GetString().ToDouble(&dminmax))
      patch_max = dminmax;
    skip_minmax = 1;
  }
  else if (event.GetString().Cmp(wxT("Input (1D)")) == 0) 
    color_scale = 1;
  else if (event.GetString().Cmp(wxT("Input (2D)")) == 0)
    color_scale = 2;
  else if (event.GetString().Cmp(wxT("Input (3D)")) == 0)
    color_scale = 3;
  else if (event.GetString().Cmp(wxT("Fermi Velocity")) == 0)
    color_scale = 4;
  else if (event.GetString().Cmp(wxT("Band Index")) == 0)
    color_scale = 5;
  else if (event.GetString().Cmp(wxT("Input (1D, Gray Scale)")) == 0)
    color_scale = 6;
  else if (event.GetString().Cmp(wxT("Fermi Velocity (Gray Scale)")) == 0)
    color_scale = 7;
  refresh_color = 1;
} /* menu_colorscale */
/**
 @brief Modify and toggle appearance of equator (::lequator)
*/
function MyFrame::checkvalue_equator(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr, ii, jj, ib;
  double deqvec;

  if (event.GetId() == icheck_equator) {
    if (lequator != 1) lequator = 1;
    else lequator = 0;
    Refresh(false);
  }/*if (event.GetId() == 1)*/
  else {
    if (event.GetId() == itext_equatorx) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[0] = deqvec;
    }
    else if (event.GetId() == itext_equatory) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[1] = deqvec;
    }
    else if (event.GetId() == itext_equatorz) {
      if (event.GetString().ToDouble(&deqvec)) eqvec_fr[2] = deqvec;
    }
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      eqvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        eqvec[ii] += eqvec_fr[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
    refresh_equator = 1;
  }/*else if (event.GetId() > 1)*/
} /*function menu_equator*/
/**
 @brief Modify interpolation ratio

 This routine modify interpolation ratio (::interpol) 
 then compute Fermi surfaces, etc.
*/
function MyFrame::textctrl_interpol(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  long let long_interpol;

  if (event.GetString().ToLong(&long_interpol)) {
    interpol = long_interpol;
    refresh_interpol = 1;
  }
}/*function menu_interpol*/
/**
 @brief Toggle Lighting
*/
function MyFrame::radio_lighting(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Both")) == 0) {
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  }
  if (event.GetString().Cmp(wxT("Unoccupy")) == 0) {
    side = 1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  if (event.GetString().Cmp(wxT("Occupy")) == 0) {
    side = -1.0;
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  }
  Refresh(false);
} /* menu_lighting */
/**
 @brief Line width
*/
function MyFrame::textctrl_line(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  double dlinewidth;

  if (event.GetString().ToDouble(&dlinewidth)) linewidth = dlinewidth;
  Refresh(false);
} /* menu_line */
/**
 @brief Change the function associated to the mouse movement(::lmouse)
*/
function MyFrame::radio_mouse(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (event.GetString().Cmp(wxT("Rotate")) == 0) lmouse = 1;
  else if (event.GetString().Cmp(wxT("Scale")) == 0) lmouse = 2;
  else if (event.GetString().Cmp(wxT("Translate")) == 0) lmouse = 3;
  Refresh(false);
} /* menu_mouse */
/**
 @brief Toggle apearance of nodale-line
*/
function MyFrame::check_nodeline(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  if (nodeline != 1) nodeline = 1;
  else nodeline = 0;
  Refresh(false);
}/*menu_nodeline*/
/**
 @brief Modify and toggle appearance of 2D Fermi lines (::lsection)
*/
function MyFrame::radiovalue_section(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr, ii, jj, ib;
  double dsecvec;

  if (event.GetId() == icheck_section) {
    if (lsection == 0)lsection = 1;
    else lsection = 0;
    Refresh(false);
  }
  else if(event.GetId() == icheck_gamma) {
    if(secscale > 0.5) secscale = 0.0;
    else secscale = 1.0;
    refresh_section = 1;
  }
  else {
    if (event.GetId() == itext_sectionx) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[0] = dsecvec;
    }
    else if (event.GetId() == itext_sectiony) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[1] = dsecvec;
    }
    else if (event.GetId() == itext_sectionz) {
      if (event.GetString().ToDouble(&dsecvec)) secvec_fr[2] = dsecvec;
    }
    /*
     Fractional -> Cartecian
    */
    for (ii = 0; ii < 3; ii++) {
      secvec[ii] = 0.0;
      for (jj = 0; jj < 3; jj++) {
        secvec[ii] += secvec_fr[jj] * bvec[jj][ii];
      }/*for (jj = 0; jj < 3; jj++)*/
    }/*for (ii = 0; ii < 3; ii++)*/
    refresh_section = 1;
  }/*else if (event.GetId() > 1)*/
} /*function menu_section*/
/**
 @brief Shift Fermi energy
*/
function MyFrame::textctrl_shift(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  double dEF;
  if (event.GetString().ToDouble(&dEF)) {
    EF = dEF;
    refresh_patch = 1;
  }
} /* menu_shift */
/**
 @brief Tern stereogram (::lstereo)
*/
function MyFrame::radio_stereo(
  wxCommandEvent& event //!<[in] Selected menu
) {
  if (event.GetString().Cmp(wxT("None")) == 0) lstereo = 1;
  else if (event.GetString().Cmp(wxT("Parallel")) == 0) lstereo = 2;
  else if (event.GetString().Cmp(wxT("Cross")) == 0) lstereo = 3;
  Refresh(false);
} /* menu_stereo */
/**
 @brief Change tetrahedron (::itet)
*/
function MyFrame::radio_tetra(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  itet = wxAtoi(event.GetString()) - 1;
  init_corner();
  refresh_patch = 1;
}/*menu_tetra*/
 /**
 @brief Setting of view

 This modify scale (::scl) & tarnslation (::trans) &
 rotation (::thetax, ::thetay, ::thetaz, ::rot),
 */
function MyFrame::textctrl_view(
  wxCommandEvent& event //!<[in] Selected menu
)
{
  let ierr;
  double dvalue;

  if (event.GetId() == itext_scale) {
    if (event.GetString().ToDouble(&dvalue)) {
      linewidth *= dvalue/scl;
      scl = dvalue;
      textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_positionx) {
    if (event.GetString().ToDouble(&dvalue)) {
      trans[0] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_positiony) {
    if (event.GetString().ToDouble(&dvalue)) {
      trans[1] = dvalue;
      Refresh(false);
    }
  }
  else  if (event.GetId() == itext_rotx ||
    event.GetId() == itext_roty ||
    event.GetId() == itext_rotz) {
    /*
    thetay = Math.asin(rot[0][2]);
    if (Math.cos(thetay) != 0.0) {
      if (-rot[1][2] / Math.cos(thetay) >= 0.0) thetax = Math.acos(rot[2][2] / Math.cos(thetay));
      else thetax = 6.283185307f - Math.acos(rot[2][2] / Math.cos(thetay));
      if (-rot[0][1] / Math.cos(thetay) >= 0.0) thetaz = Math.acos(rot[0][0] / Math.cos(thetay));
      else thetaz = 6.283185307f - Math.acos(rot[0][0] / Math.cos(thetay));
    }
    else {
      thetax = 0.0;
      if (rot[1][0] >= 0.0) thetaz = Math.acos(rot[1][1]);
      else thetaz = 6.283185307f - Math.acos(rot[1][1]);
    }
    thetax = 180.0 / 3.14159265f * thetax;
    thetay = 180.0 / 3.14159265f * thetay;
    thetaz = 180.0 / 3.14159265f * thetaz;
    printf("    Current Rotation (theta_x theta_y teta_z) in degree : %f %f %f\n", thetax, thetay, thetaz);
    printf("        New Rotation (theta_x theta_y teta_z) in degree : ");
    */
    if (event.GetId() == itext_rotx) {
      if (event.GetString().ToDouble(&dvalue))
        thetax = (dvalue) / 180.0 * 3.14159265;
    }
    else if (event.GetId() == itext_roty) {
      if (event.GetString().ToDouble(&dvalue))
        thetay = (dvalue) / 180.0 * 3.14159265;
    }
    else if (event.GetId() == itext_rotz) {
      if (event.GetString().ToDouble(&dvalue))
        thetaz = (dvalue) / 180.0 * 3.14159265;
    }
  }
  else if(event.GetId() == ibutton_rotate){
    rot[0][0] = Math.cos(thetay)* Math.cos(thetaz);
    rot[0][1] = -Math.cos(thetay)* Math.sin(thetaz);
    rot[0][2] = Math.sin(thetay);
    rot[1][0] = Math.cos(thetaz)* Math.sin(thetax)* Math.sin(thetay) + Math.cos(thetax)* Math.sin(thetaz);
    rot[1][1] = Math.cos(thetax) * Math.cos(thetaz) - Math.sin(thetax)* Math.sin(thetay)* Math.sin(thetaz);
    rot[1][2] = -Math.cos(thetay)* Math.sin(thetax);
    rot[2][0] = -Math.cos(thetax)* Math.cos(thetaz)* Math.sin(thetay) + Math.sin(thetax)* Math.sin(thetaz);
    rot[2][1] = Math.cos(thetaz)* Math.sin(thetax) + Math.cos(thetax)* Math.sin(thetay)* Math.sin(thetaz);
    rot[2][2] = Math.cos(thetax)* Math.cos(thetay);
    Refresh(false);
  }
}

MyFrame::MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
  const wxSize& size, long style)
  : wxFrame(frame, wxID_ANY, title, pos, size, style),
  m_canvas(NULL)
{
  let ib, itet;
  char menuname[8];

  SetIcon(wxICON(fermisurfer));

  // Make a menubar
  //wxMenu* fileMenu = new wxMenu;
  //wxMenuBar* menuBar = new wxMenuBar;
  //menuBar->Append(fileMenu, wxT("File"));
  //SetMenuBar(menuBar);
  // JACS
#ifdef __WXMSW__
  let* gl_attrib = NULL;
#else
  let gl_attrib[20] =
  { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
  WX_GL_DOUBLEBUFFER,
#  if defined(__WXMAC__) || defined(__WXCOCOA__)
        GL_NONE };
#  else
    None
};
#  endif
#endif

  wxBoxSizer* sizermain = new wxBoxSizer(wxVERTICAL);

  splitterV = new wxSplitterWindow(this, wxID_ANY);
  splitterV->SetSashGravity(1.0);
  splitterV->SetMinimumPaneSize(0); 
  sizermain->Add(splitterV, 1, wxEXPAND, 0);

  panel = new wxPanel(splitterV);

  gbsizer = new wxGridBagSizer();

  splitterH = new wxSplitterWindow(splitterV, wxID_ANY);
  splitterH->SetSashGravity(1.0);
  splitterH->SetMinimumPaneSize(0);

  m_canvas = new TestGLCanvas(splitterH, wxID_ANY, gl_attrib);

  terminal = new wxTextCtrl(splitterH, wxID_ANY, wxT(""),
    wxPoint(0, 250), wxSize(100, 0), wxTE_MULTILINE);
  if (lbatch == 1) splitterH->SplitHorizontally(m_canvas, terminal, -1);
  else splitterH->SplitHorizontally(m_canvas, terminal, -200);
  panel->SetSizer(gbsizer);

  if (lbatch == 1)splitterV->SplitVertically(splitterH, panel, -1);
  else splitterV->SplitVertically(splitterH, panel, 0);

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::button_compute, this, ibutton_compute);
  gbsizer->Add(new wxButton(panel, ibutton_compute, wxT("Update")), wxGBPosition(0,0), wxGBSpan(1,1));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Line width : ")),
    wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_line, this, itext_line);
  textbox_linewidth = new wxTextCtrl(panel, itext_line, wxT(""));
  gbsizer->Add(textbox_linewidth, wxGBPosition(0, 2), wxGBSpan(1, 1));
  textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_gamma);
  wxCheckBox *check_ongamma = new wxCheckBox(panel, icheck_gamma, wxT("On Gamma"));
  gbsizer->Add(check_ongamma, wxGBPosition(0, 3), wxGBSpan(1, 1));
  check_ongamma->SetValue(true);

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Section-v : ")), 
    wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionx);
  wxTextCtrl* textbox_sectionx = new wxTextCtrl(panel, itext_sectionx, wxT(""));
  gbsizer->Add(textbox_sectionx, wxGBPosition(1, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectiony);
  wxTextCtrl* textbox_sectiony = new wxTextCtrl(panel, itext_sectiony, wxT(""));
  gbsizer->Add(textbox_sectiony, wxGBPosition(1, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_section, this, itext_sectionz);
  wxTextCtrl* textbox_sectionz = new wxTextCtrl(panel, itext_sectionz, wxT(""));
  gbsizer->Add(textbox_sectionz, wxGBPosition(1, 3), wxGBSpan(1, 1));
  textbox_sectionx->ChangeValue(wxT("0"));
  textbox_sectiony->ChangeValue(wxT("0"));
  textbox_sectionz->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Equator-v : ")), 
    wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorx);
  wxTextCtrl* textbox_equatorx = new wxTextCtrl(panel, itext_equatorx, wxT(""));
  gbsizer->Add(textbox_equatorx, wxGBPosition(2, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatory);
  wxTextCtrl* textbox_equatory = new wxTextCtrl(panel, itext_equatory, wxT(""));
  gbsizer->Add(textbox_equatory, wxGBPosition(2, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::checkvalue_equator, this, itext_equatorz);
  wxTextCtrl* textbox_equatorz = new wxTextCtrl(panel, itext_equatorz, wxT(""));
  gbsizer->Add(textbox_equatorz, wxGBPosition(2, 3), wxGBSpan(1, 1));
  textbox_equatorx->ChangeValue(wxT("0"));
  textbox_equatory->ChangeValue(wxT("0"));
  textbox_equatorz->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Interpol ratio : ")),
    wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_interpol, this, itext_interpol);
  wxTextCtrl* textbox_interpol = new wxTextCtrl(panel, itext_interpol, wxT(""));
  gbsizer->Add(textbox_interpol, wxGBPosition(3, 1), wxGBSpan(1, 1));
  textbox_interpol->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Fermi energy : ")), 
    wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_shift, this, itext_shift);
  wxTextCtrl* textbox_shift = new wxTextCtrl(panel, itext_shift, wxT(""));
  gbsizer->Add(textbox_shift, wxGBPosition(4, 1), wxGBSpan(1, 1));
  textbox_shift->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Min of Scale : ")), 
    wxGBPosition(5, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemin);
  textbox_min = new wxTextCtrl(panel, itext_colorscalemin, wxT(""));
  gbsizer->Add(textbox_min, wxGBPosition(5, 1), wxGBSpan(1, 1));
  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Max of Scale : ")),
    wxGBPosition(6, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::radiovalue_colorscale, this, itext_colorscalemax);
  textbox_max = new wxTextCtrl(panel, itext_colorscalemax, wxT(""));
  gbsizer->Add(textbox_max, wxGBPosition(6, 1), wxGBSpan(1, 1));

  wxString choices_tetra[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7") ,
wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"),
 wxT("15"), wxT("16") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_tetra, this, iradio_tetra);
  gbsizer->Add(new wxRadioBox(panel, iradio_tetra, wxT("Tetrahedron"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_tetra), choices_tetra,
    4, wxRA_SPECIFY_COLS), wxGBPosition(3,2), wxGBSpan(5, 2));

  wxString choices_colorscale[] = { wxT("Input (1D)"), wxT("Input (2D)"),
    wxT("Input (3D)"), wxT("Fermi Velocity"), wxT("Band Index"),
    wxT("Input (1D, Gray Scale)"), wxT("Fermi Velocity (Gray Scale)") };
  radiobox_color = new wxRadioBox(panel, iradio_colorscale,
    wxT("Color scale mode"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_colorscale), choices_colorscale,
    1, wxRA_SPECIFY_COLS);
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radiovalue_colorscale, this, iradio_colorscale);
  gbsizer->Add(radiobox_color, wxGBPosition(7, 0), wxGBSpan(3, 2));

  wxString choices_bz[] = { wxT("First Brillouin zone"), wxT("Primitive Brillouin zone") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_brillouinzone, this, iradio_brillouinzone);
  gbsizer->Add(new wxRadioBox(panel, iradio_brillouinzone, wxT("Brillouin zone"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_bz), choices_bz,
    1, wxRA_SPECIFY_COLS), wxGBPosition(8, 2), wxGBSpan(1, 2));

  wxString choices_stereo[] = { wxT("None"), wxT("Parallel"), wxT("Cross") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_stereo, this, iradio_stereo);
  gbsizer->Add(new wxRadioBox(panel, iradio_stereo, wxT("Stereogram"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_stereo), choices_stereo,
    1, wxRA_SPECIFY_COLS), wxGBPosition(9, 2), wxGBSpan(1, 1));

  wxString choices_mouse[] = { wxT("Rotate"), wxT("Scale"), wxT("Translate") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_mouse, this, iradio_mouse);
  gbsizer->Add(new wxRadioBox(panel, iradio_mouse, wxT("Mouse Drag"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_mouse), choices_mouse,
    1, wxRA_SPECIFY_COLS), wxGBPosition(9, 3), wxGBSpan(1, 1));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("BZ number : ")),
    wxGBPosition(10, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BZ_number, this, itext_BZ_number0);
  textbox_BZ_number0 = new wxTextCtrl(panel, itext_BZ_number0, wxT(""));
  gbsizer->Add(textbox_BZ_number0, wxGBPosition(10, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BZ_number, this, itext_BZ_number1);
  textbox_BZ_number1 = new wxTextCtrl(panel, itext_BZ_number1, wxT(""));
  gbsizer->Add(textbox_BZ_number1, wxGBPosition(10, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BZ_number, this, itext_BZ_number2);
  textbox_BZ_number2 = new wxTextCtrl(panel, itext_BZ_number2, wxT(""));
  gbsizer->Add(textbox_BZ_number2, wxGBPosition(10, 3), wxGBSpan(1, 1));
  textbox_BZ_number0->ChangeValue(wxT("1"));
  textbox_BZ_number1->ChangeValue(wxT("1"));
  textbox_BZ_number2->ChangeValue(wxT("1"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Background (RGB) : ")),
    wxGBPosition(11, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundR);
  textbox_BackGroundR = new wxTextCtrl(panel, itext_BackGroundR, wxT(""));
  gbsizer->Add(textbox_BackGroundR, wxGBPosition(11, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundG);
  textbox_BackGroundG = new wxTextCtrl(panel, itext_BackGroundG, wxT(""));
  gbsizer->Add(textbox_BackGroundG, wxGBPosition(11, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_BackGround, this, itext_BackGroundB);
  textbox_BackGroundB = new wxTextCtrl(panel, itext_BackGroundB, wxT(""));
  gbsizer->Add(textbox_BackGroundB, wxGBPosition(11, 3), wxGBSpan(1, 1));
  textbox_BackGroundR->ChangeValue(wxT("0"));
  textbox_BackGroundG->ChangeValue(wxT("0"));
  textbox_BackGroundB->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Line color (RGB) : ")),
    wxGBPosition(12, 0), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorR);
  textbox_LineColorR = new wxTextCtrl(panel, itext_LineColorR, wxT(""));
  gbsizer->Add(textbox_LineColorR, wxGBPosition(12, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorG);
  textbox_LineColorG = new wxTextCtrl(panel, itext_LineColorG, wxT(""));
  gbsizer->Add(textbox_LineColorG, wxGBPosition(12, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_LineColor, this, itext_LineColorB);
  textbox_LineColorB = new wxTextCtrl(panel, itext_LineColorB, wxT(""));
  gbsizer->Add(textbox_LineColorB, wxGBPosition(12, 3), wxGBSpan(1, 1));
  textbox_LineColorR->ChangeValue(wxT("1"));
  textbox_LineColorG->ChangeValue(wxT("1"));
  textbox_LineColorB->ChangeValue(wxT("1"));

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::textctrl_view, this, ibutton_rotate);
  gbsizer->Add(new wxButton(panel, ibutton_rotate, wxT("Rotate")), 
    wxGBPosition(13, 0), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotx);
  textbox_rotatex = new wxTextCtrl(panel, itext_rotx, wxT(""));
  gbsizer->Add(textbox_rotatex, wxGBPosition(13, 1), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_roty);
  textbox_rotatey = new wxTextCtrl(panel, itext_roty, wxT(""));
  gbsizer->Add(textbox_rotatey, wxGBPosition(13, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_rotz);
  textbox_rotatez = new wxTextCtrl(panel, itext_rotz, wxT(""));
  gbsizer->Add(textbox_rotatez, wxGBPosition(13, 3), wxGBSpan(1, 1));
  textbox_rotatex->ChangeValue(wxT("0"));
  textbox_rotatey->ChangeValue(wxT("0"));
  textbox_rotatez->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Position : ")), 
    wxGBPosition(14, 1), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positionx);
  textbox_positionx = new wxTextCtrl(panel, itext_positionx, wxT(""));
  gbsizer->Add(textbox_positionx, wxGBPosition(14, 2), wxGBSpan(1, 1));
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_positiony);
  textbox_positiony = new wxTextCtrl(panel, itext_positiony, wxT(""));
  gbsizer->Add(textbox_positiony, wxGBPosition(14, 3), wxGBSpan(1, 1));
  textbox_positionx->ChangeValue(wxT("0"));
  textbox_positiony->ChangeValue(wxT("0"));

  gbsizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Scale : ")), 
    wxGBPosition(15, 2), wxGBSpan(1, 1), wxALIGN_RIGHT);
  Bind(wxEVT_COMMAND_TEXT_UPDATED, &MyFrame::textctrl_view, this, itext_scale);
  textbox_scale = new wxTextCtrl(panel, itext_scale, wxT(""));
  gbsizer->Add(textbox_scale, wxGBPosition(15, 3), wxGBSpan(1, 1));
  textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_colorbar, this, icheck_colorbar);
  wxCheckBox* check = new wxCheckBox(panel, icheck_colorbar, wxT("Color bar"));
  gbsizer->Add(check, wxGBPosition(15, 1), wxGBSpan(1, 1));
  check->SetValue(true);
  // debug fileMenu->Check(menu_colorbar_check, true);

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::checkvalue_equator, this, icheck_equator);
  gbsizer->Add(new wxCheckBox(panel, icheck_equator, wxT("Equator")), wxGBPosition(16, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_nodeline, this, icheck_nodeline);
  gbsizer->Add(new wxCheckBox(panel, icheck_nodeline, wxT("Nodal line")), wxGBPosition(17, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::radiovalue_section, this, icheck_section);
  gbsizer->Add(new wxCheckBox(panel, icheck_section, wxT("Section")), wxGBPosition(18, 1), wxGBSpan(1, 1));

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::button_section, this, ibutton_section);
  gbsizer->Add(new wxButton(panel, ibutton_section, wxT("Section file")),
    wxGBPosition(19, 1), wxGBSpan(1, 1));

  wxString choices_light[] = { wxT("Both"), wxT("Unoccupy"), wxT("Occupy") };
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_lighting, this, iradio_lighting);
  gbsizer->Add(new wxRadioBox(panel, iradio_lighting, wxT("Lighting"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_light), choices_light,
    1, wxRA_SPECIFY_COLS), wxGBPosition(16, 2), wxGBSpan(4, 1));

  wxString choices_BarColor[] = { wxT("BGR"), wxT("CMY"), wxT("MCY")};
  Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &MyFrame::radio_BarColor, this, iradio_BarColor);
  gbsizer->Add(new wxRadioBox(panel, iradio_BarColor, wxT("Bar Color"),
    wxDefaultPosition, wxDefaultSize,
    WXSIZEOF(choices_BarColor), choices_BarColor,
    1, wxRA_SPECIFY_COLS), wxGBPosition(16, 3), wxGBSpan(4, 1));

  SetSizer(sizermain);
  SetAutoLayout(true);

  // Show the frame
  Show(true);
  Raise();

  m_canvas->InitGL();
}

MyFrame::~MyFrame()
{
  delete m_canvas;
}

// Intercept menu commands
function MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  // true is to force the frame to close
  Close(true);
}

function MyFrame::modify_band() {
  let ib, width, height;
  wxCheckBox** check;

  radiobox_color->SetSelection(color_scale - 1);

  check = new wxCheckBox * [nb];

  for (ib = 0; ib < nb; ib++) {
    Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MyFrame::check_band, this, icheck_band + ib);
    check[ib] = new wxCheckBox(panel, icheck_band + ib, 
      wxString::Format(wxT("Band " + toString() + ""), ib));
    gbsizer->Add(check[ib], wxGBPosition(14 + ib, 0), wxGBSpan(1, 1));
    check[ib]->SetValue(true);
  }
  gbsizer->Layout();
  if (lbatch == 1) {
    splitterH->Unsplit();
    splitterV->Unsplit();
  }
  else {
    splitterV->SetSashPosition(-gbsizer->CalcMin().GetX());
  }
  splitterV->Layout();
  splitterH->Layout();
  Refresh(false);
  Raise();
}
/**
 @brief Window resize

 Modify : ::sx, ::sy
*/
function TestGLCanvas::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen())
    return;
  // This is normally only necessary if there is more than one wxGLCanvas
  // or more than one wxGLContext in the application.
  SetCurrent(*m_glRC);

  /*
   Scale of translation of mousepointer
  */
  sx = 1.0 / event.GetSize().x;
  sy = 1.0 / event.GetSize().y;
  // It's up to the application code to update the OpenGL viewport settings.
  // This is OK here only because there is only one canvas that uses the
  // context. See the cube sample for that case that multiple canvases are
  // made current with one context.
  glViewport(0, 0, event.GetSize().x, event.GetSize().y);
  /**/
  glMatrixMode(GL_PROJECTION);
  /**/
  glLoadIdentity();
  gluPerspective(30.0, event.GetSize().x / event.GetSize().y, 1.0, 100.0);
  /**/
  glMatrixMode(GL_MODELVIEW);
  Refresh(false);
  //myf->Show(true);
}
/**
 @brief Glut mouse function

 Modify : ::cx, ::cy, ::scl
*/
function TestGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
  static let dragging = 0;
  static float last_x, last_y;
  let i, j, wheel;
  let dx, dy, a, rot0[3][3], rot1[3][3], ax, ay;

  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  if (event.LeftIsDown())
  {
    if (!dragging)
    {
      dragging = 1;
    }
    else
    {
      /*
       Translation of mousepointer from starting point
      */
      dx = (event.GetX() - last_x) * sx;
      dy = (event.GetY() - last_y) * sy;
      /*
       Distanse from starting point
      */
      a = Math.sqrt(dx * dx + dy * dy);
      /**/
      if (lmouse == 1) {
        /**/
        if (a != 0.0) {
          /*
           Compute rotational matrix from translation of mousepointer
          */
          ax = -dy;
          ay = dx;
          /**/
          a = a * 10.0;
          /**/
          rot0[0][0] = (ax * ax + ay * ay * Math.cos(a)) / (ax * ax + ay * ay);
          rot0[0][1] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
          rot0[0][2] = ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[1][0] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
          rot0[1][1] = (ax * ax * Math.cos(a) + ay * ay) / (ax * ax + ay * ay);
          rot0[1][2] = ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][0] = -ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][1] = -ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][2] = Math.cos(a);
          /**/
          for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) rot1[i][j] = rot[i][j];
          /**/
          for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
              rot[i][j] = rot0[i][0] * rot1[0][j]
                        + rot0[i][1] * rot1[1][j]
                        + rot0[i][2] * rot1[2][j];
            }
          }
          /*
          Print angle to text Box
          */
          thetay = Math.asin(rot[0][2]);
          if (Math.cos(thetay) != 0.0) {
            if (-rot[1][2] / Math.cos(thetay) >= 0.0) thetax = Math.acos(rot[2][2] / Math.cos(thetay));
            else thetax = 6.283185307f - Math.acos(rot[2][2] / Math.cos(thetay));
            if (-rot[0][1] / Math.cos(thetay) >= 0.0) thetaz = Math.acos(rot[0][0] / Math.cos(thetay));
            else thetaz = 6.283185307f - Math.acos(rot[0][0] / Math.cos(thetay));
          }
          else {
            thetax = 0.0;
            if (rot[1][0] >= 0.0) thetaz = Math.acos(rot[1][1]);
            else thetaz = 6.283185307f - Math.acos(rot[1][1]);
          }
          thetax *= 180.0 / 3.14159265;
          thetay *= 180.0 / 3.14159265;
          thetaz *= 180.0 / 3.14159265;
          myf->textbox_rotatex->ChangeValue(wxString::Format(wxT("%f"), thetax));
          myf->textbox_rotatey->ChangeValue(wxString::Format(wxT("%f"), thetay));
          myf->textbox_rotatez->ChangeValue(wxString::Format(wxT("%f"), thetaz));
          myf->Show(true);
        }
      }
      else if (lmouse == 2) {
        scl = scl * expf(-dy);
        myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
        linewidth *= expf(-dy);
        myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
        myf->Show(true);
      }
      else {
        trans[0] = trans[0] + dx;
        trans[1] = trans[1] - dy;
        myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
        myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
        myf->Show(true);
      }
      Refresh(false);
    }
    last_x = event.GetX();
    last_y = event.GetY();
  }
  else
  {
    dragging = 0;
  }

  wheel = event.GetWheelRotation();
  if (wheel > 0) {
    scl = scl * 1.1; 
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 1.1;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  else if (wheel < 0) {
    scl = scl * 0.9;
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 0.9;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  if (event.LeftDClick()) {
    trans[0] = (event.GetX() * sx * 2.0 - 1.0) / scl;
    trans[1] = -(event.GetY() * sy * 2.0 - 1.0) / scl;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    Refresh(false);
  }
}
/**
 @brief Glut special key function

 Modify : ::trans
*/
function TestGLCanvas::OnChar(wxKeyEvent& event)
{
  switch (event.GetKeyCode())
  {
  case 'a':
  case WXK_LEFT:
    trans[0] += - 0.1;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->Show(true);
    Refresh(false);
    break;

  case 'd':
  case WXK_RIGHT:
    trans[0] += 0.1;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->Show(true);
    Refresh(false);
    break;

  case 'w':
  case WXK_UP:
    trans[1] += 0.1;
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    myf->Show(true);
    Refresh(false);
    break;

  case 's':
  case WXK_DOWN:
    trans[1] += - 0.1;
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    myf->Show(true);
    Refresh(false);
    break;

  default:
    event.Skip();
    return;
  }
}

function TestGLCanvas::InitGL()
{
  // Make the new context current (activate it for use) with this canvas.
  SetCurrent(*m_glRC);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_COLOR_MATERIAL);
  PostSizeEventToParent();
}

TestGLCanvas::TestGLCanvas(wxWindow* parent,
  wxWindowID id,
  let* gl_attrib)
  : wxGLCanvas(parent, id, gl_attrib)
{
  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);
}

TestGLCanvas::~TestGLCanvas()
{
  delete m_glRC;
}
/*
 Allocation of Kohn-Sham energies $ matrix elements
*/
function allocate_griddata(
let ng[3],
let ng0[3]
) 
{
  let i, j, ib, i0, i1, i2;

  for (i = 0; i < 3; i++) ng[i] = ng0[i];

  ntri = new let[nb];
  nnl = new let[nb];
  n2d = new let[nb];
  nequator = new let[nb];
  draw_band = new let[nb];
  for (ib = 0; ib < nb; ib++) draw_band[ib] = 1;

  scl /= Math.sqrt(bvec[0][0] * bvec[0][0] + bvec[0][1] * bvec[0][1] + bvec[0][2] * bvec[0][2]);
  linewidth /= scl;
  myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
  myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
  /*
   Direct lattice vector
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) avec[i][j] = 0.0;
    avec[i][i] = 1.0;
    solve3(bvec, avec[i]);
    terminal"    avec " + toString() + " : " + toString() + " " + toString() + " " + toString() + " \n"),
      i + 1, avec[i][0], avec[i][1], avec[i][2]);
  }/*for (i = 0; i < 3; ++i)*/
  for (i = 0; i < 3; ++i) {
    secvec[i] = bvec[2][i];
    eqvec[i] = bvec[2][i];
    eqvec_fr[i] = 0.0;
    secvec_fr[i] = 0.0;
  }
  eqvec_fr[2] = 1.0;
  secvec_fr[2] = 1.0;
  secscale = 0.0;

  eig0 = new let * **[nb];
  eig = new let * **[nb];
  mat0 = new let * ***[nb];
  mat = new let * ***[nb];
  vf = new let * ***[nb];
  for (ib = 0; ib < nb; ib++) {
    eig0[ib] = new let * *[ng0[0]];
    eig[ib] = new let * *[ng0[0]];
    mat0[ib] = new let * **[ng0[0]];
    mat[ib] = new let * **[ng0[0]];
    vf[ib] = new let * **[ng0[0]];
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib][i0] = new let * [ng0[1]];
      eig[ib][i0] = new let * [ng0[1]];
      mat0[ib][i0] = new let * *[ng0[1]];
      mat[ib][i0] = new let * *[ng0[1]];
      vf[ib][i0] = new let * *[ng0[1]];
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0][i1] = new let[ng0[2]];
        eig[ib][i0][i1] = new let[ng0[2]];
        mat0[ib][i0][i1] = new let * [ng0[2]];
        mat[ib][i0][i1] = new let * [ng0[2]];
        vf[ib][i0][i1] = new let * [ng0[2]];
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          mat0[ib][i0][i1][i2] = new let[3];
          mat[ib][i0][i1][i2] = new let[3];
          vf[ib][i0][i1][i2] = new let[3];
        }
      }
    }
  }
}
/**
 @brief Input from Fermi surface file
*/
let read_file()
{
  let ib, i, j, i0, i1, i2, ii0, ii1, ii2, ierr, iaxis;
  FILE *fp;
  char* ctemp1;
  char ctemp2[256];
  let lshift; //!< Switch for shifted Brillouin zone
  /*
   Open input file.
  */
  terminal("  Openning ") << frmsf_file_name << wxT(" ...\n");
  if ((fp = fopen(frmsf_file_name.mb_str(), "r")) == NULL) {
    terminal("file open error!!\n");
    terminal("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }
  terminal("\n");
  terminal("  ##  Brillouin zone informations  ###########\n");
  terminal("\n");
  /*
   k-point grid
  */
  ctemp1 = fgets(ctemp2, 256, fp);
  ierr = sscanf(ctemp2, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);

  if (ierr == 0) terminal("error ! reading ng\n");
  terminal"    k point grid : " + toString() + " " + toString() + " " + toString() + "\n"),
    ng0[0], ng0[1], ng0[2]);
  /*
   Shift of k-point grid
  */
  ierr = fscanf(fp, "%d", &lshift);
  if (ierr == 0) terminal("error ! reading lshift\n");

  if (lshift == 0) {
    terminal("    k point grid is the Monkhorst-Pack grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    terminal("    k point grid starts from Gamma.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    terminal("    k point grid starts from Gamma + a half grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  if (ierr == 0) terminal("error ! reading nb\n");
  terminal"    # of bands : " + toString() + "\n"), nb);
  /*
   Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
    if (ierr == 0) terminal("error ! reading bvec\n");
    terminal"    bvec " + toString() + " : " + toString() + " " + toString() + " " + toString() + " \n"), i + 1, bvec[i][0], bvec[i][1], bvec[i][2]);
  }/*for (i = 0; i < 3; ++i)*/
  allocate_griddata(ng, ng0);
  /*
   Kohn-Sham energies
  */
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      if (lshift != 0) ii0 = i0;
      else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        if (lshift != 0) ii1 = i1;
        else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (lshift != 0) ii2 = i2;
          else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
          ierr = fscanf(fp, "%e", &eig0[ib][ii0][ii1][ii2]);
        }
      }
    }
  }
  /*
   Matrix elements
  */
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < ng0[0]; ++i0) {
        if (lshift != 0) ii0 = i0;
        else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
        for (i1 = 0; i1 < ng0[1]; ++i1) {
          if (lshift != 0) ii1 = i1;
          else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
          for (i2 = 0; i2 < ng0[2]; ++i2) {
            if (lshift != 0) ii2 = i2;
            else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
            ierr = fscanf(fp, "%e", &mat0[ib][ii0][ii1][ii2][iaxis]);
            if (ierr == EOF) {
              fclose(fp);
              return iaxis;
            }
          }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
        }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
      }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }
  fclose(fp);
  return 3;
} /* read_file */
/*
  @brief Make all characters lower
*/
function Text2Lower(char *value //!<[inout] @brief Keyword or value
) {
  char value2;
  let valuelen, ii;

  valuelen = strlen(value);
  for (ii = 0; ii < valuelen; ii++) {
    value2 = tolower(value[ii]);
    value[ii] = value2;
  }
}/*function Text2Lower*/
function read_bxsf()
{
  FILE* fp;
  char ctmp[256], ctmpEF1[16], ctmpEF2[16];
  let ierr, ii, ib, i0, i1, i2, ii0, ii1, ii2;
  char* cerr;
  double EF;

  if ((fp = fopen(frmsf_file_name.mb_str(), "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }
  terminal("\n#####  Reading BXSF file ") 
    << frmsf_file_name << wxT(" #####\n\n");

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "Fermi Energy:") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  ierr = sscanf(ctmp, "%s %s %lf", ctmpEF1, ctmpEF2, &EF);
  terminal"  Fermi energy : %le\n"), EF);

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "BEGIN_BLOCK_BANDGRID_3D") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  cerr = fgets(ctmp, 256, fp);
  cerr = fgets(ctmp, 256, fp);

  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d", &nb);
  terminal"  Number of bands : " + toString() + "\n"), nb);
  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);
  for (ii = 0; ii < 3; ii++) ng0[ii] -= 1;
  terminal"  k point grid : " + toString() + " " + toString() + " " + toString() + "\n"), 
    ng0[0], ng0[1], ng0[2]);

  cerr = fgets(ctmp, 256, fp);
  for (ii = 0; ii < 3; ++ii) {
    cerr = fgets(ctmp, 256, fp);
    ierr = sscanf(ctmp, "%e%e%e", &bvec[ii][0], &bvec[ii][1], &bvec[ii][2]);
    terminal("  Bvec " + toString(ii) + " : " + toString(bvec[ii][0]) + " " + toString(bvec[ii][1]) + " " + toString(bvec[ii][2]) + "\n");
  }
  allocate_griddata(ng, ng0);

  for (ib = 0; ib < nb; ib++) {
    cerr = fgets(ctmp, 256, fp);
    terminal"  Reading %s"), ctmp);

    for (i0 = 0; i0 <= ng0[0]; i0++) {
      if (i0 == ng0[0]) ii0 = 0;
      else ii0 = i0;
      for (i1 = 0; i1 <= ng0[1]; i1++) {
        if (i1 == ng0[1]) ii1 = 0;
        else ii1 = i1;
        for (i2 = 0; i2 <= ng0[2]; i2++) {
          if (i2 == ng0[2]) ii2 = 0;
          else ii2 = i2;

          ierr = fscanf(fp, "%e", &eig0[ib][ii0][ii1][ii2]);
          eig0[ib][ii0][ii1][ii2] -= EF;
        }/*for (i2 = 0; i2 < Ng[2]; i2++)*/
      }/*for (i1 = 0; i1 < Ng[1]; i1++)*/
    }/*for (i0 = 0; i0 < Ng[0]; i0++)*/

    cerr = fgets(ctmp, 256, fp);

  }/*for (ib = 0; ib < Nb; ib++)*/
  for (ii = 0; ii < 3; ii++) shiftk[ii] = 0;
  color_scale = 4;
}/*function read_bxsf*/
/**
 @brief Project 3D \f$k\f$-vector into 2D plane. 

 Modify: ::kv2d
*/
function proj_2d(
  axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
  vec = [0.0, 0.0, 0.0]//!< [in,out] Line ends to be projected
) 
{
  let ii = 0, kk = 0;
  let vec0 = [0.0, 0.0, 0.0];

  for (kk = 0; kk < 2; kk++) {
    vec0[kk] = 0.0;
    for (ii = 0; ii < 3; ii++)
      vec0[kk] += axis2d[kk][ii] * vec[ii];
  }/*for (kk = 0; kk < 2; kk++)*/
  vec0[2] = 0.0;
  for (kk = 0; kk < 3; kk++)vec[kk] = vec0[kk];
}/*proj_2d*/
/**
 @brief Set Projection axis for 2D plane

 Modify : ::axis2d
*/
function set2daxis(
  secvec = [0.0, 0.0, 0.0],
  axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
) {
  let ii, jj;
  let snorm, norm, thr = 0.001;

  snorm = 0.0;
  for (ii = 0; ii < 3; ii++) snorm += secvec[ii] * secvec[ii];
  /*
   Define the first axis
  */
  for (ii = 0; ii < 3; ii++) {
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] = 0.0;
    axis2d[0][ii] = 1.0;
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] += - secvec[ii] * secvec[jj] / snorm;

    norm = 0.0;
    for (jj = 0; jj < 3; jj++) norm += axis2d[0][jj] * axis2d[0][jj];
    norm = Math.sqrt(norm);
    if (norm > thr) {
      for (jj = 0; jj < 3; jj++) axis2d[0][jj] /= norm;
      break;
    }/*if (norm > 0.thr)*/
  }/*for (ii = 0; ii < 3; ii++)*/
  /*
   Define the second axis with outor product
  */
  axis2d[1][0] = secvec[1] * axis2d[0][2] - secvec[2] * axis2d[0][1];
  axis2d[1][1] = secvec[2] * axis2d[0][0] - secvec[0] * axis2d[0][2];
  axis2d[1][2] = secvec[0] * axis2d[0][1] - secvec[1] * axis2d[0][0];
  norm = 0.0;
  for (jj = 0; jj < 3; jj++) norm += axis2d[1][jj] * axis2d[1][jj];
  norm = Math.sqrt(norm);
  for (jj = 0; jj < 3; jj++) axis2d[1][jj] /= norm;
}/*function set_2daxis*/
/**
 @brief Judge wheser this line is the edge of 1st BZ (or the premitive BZ)
*/
let bragg_vert2d(
  let nbragg,
  let bragg[26][3],
  let brnrm[26],
  let secvec[3],
  let secscale,
  let jbr, //!< [in] Index of a Bragg plane
  let nbr, //!< [in]
  let vert[3], //!< [inout] start point of line
  let vert2[3] //!< [in] end point of line
)
{
  let kbr, i, lbr, nbr0;
  let bmat[3][3], rhs[3], prod, thr, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < nbragg; ++kbr) {
    /**/
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = secvec[i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = 0.0;
    for (i = 0; i < 3; ++i)rhs[0] += secvec[i] * secvec[i];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = Math.sqrt(rhs[0] * rhs[1] * rhs[2]) * 0.001;
    rhs[0] *= secscale;
    /*
    if Bragg planes do not cross, roop next kbr
    */
    det = solve3(bmat, rhs);
    if (Math.abs(det) < thr) continue;
    /*
    if vert0 = vert1, roop next kbr
    */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
         + (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
         + (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if (prod < thr) continue;
    /*
    is corner really in 1st BZ ?
    */
    i = 0;
    for (lbr = 0; lbr < nbragg; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }/*for (lbr = 0; lbr < nbragg; ++lbr)*/
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }/*for (kbr = nbr0; kbr < nbragg; ++kbr)*/
  /*
  this line is not a BZ boundary
  */
  return 0;
}/* bragg_vert2d */
/**
 @brief Compute boundary of 2D BZ

 Modify : ::nbzl2d, ::bzl2d_proj
*/
function calc_2dbz() {
  let jbr, nbr, i, j, lvert, ibzl;
  let vert[2][3], vec[26][2][3], prod, thr;

  if (fbz != 1)return;
  /*
   Set Projection axis for 2D plane
  */
  set2daxis(secvec, axis2d);

  nbzl2d = 0;

  for (jbr = 0; jbr < nbragg; ++jbr) {
    /**/
    for (i = 0; i < 3; ++i) vert[1][i] = 0.0;
    nbr = 0;
    lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[0], vert[1]);
    if (lvert == 0) continue;
    nbr = lvert;
    /**/
    lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[1], vert[0]);
    if (lvert == 0) continue;
    /**/
    for (i = 0; i < 2; ++i) for (j = 0; j < 3; ++j) vec[nbzl2d][i][j] = vert[i][j];
    nbzl2d += 1;
  }/*for (jbr = 0; jbr < nbragg; ++jbr)*/
  /*
   Order bz lines
  */
  for (i = 0; i < 3; i++) bzl2d[0][i] = vec[0][0][i];
  for (i = 0; i < 3; i++) bzl2d[1][i] = vec[0][1][i];
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {

    thr = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[j][i] * bzl2d[j][i];
    thr *= 0.001;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[j][i] - vec[ibzl][j][i]) * (bzl2d[j][i] - vec[ibzl][j][i]);
    if (prod < thr) 
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[1 - j][i] - vec[ibzl][j][i]) * (bzl2d[1 - j][i] - vec[ibzl][j][i]);
    if (prod < thr)
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
  }/*for (ibzl = 1; ibzl < *nbzl2d; ibzl++)*/

  for (jbr = 1; jbr < nbzl2d - 1; jbr++) {

    thr = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[jbr][i] * bzl2d[jbr][i];
    thr *= 0.001;

    prod = 0.0;
    for (ibzl = 0; ibzl < nbzl2d; ibzl++) for (i = 0; i < 3; i++)
      prod += vec[ibzl][0][i] * vec[ibzl][0][i];
    if (prod < thr) {
      nbzl2d = jbr + 1;
      break;
    }

    for (ibzl = 1; ibzl < nbzl2d; ibzl++) {
      prod = (bzl2d[jbr][0] - vec[ibzl][0][0]) * (bzl2d[jbr][0] - vec[ibzl][0][0])
           + (bzl2d[jbr][1] - vec[ibzl][0][1]) * (bzl2d[jbr][1] - vec[ibzl][0][1])
           + (bzl2d[jbr][2] - vec[ibzl][0][2]) * (bzl2d[jbr][2] - vec[ibzl][0][2]);
      if (prod < thr) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][1][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }

      prod = (bzl2d[jbr][0] - vec[ibzl][1][0]) * (bzl2d[jbr][0] - vec[ibzl][1][0])
           + (bzl2d[jbr][1] - vec[ibzl][1][1]) * (bzl2d[jbr][1] - vec[ibzl][1][1])
           + (bzl2d[jbr][2] - vec[ibzl][1][2]) * (bzl2d[jbr][2] - vec[ibzl][1][2]);
      if (prod < thr) {
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][0][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }
    }/*for (ibzl = 1; ibzl < *nbzl2d; ibzl++)*/
  }/*for (ibzl = 1; ibzl < nbzl; ibzl++)*/
  /*
   Project into 2D plane
  */
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {
    for (i = 0; i < 3; i++) bzl2d_proj[ibzl][i] = bzl2d[ibzl][i];
    proj_2d(axis2d, bzl2d_proj[ibzl]);
  }/*for (ibzl = 0; ibzl < *nbzl2d; ibzl++)*/
}/*calc_2dbz()*/
/**
 @brief Compute Fermi-line 

 Modify : ::n2d, ::clr2d, ::kv2d
*/
function calc_section() {
  let i, j, ib, itri, ithread, n2d0;
  std::vector<std::vector<std::vector<std::vector<let> > > > kv2d_v, clr2d_v;

  kv2d_v.resize(nthreads);
  clr2d_v.resize(nthreads);

  if (fbz != 1)return;

  terminal("    band   # of Fermi-line\n");
  for (ib = 0; ib < nb; ib++) {

#pragma omp parallel default(none) \
shared(nb,ib,clr,clr2d_v,kvp,kv2d_v,ntri,secvec,secscale,axis2d) \
private(itri,i,j,ithread)
    {
      let sw[3];
      let norm[3], a[3][3];
      std::vector<std::vector<let> > kv2d_0, clr2d_0;

      kv2d_0.resize(2);
      clr2d_0.resize(2);
      for (i = 0; i < 2; i++) {
        kv2d_0[i).resize(3);
        clr2d_0[i).resize(4);
      }

      ithread = get_thread();
      kv2d_v[ithread).resize(0);
      clr2d_v[ithread).resize(0);

#pragma omp for
      for (itri = 0; itri < ntri[ib]; ++itri) {
        /**/
        for (i = 0; i < 3; i++) {
          norm[i] = 0.0;
          for (j = 0; j < 3; j++) norm[i] += secvec[j] * (kvp[ib][itri][i][j] - secscale * secvec[j]);
        }
        eigsort(3, norm, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.0 - norm[sw[j]]) / (norm[sw[i]] - norm[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/
         /**/
        if ((norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]]) || (norm[sw[0]] <= 0.0 && 0.0 < norm[sw[1]])) {
          for (i = 0; i < 3; ++i) {
            kv2d_0[0)[i)
              = kvp[ib][itri][sw[1]][i] * a[1][0] + kvp[ib][itri][sw[0]][i] * a[0][1];
            kv2d_0[1)[i)
              = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 4; ++i) {
            clr2d_0[0)[i)
              = clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][0]
              + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][1];
            clr2d_0[1)[i)
              = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
              + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
          }/*for (i = 0; i < 4; ++i)*/
          proj_2d(axis2d, &kv2d_0[0)[0));
          proj_2d(axis2d, &kv2d_0[1)[0));
          kv2d_v[ithread).push_back(kv2d_0);
          clr2d_v[ithread).push_back(clr2d_0);
        }/*else if (norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]])*/
        else if ((norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]]) || (norm[sw[1]] <= 0.0 && 0.0 < norm[sw[2]])) {
          for (i = 0; i < 3; ++i) {
            kv2d_0[0)[i)
              = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
            kv2d_0[1)[i)
              = kvp[ib][itri][sw[2]][i] * a[2][1] + kvp[ib][itri][sw[1]][i] * a[1][2];
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 4; ++i) {
            clr2d_0[0)[i)
              = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
              + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
            clr2d_0[1)[i)
              = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][1]
              + clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][2];
          }/*for (i = 0; i < 4; ++i)*/
          proj_2d(axis2d, &kv2d_0[0)[0));
          proj_2d(axis2d, &kv2d_0[1)[0));
          kv2d_v[ithread).push_back(kv2d_0);
          clr2d_v[ithread).push_back(clr2d_0);
        }/*else if (norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/* End of parallel region */
    /*
     Allocation of Fermi-lines
    */
    n2d[ib] = 0;
    for (ithread = 0; ithread < nthreads; ithread++)
      n2d[ib] += kv2d_v[ithread).size();

    terminal"    " + toString() + "       " + toString() + "\n"), ib + 1, n2d[ib]);
    kv2d[ib] = new let[6 * n2d[ib]];
    clr2d[ib] = new let[8 * n2d[ib]];

    n2d0 = 0;
    for (ithread = 0; ithread < nthreads; ithread++) {
      for (itri = 0; itri < kv2d_v[ithread).size(); itri++) {
        for (i = 0; i < 2; i++) {
          for (j = 0; j < 3; j++) {
            kv2d[ib][j + i * 3 + 6 * n2d0] = kv2d_v[ithread)[itri)[i)[j);
          }
          for (j = 0; j < 3; j++) {
            clr2d[ib][j + i * 4 + 8 * n2d0] = clr2d_v[ithread)[itri)[i)[j);
          }
        }
        n2d0 += 1;
      }
    }
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function calc_nodeline()*/
