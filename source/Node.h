#pragma once
#include <memory>

struct Node
{
    int x, y;
    int g, h, f;
    std::shared_ptr<Node> parent;
};
