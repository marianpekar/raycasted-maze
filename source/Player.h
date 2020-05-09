#pragma once

class Player
{
public:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    int m_turnDirection; // -1 for left, +1 for right
    int m_walkDirection; // -1 for back, +1 for front
    float m_rotationAngle;
    float m_walkSpeed;
    float m_turnSpeed;
};

