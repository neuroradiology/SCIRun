/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

//
// ** SCIRun version of vtkMarchingCubesCases.hh -- added nbr info to cases **
//
// marching cubes case table for generating isosurfaces
//

#include <Core/Algorithms/Fields/MarchingCubes/mcube2.h>

namespace SCIRun {

/* REFERENCED */
int edge_tab[12][2] = {{0,1}, {1,2}, {3,2}, {0,3},
		       {4,5}, {5,6}, {7,6}, {4,7},
		       {0,4}, {1,5}, {3,7}, {2,6}};

//
// Edges to intersect. Three at a time form a triangle. Comments at end of line
// indicate case number (0->255) and base case number (0->15).
//

TRIANGLE_CASES triCases[] = {
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  0},
{{ 0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 25},
{{ 0,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 26},
{{ 1,  3,  8,  9,  1,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 27},
{{ 1, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 22},
{{ 0,  3,  8,  1, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 9, 11,  2,  0,  9,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 30},
{{ 2,  3,  8,  2,  8, 11, 11,  8,  9, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 21},
{{ 0,  2, 10,  8,  0, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 29},
{{ 1,  0,  9,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 1,  2, 10,  1, 10,  9,  9, 10,  8, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 3,  1, 11, 10,  3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 23},
{{ 0,  1, 11,  0, 11,  8,  8, 11, 10, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 3,  0,  9,  3,  9, 10, 10,  9, 11, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 9, 11,  8, 11, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 15},
{{ 4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 41},
{{ 4,  0,  3,  7,  4,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 57},
{{ 0,  9,  1,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 4,  9,  1,  4,  1,  7,  7,  1,  3, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 1, 11,  2,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 3,  7,  4,  3,  4,  0,  1, 11,  2, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9, 11,  2,  9,  2,  0,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 2,  9, 11,  2,  7,  9,  2,  3,  7,  7,  4,  9, -1, -1, -1, -1}, 63},
{{ 8,  7,  4,  3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 61},
{{10,  7,  4, 10,  4,  2,  2,  4,  0, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 9,  1,  0,  8,  7,  4,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 4, 10,  7,  9, 10,  4,  9,  2, 10,  9,  1,  2, -1, -1, -1, -1}, 63},
{{ 3,  1, 11,  3, 11, 10,  7,  4,  8, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1, 11, 10,  1, 10,  4,  1,  4,  0,  7,  4, 10, -1, -1, -1, -1}, 63},
{{ 4,  8,  7,  9, 10,  0,  9, 11, 10, 10,  3,  0, -1, -1, -1, -1}, 63},
{{ 4, 10,  7,  4,  9, 10,  9, 11, 10, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 42},
{{ 9,  4,  5,  0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 0,  4,  5,  1,  0,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 58},
{{ 8,  4,  5,  8,  5,  3,  3,  5,  1, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 1, 11,  2,  9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 3,  8,  0,  1, 11,  2,  4,  5,  9, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 5, 11,  2,  5,  2,  4,  4,  2,  0, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 2,  5, 11,  3,  5,  2,  3,  4,  5,  3,  8,  4, -1, -1, -1, -1}, 63},
{{ 9,  4,  5,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 0,  2, 10,  0, 10,  8,  4,  5,  9, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 0,  4,  5,  0,  5,  1,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 2,  5,  1,  2,  8,  5,  2, 10,  8,  4,  5,  8, -1, -1, -1, -1}, 63},
{{11, 10,  3, 11,  3,  1,  9,  4,  5, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 4,  5,  9,  0,  1,  8,  8,  1, 11,  8, 11, 10, -1, -1, -1, -1}, 63},
{{ 5,  0,  4,  5, 10,  0,  5, 11, 10, 10,  3,  0, -1, -1, -1, -1}, 63},
{{ 5,  8,  4,  5, 11,  8, 11, 10,  8, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 9,  8,  7,  5,  9,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 43},
{{ 9,  0,  3,  9,  3,  5,  5,  3,  7, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 0,  8,  7,  0,  7,  1,  1,  7,  5, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 1,  3,  5,  3,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 51},
{{ 9,  8,  7,  9,  7,  5, 11,  2,  1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  2,  1,  9,  0,  5,  5,  0,  3,  5,  3,  7, -1, -1, -1, -1}, 63},
{{ 8,  2,  0,  8,  5,  2,  8,  7,  5, 11,  2,  5, -1, -1, -1, -1}, 63},
{{ 2,  5, 11,  2,  3,  5,  3,  7,  5, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 7,  5,  9,  7,  9,  8,  3,  2, 10, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9,  7,  5,  9,  2,  7,  9,  0,  2,  2, 10,  7, -1, -1, -1, -1}, 63},
{{ 2, 10,  3,  0,  8,  1,  1,  8,  7,  1,  7,  5, -1, -1, -1, -1}, 63},
{{10,  1,  2, 10,  7,  1,  7,  5,  1, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 9,  8,  5,  8,  7,  5, 11,  3,  1, 11, 10,  3, -1, -1, -1, -1}, 63},
{{ 5,  0,  7,  5,  9,  0,  7,  0, 10,  1, 11,  0, 10,  0, 11, -1}, 63},
{{10,  0, 11, 10,  3,  0, 11,  0,  5,  8,  7,  0,  5,  0,  7, -1}, 63},
{{10,  5, 11,  7,  5, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 39},
{{11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 38},
{{ 0,  3,  8,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9,  1,  0,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 1,  3,  8,  1,  8,  9,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1,  5,  6,  2,  1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 54},
{{ 1,  5,  6,  1,  6,  2,  3,  8,  0, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9,  5,  6,  9,  6,  0,  0,  6,  2, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 5,  8,  9,  5,  2,  8,  5,  6,  2,  3,  8,  2, -1, -1, -1, -1}, 63},
{{ 2, 10,  3, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 55},
{{10,  8,  0, 10,  0,  2, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 0,  9,  1,  2, 10,  3,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 5,  6, 11,  1,  2,  9,  9,  2, 10,  9, 10,  8, -1, -1, -1, -1}, 63},
{{ 6, 10,  3,  6,  3,  5,  5,  3,  1, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 0, 10,  8,  0,  5, 10,  0,  1,  5,  5,  6, 10, -1, -1, -1, -1}, 63},
{{ 3,  6, 10,  0,  6,  3,  0,  5,  6,  0,  9,  5, -1, -1, -1, -1}, 63},
{{ 6,  9,  5,  6, 10,  9, 10,  8,  9, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 5,  6, 11,  4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 4,  0,  3,  4,  3,  7,  6, 11,  5, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1,  0,  9,  5,  6, 11,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  5,  6,  1,  7,  9,  1,  3,  7,  7,  4,  9, -1, -1, -1, -1}, 63},
{{ 6,  2,  1,  6,  1,  5,  4,  8,  7, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1,  5,  2,  5,  6,  2,  3,  4,  0,  3,  7,  4, -1, -1, -1, -1}, 63},
{{ 8,  7,  4,  9,  5,  0,  0,  5,  6,  0,  6,  2, -1, -1, -1, -1}, 63},
{{ 7,  9,  3,  7,  4,  9,  3,  9,  2,  5,  6,  9,  2,  9,  6, -1}, 63},
{{ 3,  2, 10,  7,  4,  8, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 5,  6, 11,  4,  2,  7,  4,  0,  2,  2, 10,  7, -1, -1, -1, -1}, 63},
{{ 0,  9,  1,  4,  8,  7,  2, 10,  3,  5,  6, 11, -1, -1, -1, -1}, 63},
{{ 9,  1,  2,  9,  2, 10,  9, 10,  4,  7,  4, 10,  5,  6, 11, -1}, 63},
{{ 8,  7,  4,  3,  5, 10,  3,  1,  5,  5,  6, 10, -1, -1, -1, -1}, 63},
{{ 5, 10,  1,  5,  6, 10,  1, 10,  0,  7,  4, 10,  0, 10,  4, -1}, 63},
{{ 0,  9,  5,  0,  5,  6,  0,  6,  3, 10,  3,  6,  8,  7,  4, -1}, 63},
{{ 6,  9,  5,  6, 10,  9,  4,  9,  7,  7,  9, 10, -1, -1, -1, -1}, 47},
{{11,  9,  4,  6, 11,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 46},
{{ 4,  6, 11,  4, 11,  9,  0,  3,  8, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  1,  0, 11,  0,  6,  6,  0,  4, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 8,  1,  3,  8,  6,  1,  8,  4,  6,  6, 11,  1, -1, -1, -1, -1}, 63},
{{ 1,  9,  4,  1,  4,  2,  2,  4,  6, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 3,  8,  0,  1,  9,  2,  2,  9,  4,  2,  4,  6, -1, -1, -1, -1}, 63},
{{ 0,  4,  2,  4,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 60},
{{ 8,  2,  3,  8,  4,  2,  4,  6,  2, -1, -1, -1, -1, -1, -1, -1}, 61},
{{11,  9,  4, 11,  4,  6, 10,  3,  2, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 0,  2,  8,  2, 10,  8,  4, 11,  9,  4,  6, 11, -1, -1, -1, -1}, 63},
{{ 3,  2, 10,  0,  6,  1,  0,  4,  6,  6, 11,  1, -1, -1, -1, -1}, 63},
{{ 6,  1,  4,  6, 11,  1,  4,  1,  8,  2, 10,  1,  8,  1, 10, -1}, 63},
{{ 9,  4,  6,  9,  6,  3,  9,  3,  1, 10,  3,  6, -1, -1, -1, -1}, 63},
{{ 8,  1, 10,  8,  0,  1, 10,  1,  6,  9,  4,  1,  6,  1,  4, -1}, 63},
{{ 3,  6, 10,  3,  0,  6,  0,  4,  6, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 6,  8,  4, 10,  8,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 45},
{{ 7,  6, 11,  7, 11,  8,  8, 11,  9, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 0,  3,  7,  0,  7, 11,  0, 11,  9,  6, 11,  7, -1, -1, -1, -1}, 63},
{{11,  7,  6,  1,  7, 11,  1,  8,  7,  1,  0,  8, -1, -1, -1, -1}, 63},
{{11,  7,  6, 11,  1,  7,  1,  3,  7, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 1,  6,  2,  1,  8,  6,  1,  9,  8,  8,  7,  6, -1, -1, -1, -1}, 63},
{{ 2,  9,  6,  2,  1,  9,  6,  9,  7,  0,  3,  9,  7,  9,  3, -1}, 63},
{{ 7,  0,  8,  7,  6,  0,  6,  2,  0, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 7,  2,  3,  6,  2,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 53},
{{ 2, 10,  3, 11,  8,  6, 11,  9,  8,  8,  7,  6, -1, -1, -1, -1}, 63},
{{ 2,  7,  0,  2, 10,  7,  0,  7,  9,  6, 11,  7,  9,  7, 11, -1}, 63},
{{ 1,  0,  8,  1,  8,  7,  1,  7, 11,  6, 11,  7,  2, 10,  3, -1}, 63},
{{10,  1,  2, 10,  7,  1, 11,  1,  6,  6,  1,  7, -1, -1, -1, -1}, 55},
{{ 8,  6,  9,  8,  7,  6,  9,  6,  1, 10,  3,  6,  1,  6,  3, -1}, 63},
{{ 0,  1,  9, 10,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 7,  0,  8,  7,  6,  0,  3,  0, 10, 10,  0,  6, -1, -1, -1, -1}, 61},
{{ 7,  6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 37},
{{ 7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 37},
{{ 3,  8,  0, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 0,  9,  1, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 8,  9,  1,  8,  1,  3, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  2,  1,  6,  7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 1, 11,  2,  3,  8,  0,  6,  7, 10, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 2,  0,  9,  2,  9, 11,  6,  7, 10, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 6,  7, 10,  2,  3, 11, 11,  3,  8, 11,  8,  9, -1, -1, -1, -1}, 63},
{{ 7,  3,  2,  6,  7,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 53},
{{ 7,  8,  0,  7,  0,  6,  6,  0,  2, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 2,  6,  7,  2,  7,  3,  0,  9,  1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1,  2,  6,  1,  6,  8,  1,  8,  9,  8,  6,  7, -1, -1, -1, -1}, 63},
{{11,  6,  7, 11,  7,  1,  1,  7,  3, -1, -1, -1, -1, -1, -1, -1}, 55},
{{11,  6,  7,  1, 11,  7,  1,  7,  8,  1,  8,  0, -1, -1, -1, -1}, 63},
{{ 0,  7,  3,  0, 11,  7,  0,  9, 11,  6,  7, 11, -1, -1, -1, -1}, 63},
{{ 7, 11,  6,  7,  8, 11,  8,  9, 11, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 6,  4,  8, 10,  6,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 45},
{{ 3, 10,  6,  3,  6,  0,  0,  6,  4, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 8, 10,  6,  8,  6,  4,  9,  1,  0, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9,  6,  4,  9,  3,  6,  9,  1,  3, 10,  6,  3, -1, -1, -1, -1}, 63},
{{ 6,  4,  8,  6,  8, 10,  2,  1, 11, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 1, 11,  2,  3, 10,  0,  0, 10,  6,  0,  6,  4, -1, -1, -1, -1}, 63},
{{ 4,  8, 10,  4, 10,  6,  0,  9,  2,  2,  9, 11, -1, -1, -1, -1}, 63},
{{11,  3,  9, 11,  2,  3,  9,  3,  4, 10,  6,  3,  4,  3,  6, -1}, 63},
{{ 8,  3,  2,  8,  2,  4,  4,  2,  6, -1, -1, -1, -1, -1, -1, -1}, 61},
{{ 0,  2,  4,  4,  2,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 60},
{{ 1,  0,  9,  2,  4,  3,  2,  6,  4,  4,  8,  3, -1, -1, -1, -1}, 63},
{{ 1,  4,  9,  1,  2,  4,  2,  6,  4, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 8,  3,  1,  8,  1,  6,  8,  6,  4,  6,  1, 11, -1, -1, -1, -1}, 63},
{{11,  0,  1, 11,  6,  0,  6,  4,  0, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 4,  3,  6,  4,  8,  3,  6,  3, 11,  0,  9,  3, 11,  3,  9, -1}, 63},
{{11,  4,  9,  6,  4, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 46},
{{ 4,  5,  9,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 0,  3,  8,  4,  5,  9, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 5,  1,  0,  5,  0,  4,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1}, 63},
{{10,  6,  7,  8,  4,  3,  3,  4,  5,  3,  5,  1, -1, -1, -1, -1}, 63},
{{ 9,  4,  5, 11,  2,  1,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 6,  7, 10,  1, 11,  2,  0,  3,  8,  4,  5,  9, -1, -1, -1, -1}, 63},
{{ 7, 10,  6,  5, 11,  4,  4, 11,  2,  4,  2,  0, -1, -1, -1, -1}, 63},
{{ 3,  8,  4,  3,  4,  5,  3,  5,  2, 11,  2,  5, 10,  6,  7, -1}, 63},
{{ 7,  3,  2,  7,  2,  6,  5,  9,  4, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 9,  4,  5,  0,  6,  8,  0,  2,  6,  6,  7,  8, -1, -1, -1, -1}, 63},
{{ 3,  2,  6,  3,  6,  7,  1,  0,  5,  5,  0,  4, -1, -1, -1, -1}, 63},
{{ 6,  8,  2,  6,  7,  8,  2,  8,  1,  4,  5,  8,  1,  8,  5, -1}, 63},
{{ 9,  4,  5, 11,  6,  1,  1,  6,  7,  1,  7,  3, -1, -1, -1, -1}, 63},
{{ 1, 11,  6,  1,  6,  7,  1,  7,  0,  8,  0,  7,  9,  4,  5, -1}, 63},
{{ 4, 11,  0,  4,  5, 11,  0, 11,  3,  6,  7, 11,  3, 11,  7, -1}, 63},
{{ 7, 11,  6,  7,  8, 11,  5, 11,  4,  4, 11,  8, -1, -1, -1, -1}, 47},
{{ 6,  5,  9,  6,  9, 10, 10,  9,  8, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 3, 10,  6,  0,  3,  6,  0,  6,  5,  0,  5,  9, -1, -1, -1, -1}, 63},
{{ 0,  8, 10,  0, 10,  5,  0,  5,  1,  5, 10,  6, -1, -1, -1, -1}, 63},
{{ 6,  3, 10,  6,  5,  3,  5,  1,  3, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 1, 11,  2,  9, 10,  5,  9,  8, 10, 10,  6,  5, -1, -1, -1, -1}, 63},
{{ 0,  3, 10,  0, 10,  6,  0,  6,  9,  5,  9,  6,  1, 11,  2, -1}, 63},
{{10,  5,  8, 10,  6,  5,  8,  5,  0, 11,  2,  5,  0,  5,  2, -1}, 63},
{{ 6,  3, 10,  6,  5,  3,  2,  3, 11, 11,  3,  5, -1, -1, -1, -1}, 55},
{{ 5,  9,  8,  5,  8,  2,  5,  2,  6,  3,  2,  8, -1, -1, -1, -1}, 63},
{{ 9,  6,  5,  9,  0,  6,  0,  2,  6, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 1,  8,  5,  1,  0,  8,  5,  8,  6,  3,  2,  8,  6,  8,  2, -1}, 63},
{{ 1,  6,  5,  2,  6,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 54},
{{ 1,  6,  3,  1, 11,  6,  3,  6,  8,  5,  9,  6,  8,  6,  9, -1}, 63},
{{11,  0,  1, 11,  6,  0,  9,  0,  5,  5,  0,  6, -1, -1, -1, -1}, 62},
{{ 0,  8,  3,  5, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 38},
{{10, 11,  5,  7, 10,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 39},
{{10, 11,  5, 10,  5,  7,  8,  0,  3, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 5,  7, 10,  5, 10, 11,  1,  0,  9, -1, -1, -1, -1, -1, -1, -1}, 63},
{{11,  5,  7, 11,  7, 10,  9,  1,  8,  8,  1,  3, -1, -1, -1, -1}, 63},
{{10,  2,  1, 10,  1,  7,  7,  1,  5, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 0,  3,  8,  1,  7,  2,  1,  5,  7,  7, 10,  2, -1, -1, -1, -1}, 63},
{{ 9,  5,  7,  9,  7,  2,  9,  2,  0,  2,  7, 10, -1, -1, -1, -1}, 63},
{{ 7,  2,  5,  7, 10,  2,  5,  2,  9,  3,  8,  2,  9,  2,  8, -1}, 63},
{{ 2, 11,  5,  2,  5,  3,  3,  5,  7, -1, -1, -1, -1, -1, -1, -1}, 55},
{{ 8,  0,  2,  8,  2,  5,  8,  5,  7, 11,  5,  2, -1, -1, -1, -1}, 63},
{{ 9,  1,  0,  5,  3, 11,  5,  7,  3,  3,  2, 11, -1, -1, -1, -1}, 63},
{{ 9,  2,  8,  9,  1,  2,  8,  2,  7, 11,  5,  2,  7,  2,  5, -1}, 63},
{{ 1,  5,  3,  3,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 51},
{{ 0,  7,  8,  0,  1,  7,  1,  5,  7, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 9,  3,  0,  9,  5,  3,  5,  7,  3, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 9,  7,  8,  5,  7,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 43},
{{ 5,  4,  8,  5,  8, 11, 11,  8, 10, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 5,  4,  0,  5,  0, 10,  5, 10, 11, 10,  0,  3, -1, -1, -1, -1}, 63},
{{ 0,  9,  1,  8, 11,  4,  8, 10, 11, 11,  5,  4, -1, -1, -1, -1}, 63},
{{11,  4, 10, 11,  5,  4, 10,  4,  3,  9,  1,  4,  3,  4,  1, -1}, 63},
{{ 2,  1,  5,  2,  5,  8,  2,  8, 10,  4,  8,  5, -1, -1, -1, -1}, 63},
{{ 0, 10,  4,  0,  3, 10,  4, 10,  5,  2,  1, 10,  5, 10,  1, -1}, 63},
{{ 0,  5,  2,  0,  9,  5,  2,  5, 10,  4,  8,  5, 10,  5,  8, -1}, 63},
{{ 9,  5,  4,  2,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 2, 11,  5,  3,  2,  5,  3,  5,  4,  3,  4,  8, -1, -1, -1, -1}, 63},
{{ 5,  2, 11,  5,  4,  2,  4,  0,  2, -1, -1, -1, -1, -1, -1, -1}, 62},
{{ 3,  2, 11,  3, 11,  5,  3,  5,  8,  4,  8,  5,  0,  9,  1, -1}, 63},
{{ 5,  2, 11,  5,  4,  2,  1,  2,  9,  9,  2,  4, -1, -1, -1, -1}, 62},
{{ 8,  5,  4,  8,  3,  5,  3,  1,  5, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 0,  5,  4,  1,  5,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 58},
{{ 8,  5,  4,  8,  3,  5,  9,  5,  0,  0,  5,  3, -1, -1, -1, -1}, 59},
{{ 9,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 42},
{{ 4,  7, 10,  4, 10,  9,  9, 10, 11, -1, -1, -1, -1, -1, -1, -1}, 47},
{{ 0,  3,  8,  4,  7,  9,  9,  7, 10,  9, 10, 11, -1, -1, -1, -1}, 63},
{{ 1, 10, 11,  1,  4, 10,  1,  0,  4,  7, 10,  4, -1, -1, -1, -1}, 63},
{{ 3,  4,  1,  3,  8,  4,  1,  4, 11,  7, 10,  4, 11,  4, 10, -1}, 63},
{{ 4,  7, 10,  9,  4, 10,  9, 10,  2,  9,  2,  1, -1, -1, -1, -1}, 63},
{{ 9,  4,  7,  9,  7, 10,  9, 10,  1,  2,  1, 10,  0,  3,  8, -1}, 63},
{{10,  4,  7, 10,  2,  4,  2,  0,  4, -1, -1, -1, -1, -1, -1, -1}, 61},
{{10,  4,  7, 10,  2,  4,  8,  4,  3,  3,  4,  2, -1, -1, -1, -1}, 61},
{{ 2, 11,  9,  2,  9,  7,  2,  7,  3,  7,  9,  4, -1, -1, -1, -1}, 63},
{{ 9,  7, 11,  9,  4,  7, 11,  7,  2,  8,  0,  7,  2,  7,  0, -1}, 63},
{{ 3, 11,  7,  3,  2, 11,  7, 11,  4,  1,  0, 11,  4, 11,  0, -1}, 63},
{{ 1,  2, 11,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 63},
{{ 4,  1,  9,  4,  7,  1,  7,  3,  1, -1, -1, -1, -1, -1, -1, -1}, 59},
{{ 4,  1,  9,  4,  7,  1,  0,  1,  8,  8,  1,  7, -1, -1, -1, -1}, 59},
{{ 4,  3,  0,  7,  3,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 57},
{{ 4,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 41},
{{ 9,  8, 11, 11,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 15},
{{ 3,  9,  0,  3, 10,  9, 10, 11,  9, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 0, 11,  1,  0,  8, 11,  8, 10, 11, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 3, 11,  1, 10, 11,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 23},
{{ 1, 10,  2,  1,  9, 10,  9,  8, 10, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 3,  9,  0,  3, 10,  9,  1,  9,  2,  2,  9, 10, -1, -1, -1, -1}, 31},
{{ 0, 10,  2,  8, 10,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 29},
{{ 3, 10,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 21},
{{ 2,  8,  3,  2, 11,  8, 11,  9,  8, -1, -1, -1, -1, -1, -1, -1}, 31},
{{ 9,  2, 11,  0,  2,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 30},
{{ 2,  8,  3,  2, 11,  8,  0,  8,  1,  1,  8, 11, -1, -1, -1, -1}, 31},
{{ 1,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 22},
{{ 1,  8,  3,  9,  8,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 27},
{{ 0,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 26},
{{ 0,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 25},
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 0}};


} // End namespace SCIRun
