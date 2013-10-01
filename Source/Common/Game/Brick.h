//Student:			Brendan Iwanyk
//Course Number:	GAM1514
//Professor:		Danny Lingman
//Purpose:			Brick Class Header

#ifndef BRICK_H
#define BRICK_H

#include "GameObject.h"

// The brick class sets the size of the bricks and paints them.
// Inherits from GameObject, must implement update(), paint()
// and getName()
class Brick : public GameObject
{
public:
    Brick(float brickX, float brickY);
    ~Brick();

    void update(double delta);
    void paint();

    void reset();

    const char* getType();

protected:
    float m_BrickWidth;
    float m_BrickHeight;
};

#endif