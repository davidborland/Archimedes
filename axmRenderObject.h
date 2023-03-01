/*=========================================================================

  Name:        axmRenderObject.h

  Author:      David Borland

  Description: Abstract base class for drawing objects in Archimedes.

=========================================================================*/


#ifndef AXMRENDEROBJECT_H
#define AXMRENDEROBJECT_H


class axmRenderObject {
public:
    axmRenderObject();
    virtual ~axmRenderObject();

    // Color
    virtual void SetColor(double r, double g, double b);
    virtual void SetColor(double* rgb);
    virtual double* GetColor();

    // Opacity
    virtual void SetOpacity(double alpha);
    virtual double GetOpacity();

    // Position
    virtual void SetPosition(double x, double y);
    virtual void SetPosition(double* xy);
    virtual double* GetPosition();

    // Width
    virtual double GetWidth();
    virtual void SetWidth(double w);

    // Height
    virtual double GetHeight();
    virtual void SetHeight(double h);

    // Rotation in degrees
    virtual double GetRotation();
    virtual void SetRotation(double r);

    // Set the border
    virtual void BorderOn();
    virtual void BorderOff();
    virtual void SetBorderWidth(double pixels);

    // Pure virtual function to draw the object
    virtual void Draw() = 0;

protected:
    double color[3];
    double opacity;

    // The lower-left corner
    double position[2];

    double width;
    double height;

    double rotation;

    bool drawBorder;
    double borderWidth;
};


#endif