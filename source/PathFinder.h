#pragma once

#include <memory>

struct Node;
class Maze;

class PathFinder
{
public:
    static std::shared_ptr<Node> FindPath(Maze* maze, int sx, int sy, int tx, int ty);
    static std::shared_ptr<Node> SimplifyPath(std::shared_ptr<Node> node);

private:
    static int Heuristic(int sx, int sy, int tx, int ty);
};
