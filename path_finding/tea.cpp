#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <cmath>
#include <thread>
#include <unistd.h>

#define MAP_SIZE 1024

sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");


int map[MAP_SIZE][MAP_SIZE];
int future[MAP_SIZE][MAP_SIZE];

void drawMap();
void generateMap();
void gameOfLife();
void outputRegion(std::pair<int, int> left, std::pair<int, int> right);

int main()
{

    // Filling array
    {
        // map.fill(0);
        memset( &map[0][0], 0, sizeof(int)*MAP_SIZE*MAP_SIZE );
        memset( &future[0][0], 0, sizeof(int)*MAP_SIZE*MAP_SIZE );
    }

    // generate Map
    {
        srand(time(0));
        generateMap();
    }


    for (int i = 0; i < 10000; i++)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // window.clear();
        gameOfLife();
        outputRegion({(MAP_SIZE/2 - MAP_SIZE/4), (MAP_SIZE/2 - MAP_SIZE/4)}, {(MAP_SIZE/2 + MAP_SIZE/4), (MAP_SIZE/2 + MAP_SIZE/4) });
        // drawMap();
        // window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}

void drawMap()
{
    // define a 120x50 rectangle
    sf::RectangleShape white(sf::Vector2f(1, 1));
    white.setFillColor(sf::Color::White);
    sf::RectangleShape black(sf::Vector2f(1, 1));
    black.setFillColor(sf::Color::Black);
    for(int x_pos = 0; x_pos < MAP_SIZE; x_pos++)
    {
        for(int y_pos = 0; y_pos < MAP_SIZE; y_pos++)
        {

            // let's say
            // 0 = dead
            // 1 = alive

            // Unvisted
            if(map[x_pos][y_pos] == 0)
            {
                black.setPosition(x_pos,y_pos);
                window.draw(black);
            }

            // Visted
            else if(map[x_pos][y_pos] == 1)
            {
                white.setPosition(x_pos,y_pos);
                window.draw(white);
            }
        }
    }
}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap()
{
    const int temp = 8;
    for(int i = (MAP_SIZE/2 - (MAP_SIZE/temp)); i <  (MAP_SIZE/2 + (MAP_SIZE/temp)); i++)
    {
        for(int j =  (MAP_SIZE/2 - (MAP_SIZE/temp)); j <  (MAP_SIZE/2 + (MAP_SIZE/temp)); j++)
        {
            int randInt = rand() % 100;
            if(randInt == 0)
                map[i][j] = 1;
            else
                map[i][j] = 0;

        }
    }
}


void gameOfLife()
{
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            uint32_t count = 0;
            // 0 1 2
            // 3 X 4
            // 5 6 7


            // top row
            if((i != 0) && (j != 0))
                count += map[i-1][j-1];
            if((j != 0))
                count += map[i][j-1];
            if((i != (MAP_SIZE-1)) && (j != 0))
                count += map[i+1][j-1];


            // mid row
            if((i != 0))
                count += map[i-1][j];
            if((i != (MAP_SIZE -1)))
                count += map[i+1][j];

            // Bottom row
            if((i != 0) && (j != (MAP_SIZE-1)))
                count += map[i-1][j+1];
            if( (j != (MAP_SIZE-1)))
                count += map[i][j+1];
            if((i != (MAP_SIZE-1)) && (j != (MAP_SIZE-1)))
                count += map[i+1][j+1];

            /*
            if(count == 0)
                future[i][j] = 0;
            if(count == 1)
                future[i][j] = 0;
            if(count == 2)
                future[i][j] = 1;
            if(count == 3)
                future[i][j] = 0; // should be 1, but looks worse :P
            if(count > 3)
                future[i][j] = 0;
            */
            if(count != 2)
                future[i][j] = 0;
            else
                future[i][j] = 1;
        }
    }

    memset( &map[0][0], 0, sizeof(future) );
    std::memcpy(&map, &future, sizeof(int)*MAP_SIZE*MAP_SIZE);
    memset( &future[0][0], 0, sizeof(future) );
}

void outputRegion(std::pair<int, int> left, std::pair<int, int> right)
{
    for(int i = left.first; i < right.first; i++)
    {
        for(int j = left.second; j < right.second; j++)
        {
            std::cout << map[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
