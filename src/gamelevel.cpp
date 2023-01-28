/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "gamelevel.h"
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
        {
            this->init(tileData, levelWidth, levelHeight);
            this->initRoad(levelWidth,levelHeight);
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    // return true;
    // if(this.)
    return false;
}

void GameLevel::initRoad( unsigned int levelWidth, unsigned int levelHeight)
{
    for(unsigned int i = 0; i < 100; i++)
    {
        glm::vec2 pos((1200)*i, levelHeight - 600);
        glm::vec2 size(400, 600);;
        GameObject obj(pos, size, ResourceManager::GetTexture("sign"), glm::vec3(0.8f, 0.8f, 0.7f));
        this->Road.push_back(obj);
    }
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    // float unit_width = levelWidth /( 2 * static_cast<float>(width));
    float unit_height = levelHeight / height; 
    float unit_width = unit_height;

    printf("%f\n",unit_height);
    // initialize level tiles based on tileData	
	
    for (unsigned int x = 0; x < 100; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            int x_coord = x % width;
            // float offset = (float) 50 * glfwGetTime();
            float offset = 0;
            // check block type from level data (2D level array)
            if (tileData[y][x_coord] == 1) // solid
            {
                glm::vec2 pos((levelWidth/2) - offset + unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x_coord] == 2)	// non-solid; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white

                glm::vec2 pos((levelWidth/2) - offset + unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("coin"), glm::vec3(1.0f, 1.0f, 1.0f)));
            }
        }
    }
}