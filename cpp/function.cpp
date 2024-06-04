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
