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

void game(player *player, enemy *enemy, int enemies, int level)
{
    // Generating Initial Stats of Enemy
    for (int i = 0; i < enemies; ++i)
    {
        // Set Position of Enemy
        enemy[i].object.setPosition((rand() % 2) ? WIDTH : 0, rand() % HEIGHT);

        // RNG for Enemy Movement Speed
        float mv = ((rand() % (20 + (5 * level))) / 100.f) + MINMV; // Gets Original MV
        if (mv >= 0.5) mv = (rand() % 2) ? mv : mv - ENEMYMV; // Flip a coin, 50% chance to retain Fast MV
        enemy[i].mv = (mv > MAXMV) ? MAXMV : mv;

        // Setting color based on Movement Speed
        enemy[i].object.setFillColor(sf::Color(255 * enemy[i].mv, 0, 0));
        enemy[i].state = DEAD;
    }

    // Setting Initial Values of Player
    player->object.setPosition((WIDTH - player->object.getSize().x) / 2.f, (HEIGHT - player->object.getSize().y) / 2.f);
    player->shield.setPosition(player->object.getPosition().x, player->object.getPosition().y);
    player->health = 4 + (1.5 * level);
    player->state = ALIVE;
}

void displayString(sf::RenderWindow *window, sf::Text *text, sf::Vector2f pos)
{
    text->setPosition(pos.x, pos.y);
    window->draw(*text);
    return;
}

enemy::enemy()
{
    state = DEAD;
    mv = ENEMYMV;
    object.setSize(sf::Vector2f(50.f, 50.f));
    object.setPosition(-200.f, -200.f);
    object.setFillColor(sf::Color::Red);
    object.setOutlineColor(sf::Color::White);
    object.setOutlineThickness(-1.f);
    object.setOrigin(50.f/2, 50.f/2);
}

player::player()
{
    state = DEAD;
    health = 4;
    mv = BASEMV;
    
    // Body
    object.setSize(sf::Vector2f(100.f, 100.f));
    object.setPosition(0.f, 0.f);
    object.setFillColor(sf::Color::Green);
    object.setOrigin(100.f/2, 100.f/2);

    // Shield
    shield.setSize(sf::Vector2f(10.f, 150.f));
    shield.setPosition(object.getPosition().x, object.getPosition().y);
    shield.setFillColor(sf::Color(192, 192, 192));
    shield.setOrigin(-100.f, 150.f/2);
}

 int effect::time_slow(enemy *enemy, int enemies)
{
    int c = 0;
    for (int i = 0; i < enemies; ++i)
        if (enemy[i].state == ALIVE)
        {
            c++;
            enemy[i].mv /= mv;
            enemy[i].object.setFillColor(sf::Color(enemy[i].object.getFillColor().r, 0, 255));
        }
    return c;
}