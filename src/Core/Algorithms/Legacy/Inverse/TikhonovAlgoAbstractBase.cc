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
//    File       : TikhonovAlgoAbstractBase.cc
//    Author     : Jaume Coll-Font
//    Date       : September 06th, 2017 (last update)

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

// Tikhonov specific headers
#include <Core/Algorithms/Legacy/Inverse/TikhonovAlgoAbstractBase.h>
#include <Core/Algorithms/Legacy/Inverse/TikhonovImpl.h>
#include <Core/Algorithms/Legacy/Inverse/SolveInverseProblemWithStandardTikhonovImpl.h>
#include <Core/Algorithms/Legacy/Inverse/SolveInverseProblemWithTikhonovSVD_impl.h>
#include <Core/Algorithms/Legacy/Inverse/SolveInverseProblemWithTikhonovTSVD_impl.h>

// Datatypes
#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Datatypes/DenseColumnMatrix.h>
#include <Core/Datatypes/SparseRowMatrix.h>
#include <Core/Datatypes/MatrixTypeConversions.h>
#include <Core/Math/MiscMath.h>

// SCIRun structural
#include <Core/Algorithms/Base/AlgorithmPreconditions.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <Core/Logging/LoggerInterface.h>
#include <Core/Logging/Log.h>
#include <Core/Utils/Exception.h>

using namespace SCIRun;
using namespace SCIRun::Core;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Core::Logging;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Inverse;

// shared inputs
const AlgorithmInputName TikhonovAlgoAbstractBase::ForwardMatrix("ForwardMatrix");
const AlgorithmInputName TikhonovAlgoAbstractBase::MeasuredPotentials("MeasuredPotentials");
const AlgorithmInputName TikhonovAlgoAbstractBase::WeightingInSourceSpace("WeightingInSourceSpace");
const AlgorithmInputName TikhonovAlgoAbstractBase::WeightingInSensorSpace("WeightingInSensorSpace");

// Inputs specific from the Tikhonov SVD module
const AlgorithmInputName TikhonovAlgoAbstractBase::matrixU("matrixU");
const AlgorithmInputName TikhonovAlgoAbstractBase::singularValues("singularValues");
const AlgorithmInputName TikhonovAlgoAbstractBase::matrixV("matrixV");

// outputs
const AlgorithmOutputName TikhonovAlgoAbstractBase::InverseSolution("InverseSolution");
const AlgorithmOutputName TikhonovAlgoAbstractBase::RegularizationParameter("RegularizationParameter");
const AlgorithmOutputName TikhonovAlgoAbstractBase::RegInverse("RegInverse");

ALGORITHM_PARAMETER_DEF( Inverse, TikhonovImplementation);
ALGORITHM_PARAMETER_DEF( Inverse, RegularizationMethod);
ALGORITHM_PARAMETER_DEF( Inverse, regularizationChoice);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaFromDirectEntry);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaMin);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaMax);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaNum);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaResolution);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaSliderValue);
ALGORITHM_PARAMETER_DEF( Inverse, LambdaCorner);
ALGORITHM_PARAMETER_DEF( Inverse, LCurveText);
ALGORITHM_PARAMETER_DEF( Inverse, regularizationSolutionSubcase);
ALGORITHM_PARAMETER_DEF( Inverse, regularizationResidualSubcase);

TikhonovAlgoAbstractBase::TikhonovAlgoAbstractBase()
{
	addParameter(Parameters::TikhonovImplementation, std::string("NoMethodSelected") );
	addOption(Parameters::RegularizationMethod, "lcurve", "single|slider|lcurve");
	addParameter(Parameters::regularizationChoice, 0);
	addParameter(Parameters::LambdaFromDirectEntry,1e-6);
	addParameter(Parameters::LambdaMin,1e-6);
	addParameter(Parameters::LambdaMax,1);
	addParameter(Parameters::LambdaNum,200);
	addParameter(Parameters::LambdaResolution,1e-6);
	addParameter(Parameters::LambdaSliderValue,0);
	addParameter(Parameters::LambdaCorner,0);
	addParameter(Parameters::LCurveText, std::string("lcurve"));
	addParameter(Parameters::regularizationSolutionSubcase,solution_constrained);
	addParameter(Parameters::regularizationResidualSubcase,residual_constrained);
}

////// CHECK IF INPUT MATRICES HAVE THE CORRECT SIZE
bool TikhonovAlgoAbstractBase::checkInputMatrixSizes( const AlgorithmInput & input) const
{
	auto forwardMatrix_ = input.get<Matrix>(ForwardMatrix);
	auto measuredData_ = input.get<Matrix>(MeasuredPotentials);
	auto sourceWeighting_ = input.get<Matrix>(WeightingInSourceSpace);
	auto sensorWeighting_ = input.get<Matrix>(WeightingInSensorSpace);

  const int M = forwardMatrix_->nrows();
  const int N = forwardMatrix_->ncols();

  // check that rows of fwd matrix equal number of measurements
  if ( M != measuredData_->nrows() )
  {
  	THROW_ALGORITHM_INPUT_ERROR("Input matrix dimensions must agree.");
  	return false;
  }

  // check source regularization matrix sizes
  if (sourceWeighting_)
  {
    if( get(Parameters::regularizationSolutionSubcase).toInt()==solution_constrained )
    {
      // check that the matrix is of appropriate size (equal number of rows as columns in fwd matrix)
      if ( N != sourceWeighting_->ncols() )
      {
    		THROW_ALGORITHM_INPUT_ERROR("Solution Regularization Matrix must have the same number of rows as columns in the Forward Matrix !");
    		return false;
      }
    }
    // otherwise, if the source regularization is provided as the squared version (RR^T)
    else if ( get(Parameters::regularizationSolutionSubcase).toInt()==solution_constrained_squared )
    {
      // check that the matrix is of appropriate size and squared (equal number of rows as columns in fwd matrix)
      if ( ( N != sourceWeighting_->nrows() ) || ( N != sourceWeighting_->ncols() ) )
      {
        THROW_ALGORITHM_INPUT_ERROR("The squared solution Regularization Matrix must have the same number of rows and columns and must be equal to the number of columns in the Forward Matrix !");
	      return false;
      }
    }
  }

  // check measurement regularization matrix sizes
  if (sensorWeighting_)
  {
    if (get(Parameters::regularizationResidualSubcase).toInt() == residual_constrained)
    {
      // check that the matrix is of appropriate size (equal number of rows as rows in fwd matrix)
      if(M != sensorWeighting_->ncols())
      {
        THROW_ALGORITHM_INPUT_ERROR("Data Residual Weighting Matrix must have the same number of rows as the Forward Matrix !");
        return false;
      }
    }
    // otherwise if the source covariance matrix is provided in squared form
    else if  ( get(Parameters::regularizationResidualSubcase).toInt() == residual_constrained_squared )
    {
      // check that the matrix is of appropriate size and squared (equal number of rows as rows in fwd matrix)
      if( (M != sensorWeighting_->nrows()) || (M != sensorWeighting_->ncols()) )
      {
        THROW_ALGORITHM_INPUT_ERROR("Squared data Residual Weighting Matrix must have the same number of rows and columns as number of rows in the Forward Matrix !");
        return false;
      }
    }
  }
  return true;
}

AlgorithmOutput TikhonovAlgoAbstractBase::run(const AlgorithmInput & input) const
{
	auto forwardMatrix = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::ForwardMatrix));
	auto measuredData = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::MeasuredPotentials));
	auto sourceWeighting = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::WeightingInSourceSpace));
	auto sensorWeighting = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::WeightingInSensorSpace));

	auto RegularizationMethod_gotten = getOption(Parameters::RegularizationMethod);
	auto implOption = get(Parameters::TikhonovImplementation).toString();

	// check input MATRICES
	checkInputMatrixSizes( input );

	// Determine specific Tikhonov Implementation
	std::shared_ptr<TikhonovImpl> algoImpl;
	if (implOption == "standardTikhonov")
  {
		int regularizationChoice = get(Parameters::regularizationChoice).toInt();
		int regularizationSolutionSubcase = get(Parameters::regularizationSolutionSubcase).toInt();
		int regularizationResidualSubcase = get(Parameters::regularizationResidualSubcase).toInt();

		algoImpl = std::make_shared<SolveInverseProblemWithStandardTikhonovImpl>( *forwardMatrix, *measuredData, *sourceWeighting, *sensorWeighting,
      regularizationChoice, regularizationSolutionSubcase, regularizationResidualSubcase);
	}
	else if (implOption == "TikhonovSVD")
  {
		// get TikhonovSVD special inputs
		auto matrixU = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::matrixU));
		auto singularValues = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::singularValues));
		auto matrixV = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::matrixV));

		// If there is a missing matrix from the precomputed SVD input
		if (!matrixU || !singularValues || !matrixV)
			algoImpl = std::make_shared<SolveInverseProblemWithTikhonovSVD_impl>(*forwardMatrix, *measuredData, *sourceWeighting, *sensorWeighting);
		else
			algoImpl = std::make_shared<SolveInverseProblemWithTikhonovSVD_impl>(*forwardMatrix, *measuredData, *sourceWeighting, *sensorWeighting, *matrixU, *singularValues, *matrixV);
	}
	else if (implOption == "TikhonovTSVD")
  {
		// get TikhonovSVD special inputs
		auto matrixU = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::matrixU));
		auto singularValues = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::singularValues));
		auto matrixV = castMatrix::toDense(input.get<Matrix>(TikhonovAlgoAbstractBase::matrixV));

		// If there is a missing matrix from the precomputed SVD input
		if (!matrixU || !singularValues || !matrixV)
			algoImpl = std::make_shared<SolveInverseProblemWithTikhonovTSVD_impl>(*forwardMatrix, *measuredData, *sourceWeighting, *sensorWeighting);
		else
			algoImpl = std::make_shared<SolveInverseProblemWithTikhonovTSVD_impl>(*forwardMatrix, *measuredData, *sourceWeighting, *sensorWeighting, *matrixU, *singularValues, *matrixV);
	}
	else
  {
		THROW_ALGORITHM_PROCESSING_ERROR("Not a valid Tikhonov Implementation selection");
	}

  double lambda = 0;
  //Get Regularization parameter(s) : Lambda
  if ((RegularizationMethod_gotten == "single") || (RegularizationMethod_gotten == "slider"))
  {
    if (RegularizationMethod_gotten == "single")
    {
      // Use single fixed lambda value, entered in UI
      lambda = get(Parameters::LambdaFromDirectEntry).toDouble();
    }
    else if (RegularizationMethod_gotten == "slider")
    {
      // Use single fixed lambda value, select via slider
      lambda = get(Parameters::LambdaSliderValue).toDouble();
    }
  }
  else if (RegularizationMethod_gotten == "lcurve")
  {
    lambda = computeLcurve( *algoImpl, input );
  }
	else
	{
		THROW_ALGORITHM_PROCESSING_ERROR("Lambda selection was never set");
	}

  // compute final inverse solution
	auto solution = algoImpl->computeInverseSolution(lambda, true);

	// Set outputs
	AlgorithmOutput output;
	output[InverseSolution] = boost::make_shared<DenseMatrix>(solution);
	output[RegularizationParameter] = boost::make_shared<DenseMatrix>(1, 1, lambda);

	return output;
}

double TikhonovAlgoAbstractBase::computeLcurve( const SCIRun::Core::Algorithms::Inverse::TikhonovImpl& algoImpl, const AlgorithmInput & input ) const
{
	// get inputs
	auto forwardMatrix = input.get<Matrix>(TikhonovAlgoAbstractBase::ForwardMatrix);
	auto measuredData = input.get<Matrix>(TikhonovAlgoAbstractBase::MeasuredPotentials);
	auto sourceWeighting = input.get<Matrix>(TikhonovAlgoAbstractBase::WeightingInSourceSpace);
	auto sensorWeighting = input.get<Matrix>(TikhonovAlgoAbstractBase::WeightingInSensorSpace);
  // define the step size of the lambda vector to be computed  (distance between min and max divided by number of desired lambdas in log scale)
  const int nLambda = get(Parameters::LambdaNum).toInt();
	const double lambdaMin = get(Parameters::LambdaMin).toDouble();
	const double lambdaMax = get(Parameters::LambdaMax).toDouble();
	double lambda = 0;

	// prealocate vector of lambdas and eta and rho
  std::vector<double> rho(nLambda, 0.0);
  std::vector<double> eta(nLambda, 0.0);

  auto lambdaArray = algoImpl.computeLambdaArray( lambdaMin, lambdaMax, nLambda );

  DenseMatrix CAx, Rx;
  DenseMatrix solution;

  lambdaArray[0] = lambdaMin;

  // for all lambdas
  for (int j = 0; j < nLambda; j++)
  {
    solution = algoImpl.computeInverseSolution( lambdaArray[j], false);

    // if using source regularization matrix, apply it to compute Rx (for the eta computations)
    if (sourceWeighting)
    {
      if (solution.nrows() == sourceWeighting->ncols()) // check that regularization matrix and solution match sizes
      {
        auto sw = castMatrix::toDense(sourceWeighting);
        Rx = (*sw) * solution;
      }
      else
      {
        BOOST_THROW_EXCEPTION(AlgorithmProcessingException() << ErrorMessage(" Solution weighting matrix unexpectedly does not fit to compute the weighted solution norm. "));
      }
    }
    else
      Rx = solution;

    auto forward = castMatrix::toDense(forwardMatrix);
    auto Ax = (*forward) * solution;
    auto measured = castMatrix::toDense(measuredData);
    auto residualSolution = Ax - (*measured);

    // if using source regularization matrix, apply it to compute Rx (for the eta computations)
    if (sensorWeighting)
    {
      auto sw = castMatrix::toDense(sensorWeighting);
      CAx = (*sw) * residualSolution;
    }
    else
      CAx = residualSolution;

    // compute rho and eta. Using Frobenious norm when using matrices
    rho[j] = CAx.norm();
    eta[j] = Rx.norm();
  }

  // Find corner in L-curve
  lambda = FindCorner( rho, eta, lambdaArray, nLambda );

  // TODO: update GUI

  return lambda;
}

///// Find Corner, find the maximal curvature which corresponds to the L-curve corner
double TikhonovAlgoAbstractBase::FindCorner( const std::vector<double>& rho, const std::vector<double>& eta, const std::vector<double>& lambdaArray, const int nLambda )
{
  std::vector<double> deta(nLambda);
  std::vector<double> ddeta(nLambda);
  std::vector<double> drho(nLambda);
  std::vector<double> ddrho(nLambda);
  std::vector<double> lrho(nLambda);
  std::vector<double> leta(nLambda);
  DenseColumnMatrix kapa(nLambda);

  double maxKapa = -1.0e10;
  for (int i = 0; i < nLambda; i++)
  {
    lrho[i] = std::log10(rho[i]);
    leta[i] = std::log10(eta[i]);
    if(i>0)
    {
      deta[i] = (leta[i]-leta[i-1]) / (lambdaArray[i]-lambdaArray[i-1]); // compute first derivative
      drho[i] = (lrho[i]-lrho[i-1]) / (lambdaArray[i]-lambdaArray[i-1]);
    }
    if(i>1)
    {
      ddeta[i] = (deta[i]-deta[i-1]) / (lambdaArray[i]-lambdaArray[i-1]); // compute second derivative from first
      ddrho[i] = (drho[i]-drho[i-1]) / (lambdaArray[i]-lambdaArray[i-1]);
    }
  }
  drho[0] = drho[1];
  deta[0] = deta[1];
  ddrho[0] = ddrho[2];
  ddrho[1] = ddrho[2];
  ddeta[0] = ddeta[2];
  ddeta[1] = ddeta[2];

  int lambda_index = 0;
  for (int i = 0; i < nLambda; i++)
  {
    kapa[i] = std::abs((drho[i] * ddeta[i] - ddrho[i] * deta[i]) /  //compute curvature
                       std::sqrt(std::pow((deta[i]*deta[i]+drho[i]*drho[i]), 3.0)));
    if (kapa[i] > maxKapa) // find max curvature
    {
      maxKapa = kapa[i];
      lambda_index = i;
    }
  }

  return lambdaArray[lambda_index];
}
