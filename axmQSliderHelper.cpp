/*=========================================================================

  Name:        axmQSliderHelper.cpp

  Author:      David Borland

  Description: Handles converting values between a QSlider, which uses 
               integer values, and a continuous floating-point range.

=========================================================================*/


#include "axmQSliderHelper.h"

#include <QDoubleValidator>


int roundToInt(double v) {
    int i = v;
    if (v - i >= 0.5) return i + 1;
    else return i;
}


axmQSliderHelper::axmQSliderHelper(QSlider* qSlider, QLineEdit* qLineEdit, 
                                   double rangeMin, double rangeMax, double rangeStep) 
: slider(qSlider), lineEdit(qLineEdit) {
    // Set the continuous floating-point range
    range[0] = rangeMin;
    range[1] = rangeMax;
    step = rangeStep;

    // Set the slider range, accounting for potential floating point error
    int numSteps = roundToInt((range[1] - range[0]) / step) + 1;

    slider->setRange(0, numSteps - 1);
    slider->setTickInterval(numSteps / 10);

    // Add a validator to the line edit
    // XXX: Calculate number of decimals from range and num steps?  Add to constructor?
    QDoubleValidator* validator = new QDoubleValidator(lineEdit);
    validator->setDecimals(2);
    lineEdit->setValidator(validator);
}

axmQSliderHelper::~axmQSliderHelper() {
}


void axmQSliderHelper::SetValue(double value) {
    QString text = QString().sprintf("%0.2f", value);
    lineEdit->setText(text);

    ToSlider();
}


double axmQSliderHelper::ToSlider() {
    double value = lineEdit->text().toDouble();

    if (value < range[0]) value = range[0];
    if (value > range[1]) value = range[1];

    slider->setValue(roundToInt((value - range[0]) / step));

    QString text = QString().sprintf("%0.2f", value);
    lineEdit->setText(text);

    return value;
}

double axmQSliderHelper::FromSlider() {
    double value = range[0] + slider->value() * step;
    
    QString text = QString().sprintf("%0.2f", value);
    lineEdit->setText(text);

    return value;
}