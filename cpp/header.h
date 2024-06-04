#ifndef _HEADER_H_
#define _HEADER_H_

// To be added
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <cstdlib>

const unsigned int WIDTH = 1600;
const unsigned int HEIGHT = 1050;
const float BASEMV = 0.5;
const float ENEMYMV = 0.3;
const float MINMV = 0.1;
const float MAXMV = 1;
const double PI = 3.141592653589793;

enum objectState {
    DEAD = 0, ALIVE
};

class enemy{
public:
    sf::RectangleShape object;
    int state;
    float mv;
    enemy();
};

class player{
    public:
    sf::RectangleShape object;
    sf::RectangleShape shield;
    int state, health;
    float mv;
    player();
};

bool collision(sf::Shape *shape1, sf::Shape *shape2);
void boundary(sf::Shape *shape1, int width, int height);

#endif