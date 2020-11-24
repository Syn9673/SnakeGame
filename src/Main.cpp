#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define START_SIZE 4
#define Y_GRID 32 * 10
#define X_GRID 32 * 20

struct Cell {
  float x;
  float y;
};

void draw(sf::RenderWindow &window,
          std::vector<Cell> &vec,
          Cell &apple)
{
  window.clear();
  for (auto i : vec)
  {
    sf::Texture cell;
    cell.loadFromFile("assets/cell.png");

    sf::Sprite cellSpr;
    cellSpr.setTexture(cell);
    cellSpr.setPosition(sf::Vector2f(i.x, i.y));
    
    window.draw(cellSpr);
  }

  // apple
  sf::Texture cell;
  cell.loadFromFile("assets/apple.png");

  sf::Sprite cellSpr;
  cellSpr.setTexture(cell);
  cellSpr.setPosition(sf::Vector2f(apple.x, apple.y));

  window.draw(cellSpr);

  window.display();
}

enum Directions {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

int main()
{
  unsigned int score = 0;
  Directions currentDirection = Directions::DOWN;
  float appleX = (float)(rand() % 20);
  float appleY = (float)(rand() % 10);

  Cell apple = {appleX * 32, appleY * 32};

  std::vector<Cell> cells;
  sf::RenderWindow window(sf::VideoMode(X_GRID, Y_GRID), "Snake Game | Score: " + std::to_string(score));

  for (int i = 0; i < 4; ++i)
  {
    Cell cell;

    cell.x = 0;
    cell.y = (float) 32 * i;

    cells.push_back(cell);
  }

  sf::Clock clock;

  while (window.isOpen())
  {

    auto time = clock.getElapsedTime();
    if (time.asSeconds() >= 0.1f)
    {
      clock.restart();
      auto size = cells.size();

      if (size < 1)
      {
        for (int i = 0; i < 4; ++i)
        {
          Cell cell;

          cell.x = 0;
          cell.y = (float) 32 * i;

          cells.push_back(cell);
        }

        currentDirection = Directions::DOWN;
        size = 4;
      }

      auto cell = cells[size - 1]; // get last cell
      switch (currentDirection)
      {
        case DOWN:
          cell.y += 32.0f;
          break;

        case RIGHT:
          cell.x += 32.0f;
          break;

        case UP:
          cell.y -= 32.0f;
          break;

        case LEFT:
          cell.x -= 32.0f;
          break;
      }

      if (cell.y >= Y_GRID)
        cell.y = 0;

      if (cell.y < 0)
        cell.y = Y_GRID;

      if (cell.x >= X_GRID)
        cell.x = 0;

      if (cell.x < 0)
        cell.x = X_GRID;

      for (int i = 0; i < size - 1; ++i)
      {
        auto iCell = cells[i];
        if (cell.x == iCell.x &&
            cell.y == iCell.y)
        {
          cells.clear();
          score = 0;

          window.setTitle("Snake Game | Score: 0");

          break;
        } else if (cell.x == apple.x &&
                    cell.y == apple.y)
        {
          apple.x = (float)(rand() % 20) * 32;
          apple.y = (float)(rand() % 10) * 32;

          auto firstCell = cells[0];

          // push new cell
          cells.insert(cells.begin(), firstCell);
          window.setTitle("Snake Game | Score: " + std::to_string(++score));
        }
      }

      cells.push_back(cell);
      cells.erase(cells.begin());

      draw(window, cells, apple);
    }
    
    sf::Event event;
    while (window.pollEvent(event))
    {

      switch (event.type)
      {

        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code)
          {
            case sf::Keyboard::D:
              if (currentDirection == Directions::LEFT)
                break;

              currentDirection = Directions::RIGHT;
              break;
            
            case sf::Keyboard::A:
              if (currentDirection == Directions::RIGHT)
                break;

              currentDirection = Directions::LEFT;
              break;

            case sf::Keyboard::W:
              if (currentDirection == Directions::DOWN)
                break;

              currentDirection = Directions::UP;
              break;

            case sf::Keyboard::S:
              if (currentDirection == Directions::UP)
                break;

              currentDirection = Directions::DOWN;
              break;
          }
          break;

      }

    }

  }

  return 0;
}