#pragma once

struct Ray {
    float m_rayAngle;
    float m_wallHitX;
    float m_wallHitY;
    float m_distance;
    int m_wasHitVertical;
    int m_isRayFacingUp;
    int m_isRayFacingDown;
    int m_isRayFacingLeft;
    int m_isRayFacingRight;
    int m_wallHitContent;
};