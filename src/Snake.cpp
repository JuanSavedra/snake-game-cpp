#include "Snake.h"
#include <iostream>

Snake::Snake(int startX, int startY)
{
  body.push_back({startX, startY});
  currentDirection = Direction::RIGHT;
  nextDirection = Direction::RIGHT;
}

void Snake::move(bool grow)
{
  currentDirection = nextDirection;
  GridPosition newHead = getHead();

  switch (currentDirection)
  {
  case Direction::UP:
    newHead.y += 1;
    break;
  case Direction::DOWN:
    newHead.y -= 1;
    break;
  case Direction::RIGHT:
    newHead.x += 1;
    break;
  case Direction::LEFT:
    newHead.x -= 1;
    break;
  }

  body.insert(body.begin(), newHead);

  if (!grow)
  {
    body.pop_back();
  }
}

void Snake::changeDirection(Direction newDirection)
{
  if (currentDirection == Direction::UP && newDirection == Direction::DOWN)
    return;
  if (currentDirection == Direction::DOWN && newDirection == Direction::UP)
    return;
  if (currentDirection == Direction::LEFT && newDirection == Direction::RIGHT)
    return;
  if (currentDirection == Direction::RIGHT && newDirection == Direction::LEFT)
    return;

  nextDirection = newDirection;
}

bool Snake::isOutOfBounds(int gridWidth, int gridHeight) const
{
  const GridPosition& head = getHead();
  return head.x < 0 || head.x >= (gridWidth + 6) ||
         head.y < 0 || head.y >= gridHeight;
}

bool Snake::isSelfCollision() const
{
  if (body.size() < 4)
  {
    return false;
  }

  const GridPosition &head = getHead();
  for (size_t i = 1; i < body.size(); i++)
  {
    if (head == body[i])
    {
      return true;
    }
  }

  return false;
}

GridPosition Snake::getHead() const {
  return body[0];
}