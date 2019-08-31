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
@brief Create & modify right-click menu. And operate their function.
*/
#include "free_patch.hpp"
#include "fermi_patch.hpp"
#include "calc_nodeline.hpp"
#include "kumo.hpp"
#include "section.hpp"
#include "equator.hpp"
#include "draw.hpp"
#include "variable.hpp"

void compute_patch_segment() {
  if (refresh_interpol == 1){
    interpol_energy();
    refresh_patch = 1;
    refresh_interpol = 0;
  }
  if (refresh_patch == 1) {
    query = 1; fermi_patch();
    query = 0; fermi_patch();
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
    query = 1; calc_nodeline();
    query = 0; calc_nodeline();
    refresh_nodeline = 0;
  }
  if (refresh_section == 1) {
    calc_2dbz();
    query = 1; calc_section();
    query = 0; calc_section();
    refresh_section = 0;
  }
  if (refresh_equator == 1) {
    query = 1; equator();
    query = 0; equator();
    refresh_equator = 0;
  }
}

void refresh_patch_segment() {
  free_patch();
  compute_patch_segment();
}
