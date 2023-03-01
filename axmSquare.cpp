/*=========================================================================

  Name:        axmSquare.cpp

  Author:      David Borland

  Description: Represents a square shape

=========================================================================*/


#include "axmSquare.h"


axmSquare::axmSquare() {
}

axmSquare::~axmSquare() {
}


void axmSquare::SetWidth(double w) {
    width = w;
    height = w;
}

void axmSquare::SetHeight(double h) {
    SetWidth(h);
}
