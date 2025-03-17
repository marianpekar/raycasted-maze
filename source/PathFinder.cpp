#include "PathFinder.h"
#include "Node.h"
#include "Maze.h"

#include <vector>
#include <algorithm>
#include <cmath>

static constexpr int directionsX[4] = {0,  1,  0, -1};
static constexpr int directionsY[4] = {-1, 0,  1,  0};

std::shared_ptr<Node> PathFinder::FindPath(Maze* maze, int sx, int sy, int tx, int ty)
{
    std::vector<std::shared_ptr<Node>> open;
    std::vector<std::shared_ptr<Node>> closed;

    auto startNode = std::make_shared<Node>(Node{
        sx,
        sy,
        0,
        Heuristic(sx, sy, tx, ty),
        0,
        nullptr
    });
    startNode->f = startNode->g + startNode->h;
    open.push_back(startNode);

    while (!open.empty())
    {
        auto currentIt = std::ranges::min_element(
            open,
            [](const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) {
                return lhs->f < rhs->f;
            }
        );
        std::shared_ptr<Node> currentNode = *currentIt;

        std::erase(open, currentNode);

        if (currentNode->x == tx && currentNode->y == ty)
        {
            return currentNode;
        }

        closed.push_back(currentNode);

        for (int i = 0; i < 4; ++i)
        {
            int newRow = currentNode->y + directionsY[i];
            int newCol = currentNode->x + directionsX[i];

            if (!maze->IsValid(newRow, newCol))
                continue;

            if (maze->m_maze[newRow][newCol])
                continue;

            bool alreadyClosed = std::ranges::any_of(
                closed,
                [newRow, newCol](const std::shared_ptr<Node>& node) {
                    return (node->x == newCol && node->y == newRow);
                }
            );
            
            if (alreadyClosed)
                continue;

            int newG = currentNode->g + 1;
            int newH = Heuristic(newCol, newRow, tx, ty);
            int newF = newG + newH;

            auto itOpen = std::ranges::find_if(
                open,
                [newRow, newCol](const std::shared_ptr<Node>& node) {
                    return (node->x == newCol && node->y == newRow);
                }
            );

            if (itOpen == open.end() || newG < (*itOpen)->g)
            {
                if (itOpen != open.end()) {
                    open.erase(itOpen);
                }

                auto neighbor = std::make_shared<Node>(Node{
                    newCol,
                    newRow,
                    newG,
                    newH,
                    newF,
                    currentNode
                });
                open.push_back(neighbor);
            }
        }
    }

    return {};
}

int PathFinder::Heuristic(int sx, int sy, int tx, int ty)
{
    return std::abs(sx - tx) + std::abs(sy - ty);
}
