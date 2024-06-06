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

void game(player *player, entity *enemy, int enemies, int level)
{
    // Generating Initial Stats of Enemy
    for (int i = 0; i < enemies; ++i)
    {
        // Set Position of Enemy
        enemy[i].body.setPosition((rand() % 2) ? WIDTH : 0, rand() % HEIGHT);

        // RNG for Enemy Movement Speed
        float mv = ((rand() % (20 + (5 * level))) / 100.f) + MINMV; // Gets Original MV
        if (mv >= 0.5) mv = (rand() % 2) ? mv : mv - ENEMYMV; // Flip a coin, 50% chance to retain Fast MV
        enemy[i].mv = (mv > MAXMV) ? MAXMV : mv;

        // Setting color based on Movement Speed
        enemy[i].body.setFillColor(sf::Color(255 * enemy[i].mv, 0, 0));
        enemy[i].state = DEAD;
    }

    // Setting Initial Values of Player
    player->body.setPosition((WIDTH - player->body.getSize().x) / 2.f, (HEIGHT - player->body.getSize().y) / 2.f);
    player->shield.setPosition(player->body.getPosition().x, player->body.getPosition().y);
    player->health = 4 + (1.5 * level);
    player->state = ALIVE;
}

void displayString(sf::RenderWindow *window, sf::Text *text, sf::Vector2f pos)
{
    text->setPosition(pos.x, pos.y);
    window->draw(*text);
    return;
}

entity::entity()
{
    state = DEAD;
    mv = ENEMYMV;
    body.setSize(sf::Vector2f(50.f, 50.f));
    body.setPosition(-200.f, -200.f);
    body.setFillColor(sf::Color::Red);
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(-1.f);
    body.setOrigin(50.f/2, 50.f/2);
}

player::player()
{
    state = DEAD;
    health = 4;
    mv = BASEMV;
    
    // Body
    body.setSize(sf::Vector2f(100.f, 100.f));
    body.setPosition(0.f, 0.f);
    body.setFillColor(sf::Color::Green);
    body.setOrigin(100.f/2, 100.f/2);

    // Shield
    shield.setSize(sf::Vector2f(10.f, 150.f));
    shield.setPosition(body.getPosition().x, body.getPosition().y);
    shield.setFillColor(sf::Color(192, 192, 192));
    shield.setOutlineColor(sf::Color(255, 215, 0));
    shield.setOutlineThickness(1.f);
    shield.setOrigin(-100.f, 150.f/2);
}

void skill::check_cooldown(int elapsed_time)
{
    if (elapsed_time - timeAtCast >= timer)
    {
        cooldown = true;
        timeAtCast = 0;
    }
}

 int effect::time_slow(entity *enemy, int enemies)
{
    int c = 0;
    for (int i = 0; i < enemies; ++i)
        if (enemy[i].state == ALIVE)
        {
            c++;
            enemy[i].mv /= mv;
            enemy[i].body.setFillColor(sf::Color(enemy[i].body.getFillColor().r, enemy[i].body.getFillColor().r, 0));
        }
    return c;
}

void effect::reset()
{
    cooldown = true;
    active = false;
}

void manifest::barrier(player *player)
{
    if (active)
    {
        player->shield.setSize(sf::Vector2f(150.f, 150.f));
        player->shield.setFillColor(sf::Color(255, 255, 0, 100));
        player->shield.setOrigin(player->shield.getSize().x / 2.f, player->shield.getSize().y / 2.f);
    }
    else
    {
        player->shield.setSize(sf::Vector2f(10.f, 150.f));
        player->shield.setFillColor(sf::Color(192, 192, 192));
        player->shield.setOrigin(-100.f, player->shield.getSize().y / 2.f);
    }
}

void manifest::reset(player *player)
{
    cooldown = true;
    active = false;
    barrier(player);
}

void manifest::sun_shot(double angle, sf::Vector2f pos)
{
    // Searching Unused Entity for bullet
    int i = 0;
    for (; i < MAXENTITIES; ++i)
        if (object[i].state == DEAD)
            break;
    if (i >= MAXENTITIES)
        return;
    // Setting initial values for bullet
    bullet_angle[i] = angle;
    object[i].body.setPosition(pos.x, pos.y);
    object[i].state = ALIVE;
    object[i].mv = mv;
    object[i].body.setSize(sf::Vector2f(25.f, 15.f));
    object[i].body.setFillColor(sf::Color(255, 215, 0));
    object[i].body.setOutlineColor(sf::Color(192, 192, 192));
    object[i].body.setOrigin(25.f/2.f, 15.f/2.f);
    object[i].body.setOutlineThickness(1.f);
    object[i].body.setRotation(angle);
    return;
}