/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2015 Scientific Computing and Imaging Institute,
University of Utah.

License for the specific language governing rights and limitations under
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

#include <Interface/Modules/Visualization/ShowFieldGlyphsDialog.h>
#include <Modules/Visualization/ShowFieldGlyphs.h>
#include <Dataflow/Network/ModuleStateInterface.h>  //TODO: extract into intermediate
#include <Core/Datatypes/Color.h>
#include <QColorDialog>

using namespace SCIRun::Gui;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Modules::Visualization;
using namespace SCIRun::Core::Datatypes;
//using namespace SCIRun::Core::Algorithms::Visualization;

ShowFieldGlyphsDialog::ShowFieldGlyphsDialog(const std::string& name, ModuleStateHandle state,
  QWidget* parent /* = 0 */)
  : ModuleDialogGeneric(state, parent)
{
  setupUi(this);
  setWindowTitle(QString::fromStdString(name));

  setupVectorsTab();
  setupScalarsTab();
  setupTensorsTab();

  addLineEditManager(lineEdit, ShowFieldGlyphs::FieldName);
  WidgetStyleMixin::tabStyle(this->displayOptionsTabs_);


  createExecuteInteractivelyToggleAction();

  connect(defaultMeshColorButton_, SIGNAL(clicked()), this, SLOT(assignDefaultMeshColor()));
  connectButtonToExecuteSignal(defaultMeshColorButton_);
}

void ShowFieldGlyphsDialog::push()
{
  if (!pulling_)
  {
    pushColor();
  }
}

void ShowFieldGlyphsDialog::createStartupNote()
{
  auto showFieldGlyphId = windowTitle().split(':')[1];
  setStartupNote("ID: " + showFieldGlyphId);
}

void ShowFieldGlyphsDialog::pullSpecial()
{
  ColorRGB color(state_->getValue(ShowFieldGlyphs::DefaultMeshColor).toString());
  //std::cout << "pull color: " << color.r() << " " << color.g() << " " << color.b() << std::endl;
  // check for old saved color format: integers 0-255.
  defaultMeshColor_ = QColor(
    static_cast<int>(color.r() > 1 ? color.r() : color.r() * 255.0),
    static_cast<int>(color.g() > 1 ? color.g() : color.g() * 255.0),
    static_cast<int>(color.b() > 1 ? color.b() : color.b() * 255.0));

  //  checkTabs();
}


void ShowFieldGlyphsDialog::assignDefaultMeshColor()
{
  auto newColor = QColorDialog::getColor(defaultMeshColor_, this, "Choose default mesh color");
  if (newColor.isValid())
  {
    defaultMeshColor_ = newColor;
    //TODO: set color of button to this color
    pushColor();
  }
}

void ShowFieldGlyphsDialog::pushColor()
{
  state_->setValue(ShowFieldGlyphs::DefaultMeshColor, ColorRGB(defaultMeshColor_.redF(), defaultMeshColor_.greenF(), defaultMeshColor_.blueF()).toString());
}

void ShowFieldGlyphsDialog::setupScalarsTab()
{
  // Show Scalars
  addCheckableButtonManager(this->showScalarsCheckBox_, ShowFieldGlyphs::ShowScalars);
  // Display Type
  addComboBoxManager(this->scalarsDisplayTypeComboBox_, ShowFieldGlyphs::ScalarsDisplayType);
  this->scalarsDisplayTypeComboBox_->setCurrentIndex(0);
  // Coloring
  addComboBoxManager(scalarsColorTypeComboBox_, ShowFieldGlyphs::ScalarsColoring);
  // Coloring Data Input
  addComboBoxManager(this->scalarsColoringInputComboBox_, ShowFieldGlyphs::ScalarsColoringDataInput);
  // Transparency
  addRadioButtonGroupManager({ this->scalarsTransparencyOffRButton_, this->scalarsUniformTransparencyRButton_}, ShowFieldGlyphs::ScalarsTransparency);
  addDoubleSpinBoxManager(this->scalarsTransparencyDoubleSpinBox_, ShowFieldGlyphs::ScalarsUniformTransparencyValue);
  // Transparency Data Input
  //  addComboBoxManager(this->scalarsTransparencyInputComboBox_, ShowFieldGlyphs::ScalarsTransparencyDataInput);
  // Scale
  addDoubleSpinBoxManager(this->scaleScalarsDoubleSpinBox_, ShowFieldGlyphs::ScalarsScale);
  // Resolution
  addSpinBoxManager(this->scalarsResolutionSpinBox_, ShowFieldGlyphs::ScalarsResolution);
  // Threshold
  addDoubleSpinBoxManager(this->scalarsThresholdDoubleSpinBox_, ShowFieldGlyphs::ScalarsThreshold);

  connectButtonToExecuteSignal(this->showScalarsCheckBox_);
  connectButtonToExecuteSignal(this->scalarsTransparencyOffRButton_);
  connectButtonToExecuteSignal(this->scalarsUniformTransparencyRButton_);
  //connectButtonToExecuteSignal(this->defaultScalarsColoringRButton_);
  //connectButtonToExecuteSignal(this->colormapLookupScalarsColoringRButton_);
  //connectButtonToExecuteSignal(this->conversionRGBScalarsColoringRButton_);
  //connectButtonToExecuteSignal(this->scalarsAsPointsRButton_);
  //connectButtonToExecuteSignal(this->scalarsAsSpheresRButton_);
  //connectButtonToExecuteSignal(this->scalarsAsBoxesRButton_);
  //connectButtonToExecuteSignal(this->scalarsAsAxisRButton_);

  // Text Labels
  this->scalarColorTypeLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->scalarColorInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->normalizeScalarsCheckBox_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->scalarScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}

void ShowFieldGlyphsDialog::setupVectorsTab()
{
  // Show Vectors
  addCheckableButtonManager(this->showVectorsCheckBox_, ShowFieldGlyphs::ShowVectors);
  // Display Type
  addComboBoxManager(this->vectorsDisplayTypeComboBox_, ShowFieldGlyphs::VectorsDisplayType);
  // Coloring
  addComboBoxManager(this->vectorsColorTypeComboBox_, ShowFieldGlyphs::VectorsColoring);
  // Coloring Data Input
  addComboBoxManager(this->vectorsColoringInputComboBox_, ShowFieldGlyphs::VectorsColoringDataInput);
  // Transparency
  addRadioButtonGroupManager({ this->vectorsTransparencyOffRButton_, this->vectorsUniformTransparencyRButton_}, ShowFieldGlyphs::VectorsTransparency);
  addDoubleSpinBoxManager(this->vectorsTransparencyDoubleSpinBox_, ShowFieldGlyphs::VectorsUniformTransparencyValue);
  // Transparency Data Input
  //  addComboBoxManager(this->vectorsTransparencyInputComboBox_, ShowFieldGlyphs::VectorsTransparencyDataInput);
  // Normalize
  addCheckableButtonManager(this->normalizeVectorsCheckBox_, ShowFieldGlyphs::NormalizeVectors);
  // Scale
  addDoubleSpinBoxManager(this->scaleVectorsDoubleSpinBox_, ShowFieldGlyphs::VectorsScale);
  // Resolution
  addSpinBoxManager(this->vectorsResolutionSpinBox_, ShowFieldGlyphs::VectorsResolution);
  // Threshold
  addCheckableButtonManager(this->renderVectorsBelowThresholdCheckBox_, ShowFieldGlyphs::RenderVectorsBelowThreshold);
  addDoubleSpinBoxManager(this->vectorsThresholdDoubleSpinBox_, ShowFieldGlyphs::VectorsThreshold);
  // Radius/Width
  addComboBoxManager(this->vectorsRadiusWidthPortComboBox_, ShowFieldGlyphs::VectorsRadiusWidthDataInput);
  addDoubleSpinBoxManager(this->vectorsRadiusWidthDoubleSpinBox_, ShowFieldGlyphs::VectorsRadiusWidthScale);
  // Arrow Head Ratio
  addDoubleSpinBoxManager(this->arrowHeadRatioDoubleSpinBox_, ShowFieldGlyphs::ArrowHeadRatio);
  // Bidirectional
  addCheckableButtonManager(this->bidirectionalVectorsCheckBox_, ShowFieldGlyphs::RenderBidirectionaly);

  connectButtonToExecuteSignal(this->showVectorsCheckBox_);
  connectButtonToExecuteSignal(this->vectorsTransparencyOffRButton_);
  connectButtonToExecuteSignal(this->vectorsUniformTransparencyRButton_);
  //connectButtonToExecuteSignal(this->defaultVectorsColoringRButton_);
  //connectButtonToExecuteSignal(this->colormapLookupVectorsColoringRButton_);
  //connectButtonToExecuteSignal(this->conversionRGBVectorsColoringRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsLinesRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsNeedlesRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsCometsRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsConesRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsArrowsRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsDisksRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsRingsRButton_);
  //connectButtonToExecuteSignal(this->vectorsAsSpringsRButton_);
  connectButtonToExecuteSignal(this->normalizeVectorsCheckBox_);
  connectButtonToExecuteSignal(this->bidirectionalVectorsCheckBox_);
  connectButtonToExecuteSignal(this->renderVectorsBelowThresholdCheckBox_);

  // Text Labels
  this->vectorColorTypeLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->vectorColorInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->normalizeVectorsCheckBox_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->vectorScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->vectorRadiusWidthInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->vectorRadiusWidthScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->bidirectionalVectorsCheckBox_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsMajorRadiusInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsMajorRadiusScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsMinorRadiusInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsMinorRadiusScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsPitchInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->springsPitchScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}

void ShowFieldGlyphsDialog::setupTensorsTab()
{
  // Show Tensors
  addCheckableButtonManager(this->showTensorsCheckBox_, ShowFieldGlyphs::ShowTensors);
  // Display Type
  addComboBoxManager(this->tensorsDisplayTypeComboBox_, ShowFieldGlyphs::TensorsDisplayType);
  // Coloring
  addComboBoxManager(this->tensorsColorTypeComboBox_, ShowFieldGlyphs::TensorsColoring);
  // Coloring Data Input
  addComboBoxManager(this->tensorsColoringInputComboBox_, ShowFieldGlyphs::TensorsColoringDataInput);
  // Transparency
  addRadioButtonGroupManager({ this->tensorsTransparencyOffRButton_, this->tensorsUniformTransparencyRButton_}, ShowFieldGlyphs::TensorsTransparency);
  addDoubleSpinBoxManager(this->tensorsTransparencyDoubleSpinBox_, ShowFieldGlyphs::TensorsUniformTransparencyValue);
  // Transparency Data Input
  //  addComboBoxManager(this->tensorsTransparencyInputComboBox_, ShowFieldGlyphs::TensorsTransparencyDataInput);
  // Normalize
  addCheckableButtonManager(this->normalizeTensorsCheckBox_, ShowFieldGlyphs::NormalizeTensors);
  // Scale
  addDoubleSpinBoxManager(this->scaleTensorsDoubleSpinBox_, ShowFieldGlyphs::TensorsScale);
  // Resolution
  addSpinBoxManager(this->tensorsResolutionSpinBox_, ShowFieldGlyphs::TensorsResolution);
  // Threshold
  addCheckableButtonManager(this->renderVectorsBelowThresholdCheckBox_, ShowFieldGlyphs::RenderTensorsBelowThreshold);
  addDoubleSpinBoxManager(this->tensorsThresholdDoubleSpinBox_, ShowFieldGlyphs::TensorsThreshold);

  connectButtonToExecuteSignal(this->showTensorsCheckBox_);
  connectButtonToExecuteSignal(this->tensorsTransparencyOffRButton_);
  connectButtonToExecuteSignal(this->tensorsUniformTransparencyRButton_);
  //connectButtonToExecuteSignal(this->defaultTensorsColoringRButton_);
  //connectButtonToExecuteSignal(this->colormapLookupTensorsColoringRButton_);
  //connectButtonToExecuteSignal(this->conversionRGBTensorsColoringRButton_);
  //connectButtonToExecuteSignal(this->tensorsAsEllipsoidsRButton_);
  //connectButtonToExecuteSignal(this->tensorsAsBoxesRButton_);
  //connectButtonToExecuteSignal(this->tensorsAsColoredBoxesRButton_);
  //connectButtonToExecuteSignal(this->tensorsAsSuperquadricsRButton_);

  // Text Labels
  this->tensorSuperquadricsEmphasisSlider_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->tensorColorTypeLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->tensorColorInputLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->normalizeTensorsCheckBox_->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
  this->tensorScaleLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}
