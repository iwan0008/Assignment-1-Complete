//Student:			Brendan Iwanyk
//Course Number:	GAM1514
//Professor:		Danny Lingman
//Purpose:			Brick Class Header

#include "Brick.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"

Brick::Brick(float posX, float posY)
{
    m_Texture = new OpenGLTexture("metal_texture");
    setPosition(posX, posY);

}

Brick::~Brick()
{
    if (m_Texture != NULL)
    {
        delete m_Texture;
        m_Texture = NULL;
    }
}

void Brick::update(double delta)
{

}

void Brick::paint()
{
    /*OpenGLRenderer::getInstance()->setLineWidth(2.0f);
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
    OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), GAME_BRICK_WIDTH(), getBrickHeight());
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
    OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), GAME_BRICK_WIDTH(), getBrickHeight(),false);
    OpenGLRenderer::getInstance()->setLineWidth(1.0f);*/

    OpenGLRenderer::getInstance()->drawTexture(m_Texture, getX(), getY(), GAME_BRICK_WIDTH, GAME_BRICK_HEIGHT);
}

void Brick::reset()
{
    //Set the brick back to being active
    setIsActive(true);
}

const char* Brick::getType()
{
    return GAME_BRICK_TYPE;
}