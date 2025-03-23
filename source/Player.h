#pragma once

struct Player
{
    float x;
    float y;
    float width;
    float height;
    float turnDirection; // -1 for left, +1 for right
    float walkDirection; // -1 for back, +1 for front
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
    bool isBot = false;
};

