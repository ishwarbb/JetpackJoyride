/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "gamelevel.h"
#include "ball_object.h"
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight, int level)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;

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
            this->init(tileData, levelWidth, levelHeight, level);
            this->initRoad(levelWidth,levelHeight);
            // this->initZapper(levelWidth,levelHeight);
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    Shader ballShader = ResourceManager::GetShader("ballshader");
    SpriteRenderer * BallRenderer = new SpriteRenderer(ballShader);
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 
        600.0f, 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("ballshader").Use().SetInteger("image", 0);
    ResourceManager::GetShader("ballshader").SetMatrix4("projection", projection);

    // for (GameObject &tile : this->Bricks)
    //     if (!tile.Destroyed)
    //         tile.Draw(renderer);

    // for (GameObject &tile : this->Zappers)
    //     if (!tile.Destroyed)
    //         tile.Draw(renderer);

    // for (GameObject &tile : this->ZapperBalls)
    //     if (!tile.Destroyed)
    //         tile.Draw(renderer);

        for (std::pair<GameObject,std::pair<BallObject,BallObject>> &tile : this->ZapperObjects)
        {
            tile.first.Draw(renderer);
            tile.second.first.Draw(*BallRenderer);
            // tile.second.first.Draw(renderer);
            tile.second.second.Draw(*BallRenderer);
            // tile.second.second.Draw(renderer);
        }

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

std::vector<float> Angles = {45.0f,91.0f,135.0f};

void GameLevel::initZapper( unsigned int levelWidth, unsigned int levelHeight, int level)
{
        glm::vec2 pos(levelWidth, levelHeight);
        glm::vec2 size(8, 200);
        GameObject obj(pos, size, ResourceManager::GetTexture("white"), glm::vec3(1.0f, 1.0f, 1.0f));
        obj.isZapper = true;
        obj.isRotate = false;
        if(level > 0) obj.isRotate = rand()%2;
        if(level > 1) obj.isBackandForth = rand()%2;
        if(level > 1) obj.isUpandDown = rand()%2;
        if(level == 3) obj.isBackandForth = true;
        if(level == 3) obj.isUpandDown = true;
        obj.Rotation = Angles[rand()%3];

        obj.center = pos  + (size /2.0f);

        glm::vec2 ball1Pos = glm::vec2(pos.x + size.x/2.0f - BALL_RADIUS, pos.y - BALL_RADIUS); 
        BallObject ball1(ball1Pos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("whitecircle"));
        ball1.Color = glm::vec4 (1.0f,1.0f,1.0f,1.0f);

        glm::vec2 ball2Pos = glm::vec2(pos.x + size.x/2.0f - BALL_RADIUS, pos.y + size.y - BALL_RADIUS); 
        BallObject ball2(ball2Pos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("whitecircle"));
        ball2.Color = glm::vec4 (1.0f,1.0f,1.0f,1.0f);

        // obj.Balls.push_back(ball1);
        // obj.Balls.push_back(ball2);

        this->Zappers.push_back(obj);
        // this->ZapperBalls.push_back(ball1);
        // this->ZapperBalls.push_back(ball2);

        this->ZapperObjects.push_back({obj,{ball1,ball2}});
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

std::vector<std::vector<int>> coins3 = {{1, 1, 1, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 1, 1, 1},
                                    {0, 0, 1, 1, 1, 1, 0, 0},
                                    {1, 1, 1, 1, 1, 1, 1, 1},
                                    {0, 0, 0, 0, 0, 0, 0, 0}};

std::vector<std::vector<int>> coins2 = {{0, 1, 0, 0, 0, 0, 1, 0},
                                        {0, 0, 0, 1, 1, 0, 0, 0},
                                        {0, 0, 1, 0, 0, 1, 0, 0},
                                        {1, 1, 0, 0, 0, 0, 1, 1},
                                        {0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0}
                                        };

std::vector<std::vector<int>> zappers = {{0, 0, 0, 0, 0, 0, 2, 0},
                                        {0, 2, 0, 0, 0, 0, 0, 0},
                                        // {0, 0, 2, 0, 0, 2, 0, 0},
                                        {0, 0, 2, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 2, 0, 0},
                                        {0, 2, 0, 0, 0, 0, 2, 0}
                                        };

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight, int level)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    // float unit_width = levelWidth /( 2 * static_cast<float>(width));
    float unit_height = 600 / 8;; 
    float unit_width = unit_height;

    // initialize level tiles based on tileData	
	
    for (unsigned int x = 0; x < 100; ++x)
    {
        std::vector<int> col;

        if(x%8 < 1) col = coins3[rand()%coins3.size()];
        else if(x%8 < 7) col = coins2[rand()%coins2.size()];
        else col = zappers[rand()%zappers.size()];

        for (unsigned int y = 0; y < col.size(); ++y)
        {
            // int x_coord = x % width;
            // float offset = (float) 50 * glfwGetTime();
            float offset = 0;
            // check block type from level data (2D level array)
            if (col[y] == 1) // solid
            {
                glm::vec2 pos((levelWidth/2) - offset + unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("coin"), glm::vec3(0.8f, 0.8f, 0.7f));
                // obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (col[y] == 2)	// non-solid; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white

                glm::vec2 pos((levelWidth/2) - offset + unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                this->initZapper((levelWidth/2) - offset + unit_width * x, unit_height * y,level);
                // this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("coin"), glm::vec3(1.0f, 1.0f, 1.0f)));
            }
        }
    }
}


// void DrawSpriteBall(Shader &shader,Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
// {
//     // prepare transformations
//     shader.Use();
//     glm::mat4 model = glm::mat4(1.0f);
//     model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

//     model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
//     model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
//     model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

//     model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

//     shader.SetMatrix4("model", model);

//     // render textured quad
//     shader.SetVector3f("spriteColor", color);

//     glActiveTexture(GL_TEXTURE0);
//     texture.Bind();

//     glBindVertexArray(quadVAO);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
//     glBindVertexArray(0);
// }