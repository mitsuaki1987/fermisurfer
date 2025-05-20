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
@brief Read .frmsf file
*/
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <cstring>
#include "basic_math.hpp"
#include "variable.hpp"
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(HAVE_GL_GL_H)
#include <GL/gl.h>
#elif defined(HAVE_OPENGL_GL_H)
#include <OpenGL/gl.h>
#endif
#include <wx/wx.h>
/*
 Allocation of Kohn-Sham energies $ matrix elements
*/
static void allocate_griddata(
int ng[3],
int ng0[3]
) 
{
  int i, j, ib, i0, i1, i2, i3;

  for (i = 0; i < 3; i++) ng[i] = ng0[i];

  ntri = new int[nb];
  nnl = new int[nb];
  n2d = new int[nb];
  nequator = new int[nb];
  draw_band = new int[nb];
  rgb_band = new GLfloat *[nb];
  for (ib = 0; ib < nb; ib++) {
    draw_band[ib] = 1;
    rgb_band[ib] = new GLfloat [4];
  }

  scl /= sqrtf(bvec[0][0] * bvec[0][0] + bvec[0][1] * bvec[0][1] + bvec[0][2] * bvec[0][2]);
  myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
  /*
   Direct lattice vector
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) avec[i][j] = 0.0f;
    avec[i][i] = 1.0f;
    solve3(bvec, avec[i]);
    *terminal << wxString::Format(wxT("    avec %d : %f %f %f \n"),
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

  eig0 = new GLfloat * **[nb];
  eig = new GLfloat * **[nb];
  mat0 = new GLfloat * ***[nb];
  mat = new GLfloat * ***[nb];
  vf = new GLfloat * ***[nb];
  for (ib = 0; ib < nb; ib++) {
    eig0[ib] = new GLfloat * *[ng0[0]];
    eig[ib] = new GLfloat * *[ng0[0]];
    mat0[ib] = new GLfloat * **[ng0[0]];
    mat[ib] = new GLfloat * **[ng0[0]];
    vf[ib] = new GLfloat * **[ng0[0]];
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib][i0] = new GLfloat * [ng0[1]];
      eig[ib][i0] = new GLfloat * [ng0[1]];
      mat0[ib][i0] = new GLfloat * *[ng0[1]];
      mat[ib][i0] = new GLfloat * *[ng0[1]];
      vf[ib][i0] = new GLfloat * *[ng0[1]];
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0][i1] = new GLfloat[ng0[2]];
        eig[ib][i0][i1] = new GLfloat[ng0[2]];
        mat0[ib][i0][i1] = new GLfloat * [ng0[2]];
        mat[ib][i0][i1] = new GLfloat * [ng0[2]];
        vf[ib][i0][i1] = new GLfloat * [ng0[2]];
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          mat0[ib][i0][i1][i2] = new GLfloat[3];
          mat[ib][i0][i1][i2] = new GLfloat[3];
          vf[ib][i0][i1][i2] = new GLfloat[3];
          for (i3 = 0; i3 < 3; i3++) mat0[ib][i0][i1][i2][i3] = 0.0f;
        }
      }
    }
  }
}
/**
 @brief Input from Fermi surface file
*/
int read_file()
{
  int ib, i, i0, i1, i2, ii0, ii1, ii2, ierr, iaxis;
  FILE *fp;
  char* ctemp1;
  char ctemp2[256];
  int lshift; //!< Switch for shifted Brillouin zone
  /*
   Open input file.
  */
  *terminal << wxT("  Openning ") << frmsf_file_name << wxT(" ...\n");
  if ((fp = fopen(frmsf_file_name.mb_str(), "r")) == NULL) {
    *terminal << wxT("file open error!!\n");
    *terminal << wxT("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }
  *terminal << wxT("\n");
  *terminal << wxT("  ##  Brillouin zone informations  ###########\n");
  *terminal << wxT("\n");
  /*
   k-point grid
  */
  ctemp1 = fgets(ctemp2, 256, fp);
  ierr = sscanf(ctemp2, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);

  if (ierr == 0) *terminal << wxT("error ! reading ng\n");
  *terminal << wxString::Format(wxT("    k point grid : %d %d %d\n"),
    ng0[0], ng0[1], ng0[2]);
  /*
   Shift of k-point grid
  */
  ierr = fscanf(fp, "%d", &lshift);
  if (ierr == 0) *terminal << wxT("error ! reading lshift\n");

  if (lshift == 0) {
    *terminal << wxT("    k point grid is the Monkhorst-Pack grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    *terminal << wxT("    k point grid starts from Gamma.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    *terminal << wxT("    k point grid starts from Gamma + a half grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   # of bands
  */
  ierr = fscanf(fp, "%d", &nb);
  if (ierr == 0) *terminal << wxT("error ! reading nb\n");
  *terminal << wxString::Format(wxT("    # of bands : %d\n"), nb);
  /*
   Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    ierr = fscanf(fp, "%e%e%e", &bvec[i][0], &bvec[i][1], &bvec[i][2]);
    if (ierr == 0) *terminal << wxT("error ! reading bvec\n");
    *terminal << wxString::Format(wxT("    bvec %d : %f %f %f \n"), i + 1, bvec[i][0], bvec[i][1], bvec[i][2]);
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
static void Text2Lower(char *value //!<[inout] @brief Keyword or value
) {
  char value2;
  int valuelen, ii;

  valuelen = strlen(value);
  for (ii = 0; ii < valuelen; ii++) {
    value2 = tolower(value[ii]);
    value[ii] = value2;
  }
}/*static void Text2Lower*/
void read_batch()
{
  char keyword[256], value[256];
  FILE *fp;
  char *ctmp;
  int ierr, ib, iminmax;

  *terminal << wxT("  Openning ") << batch_name << wxT(" ...\n");
  if ((fp = fopen(batch_name.mb_str(), "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }

  iminmax = 0;
  *terminal << wxT("  Reading...\n");
  while (fscanf(fp, "%s", keyword) != EOF) {

    Text2Lower(keyword);
    *terminal << wxString::Format(wxT("%s\n"), keyword);
    if (keyword[0] == '#') {
      ctmp = fgets(keyword, 256, fp);
      continue;
    }

    if (strcmp(keyword, "backgroundcolor") == 0) {
      ierr = fscanf(fp, "%f%f%f", &BackGroundColor[0], &BackGroundColor[1], &BackGroundColor[2]);
    }
    else if (strcmp(keyword, "band") == 0) {
      for (ib = 0; ib < nb; ib++)
        ierr = fscanf(fp, "%d", &draw_band[ib]);
    }
    else if (strcmp(keyword, "brillouinzone") == 0) {
      ierr = fscanf(fp, "%s", value);
      Text2Lower(value);
      if (strcmp(value, "first") == 0) fbz = 1;
      else if (strcmp(value, "primitive") == 0) fbz = -1;
      else {
        printf("Error! %s = %s", keyword, value);
        exit(-1);
      }
      refresh_patch = 1;
    }
    else if (strcmp(keyword, "colorbar") == 0) {
      ierr = fscanf(fp, "%d", &lcolorbar);
    }
    else if (strcmp(keyword, "colorscale") == 0) {
      ierr = fscanf(fp, "%s", value);
      Text2Lower(value);
      if (strcmp(value, "input1d") == 0) color_scale = 1;
      else if (strcmp(value, "input2d") == 0) color_scale = 2;
      else if (strcmp(value, "input3d") == 0) color_scale = 3;
      else if (strcmp(value, "fermivelocity") == 0) color_scale = 4;
      else if (strcmp(value, "bandindex") == 0) color_scale = 5;
      else if (strcmp(value, "inputgray") == 0) color_scale = 6;
      else if (strcmp(value, "fermivelocitygray") == 0) color_scale = 7;
      else {
        printf("Error! %s = %s", keyword, value);
        exit(-1);
      }
      refresh_color = 1;
    }
    else if (strcmp(keyword, "minmax") == 0) {
      ierr = fscanf(fp, "%f%f", &patch_min, &patch_max);
      skip_minmax = 1;
      refresh_color = 1;
    }
    else if (strcmp(keyword, "equator") == 0) {
      ierr = fscanf(fp, "%f%f%f", &eqvec[0], &eqvec[1], &eqvec[2]);
      lequator = 1;
      refresh_equator = 1;
    }
    else if (strcmp(keyword, "interpol") == 0) {
      ierr = fscanf(fp, "%d", &interpol);
      refresh_interpol = 1;
    }
    else if (strcmp(keyword, "linecolor") == 0) {
      ierr = fscanf(fp, "%f%f%f", &LineColor[0], &LineColor[1], &LineColor[2]);
    }
    else if (strcmp(keyword, "linewidth") == 0) {
      ierr = fscanf(fp, "%f", &linewidth);
    }
    else if (strcmp(keyword, "lighting") == 0) {
      ierr = fscanf(fp, "%s", value);
      Text2Lower(value);
      if (strcmp(value, "both") == 0) {
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        side = 1.0;
      }
      else if (strcmp(value, "unoccupied") == 0) {
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        side = 1.0;
      }
      else if (strcmp(value, "occupied") == 0) {
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        side = -1.0;
      }
      else {
        printf("Error! %s = %s", keyword, value);
        exit(-1);
      }
    }
    else if (strcmp(keyword, "nodalline") == 0) {
      ierr = fscanf(fp, "%d", &nodeline);
    }
    else if (strcmp(keyword, "section") == 0) {
      ierr = fscanf(fp, "%f%f%f", &secvec[0], &secvec[1], &secvec[2]);
      lsection = 1;
      refresh_section = 1;
    }
    else if (strcmp(keyword, "acrossgamma") == 0) {
      ierr = fscanf(fp, "%f", &secscale);
      secscale = 1.0f - secscale;
      refresh_section = 1;
    }
    else if (strcmp(keyword, "position") == 0) {
      ierr = fscanf(fp, "%f%f%f", &trans[0], &trans[1], &trans[2]);
    }
    else if (strcmp(keyword, "scale") == 0) {
      ierr = fscanf(fp, "%f", &scl);
    }
    else if (strcmp(keyword, "rotation") == 0) {
      ierr = fscanf(fp, "%f%f%f", &thetax, &thetay, &thetaz);
    }
    else if (strcmp(keyword, "fermienergy") == 0) {
      ierr = fscanf(fp, "%f", &EF);
      refresh_patch = 1;
    }
    else if (strcmp(keyword, "stereogram") == 0) {
      ierr = fscanf(fp, "%s", value);
      Text2Lower(value);
      if (strcmp(value, "none") == 0) lstereo = 1;
      else if (strcmp(value, "parallel") == 0) lstereo = 2;
      else if (strcmp(value, "cross") == 0) lstereo = 3;
      else {
        printf("Error! %s = %s", keyword, value);
        exit(-1);
      }
    }
    else if (strcmp(keyword, "tetrahedron") == 0) {
      ierr = fscanf(fp, "%d", &itet);
      refresh_patch = 1;
    }
    else if (strcmp(keyword, "bznumber") == 0) {
      ierr = fscanf(fp, "%d%d%d", &BZ_number[0], &BZ_number[1], &BZ_number[2]);
      refresh_section = 1;
    }
    else if (strcmp(keyword, "perspective") == 0) {
      ierr = fscanf(fp, "%d", &lperspective);
    }
    else {
      printf("Error! %s", keyword);
      exit(-1);
    }
  }
  fclose(fp);

  glClearColor(BackGroundColor[0], BackGroundColor[1], BackGroundColor[2], BackGroundColor[2]);

  thetax = 3.14159265f / 180.0f * thetax;
  thetay = 3.14159265f / 180.0f * thetay;
  thetaz = 3.14159265f / 180.0f * thetaz;

  rot[0][0] = cosf(thetay)* cosf(thetaz);
  rot[0][1] = -cosf(thetay)* sinf(thetaz);
  rot[0][2] = sinf(thetay);
  rot[1][0] = cosf(thetaz)* sinf(thetax)* sinf(thetay) + cosf(thetax)* sinf(thetaz);
  rot[1][1] = cosf(thetax) * cosf(thetaz) - sinf(thetax)* sinf(thetay)* sinf(thetaz);
  rot[1][2] = -cosf(thetay)* sinf(thetax);
  rot[2][0] = -cosf(thetax)* cosf(thetaz)* sinf(thetay) + sinf(thetax)* sinf(thetaz);
  rot[2][1] = cosf(thetaz)* sinf(thetax) + cosf(thetax)* sinf(thetay)* sinf(thetaz);
  rot[2][2] = cosf(thetax)* cosf(thetay);
}

void read_bxsf()
{
  FILE* fp;
  char ctmp[256], ctmpEF1[16], ctmpEF2[16];
  int ierr, ii, ib, i0, i1, i2, ii0, ii1, ii2;
  char* cerr;
  double EF;

  if ((fp = fopen(frmsf_file_name.mb_str(), "r")) == NULL) {
    printf("file open error!!\n");
    printf("  Press any key to exit.\n");
    ierr = getchar();
    exit(EXIT_FAILURE);
  }
  *terminal << wxT("\n#####  Reading BXSF file ") 
    << frmsf_file_name << wxT(" #####\n\n");

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "Fermi Energy:") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  ierr = sscanf(ctmp, "%s %s %lf", ctmpEF1, ctmpEF2, &EF);
  *terminal << wxString::Format(wxT("  Fermi energy : %le\n"), EF);

  cerr = fgets(ctmp, 256, fp);
  while (strstr(ctmp, "BEGIN_BLOCK_BANDGRID_3D") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  while (strstr(ctmp, "BEGIN_BANDGRID_3D") == NULL) {
    cerr = fgets(ctmp, 256, fp);
  }
  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d", &nb);
  *terminal << wxString::Format(wxT("  Number of bands : %d\n"), nb);
  cerr = fgets(ctmp, 256, fp);
  ierr = sscanf(ctmp, "%d%d%d", &ng0[0], &ng0[1], &ng0[2]);
  for (ii = 0; ii < 3; ii++) ng0[ii] -= 1;
  *terminal << wxString::Format(wxT("  k point grid : %d %d %d\n"), 
    ng0[0], ng0[1], ng0[2]);

  cerr = fgets(ctmp, 256, fp);
  for (ii = 0; ii < 3; ++ii) {
    cerr = fgets(ctmp, 256, fp);
    ierr = sscanf(ctmp, "%e%e%e", &bvec[ii][0], &bvec[ii][1], &bvec[ii][2]);
    *terminal << wxString::Format(wxT("  Bvec %d : %f %f %f\n"), 
      ii, bvec[ii][0], bvec[ii][1], bvec[ii][2]);
  }
  allocate_griddata(ng, ng0);

  for (ib = 0; ib < nb; ib++) {
    cerr = fgets(ctmp, 256, fp);
    while (strstr(ctmp, "BAND:") == NULL) {
      cerr = fgets(ctmp, 256, fp);
    }
    *terminal << wxString::Format(wxT("  Reading %s"), ctmp);

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
}/*void read_bxsf*/
