/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameobject.h"
#include "ball_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"



/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // level state
    std::vector<GameObject> Bricks;
    std::vector<GameObject> Road;
    std::vector<GameObject> Zappers;
    std::vector<BallObject> ZapperBalls;
    std::vector<std::pair<GameObject, std::pair<BallObject,BallObject>>> ZapperObjects; 
    std::vector<std::pair<float, std::pair<float,float>>> Bars; 
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight,int level);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();

private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight, int level);
    void initRoad( unsigned int levelWidth, unsigned int levelHeight);
    void initZapper( unsigned int levelWidth, unsigned int levelHeight, int level);
};

#endif