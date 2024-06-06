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
const int MAXENTITIES = 1000;

// Classes
enum objectState {
    DEAD = 0, ALIVE
};

class entity{
public:
    sf::RectangleShape body;
    int state;
    float mv;
    entity();
};

class player: public entity{
public:
    sf::RectangleShape shield;
    int health;
    player();
};

class skill{
public:
    entity object[MAXENTITIES];
    int timer, timeAtCast, duration;
    bool cooldown, active;
    skill(){
        cooldown = true;
        active = false;
        timer = timeAtCast = duration = 0;
    }
    void reset();
    void check_cooldown(int elapsed_time);
    void check_duration(int elapsed_time);
};

class manifest: public skill{
protected:
    double bullet_angle[MAXENTITIES]; // <----- FOR SUN SHOT ONLY
public:
    float mv;
    
    manifest(){
        mv = 0;
        for (int i = 0; i < MAXENTITIES; ++i)
            bullet_angle[i] = -1;
    }
    manifest(float m, int t, int d){
        mv = m;
        timer = t;
        duration = d;
        for (int i = 0; i < MAXENTITIES; ++i)
            bullet_angle[i] = -1;
    }
    void reset(player *player);
    void barrier(player *player);
    void sun_shot(double angle, sf::Vector2f pos);
    double get_sun_shot_angle(int i){
        return bullet_angle[i];
    }
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
    void reset();
    int time_slow(entity *entity, int enemies);
};

void game(player *player, entity *entity, int enemies, int level);
bool collision(sf::Shape *shape1, sf::Shape *shape2);
void boundary(sf::Shape *shape1, int width, int height);
void displayString(sf::RenderWindow *window, sf::Text *text, sf::Vector2f pos);

#endif