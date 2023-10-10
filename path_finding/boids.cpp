#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>

#define MAP_SIZE 128

struct bird
{
    std::pair<int, int> cord;
    int rotation = 0;
    int velocity = 0;
};




sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

std::array<std::array<int, 128>, 128> map;
std::array<std::array<bird, 49>, 5> boids;

void drawMap();
void generateMap(int drunkards);
void generateBoids();
void drawBoids();
int avgSpeed(int index, int group);
std::pair<int, int> avgPoint(int index, int group);

#include "randomStuff/BFSmap.hpp"

int main()
{

    // Load texture
    {
        if(!texture.loadFromFile("randomStuff/tileSprite.png")) { std::cout << "file did not infact load" << std::endl; }
        texture.setSmooth(true);
    }

    // Setting sprites
    {
        unvisted.setTexture(texture);
        unvisted.setTextureRect(sf::IntRect(0, 0, 8, 8));
        // window.draw(unvisted);

        visited.setTexture(texture);
        visited.setTextureRect(sf::IntRect(8,0,8,8));

        start.setTexture(texture);
        start.setTextureRect(sf::IntRect(16,0,8,8));

        end.setTexture(texture);
        end.setTextureRect(sf::IntRect(24,0,8,8));

        path.setTexture(texture);
        path.setTextureRect(sf::IntRect(32,0,8,8));

        wall.setTexture(texture);
        wall.setTextureRect(sf::IntRect(40,0,8,8));
    }

    // Filling array
    {
        // map.fill(0);
        memset( &map[0][0], 0, sizeof(map) );
    }

    // placing start & end points
    {
        srand(time(0));
        generateMap(100);
        std::pair<int, int> start = {(rand() % 128), (rand() % 128)};
        map[start.first][start.second] = 2;
        map[rand() % 128][rand() % 128] = 3;

        closeOpenSpaces(start);
        generateBoids();

    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        drawMap();
        drawBoids();
        window.display();
    }

    return 0;
}

void drawMap()
{
    for(int x_pos = 0; x_pos < 128; x_pos++)
    {
        for(int y_pos = 0; y_pos < 128; y_pos++)
        {

            // let's say
            // 0 = unvisted
            // 1 = visted
            // 2 = start
            // 3 = end
            // 4 = path
            // 5 = wall

            // Unvisted
            if(map[x_pos][y_pos] == 0)
            {
                unvisted.setPosition(x_pos*8,y_pos*8);
                window.draw(unvisted);
            }

            // Visted
            else if(map[x_pos][y_pos] == 1)
            {
                visited.setPosition(x_pos*8,y_pos*8);
                window.draw(visited);
            }

            // Start
            else if(map[x_pos][y_pos] == 2)
            {
                start.setPosition(x_pos*8,y_pos*8);
                window.draw(start);
            }

            // end
            else if(map[x_pos][y_pos] == 3)
            {
                end.setPosition(x_pos*8,y_pos*8);
                window.draw(end);
            }

            // Path
            else if(map[x_pos][y_pos] == 4)
            {
                path.setPosition(x_pos*8,y_pos*8);
                window.draw(path);
            }


            else if(map[x_pos][y_pos] == 5)
            {
                wall.setPosition(x_pos*8,y_pos*8);
                window.draw(wall);
            }

        }
    }
}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap(int drunkards)
{
    for(int i = 0; i < drunkards; i++)
    {
        // generate the spawn point
        int x_pos = rand() % 128, y_pos = rand() % 128;

        for(int j = 0; j < 50; j++)
        {
            map[x_pos][y_pos] = 5;

            // X 0 X
            // 3 # 1
            // X 2 X
            int randInt = rand() % 4;
            if(randInt == 0 && y_pos > 0)
            {
                y_pos -= 1;
            }

            if(randInt == 1 && x_pos < 128)
            {
                x_pos += 1;
            }

            if(randInt == 2 && y_pos < 128)
            {
                y_pos += 1;
            }

            if(randInt == 3 && x_pos > 0)
            {
                x_pos -= 1;
            }

        }
    }
}

// index (current point), group
std::pair<int, int> avgPoint(int index, int group)
{
    // Find the delta position
    std::pair<int, int> avgPosition;
    avgPosition.first = avgPosition.second = 0;


    for(int i = 0; i < 49; i++)
    {
        if(i != index)
        {
            avgPosition.first  += boids[group][i].cord.first;
            avgPosition.second += boids[group][i].cord.second;
        }
    }

    avgPosition.first  /= 48;
    avgPosition.second /= 48;

    return avgPosition;
}

// index (current point), group
int avgSpeed(int index, int group)
{
    int avgVelocity = 0;

    for(int i = 0; i < 49; i++)
    {
        if(i != index)
        {
            avgVelocity  += boids[group][i].velocity;
        }
    }

    avgVelocity /= 48;

    return avgVelocity;
}

void generateBoids()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 49; j++)
        {
            boids[i][j].cord.first = (rand() % 128);
            boids[i][j].cord.second = (rand() % 128);

            boids[i][j].rotation = (rand() % 360);
            boids[i][j].velocity = (rand() % 5);
        }
    }
}

// Note, must be drawn AFTER map is drawn :)
void drawBoids()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 49; j++)
        {
            path.setPosition(boids[i][j].cord.first*8,boids[i][j].cord.second*8);
            window.draw(path);
        }
    }
}
