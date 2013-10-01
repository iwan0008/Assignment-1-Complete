//Student:			Brendan Iwanyk
//Course Number:	GAM1514
//Professor:		Danny Lingman
//Purpose:			Main game class header

#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Brick.h"
#include "../Screen Manager/Screen.h"

class GameObject;
class OpenGLTexture;

//The Game class creates game objects and handles
//user input and game logic. Inherits from Screen
//and must implement update(), paint() and getName()
class Game : public Screen
{
public:
    Game();
    ~Game();

    Brick* tempBrick;

    //Game lifecycle methods
    void update(double delta);
    void paint();
    void reset();

    //Game Over method, call this when to end the game
    void gameOver();

    //Screen name, must be implemented, it's a pure
    //virtual method in the Screen class
    const char* getName();

    //Screen event method, inherited from the screen class
    void screenWillAppear();

    //GameObject Methods
    void addGameObject(GameObject* gameObject);
    GameObject* getGameObjectByType(const char* type);

    //Additional functions
    int getNumberOfLives();
    void setNumberOfLives(int lives);

    int getNumberOfBricksLeft();
    void setNumberOfBricksLeft(int bricksLeft);

    //Vector to hold the GameObjects
    //std::vector<GameObject*> m_GameObjects;

private:
    //Mouse Events
    void mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY);

    //Key Events
    void keyUpEvent(int keyCode);

    //Vector to hold the GameObjects
    std::vector<GameObject*> m_GameObjects;

    //Timer variable to delay reseting the game has ended
    //double m_GameOverTimer;

    //Additional Variables
    int m_CurrentLevel;
    int m_NumberOfBricksLeft;
    int m_NumberOfLives;
    bool m_ReplayTrigger;
    bool m_GameOver;

    //Texture Variables
    OpenGLTexture* m_BackGroundTexture;
    OpenGLTexture* m_GameOverTexture;
    OpenGLTexture* m_HealthTexture;
};

#endif