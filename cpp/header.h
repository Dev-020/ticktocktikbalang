#ifndef _HEADER_H_
#define _HEADER_H_

// To be added
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <mutex>

bool collision(sf::Shape *shape1, sf::Shape *shape2);
void boundary(sf::Shape *shape1, int width, int height);

#endif