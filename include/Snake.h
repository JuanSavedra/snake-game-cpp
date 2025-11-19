#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
//#include <algorithm>

struct GridPosition {
    int x;
    int y;

    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y;
    }
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    Snake(int startX, int startY);

    void move(bool grow);
    void changeDirection(Direction newDirection);
    
    bool isOutOfBounds(int gridWidth, int gridHeight) const;
    bool isSelfCollision() const;

    GridPosition getHead() const;
    const std::vector<GridPosition>& getBody() const { return body; }
    Direction getCurrentDirection() const { return currentDirection; }

private:
    std::vector<GridPosition> body;
    Direction currentDirection;
    Direction nextDirection; 
};

#endif