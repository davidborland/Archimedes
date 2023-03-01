/*=========================================================================

  Name:        Archimedes.cpp

  Author:      David Borland

  Description: Contains the main function for Archimedes program.  Just 
               instantiates a QApplication object and the application 
               main window using Qt.

=========================================================================*/


#include "axmMainWindow.h"

#include <qapplication.h>


int main(int argc, char** argv) {
    // Initialize Qt
    QApplication app(argc, argv);

    axmMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

