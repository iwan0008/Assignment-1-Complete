//Student:			Brendan Iwanyk
//Course Number:	GAM1514
//Professor:		Danny Lingman
//Purpose:			Main game class

#include "Game.h"
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "..//Utils/Utils.h"
#include "../Screen Manager/ScreenManager.h"

//// here there be dragons
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG
//
//// end of dragons

Game::Game()
{
    //Initialize the variables
    m_NumberOfBricksLeft = 24;
    m_NumberOfLives = 3;
    m_CurrentLevel = 1;
    m_ReplayTrigger = false;
    m_GameOver = false;

    float t_PositionX = 50;
    float t_PositionY = 50;

    //Create new textures
    m_BackGroundTexture = new OpenGLTexture("space");
    m_GameOverTexture = new OpenGLTexture("GameOver");
    m_HealthTexture = new OpenGLTexture("heart");

    //Create a new paddle and ball
    addGameObject(new Paddle());
    addGameObject(new Ball());

    //Create new bricks
    for (int i = 0; i < 24; i++)
    {
        if (i == 8)
        {
            t_PositionX = 50;
            t_PositionY = 150;
        }
        else if (i == 16)
        {
            t_PositionX = 50;
            t_PositionY = 250;
        }
        tempBrick = new Brick(t_PositionX, t_PositionY);
        addGameObject(tempBrick);
        t_PositionX += 120;
    }
    /*tempBrick = new Brick(100,100,200,200);
    addGameObject(tempBrick);*/

    reset();

    // apparently missed a dragon somewhere.
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    // that should be all of them
}

Game::~Game()
{
    //Delete the textures
    if (m_BackGroundTexture != NULL)
    {
        delete m_BackGroundTexture;
        m_BackGroundTexture = NULL;
    }

    if (m_GameOverTexture != NULL)
    {
        delete m_GameOverTexture;
        m_GameOverTexture = NULL;
    }

    if (m_HealthTexture != NULL)
    {
        delete m_HealthTexture;
        m_HealthTexture = NULL;
    }

    //Delete all the GameObject's in the vector
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        if (m_GameObjects.at(i) != NULL)
        {
            delete m_GameObjects.at(i);
            m_GameObjects.at(i) = NULL;
        }
    }

    //Clear the pointers from the vector
    m_GameObjects.clear();
}

void Game::update(double aDelta)
{
    //If the GameOver Timer is greater that zero, countdown
    //if(m_GameOverTimer > 0.0)
    //{
    //	m_GameOverTimer -= aDelta;	
    //	if(m_GameOverTimer < 0.0)
    //	{		
    //		if (m_UserInput != 'r' || m_UserInput != 'R')
    //		{
    //			return;
    //		}
    //		//m_GameOverTimer = 0.0;
    //		reset();
    //	}
    //	return;
    //}

    //Get the ball GameObject, we'll use this for collision detection and to set the balls speed
    Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);

    // If there are no bricks left, increment the level, increase the balls speed, increase lives and reset the number of bricks
    if (m_NumberOfBricksLeft == 0 && m_GameOver == false)
    {
        m_CurrentLevel++;
        m_NumberOfLives++;
        reset();
        if (ball->getSpeed() >= GAME_BALL_MAX_SPEED)
        {

        }
        else
        {
            ball->setSpeed(ball->getSpeed() + 150);
        }	
    }

    //If the user is out of lives, check to see if they hit the 'r' key to reset
    if (m_GameOver == true)
    {
        if (m_ReplayTrigger)
        {
            ball->setSpeed(GAME_BALL_DEFAULT_SPEED);
            reset();
        }
        return;
    }

    //If all three lives are lost end the game
    if(m_NumberOfLives <= 0)
    {
        gameOver();
    }


    //Cycle through all the game objects update them and check their collision detection
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        //Make sure the GameObject is active
        if(m_GameObjects.at(i)->getIsActive() == true)
        {
            //Update the GameObject
            m_GameObjects.at(i)->update(aDelta);

            //Check collision detection against the ball
            if(m_GameObjects.at(i) != ball)
            {
                ball->checkCollision(m_GameObjects.at(i));
            }
        }
    }
}

void Game::paint()
{
    //Get the screen width and height
    float screenWidth = ScreenManager::getInstance()->getScreenWidth();
    float screenHeight = ScreenManager::getInstance()->getScreenHeight();

    //If the user is out of lives, paint the game over screen until they hit the 'r' key
    if(m_GameOver == true)
    {
        OpenGLRenderer::getInstance()->drawTexture(m_GameOverTexture,0.0f,0.0f,screenWidth,screenHeight);
        return;
    }

    //Draw the background first
    OpenGLRenderer::getInstance()->drawTexture(m_BackGroundTexture,0.0f,0.0f,screenWidth,screenHeight);

    //Cycle through and draw all the game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        if(m_GameObjects.at(i)->getIsActive() == true)
        {
            m_GameObjects.at(i)->paint();
        }
    }

    //Draw the outer white walls
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
    OpenGLRenderer::getInstance()->setLineWidth(4.0f);
    OpenGLRenderer::getInstance()->drawLine(1.0f, 0.0f, 1.0f, getHeight());
    OpenGLRenderer::getInstance()->drawLine(0.0f, 1.0f, getWidth(), 1.0f);
    OpenGLRenderer::getInstance()->drawLine(getWidth() - 1, 0.0f, getWidth() - 1, getHeight());
    OpenGLRenderer::getInstance()->setLineWidth(1.0f);

    //Draw the health counters
    int DistanceBetweenHealth = 50;
    for (int i = 0; i < m_NumberOfLives; i++)
    {
        OpenGLRenderer::getInstance()->drawTexture(m_HealthTexture, screenWidth - DistanceBetweenHealth, screenHeight - 40.0f, GAME_HEALTH_COUNTER_SIZE, GAME_HEALTH_COUNTER_SIZE);
        DistanceBetweenHealth += 40;
    }
}

void Game::reset()
{
    //Cycle through and reset all the game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        m_GameObjects.at(i)->reset();
    }

    //Reset the lives
    if (m_GameOver == true)
    {
        m_NumberOfLives = 3;
        m_CurrentLevel = 1;
    }

    //Reset the game over timer to zero
    //m_GameOverTimer = 0.0;
    m_GameOver = false;

    //Reser the user input
    m_ReplayTrigger = false;

    //Reset the number of bricks
    m_NumberOfBricksLeft = 24;

}

void Game::gameOver()
{
    //m_GameOverTimer = GAME_OVER_TIMER;
    m_GameOver = true;
}

const char* Game::getName()
{
    return GAME_SCREEN_NAME;
}

void Game::screenWillAppear()
{
    OpenGLRenderer::getInstance()->setBackgroundColor(OpenGLColorCornflowerBlue());
}

void Game::addGameObject(GameObject* aGameObject)
{
    if(aGameObject != NULL)
    {
        m_GameObjects.push_back(aGameObject);
    }
}

GameObject* Game::getGameObjectByType(const char* aType)
{
    //Cycle through a find the game object (if it exists)
    for(unsigned int i = 0; i < m_GameObjects.size(); i++)
    {
        if(strcmp(m_GameObjects.at(i)->getType(), aType) == 0)
        {
            return m_GameObjects.at(i);
        }
    }
    return NULL;
}

void Game::mouseMovementEvent(float aDeltaX, float aDeltaY, float aPositionX, float aPositionY)
{
    //Set the paddle to the x position of the mouse
    Paddle* paddle = (Paddle*)getGameObjectByType(GAME_PADDLE_TYPE);

    //Safety check, paddle could be NULL
    if(paddle != NULL)
    {
        paddle->setX(aPositionX - (paddle->getWidth() / 2.0f));
    }
}

void Game::keyUpEvent(int aKeyCode)
{
    //Check to see if the user hit the 'r' key
    if (m_GameOver == true)
    {
        if (/*aKeyCode == 114 || aKeyCode == 82*/aKeyCode == KEYCODE_R)
        {
            m_ReplayTrigger = true;
        }
    }
}

int Game::getNumberOfLives()
{
    return m_NumberOfLives;
}

void Game::setNumberOfLives(int lives)
{
    m_NumberOfLives = lives;
}

int Game::getNumberOfBricksLeft()
{
    return m_NumberOfBricksLeft;
}

void Game::setNumberOfBricksLeft(int bricksLeft)
{
    m_NumberOfBricksLeft = bricksLeft;
}
