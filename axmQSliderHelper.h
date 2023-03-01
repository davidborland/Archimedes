/*=========================================================================

  Name:        axmQSliderHelper.h

  Author:      David Borland

  Description: Handles converting values between a QSlider, which uses 
               integer values, and a continuous floating-point range.

=========================================================================*/


#ifndef AXMQSLIDERHELPER_H
#define AXMQSLIDERHELPER_H


#include <QSlider>
#include <QLineEdit>


class axmQSliderHelper {
public:
    axmQSliderHelper(QSlider* qSlider, QLineEdit* qLineEdit, 
                     double rangeMin, double rangeMax, double rangeStep);
    ~axmQSliderHelper();

    void SetValue(double value);

    double ToSlider();
    double FromSlider();

protected:
    QSlider* slider;
    QLineEdit* lineEdit;
    
    double range[2];
    double step;
};


#endif