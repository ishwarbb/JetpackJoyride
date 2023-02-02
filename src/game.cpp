/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "gameobject.h"
#include "text_renderer.h"
#include <bits/stdc++.h>

glm::vec2 da = glm::vec2(0.0f,0.0f);
glm::vec2 db = glm::vec2(0.0f,0.0f);

void
PrintMat4( glm::mat4 mat )
{
        for( int col = 0; col < 4; col++ )
        {
		// transpose the matrix here:
                fprintf( stderr, "  %7.2f %7.2f %7.2f %7.2f\n",
			mat[0][col], mat[1][col], mat[2][col], mat[3][col] );
        }
}

// GameObject      *Player;
SpriteRenderer  *Renderer;
SpriteRenderer  *EntryRenderer;
BallObject     *Ball; 
GameObject     *BallShadow;
GameObject     *Entry; 
TextRenderer  *Text;

glm::mat4 transform = glm::mat4(1.0f); 
float road_pos;
float road_width;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    // delete Player;
    delete BallShadow;
    delete Ball;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../shaders/sprite.vs", "../shaders/sprite.frag", nullptr, "sprite");
    // ResourceManager::LoadShader("../shaders/sprite.vs", "../shaders/entry.frag", nullptr, "entry");
    ResourceManager::LoadShader("../shaders/ball.vs", "../shaders/ball.frag", nullptr, "ballshader");
    ResourceManager::LoadShader("../shaders/ballshadow.vs", "../shaders/ballshadow.frag", nullptr, "ballshadowshader");
    ResourceManager::LoadShader("../shaders/bar.vs", "../shaders/bar.frag", nullptr, "bar");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // ResourceManager::GetShader("entry").Use().SetInteger("image", 0);
    // ResourceManager::GetShader("entry").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader spriteShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(spriteShader);

    // Shader entryShader = ResourceManager::GetShader("entry");
    // EntryRenderer = new SpriteRenderer(entryShader);    
        // load textures
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/redface.png", true, "redface");
    // ResourceManager::LoadTexture("../textures/barry.jpeg", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/coin2.png", true, "coin");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    // ResourceManager::LoadTexture("../textures/fireball.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("../textures/road.jpg", false, "road");
    ResourceManager::LoadTexture("../textures/sign.png", true, "sign");
    ResourceManager::LoadTexture("../textures/white.png", false, "white");
    ResourceManager::LoadTexture("../textures/whitecircle.png", true, "whitecircle");

    // load levels
    GameLevel zero; zero.Load("../levels/zero.lvl", this->Width, this->Height,4);
    GameLevel one; one.Load("../levels/one.lvl", this->Width, this->Height,0);
    GameLevel two; two.Load("../levels/two.lvl", this->Width, this->Height ,1);
    GameLevel three; three.Load("../levels/three.lvl", this->Width, this->Height,2);
    GameLevel four; four.Load("../levels/four.lvl", this->Width, this->Height,3);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Levels.push_back(zero);
    this->Level = 0;

    glm::vec2 playerPos = glm::vec2(
        0,0
    );
    // Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = + glm::vec2(
        60, 
        0 );
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("face"));
    // BallShadow = new BallObject(Ball->Position - glm::vec2(30,30), BALL_RADIUS + 30, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("whitecircle"));
    // BallShadow->Radius = BALL_RADIUS + 30;

    float BallShadowSize = 2 * BALL_RADIUS;
    BallShadow = new GameObject(ballPos -  glm::vec2(30,30), glm::vec2(60 + BallShadowSize,60 + BallShadowSize), ResourceManager::GetTexture("whitecircle"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../fonts/ocraext.ttf", 24);

    this->Lives = 30;
    this->Coins = 0;
    this->Total_Coins = 0;
    this->Distance_Travelled = 0;
    this->Length = 1000000;

    road_pos = 0;
    road_width = this->Width;
    Ball->Acceleration = glm::vec2(0,9.8);
    // BallShadow->Acceleration = glm::vec2(0,9.8);

    // Entry = new GameObject(playerPos, glm::vec2(this->Width,this->Height), ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width, this->Height);
    // BallShadow->Move(dt, this->Width, this->Height);

    BallShadow->Position = Ball->Position - glm::vec2(30,30);
    // check for collisions
    this->DoCollisions();
    // check loss condition
    // if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    // {
    //     --this->Lives;
    //     if (this->Lives == 0)
    //     {
    //         this->ResetLevel();
    //         this->State = GAME_MENU;
    //     }

    //     this->ResetPlayer();
    // }

    if(this->State == GAME_ACTIVE && this->Lives == 0)
    {
            this->State = GAME_LOSE;
            this->Level = 4;
            ResetLevel();
    }

    // if(this->State == GAME_WIN || GAME_LOSE)
    // {
    //     this->Level = 4;
    // }

    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        this->State = GAME_WIN;
    }

    if(this->Length <= this->Distance_Travelled)
    {
        if(this->Level == 3)
        {
            this->State = GAME_WIN;
            this->Level = 4;
            ResetLevel();
        }
        else
        {
            this->Level++;
            ResetLevel();
        }
    }

    if(Ball->Velocity.y > 0)
    {
        this->Ball_Shadow = 0;
        Ball = new BallObject(Ball->Position, BALL_RADIUS, Ball->Velocity,ResourceManager::GetTexture("face"));
        // BallShadow = new BallObject(Ball->Position - glm::vec2(30,30), BALL_RADIUS, Ball->Velocity,ResourceManager::GetTexture("whitecircle"));
    }
}


void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_UP])
        {
            Ball->Velocity.y -= 55.0f;
            // BallShadow->Velocity.y -= 55.0f;
            this->Ball_Shadow = 1;
            Ball = new BallObject(Ball->Position, BALL_RADIUS, Ball->Velocity,ResourceManager::GetTexture("redface"));
            // BallShadow = new BallObject(Ball->Position, BALL_RADIUS, Ball->Velocity,ResourceManager::GetTexture("whitecircle"));
        }
    }


    if (this->State == GAME_MENU)
    {
        this->Level = 4;   // TO BE DECIDED
        this->Total_Coins = 0;


        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {

            this->Level = 0;     // TO BE DECIDED

            this->State = GAME_ACTIVE;
            this->ResetLevel();
            this->ResetPlayer();
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        // if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        // {
        //     this->Level = (this->Level + 1) % 4;
        //     this->KeysProcessed[GLFW_KEY_W] = true;
        // }
        // if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        // {
        //     if (this->Level > 0)
        //         --this->Level;
        //     else
        //         this->Level = 3;
        //     this->KeysProcessed[GLFW_KEY_S] = true;
        // }
    }


    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->State = GAME_MENU;
            this->Level = 4; /////////////////////
            ResetLevel();
        }
    }

    if (this->State == GAME_LOSE)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->State = GAME_MENU;
            this->Level = 4; ///////////////////////
            ResetLevel();
        }
    }
}

void Game::Render(float offset)
{
    // Texture2D myTexture =  ResourceManager::GetTexture("face");
    // Renderer->DrawSprite(myTexture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    if(this->State == GAME_ACTIVE || GAME_MENU)
    {
        // draw background
        Texture2D background = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(background, da,db,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
        );

        for (GameObject &tile : this->Levels[this->Level].Road)
        {
        if (!tile.Destroyed)
            tile.Draw(*Renderer);
        }

        // draw level
        // GameLevel one; one.Load("../levels/one.lvl", this->Width , this->Height );
        GameLevel &one = this->Levels[this->Level];
        if(this->State == GAME_ACTIVE)
        {
            for(int i = 0 ; i < one.Bricks.size(); i++)
            {
                one.Bricks[i].Position[0] = one.Bricks[i].Position[0] - offset;
                this->Distance_Travelled += offset;
            }

            for(int i = 0 ; i < one.Road.size(); i++)
            {
                one.Road[i].Position[0] = one.Road[i].Position[0] - offset;
            }

            // for(int i = 0 ; i < one.Zappers.size(); i++)
            // {
            //     one.Zappers[i].Position[0] = one.Zappers[i].Position[0] - offset;
            // }

            // for(int i = 0 ; i < one.ZapperBalls.size(); i++)
            // {
            //     one.ZapperBalls[i].Position[0] = one.ZapperBalls[i].Position[0] - offset;
            // }


            for(int i = 0 ; i < one.ZapperObjects.size(); i++)
            {
                one.ZapperObjects[i].first.Position[0] = one.ZapperObjects[i].first.Position[0] - offset;
                one.ZapperObjects[i].first.center.x -= offset;

                if( one.ZapperObjects[i].first.isRotate)
                    one.ZapperObjects[i].first.Rotation = one.ZapperObjects[i].first.Rotation +1;
                if( one.ZapperObjects[i].first.isUpandDown)
                {
                    float f = static_cast<float>(sin(glm::radians(glfwGetTime())));
                    one.ZapperObjects[i].first.Position[0] += f;
                    one.ZapperObjects[i].first.center.x += f;
                }
                if( one.ZapperObjects[i].first.isBackandForth)
                {
                    float g = static_cast<float>(cos(glm::radians(glfwGetTime()))) ;
                    if(g < 0) g *= -1;
                    one.ZapperObjects[i].first.Position[1] -= 0.5*g ;
                    one.ZapperObjects[i].first.center.y -= 0.5*g;
                }

                float rot = glm::radians(one.ZapperObjects[i].first.Rotation);
                // float rot = glm::radians(0.0f);;
                glm::vec2 center = one.ZapperObjects[i].first.center;
                int l = 200;
                int b = 8;

            one.ZapperObjects[i].second.first.Position.x = center.x + (static_cast<float>(sin(rot)) * ( l/ 2.0f)) - (BALL_RADIUS) ;
            one.ZapperObjects[i].second.first.Position.y = center.y - (static_cast<float>(cos(rot)) * ( l/ 2.0f)) - (BALL_RADIUS);
 
            one.ZapperObjects[i].second.second.Position.x = center.x - (static_cast<float>(sin(rot)) * ( l/ 2.0f)) - (BALL_RADIUS) ;
            one.ZapperObjects[i].second.second.Position.y = center.y + (static_cast<float>(cos(rot)) * ( l/ 2.0f)) - (BALL_RADIUS);

            one.ZapperObjects[i].first.end1 = one.ZapperObjects[i].second.first.Position + BALL_RADIUS;
            one.ZapperObjects[i].first.end2 = one.ZapperObjects[i].second.second.Position + BALL_RADIUS;

            // one.ZapperObjects[i].first.end1 = one.ZapperObjects[i].second.first.Position - BALL_RADIUS;
            // one.ZapperObjects[i].first.end2 = one.ZapperObjects[i].second.second.Position - BALL_RADIUS;
            }

        // this->Levels[this->Level] = one;

        // this->Levels[this->Level].Draw(*Renderer);

        // Player->Draw(*Renderer); 
        Ball->Draw(*Renderer);

        Shader ballShadowShader = ResourceManager::GetShader("ballshadowshader");
        SpriteRenderer* BallShadowRenderer = new SpriteRenderer(ballShadowShader);
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 
            600.0f, 0.0f, -1.0f, 1.0f);
        ResourceManager::GetShader("ballshadowshader").Use().SetInteger("image", 0);
        ResourceManager::GetShader("ballshadowshader").SetMatrix4("projection", projection);

        if(this->Ball_Shadow)
        {
            BallShadow->Draw(*BallShadowRenderer);
        }

        std::stringstream ss1; 
        std::stringstream ss2; 
        std::stringstream ss3; 
        std::stringstream ss4; 
        std::stringstream ss5; 
        ss1 << this->Coins;
        Text->RenderText("Coins:" + ss1.str(), 5.0f, 5.0f, 1.0f);

        ss2 << this->Lives;
        Text->RenderText("Lives:" + ss2.str(), (float)this->Width - 110.0f, 5.0f, 1.0f);

        ss3 << this->Distance_Travelled / 1000; 
        ss4 << this->Length / 1000; 
        Text->RenderText("Distance_Travelled:" + ss3.str() + "/" + ss4.str(), 5.0f, 25.0f, 1.0f);

        ss5 << (this->Level + 1);
        Text->RenderText("Level:" + ss5.str(), (float)this->Width - 110.0f, 25.0f, 1.0f);
        }

        one.Draw(*Renderer);

    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
        Text->RenderText("Use UP ARROW for controls", 245.0f, Height / 2 + 20.0f, 0.75f);
    }

    if (this->State == GAME_WIN)
    {
        Text->RenderText(
            "You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
            "Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
        std::stringstream ss;
        ss << this->Total_Coins; 
        Text->RenderText(
            "Score : "+ ss.str(), 130.0, Height / 2 + 20.0, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }

    if (this->State == GAME_LOSE)
    {
        Text->RenderText(
            "You LOSE :(", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
            "Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
        std::stringstream sss;
        sss << this->Total_Coins; 
        Text->RenderText(
            "Score : "+ sss.str(), 130.0, Height / 2 + 20.0, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
}


void Game::ResetLevel()
{
    this->Lives = 30;
    this->Total_Coins += this->Coins;
    this->Coins = 0;
    this->Distance_Travelled = 0;
    this->Levels[this->Level].Zappers.clear();
    this->Levels[this->Level].ZapperBalls.clear();
    this->Levels[this->Level].ZapperObjects.clear();
        this->Levels[this->Level].Road.clear();
    if (this->Level == 0)
        this->Levels[0].Load("../levels/one.lvl", this->Width, this->Height,0);
    else if (this->Level == 1)
        this->Levels[1].Load("../levels/two.lvl", this->Width, this->Height,1 );
    else if (this->Level == 2)
        this->Levels[2].Load("../levels/three.lvl", this->Width, this->Height,2 );
    else if (this->Level == 3)
        this->Levels[3].Load("../levels/four.lvl", this->Width, this->Height,3 );
}

void Game::ResetPlayer()
{
    // reset player/ball stats
    // Player->Size = PLAYER_SIZE;
    // Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset( glm::vec2( 60,0), INITIAL_BALL_VELOCITY);
    // BallShadow->Reset( glm::vec2( 10,0), INITIAL_BALL_VELOCITY);
}

// collision detection
bool CheckCollision(GameObject &one, GameObject &two);
bool CheckCollisionBalls(BallObject &one, BallObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
bool CheckCollisionNonAABB(BallObject &one, GameObject &two, glm::vec2 end1, glm::vec2 end2);
Collision CheckCollisionBallwithBoxAsBall(BallObject &one, GameObject &two); // AABB - Circle collision
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollisionBallwithBoxAsBall(*Ball, box);
            if (std::get<0>(collision)) // if collision is true
            {
                // destroy block if not solid
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                    this->Coins++;
                }
                else{
                    ResetPlayer();
                    this->Lives--;
                }         
            }
        }    
    }

    // for (GameObject &box : this->Levels[this->Level].Zappers)
    // {
    //     Collision collision = CheckCollision(*Ball, box);
    //     if(std::get<0>(collision))
    //     {
    //         ResetPlayer();
    //         this->Lives--;
    //                     // collision resolution
    //             Direction dir = std::get<1>(collision);
    //             glm::vec2 diff_vector = std::get<2>(collision);
    //             if (dir == LEFT || dir == RIGHT) // horizontal collision
    //             {
    //                 Ball->Velocity.x = -Ball->Velocity.x; // reverse horizontal velocity
    //                 // relocate
    //                 float penetration = Ball->Radius - std::abs(diff_vector.x);
    //                 if (dir == LEFT)
    //                     Ball->Position.x += penetration; // move ball to right
    //                 else
    //                     Ball->Position.x -= penetration; // move ball to left;
    //             }
    //             else // vertical collision
    //             {
    //                 Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
    //                 // relocate
    //                 float penetration = Ball->Radius - std::abs(diff_vector.y);
    //                 if (dir == UP)
    //                     Ball->Position.y -= penetration; // move ball bback up
    //                 else
    //                     Ball->Position.y += penetration; // move ball back down
    //             }   
    //     }       
    // }

    // for (BallObject &ball : this->Levels[this->Level].ZapperBalls)
    // {
    //     if(CheckCollisionBalls(*Ball,ball))
    //     {
    //         ResetPlayer();
    //         this->Lives--;
    //     }
    // }

    for (std::pair<GameObject, std::pair<BallObject, BallObject>> &boxi : this->Levels[this->Level].ZapperObjects)
    {
        GameObject box = boxi.first;
        glm::vec2 end1 = boxi.first.end1;
        glm::vec2 end2 = boxi.first.end2;
        bool collision = CheckCollisionNonAABB(*Ball, box,end1,end2);
        if (collision)
        {
            ResetPlayer();
            this->Lives--;
            // collision resolution
            // Direction dir = std::get<1>(collision);
            // glm::vec2 diff_vector = std::get<2>(collision);
            int penetration = 5;
            // if (dir == LEFT || dir == RIGHT) // horizontal collision
            // {
            //     Ball->Velocity.x = -Ball->Velocity.x; // reverse horizontal velocity
            //     // relocate
            //     float penetration = Ball->Radius - std::abs(diff_vector.x);
            //     if (dir == LEFT)
                    // Ball->Position.x += penetration; // move ball to right
            //     else
                    Ball->Position.x -= penetration; // move ball to left;
                    // BallShadow->Position.x -= penetration; // move ball to left;
            // }
            // else // vertical collision
            // {
            //     Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
            //     // relocate
            //     float penetration = Ball->Radius - std::abs(diff_vector.y);
            //     if (dir == UP)
                    Ball->Position.y -= penetration; // move ball bback up
                    // BallShadow->Position.y -= penetration; // move ball bback up
            //     else
            //         Ball->Position.y += penetration; // move ball back down
            // }
        }

        BallObject ball = boxi.second.first;
        if(CheckCollisionBalls(*Ball,ball))
        {
            ResetPlayer();
            this->Lives--;
        }

        ball = boxi.second.second;
        if(CheckCollisionBalls(*Ball,ball))
        {
            ResetPlayer();
            this->Lives--;
        }
    }

    // check collisions for player pad (unless stuck)
    // Collision result = CheckCollision(*Ball, *Player);
    // if (!Ball->Stuck && std::get<0>(result))
    // {
    //     // check where it hit the board, and change velocity based on where it hit the board
    //     float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
    //     float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
    //     float percentage = distance / (Player->Size.x / 2.0f);
    //     // then move accordingly
    //     float strength = 2.0f;
    //     glm::vec2 oldVelocity = Ball->Velocity;
    //     Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
    //     //Ball->Velocity.y = -Ball->Velocity.y;
    //     Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
    //     // fix sticky paddle
    //     Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
    // }
}

bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool CheckCollisionNonAABB(BallObject &one, GameObject &two, glm::vec2 end1, glm::vec2 end2)
{
    float x = one.Position.x + BALL_RADIUS;
    float y = one.Position.y + BALL_RADIUS;

    float x1 = end1.x;
    float x2 = end2.x;
    float y1 = end1.y;
    float y2 = end2.y;

    float a = y2 - y1;
    float b = x1 - x2;
    float c = (x2*y1) - (x1*y2);
    float normaliser = glm::sqrt(a*a + b*b);

    if(x <= x1 and x <= x2 ) return false;
    if(x >= x1 and x >= x2 ) return false;
    if(y <= y1 and y <= y2 ) return false;
    if(y >= y1 and y >= y2 ) return false;

    float d = (a*x + b*y + c) / normaliser;
    d = abs(d);

    // printf("%f\n",d);

    if(d <= (BALL_RADIUS + 10))
    {
        // printf("Hello\n");
        return true;
    }
    
    return false;
}

bool CheckCollisionBalls(BallObject &one, BallObject &two)
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 center2(two.Position + two.Radius);

    glm::vec2 difference = center - center2;

    if (glm::length(difference) <= one.Radius + two.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return true;
    else
        return false;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius + (two.Size.x / 2.0f)) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Collision CheckCollisionBallwithBoxAsBall(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    // glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    // glm::vec2 closest = aabb_center + clamped;
    // // now retrieve vector between center circle and closest point AABB and check if length < radius
    // difference = closest - center;

    if (glm::length(difference) < one.Radius + (two.Size.x / 2.0f)) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

// calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}


