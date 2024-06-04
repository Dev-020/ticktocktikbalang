#include "header.h"

bool collision(sf::Shape *shape1, sf::Shape *shape2)
{
    return (shape1->getGlobalBounds().intersects(shape2->getGlobalBounds())) ? true : false;
}

void boundary(sf::Shape *shape1, int width, int height)
{
    sf::Vector2f pos = shape1->getPosition();
    if (pos.x > width)
        shape1->setPosition(0.f, pos.y);
    if (pos.x < -100)
        shape1->setPosition(width, pos.y);
    if (pos.y > height)
        shape1->setPosition(pos.x, 0.f);
    if (pos.y < -100)
        shape1->setPosition(pos.x, height);
}

enemy::enemy()
{
    state = DEAD;
    mv = ENEMYMV;
    object.setSize(sf::Vector2f(50.f, 50.f));
    object.setPosition(-200.f, -200.f);
    object.setFillColor(sf::Color::Red);
    object.setOrigin(50.f/2, 50.f/2);
}

player::player()
{
    state = ALIVE;
    health = 0;
    mv = BASEMV;
    
    // Body
    object.setSize(sf::Vector2f(100.f, 100.f));
    object.setPosition(0.f, 0.f);
    object.setFillColor(sf::Color::Green);
    object.setOrigin(100.f/2, 100.f/2);

    // Shield
    shield.setSize(sf::Vector2f(10.f, 150.f));
    shield.setPosition(0.f, 0.f);
    shield.setFillColor(sf::Color(192, 192, 192));
    shield.setOrigin(-100.f, 150.f/2);
}