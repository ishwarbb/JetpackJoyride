#include "ball_object.h"


BallObject::BallObject() 
    : GameObject(), Radius(BALL_RADIUS), Stuck(true) { }

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 BallObject::Move(float dt, unsigned int window_width, unsigned int window_height)
{
    // if not stuck to player board
    // if (!this->Stuck)
    // {
        // move the ball
        this->Position += this->Velocity * dt;
        this->Velocity.y = this->Velocity.y + 29.8;
        // printf("%f",this->Velocity.y);
        // then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = 1.0f;
            this->Position.y = 0.0f;
        }
        else if (this->Position.y + this->Size.y >= window_height)
        {
            this->Velocity.y = 0.0f;
            this->Position.y = window_height - this->Size.y;
        }
    // }
    return this->Position;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = glm::vec2(0,0);
    this->Stuck = true;
}