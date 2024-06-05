#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <cstdlib>

// Constant Variables
const unsigned int WIDTH = 1600;
const unsigned int HEIGHT = 1050;
const float BASEMV = 0.5;
const float ENEMYMV = 0.3;
const float MINMV = 0.1;
const float MAXMV = 1;
const double PI = 3.141592653589793;
const int MAXENEMIES = 1000;

// Classes
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

class player: public enemy{
public:
    sf::RectangleShape shield;
    int health;
    player();
};

class skill{
public:
    sf::RectangleShape object;
    int timer, timeAtCast, duration;
    bool cooldown;
    skill(){
        cooldown = true;
        timer = timeAtCast = duration = 0;
    }
};

class manifest: public skill{
public:
    float mv;
    manifest(){
        mv = 0;
    }
    manifest(float m, int t, int d){
        mv = m;
        timer = t;
        duration = d;
    }
    void barrier(player player);
};

class effect: public skill{
public:
    int health, state;
    float mv;
    effect(){
        health = state = mv = 0;
    }
    effect(int h, int s, float m, int t, int d){
        health = health;
        state = s;
        mv = m;
        timer = t;
        duration = d;
    }
    int time_slow(enemy *enemy, int enemies);
};

void game(player *player, enemy *enemy, int enemies, int level);
bool collision(sf::Shape *shape1, sf::Shape *shape2);
void boundary(sf::Shape *shape1, int width, int height);
void displayString(sf::RenderWindow *window, sf::Text *text, sf::Vector2f pos);

#endif