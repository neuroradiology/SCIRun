/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2020 Scientific Computing and Imaging Institute,
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


// Uniforms
uniform mat4    uProjection;
uniform mat4    uModel;
uniform mat4    uView;

// Attributes
attribute vec3  aPos;
attribute vec2  aTexCoords;

// Outputs to the fragment shader.
varying vec4    vPosWorld;
varying vec4    vPosView;
varying vec2    vTexCoords;

void main( void )
{
  vPosWorld = uModel * vec4(aPos, 1.0);
  vPosView = uView * vPosWorld;
  vTexCoords = aTexCoords;

  gl_Position = (uProjection * (vPosView));
  gl_Position += vec4(0.0, 0.0, -0.00001, 0.0);
}
