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

Author            : Moritz Dannhauer
Author            : Spencer Frisby
Last modification : 9/5/2014

*/

#include <Core/Algorithms/Math/AddKnownsToLinearSystem.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Datatypes/DenseColumnMatrix.h>
#include <Core/Datatypes/MatrixTypeConversions.h>
#include <Core/Datatypes/SparseRowMatrix.h>
#include <Core/Datatypes/SparseRowMatrixFromMap.h>
#include <Core/Algorithms/Base/AlgorithmPreconditions.h>

#include <Core/GeometryPrimitives/Point.h>
#include <Core/GeometryPrimitives/Tensor.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace SCIRun;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Core::Algorithms::Math;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Geometry;

double AddKnownsToLinearSystemAlgo::bound_for_equality = 1e-7;

bool AddKnownsToLinearSystemAlgo::run(SparseRowMatrixHandle stiff,
  DenseMatrixHandle rhs,
  DenseMatrixHandle x,
  SparseRowMatrixHandle& output_stiff,
  DenseColumnMatrixHandle& output_rhs) const
{

  SparseRowMatrixFromMap::Values additionalData;

  // Making sure the stiff matrix (left hand side) is symmetric
  if (!isSymmetricMatrix(*stiff,bound_for_equality))
  {
    std::ostringstream ostr1;
    ostr1 << "matrix A is not symmetrical due to a difference of " << bound_for_equality << std::endl;
    THROW_ALGORITHM_INPUT_ERROR(ostr1.str());    
  }

  // Storing the number of columns in m and rows in n from the stiff matrix, m == n
  const unsigned int numCols = static_cast<unsigned int>(stiff->ncols());
  const unsigned int numRows = static_cast<unsigned int>(stiff->nrows());

  // Checking if the rhs matrix is allocated and that the dimensions agree with the stiff matrix
  if (rhs)
  {
    if ( !(((rhs->ncols() == numCols) && (rhs->nrows() == 1)) || ((rhs->ncols() == 1) && (rhs->nrows() == numCols))) )
    {
      THROW_ALGORITHM_INPUT_ERROR("The dimensions of vector b do not match the dimensions of matrix A"); 
    }
  }

  // casting rhs to be a column
  auto rhsCol = rhs ?  matrix_convert::to_column(rhs) : boost::make_shared<DenseColumnMatrix>(DenseColumnMatrix::Zero(numCols));
  ENSURE_NOT_NULL(rhsCol, "rhsCol");
  DenseColumnMatrix& rhsColRef = *rhsCol;

  // Checking if x matrix was given and that the dimensions agree with the stiff matrix
  if (!x)
  {
    THROW_ALGORITHM_INPUT_ERROR("No x vector was given");
  }
  else if ( !(((x->ncols() == numCols) && (x->nrows() == 1)) || ((x->ncols() == 1) && (x->nrows() == numCols))) )
  {
    THROW_ALGORITHM_INPUT_ERROR("The dimensions of vector x do not match the dimensions of matrix A");
  } 

  // casting x to be a column
  auto xCol = matrix_cast::as_column(x);
  if (!xCol) xCol = matrix_convert::to_column(x);
  ENSURE_NOT_NULL(xCol, "xColumn");
  const DenseColumnMatrix& xColRef = *xCol;

  // cnt used for updating the progress bar
  index_type cnt = 0;

  bool just_copying_inputs = true;

  // performs calculation adjustments for setting row and col values to zero
  // NOTE: right hand side vector values are reset multiple times during this
  //   process, thus it was necessary to have a second for loop to set the
  //   right hand side vector equal to the known values
  for (index_type i=0; i<numRows; i++)
  {
    for (SparseRowMatrix::InnerIterator it(*stiff,i); it; ++it)
    //for (index_type p=0; p<numCols; p++)    
    {
      const int p = it.col();
      // making sure the rhs vector is finite
      if (!IsFinite(rhsColRef[p]))
        THROW_ALGORITHM_INPUT_ERROR("NaN exist in the b vector");

      const double xCol_p = xColRef[p];
      if (IsFinite(xCol_p))
      {
        just_copying_inputs = false;
        if (i!=p)
        {
          rhsColRef[i] += -it.value() * xCol_p;
          additionalData[i][p] = 0.0;
          additionalData[p][i] = 0.0;
        }
      }
      cnt++;
      if (cnt == 1000)
      {
        cnt = 0;
        update_progress_max(i, numRows);
      }
    }
  }

  for (int i = 0; i < std::min(numRows, numCols); ++i)
  {
    if (IsFinite(xColRef[i]))
      additionalData[i][i] = 1.0;
  }

  // assigns value for right hand side vector
  for (index_type p=0; p<numCols; p++)
  {
    if (IsFinite(xColRef[p]))
    {
      just_copying_inputs = false;
      rhsColRef[p] = xColRef[p];
    }
    cnt++;
    if (cnt == 100)
    {
      cnt = 0;
      update_progress((double)p/numCols);
    }
  }

  if (just_copying_inputs)
    remark("X vector does not contain any knowns! Copying inputs to outputs.");

  output_stiff = SparseRowMatrixFromMap::appendToSparseMatrix(numCols, numRows, *stiff, additionalData);
  output_rhs = rhsCol;

  return true;
}

AlgorithmInputName AddKnownsToLinearSystemAlgo::LHS_Matrix("LHS_Matrix");
AlgorithmInputName AddKnownsToLinearSystemAlgo::RHS_Vector("RHS_Vector");
AlgorithmInputName AddKnownsToLinearSystemAlgo::X_Vector("X_Vector");
AlgorithmInputName AddKnownsToLinearSystemAlgo::OutPutLHSMatrix("OutPutLHSMatrix");
AlgorithmInputName AddKnownsToLinearSystemAlgo::OutPutRHSVector("OutPutRHSVector");

AlgorithmOutput AddKnownsToLinearSystemAlgo::run_generic(const AlgorithmInput & input) const
{ 
  auto input_lhs = input.get<SparseRowMatrix>(LHS_Matrix);
  auto input_rhs = input.get<DenseMatrix>(RHS_Vector);
  auto input_x = input.get<DenseMatrix>(X_Vector);

  if (input_lhs->nrows() != input_lhs->ncols()) 
    THROW_ALGORITHM_INPUT_ERROR("Stiffness matrix input needs to be a sparse square matrix!");

  SparseRowMatrixHandle output_lhs;
  DenseColumnMatrixHandle output_rhs;

  if (!run(input_lhs,input_rhs,input_x,output_lhs,output_rhs))
    THROW_ALGORITHM_INPUT_ERROR("False returned on legacy run call.");

  AlgorithmOutput output; 
  output[OutPutLHSMatrix] = output_lhs;
  output[OutPutRHSVector] = output_rhs;

  return output;
}
